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

#define TSNBUF 8096 // 8kb at a time

int main () {
	cgi_out = stdout;

	char * data = getenv("QUERY_STRING");
	int id;
	if(data == NULL){
		// TODO: add error message out macro/function to decorator, EG 404
		cgiHeader(NULL);
		cgiOut("<P>Error! Error in passing data from form to script.");
	    cgiFooter(NULL);
	}
	else if(sscanf(data,"id=%d",&id)!=1){
		cgiHeader(NULL);
		cgiOut("<P>Error! Invalid data. Data must be numeric.");
		cgiFooter(NULL);
	}
	else {
		const char * path = fileListName(id);

		// TODO: fix return value evaluations and exceptions
		if (NULL == path){
			cgiHeader(NULL);
			cgiOut("<P>Error! Path data. %d</p>", id);
		    cgiFooter(NULL);
		    return 0;
		}


		struct stat st;
				
		if (access(path, F_OK) || !stat(path, &st) || 0 == st.st_size){
			cgiHeader(NULL);
			cgiOut("<P>Error! File not found.</p>", NULL); // FIXME: internal server error if I remove the NULL - Docker container only
			cgiFooter(NULL);
			return 0;
		}

		cgiOut("Content-type: application/pdf\r\n");
		cgiOut("Content-Disposition: inline; filename=\"document.pdf\"\r\n");
		cgiOut("Content-Transfer-Encoding: binary\r\n");
		cgiOut("Content-length: %ld\r\n", st.st_size);
		cgiOut("Accept-Ranges: bytes\r\n\r\n");

		fflush(cgi_out); /* force it to go out */

		// TODO: change to memread (heap) if size is contained < 10 MB
		int fd = open(path, O_RDONLY);

		char buf[TSNBUF];
		int buflen;

		while((buflen = read(fd, buf, TSNBUF)) > 0) {
			write(1, buf, buflen);
		}

		close(fd);
	}
	return 0;

}
