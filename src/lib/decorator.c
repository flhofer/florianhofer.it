/*
 * decorator.c
 *
 *  Created on: Dec 12, 2019
 *      Author: Florian Hofer
 */

#include "decorator.h"

#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>

// Parameters for tags, to enable defaults and auto-close
#define TAGPARSIZE 4
static struct tagDefault{
	enum tagType tag; 	// Enumeration constant
	char tagAcr[6];		// Acronym in the <> context
	char * parList[TAGPARSIZE];	// pointer to parameter list
	int  tagClose;		// self-contained tag?
}
const
	tagDefault[] = {
		{ tt_HTML,	"HTML", {NULL}, -1},
		{ tt_HEAD,	"HEAD", {NULL}, -1},
		{ tt_META,	"META", {"name", "content", NULL}, 1},
		{ tt_LINK,	"LINK", {"rel", "href", NULL}, 1},
		{ tt_TITLE, "TITLE", {NULL}, 0},
		{ tt_BODY,	"BODY", {NULL}, -1},
		{ tt_SCRIPT,"SCRIPT", {"type", "src", NULL}, 0},
		{ tt_H1,	"H1", {"style", NULL}, 0},
		{ tt_H2,	"H2", {"style", NULL}, 0},
		{ tt_B,		"B", {NULL}, 0},
		{ tt_I,		"I", {"class", NULL}, 0},
		{ tt_DIV,	"DIV", {"class", "id", "style", NULL}, -1},
		{ tt_A,		"A", {"href", "class", "onclick", NULL}, 0},
		{ tt_SPAN,	"SPAN", {"style", NULL}, 0},
		{ tt_IMG,	"IMG", {"src", "width", "height", NULL}, 1},
		{ tt_TABLE,	"TABLE", {NULL}, -1},
		{ tt_TR,	"TR", {NULL}, -1},
		{ tt_TD,	"TD", {"colspan", NULL}, 0},
		{ tt_HR,	"HR", {"style", "id", NULL}, 0},
		{ 0, "", {NULL}, 0}
	};

// stack of tag pointers, points to list of open tags
// TODO -> global to local for eventual multi-threading/dynamic
static enum tagType * tagstack;
#define NUMTAGS 50 // max number of tags in buffer -1 (for zero)
static int tagstackP = 0;
static int tagInline = 0;

static inline void
cgiTagIndent(){
	for (int i=tagstackP; i>0; i--)
		cgiOut("  ");
}

/*
 *
 */
static int
cgiTagParams2 (va_list parList, const struct tagDefault * tag){

	if (tagInline)
		cgiOut("\n");
	tagInline = 0;

	cgiTagIndent();

	cgiOut("<%s", tag->tagAcr);

	const char * t = NULL; // next parameter tag name
	const char * p = NULL; // next parameter value

	// init parameter var
	char * const *parTags = tag->parList;

	// repeat until NULL
	while ((t = *parTags))
	{
		if ((p = va_arg(parList, char *)))
			cgiOut(" %s=\"%s\"", t, p);
		else
			break;
		parTags++;
	}

	if (tag->tagClose > 0)
		cgiOut("/>\n");
	else{
		if (tag->tagClose == -1)
			cgiOut(">\n");
		else{
			cgiOut(">");
			tagInline = 1;
		}
		tagstack[tagstackP] = tag->tag;
		tagstackP++;
	}

	return 0;
}

/*
 *
 */
static int
cgiTagParams (va_list parList, ...){
	va_list tagList;
	va_start(tagList, parList);

	char * t = NULL; // next parameter tag name
	char * p = NULL; // next parameter value

	// repeat until NULL
	while ((t = va_arg(tagList, char *)))
	{
		if ((p = va_arg(parList, char *)))
			cgiOut(" %s=\"%s\"", t, p);
	}
	va_end(tagList);
	return 0;
}

/*
 *	unrollStack: close all tags in stack
 *
 *	Arguments:	-
 *
 *	Return Value: 0 on success, error code otherwise
 */
static int
unrollStack(){
	return cgiTagClose(0);
}

