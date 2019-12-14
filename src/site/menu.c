#include <stdio.h> 
#include <stdlib.h>
#include "decorator.h"
//#include <sqlite3.h>


int main () {
	cgiHeader();
    (void)printf ("<div style=\"padding-left:16px\">\n"
    		"<p>This page is temporarly offline. (check back in a month)</p>\n"
    		"</div>\n");
//    printf("%s\n", sqlite3_libversion());
    cgiFooter();

    return EXIT_SUCCESS;
}
