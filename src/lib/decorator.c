/*
 * decorator.c
 *
 *  Created on: Dec 12, 2019
 *      Author: Florian Hofer
 */

#include "decorator.h"

static int menuMain();

int cgiHeader() {
	cgiOut ("Content-Type: text/html\n\n"
			 "<!DOCTYPE html>\n"
			 "<html>\n"
			 "<head>\n"
			 "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
			 "<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css\">\n"
			 "<link rel=\"stylesheet\" href=\"../css/menu.css\">\n"
			 "<title>" WEB_TITLE "</title>\n"
			 "</head>\n"
			 "<body>\n");
	(void)menuMain();
	return 0; // TODO: fix return value
}

static int menuMain() {
	cgiOut ("<div class=\"topnav\" id=\"myTopnav\">\n"
			"<a href=\"#home\" class=\"active\">Home</a>\n"
			"<a href=\"#news\">News</a>\n"
			"<a href=\"#contact\">Contact</a>\n"
			"<a href=\"#locations\">Locations</a>\n"
			"<a href=\"#about\">About</a>\n"
			"<a href=\"javascript:void(0);\" class=\"icon\" onclick=\"menuOpen()\">\n"
			"<i class=\"fa fa-bars\"></i>\n"
			"</a>\n"
			"</div>\n");
	return 0; // TODO: fix return value
}

int cgiFooter() {
	cgiOut ("<div class=\"bnavbar\" id=\"bnav\">\n"
			"&#9400; 2019 Florian Hofer. Proudly implemented using C\n"
			"</div>\n");
	cgiOut ("<script type=\"text/javascript\" src=\"../js/menu.js\"></script>\n"
			"</body>\n"
			"</html>");
	return 0; // TODO: fix return value
}
