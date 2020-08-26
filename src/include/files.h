/*
 * files.h
 *
 *  Created on: Dec 14, 2019
 *      Author: Florian Hofer
 */

#ifndef _FILES_H_
	#define _FILES_H_

	int listFiles ();
	const char * fileListName (int id);

	int includeFile(char* fname);

	#define FBUFSZ 1024

#endif /* _FILES_H_ */
