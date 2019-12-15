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
#include <sys/stat.h>

#include "site.h"
#include "decorator.h"
#include "files.h"

int main () {
	cgi_out = stdout;

	char * data = getenv("QUERY_STRING");
	int id;
	if(data == NULL){
		// TODO: add error message out macro/function to decorator, EG 404
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
		const char * path = fileListName(id);

		// TODO: fix return value evaluations and exceptions
		if (NULL == path){
			cgiHeader();
			cgiOut("<P>Error! Path data. %d</p>", id);
		    cgiFooter();
		    return 0;
		}

		struct stat st;
		stat(path, &st);

		cgiOut("Content-type: application/pdf\r\n");
		cgiOut("Content-Disposition: inline; filename=\"document.pdf\"\r\n");
		cgiOut("Content-Transfer-Encoding: binary\r\n");
		cgiOut("Content-length: %ld\r\n", st.st_size);
		cgiOut("Accept-Ranges: bytes\r\n\r\n");

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
