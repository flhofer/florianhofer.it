/* 
###############################
# test script by Florian Hofer
# last change: 31/03/2023
# ©2019 all rights reserved ☺
###############################
*/

#include "../test.h"

#include "site_suite.h"

#include "mainTest.h"

Suite * orchestrator_suite(void) {

	Suite *s = suite_create("Orchestrator");

	// call tests and append test cases	

	// these use dbgprint. check first
	if (!dbg_out) {
		dbg_out = (FILE *)stderr;
	}
	site_main(s);

	return s;
}
