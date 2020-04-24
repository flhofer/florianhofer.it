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

static int menuMain();

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
		{ tt_H1,	"H1", {NULL}, 0},
		{ tt_H2,	"H2", {NULL}, 0},
		{ tt_B,		"B", {NULL}, 0},
		{ tt_I,		"I", {"class", NULL}, 0},
		{ tt_DIV,	"DIV", {"class", "id", NULL}, -1},
		{ tt_A,		"A", {"href", "class", "on-click", NULL}, 0},
		{ tt_TABLE,	"TABLE", {NULL}, -1},
		{ tt_TR,	"TR", {NULL}, -1},
		{ tt_TD,	"TD", {NULL}, 0},
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
 *
 */
int
cgiTag (enum tagType tag, ...){
	va_list parList;
	va_start(parList, tag);

	int pos = -1;

	// loop through available tags
	for (int i=0; tagDefault[i].tag>0; i++ )
		if ( tag == tagDefault[i].tag ) {
			pos = i;
			break;
		}

	if (pos >= 0) {

		switch (tag) {

		case tt_TITLE: // Self-enclosed page title

			cgiTagIndent();

			char * s = va_arg(parList, char *);

			cgiOut("<%s>%s</%s>\n", tagDefault[pos].tagAcr,
					s,
					tagDefault[pos].tagAcr);
			break;

		case tt_BODY: // close first
			cgiTagClose(tt_HEAD);
			// then fall through
			// no break
		default:
			cgiTagParams2(parList, &tagDefault[pos]);
		}
	}
	else
		// TODO: internal server error? DIE
//		die("Internal server error");
		cgiOut("INTERNAL SERVER ERROR");

	va_end(parList);
	return 0;
}

static int
unrollStack(){
	return cgiTagClose(0);
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
	cgiTag(tt_TITLE, WEB_TITLE);
	BODY

	(void)menuMain();
	return 0; // TODO: fix return value
}

static int menuMain() {
	cgiTag(tt_DIV, "topnav", "myTopnav");

	AFULL("Home", "#home", NULL)
	AFULL("Contact", "#contact", NULL)
	AFULL("Locations", "#locations", NULL)
	AFULL("Kernel patches", "#patch", NULL)
	AFULL("About", "#about", NULL)
	cgiTag(tt_A, "javascript:void(0);", NULL);
	cgiTag(tt_I, "fa fa-bars", NULL);
	// autoclose I/ autoclose A
	cgiTagClose(tt_DIV);

	return 0; // TODO: fix return value
}

int cgiFooter() {
	cgiOut ("<div class=\"bnavbar\" id=\"bnav\">\n"
			"&#9400; 2019 Florian Hofer. Proudly implemented using C. V%s\n"
			"</div>\n", VERSION);
	cgiOut ("<script type=\"text/javascript\" src=\"../js/menu.js\"></script>\n");

	(void)unrollStack();

	free(tagstack); // free stack

	return 0; // TODO: fix return value
}