static int
menuMain() {
	cgiTag(tt_DIV, "topnav", "myTopnav");

	AFULL("Home", "#home", "active", NULL)
	AFULL("News", "#news", NULL)
	AFULL("Publications", "#pubs", NULL)
	AFULL("Locations", "#locations", NULL)
	AFULL("Kernel patches", "#patch", NULL)
	cgiTag(tt_A, "javascript:void(0);", "icon", "menuOpen()");
	cgiTag(tt_I, "fa fa-bars", NULL);
	// autoclose I/ autoclose A
	cgiTagClose(tt_DIV);

	return 0; // TODO: fix return value
}

/* --------------	Public functions ----------------- */

/*
 *	cgiTagClose: unroll stack of tags and close all until <tag>
 *
 *	Arguments:	- tag to stop unroll tag stack
 *
 *	Return Value: 0 on success, error code otherwise
 */
int
cgiTagClose(enum tagType tag){

	int pos = 0;

	while (tagstackP > 0)
	{
		pos = -1;
		// loop through available tags
		for (int i=0; tagDefault[i].tag>0; i++ )
			if ( tagstack[(tagstackP-1)] == tagDefault[i].tag ) {
				pos = i;
				break;
			}

		if (pos >= 0){

			tagstackP--;

			// IF -1  indent, if 0 don't
			if ((!tagInline))
				cgiTagIndent();

			tagInline= 0;

			cgiOut("</%s>\n", tagDefault[pos].tagAcr);

			if (tagstack[(tagstackP)] != tag)
				continue;
			break;
		}
		// TODO: internal server error? DIE
//		die("Internal server error");
		cgiOut("INTERNAL SERVER ERROR\n");
		return EINVAL;
	}
	return 0;
}

/*
 *	cgiTag: open a new tag with parameters, push to stack
 *
 *	Arguments:	- tag to add to stack
 *				- .. parameters that come with tag as in tagDefault[]
 *
 *	Return Value: 0 on success, error code otherwise
 */
int
cgiTag (enum tagType tag, ...){
	va_list parList;
	va_start(parList, tag);

	int pos = -1;
	int ret = 0;

	// loop through available tags
	for (int i=0; tagDefault[i].tag>0; i++ )
		if ( tag == tagDefault[i].tag ) {
			pos = i;
			break;
		}

	if (pos >= 0) {
			cgiTagParams2(parList, &tagDefault[pos]);
	}
	else {
		// TODO: internal server error? DIE
//		die("Internal server error");
		cgiOut("INTERNAL SERVER ERROR");
		ret = EINVAL;
	}

	va_end(parList);
	return ret;
}

int cgiSection(char * title, char * id) {
	cgiTag(tt_HR, "margin-bottom: 70px;", id);
	cgiTagClose(tt_HR);
	cgiTag(tt_H2, "text-align: center;");
	cgiOut("%s", title);
	cgiTagClose(tt_H2);
	return 0;// TODO: fix return value
}

int cgiHeader() {
	tagstack = malloc (sizeof(enum tagType) * NUMTAGS);
	*tagstack = 0;

	cgiOut ("Content-Type: text/html\n\n"
			 "<!DOCTYPE html>\n");

	HTML HEAD
	cgiTag(tt_META, "viewport", "width=device-width, initial-scale=1");
	cgiTag(tt_LINK,	"stylesheet", "https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css");
	cgiTag(tt_LINK,	"stylesheet","../css/menu.css");
	TITLE(WEB_TITLE);
	BODY

	(void)menuMain();
	return 0; // TODO: fix return value
}

int cgiFooter() {
	cgiTag(tt_DIV, "bnavbar", "bnav");
	cgiTagIndent();
	cgiOut ("&#9400; 2019 Florian Hofer. Proudly implemented using C. V%s\n", VERSION);
	cgiTagClose(tt_DIV);
	cgiTag(tt_SCRIPT, "text/javascript", "../js/menu.js" );
	// auto-close
	(void)unrollStack();

	free(tagstack); // free stack

	return 0; // TODO: fix return value
}
