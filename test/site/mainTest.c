/* 
###############################
# test script by Florian Hofer
# last change: 31/03/2023
# ©2020 all rights reserved ☺
###############################
*/

#include "../test.h"

// Includes from library
#include "../../src/include/decorator.h"
#include "../../src/include/files.h"

#include "mainTest.h"


/// TEST CASE -> test something
/// EXPECTED -> comparison value OK
START_TEST(checkValueTest)
{	
	int rv = 1;

	ck_assert_int_eq(2, rv);
}
END_TEST

void site_main (Suite * s) {
	TCase *tc1 = tcase_create("main");
	tcase_add_test(tc1, checkValueTest);

    suite_add_tcase(s, tc1);

	return;
}
