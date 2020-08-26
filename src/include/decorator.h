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
		tt_SCRIPT,
		tt_H1 = 100,
		tt_H2,
		tt_B,
		tt_I,
		tt_P,
		tt_DIV,
		tt_A,
		tt_HR,
		tt_SPAN,
		tt_IMG,
		tt_TABLE = 200,
		tt_TR,
		tt_TD
	};

	int cgiHeader(void * callb() );
	int cgiFooter(void * callb() );

	// Macro replacements
	extern FILE * cgi_out; // debug output file, defined in main
	#define cgiOut(...) (void)fprintf (cgi_out, __VA_ARGS__)

	void cgiOutI(char * fmt,...);
	int	cgiTag (enum tagType tag, ...);
	int	cgiTagClose (enum tagType tag);

	#define HTML cgiTag(tt_HTML);
	#define HEAD cgiTag(tt_HEAD);
	#define BODY cgiTagClose(tt_HEAD); \
					cgiTag(tt_BODY);
	#define AFULL(cont, ...) cgiTag(tt_A, __VA_ARGS__); \
						 	 cgiOut(cont); \
							 cgiTagClose(tt_A);
	#define TITLE(cont)  cgiTag(tt_TITLE); \
					 	 cgiOut(cont); \
						 cgiTagClose(tt_TITLE);
	#define BR			 cgiOut("<BR/>\n");

	int cgiSection(char * title, char * id);

	// Constants
	#define WEB_TITLE "Florian Hofer - www.florianhofer.it"
#endif /* _DECORATOR_H_ */
