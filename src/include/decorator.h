/*
 * decorator.h
 *
 *  Created on: Dec 12, 2019
 *      Author: Florian Hofer
 */
#include <stdio.h>

#ifndef _DECORATOR_H_
	#define _DECORATOR_H_

	int cgiHeader();
	int cgiFooter();

	// Macro replacements
	extern FILE * cgi_out; // debug output file, defined in main
	#define cgiOut(...) (void)fprintf (cgi_out, __VA_ARGS__)

	// Constants
	#define WEB_TITLE "Florian Hofer - www.florianhofer.it"
#endif /* _DECORATOR_H_ */
