/*
 * files.h
 *
 *  Created on: Dec 14, 2019
 *      Author: Florian Hofer
 */

#ifndef _FILES_H_
	#define _FILES_H_

	int listFiles ();
	const char * fileListName (long id);
	int listProjects ();
	int listLocations ();
	int listNews ();

	int includeFile(char* fname);

	#define FBUFSZ 1024

#endif /* _FILES_H_ */
