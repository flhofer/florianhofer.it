#include <stdio.h> 
#include <stdlib.h>
#include "decorator.h"
//#include <sqlite3.h>


int main () {
	cgiHeader();
    (void)printf ("<div style=\"padding-left:16px\">\n"
    		"<h2>This page is temporarly offline.</h2>\n"
    		"<p>Check back in a month or follow development <a href=\"https://github.com/flhofer/florianhofer.it\">here</a></p>\n"
    		"</div>\n");
//    printf("%s\n", sqlite3_libversion());
    cgiFooter();

    return EXIT_SUCCESS;
}
