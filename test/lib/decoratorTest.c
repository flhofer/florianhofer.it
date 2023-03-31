/*
###############################
# test script by Florian Hofer
# last change: 31/03/2023
# ©2019 all rights reserved ☺
###############################
*/

#include "decoratorTest.h"
#include "../test.h"

// library to test
#include "decorator.h"

START_TEST(decorator_cgiOut)
{
	FILE * out = fopen(OUTFILE, "r");
	char str[255] = "t";
	int ret;

	cgiOut("Test");
	fflush(cgi_out);

	ret = fread(str,sizeof(str), sizeof(str), out);
	ck_assert_int_eq(5, ret);
	ck_assert_str_eq("Test", str);

	fclose(out);
}
END_TEST

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

	tcase_add_test(tc1, decorator_cgiOut);
	tcase_add_test(tc1, decorator_cgiTag);

    suite_add_tcase(s, tc1);

	return;
}
