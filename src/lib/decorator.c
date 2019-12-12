/*
 * decorator.c
 *
 *  Created on: Dec 12, 2019
 *      Author: florianhofer
 */

#include "decorator.h"

int cgiHeader() {
	 return printf ("Content-Type: text/html\n\n");
}
