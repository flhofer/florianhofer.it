#include <stdlib.h>
#include "site.h"
#include "decorator.h"

int main () {
	// assign file to standard out TODO: change to parameter
	cgi_out = stdout;

	cgiHeader();
    cgiOut ("<div style=\"padding-left:16px\">\n"
    		"<h2>This page is temporarly offline.</h2>\n"
    		"<p>Check back in a month or follow development <a href=\"https://github.com/flhofer/florianhofer.it\">here</a></p>\n"
    		"</div>\n");

    cgiFooter();

    return EXIT_SUCCESS;
}
