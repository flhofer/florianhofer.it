#include <stdio.h> 
#include <stdlib.h>
#include "decorator.h"

int main () {
    printf ("Content-Type: text/html\n\n"
			"<html><head><title>www.florianhofer.it</title></head>\n"
			"<body>This page is temporarly offline. (check back in a month)\n"
			"<br><br><br>\n"
			"<small>This web page is proudly made using C</small>\n"
			"</body><html>\n");

}
