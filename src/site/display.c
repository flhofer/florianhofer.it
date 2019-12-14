/*
 * display.c
 *
 *  Created on: Dec 14, 2019
 *      Author: Florian Hofer
 */
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "site.h"
#include "decorator.h"
#include "files.h"

int main () {
	cgi_out = stdout;

	char * data = getenv("QUERY_STRING");
	int id;
	if(data == NULL){
		cgiHeader();
		cgiOut("<P>Error! Error in passing data from form to script.");
	    cgiFooter();
	}
	else if(sscanf(data,"id=%d",&id)!=1){
		cgiHeader();
		cgiOut("<P>Error! Invalid data. Data must be numeric.");
		cgiFooter();
	}
	else {
/*		const char * path = fileListName(id);

		if (NULL == path){
			cgiHeader();
			cgiOut("<P>Error! Path data. %d</p>", id);
		    cgiFooter();
		    return 0;
		}
*/
		char path[] = "../htdocs/papers/esem18.pdf";

		cgiOut("Content-type: application/pdf\n");
		cgiOut("Content-Disposition: inline; filename=\"document.pdf\"\n");
		cgiOut("Content-Transfer-Encoding: binary\n");
		cgiOut("Accept-Ranges: bytes\n");

		fflush(cgi_out); /* force it to go out */

		int fd = open(path, O_RDONLY);

		char buf[1024];
		int buflen;

		while((buflen = read(fd, buf, 1024)) > 0) {
			write(1, buf, buflen);
		}

		close(fd);
	}
	return 0;

}
