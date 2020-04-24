/*
 * decorator.h
 *
 *  Created on: Dec 12, 2019
 *      Author: Florian Hofer
 */
#include <stdio.h>

#ifndef _DECORATOR_H_
	#define _DECORATOR_H_

	enum tagType {
		tt_HTML = 1,
		tt_HEAD,
		tt_META,
		tt_LINK,
		tt_TITLE,
		tt_BODY,
		tt_H1 = 100,
		tt_H2,
		tt_B,
		tt_IT,
		tt_DIV,
		tt_A,
		tt_TABLE = 200,
		tt_TR,
		tt_TD
	};

	int cgiHeader();
	int cgiFooter();

	// Macro replacements
	extern FILE * cgi_out; // debug output file, defined in main
	#define cgiOut(...) (void)fprintf (cgi_out, __VA_ARGS__)

	int	cgiTag (enum tagType tag, ...);
	#define HTML cgiTag(tt_HTML);
	#define HEAD cgiTag(tt_HEAD);
	#define BODY cgiTag(tt_BODY);

	// Constants
	#define WEB_TITLE "Florian Hofer - www.florianhofer.it"
#endif /* _DECORATOR_H_ */
