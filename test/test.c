/* 
###############################
# test runner main script by Florian Hofer
# last change: 31/03/2023
# ©2023 all rights reserved ☺
###############################
*/
#include "test.h"

// ############################ common global variables ###########################333

// debug output file
FILE  * dbg_out;
FILE  * cgi_out;

// ############################ end common global variables ###########################333

// include library and main suite code
#include "lib/library_suite.h"
#include "site/site_suite.h"

#include "decorator.h"

#include <time.h>
#include <stdlib.h>

int main(void)
{
	// init pseudo-random tables
	srand(time(NULL));

	//prepare output file
	cgi_out = fopen(OUTFILE, "w");

    int nf=0;
    SRunner *sr;

	Suite * s1 = library_suite();
	Suite * s2 = site_suite();
	sr = srunner_create(s1);
	srunner_add_suite(sr, s2);
//	srunner_set_xml(sr, "UnitTest.xml");
	srunner_run_all(sr, CK_NORMAL);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    fclose(cgi_out);
    return nf == 0 ? 0 : 1;
}


