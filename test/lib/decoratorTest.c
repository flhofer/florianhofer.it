/*
###############################
# test script by Florian Hofer
# last change: 31/03/2023
# ©2019 all rights reserved ☺
###############################
*/

#include "decoratorTest.h"


START_TEST(decorator_cgiTag)
{
	int ret = 0;
	char str[18] = "";
	ck_assert_int_eq(ret, -1);

	ck_assert_str_eq(str, "<t>");
}
END_TEST

void library_decorator (Suite * s) {

	TCase *tc1 = tcase_create("decorator");

	tcase_add_test(tc1, decorator_cgiTag);

    suite_add_tcase(s, tc1);

	return;
}
