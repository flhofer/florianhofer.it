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
static struct {
	enum tagType tag; 	// Enumeration constant
	char tagAcr[6];		// Acronym in the <> context
} tagDefault[] = {
		{ tt_HTML,	"HTML"},
		{ tt_HEAD,	"HEAD"},
		{ tt_META,	"META"},
		{ tt_LINK,	"LINK"},
		{ tt_TITLE, "TITLE"},
		{ tt_BODY,	"BODY"},
		{ tt_H1,	"H1"},
		{ tt_H2,	"H2"},
		{ tt_B,		"B"},
		{ tt_IT,	"IT"},
		{ tt_DIV,	"DIV"},
		{ tt_A,		"A"},
		{ tt_TABLE,	"TABLE"},
		{ tt_TR,	"TR"},
		{ tt_TD,	"TD"},
		{ 0, "" }
	};

// stack of tag pointers, points to list of open tags
static enum tagType * tagstack;
#define NUMTAGS 50 // max number of tags in buffer -1 (for zero)
static int tagstackP = 0;

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

			cgiOut("</%s>\n", tagDefault[pos].tagAcr);
			tagstackP--;

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
int
cgiTag (enum tagType tag, ...){
	va_list ap;
	va_start(ap, tag);

	int pos = -1;

	// loop through available tags
	for (int i=0; tagDefault[i].tag>0; i++ )
		if ( tag == tagDefault[i].tag ) {
			pos = i;
			break;
		}

	if (pos >= 0) {
		char * s = NULL;

		switch (tag) {

		case tt_TITLE: // Self-enclosed page title

			s = va_arg(ap, char *);

			cgiOut("<%s>%s</%s>\n", tagDefault[pos].tagAcr,
					s,
					tagDefault[pos].tagAcr);
			break;

		case tt_LINK: // Self-enclosed Header link

			cgiOut("<%s", tagDefault[pos].tagAcr);

			// check first parameter, relation type
			if ((s = va_arg(ap, char *)))
				cgiOut(" rel=\"%s\"", s);

			// check second parameter, hyper reference
			if ((s = va_arg(ap, char *)))
				cgiOut(" href=\"%s\"", s);

			cgiOut("/>\n");
			break;

		case tt_META: // Self-enclosed meta information

			cgiOut("<%s", tagDefault[pos].tagAcr);

			// check first parameter, meta name
			if ((s = va_arg(ap, char *)))
				cgiOut(" name=\"%s\"", s);

			// check second parameter, meta value
			if ((s = va_arg(ap, char *)))
				cgiOut(" content=\"%s\"", s);

			cgiOut("/>\n");
			break;

		case tt_BODY: // close first
			cgiTagClose(tt_HEAD);
			// then fall through
			// no break
		default:
			cgiOut("<%s>", tagDefault[pos].tagAcr);
			tagstack[tagstackP] = tagDefault[pos].tag;
			tagstackP++;

		}
	}
	else
		// TODO: internal server error? DIE
//		die("Internal server error");
		cgiOut("INTERNAL SERVER ERROR");

	va_end(ap);
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
	cgiOut ("<div class=\"topnav\" id=\"myTopnav\">\n"
			"<a href=\"#home\" class=\"active\">Home</a>\n"
			"<a href=\"#news\">News</a>\n"
			"<a href=\"#contact\">Contact</a>\n"
			"<a href=\"#locations\">Locations</a>\n"
			"<a href=\"#patch\">Kernel patches</a>\n"
			"<a href=\"#about\">About</a>\n"
			"<a href=\"javascript:void(0);\" class=\"icon\" onclick=\"menuOpen()\">\n"
			"<i class=\"fa fa-bars\"></i>\n"
			"</a>\n"
			"</div>\n");
	return 0; // TODO: fix return value
}

int cgiFooter() {
	cgiOut ("<div class=\"bnavbar\" id=\"bnav\">\n"
			"&#9400; 2019 Florian Hofer. Proudly implemented using C. V%s\n"
			"</div>\n", VERSION);
	cgiOut ("<script type=\"text/javascript\" src=\"../js/menu.js\"></script>\n"
			"</body>\n"
			"</html>");

	(void)unrollStack();

	free(tagstack); // free stack

	return 0; // TODO: fix return value
}
