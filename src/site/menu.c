#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "site.h"
#include "decorator.h"
#include "files.h"

void
headCallBack () {
	// TODO: return value
	(void)includeFile("analyticstracking.inc");
}

/*
 *
 */
int
arrangeImg(char * img1,...){
	va_list imgList;
	va_start(imgList, img1);

    cgiTag(tt_DIV, NULL);

    char size[5];
    (void)sprintf(size, "%i", 192); // TODO: replace with parameter?

    char * path = NULL;
	if ((path = malloc(255)))
		// repeat until NULLchar
		do
		{
			(void)strcpy(path , "../images/");
			cgiTag(tt_IMG, strcat(path, img1), size, size," class=\"\" border=\"0\"");

			;
		}
	while ((img1 = va_arg(imgList, char *)));

	free(path);
	cgiTagClose(tt_DIV);

	va_end(imgList);
	return 0;
}

/*
 *
 */
int
main () {
	// assign file to standard out TODO: change to parameter
	cgi_out = stdout;

	cgiHeader(&headCallBack);
    cgiTag(tt_DIV, "entry-content", NULL);

    cgiSection("Florian Hofer", "home");

    cgiTag(tt_DIV, NULL);

    cgiTag(tt_DIV, NULL);
    cgiTag(tt_DIV, "", "", "display: inline; margin: 5px 10px; float: right;");
    cgiTag(tt_SPAN, "font-family: arial,sans-serif;");
    cgiTag(tt_SPAN, "font-size: medium;");
    cgiTag(tt_IMG, "../images/portrait.png", "195", "247"," class=\"\" border=\"0\"");
    cgiTagClose(tt_DIV);

    cgiTag(tt_P);
    cgiTag(tt_SPAN, "font-size: medium;");cgiOut("<!--Florian Hofer-->"); BR
    cgiOutI("Freelancer and PostDoc Researcher"); BR
    AFULL("Free University of Bolzano", "https://www.unibz.it", NULL); BR
	AFULL("University of California", "https://www.berkeley.edu", NULL); BR
	cgiOutI("POS 1.14"); BR
	cgiOutI("Faculty of Computer Science ("); AFULL("map", "https://goo.gl/maps/BDccUeNKTtdwd7kn7", NULL); cgiOut(")"); BR
	cgiOutI("Bolzano"); BR
	cgiOutI("IT"); BR
	cgiOutI("+39 0471 016029"); BR
	cgiOutI("florian.hofer@unibz.it ("); AFULL("PGP", "../docmisc/public.pgp", NULL); cgiOut(")");
    cgiTagClose(tt_DIV);

    cgiTag(tt_P);
    cgiTag(tt_SPAN, "font-size: medium;"); cgiTag(tt_B, NULL); cgiOut("About");
    cgiTagClose(tt_P);
    cgiTag(tt_P);
    cgiOutI("Florian Hofer is a Freelancer for Industrial automation and Industry 4.0, and a PostDoc Researcher. He was previously a Visiting Researcher at the University of California, Berkeley, where he collaborated with Siemens USA on a Real-Time Control Container project.");
    cgiTagClose(tt_P);
    cgiTagClose(tt_DIV);

    cgiSection("News", "news");

    listNews();

    cgiSection("Project and collaboration partners", "partner");

    arrangeImg("bts.jpg");

    cgiTagClose(tt_DIV);

    cgiFooter(NULL);

    return EXIT_SUCCESS;
}
