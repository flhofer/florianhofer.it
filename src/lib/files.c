/*
 * files.c
 *
 *  Created on: Dec 14, 2019
 *      Author: Florian Hofer
 */
#include "files.h"
#include "decorator.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// -- dummy structure for generic push and pop functions
struct base {
	struct base * next;
};

void push(void ** head, size_t size) {
    struct base * new_node = calloc(1,size);
	if (!new_node)
		cgiOut("could not allocate memory!"); // TODO: error output decorator

    new_node->next = *head;
    *head = new_node;
}

void pop(void ** head) {
    if (NULL == *head) {
        return;
    }

    struct base * next_node = NULL;
    next_node = ((struct base *)*head)->next;
	free(*head);
    *head = next_node;
}

#define FILEDB "../res/files.csv"

typedef struct filedata {
	struct filedata * next;
	int id;		// TODO: change id to 128 bit hash or use session ID
	char* title;
	char* author;
	char* year;
	char* folder;
	char* filen;
	char* target;
} filed_t;

static filed_t * fhead;

/*
 * getfields() : parse buffer and get entry fields for paper specification
 *
 * Arguments : - pointer to buffer containing CSV
 *
 * Return : -
 */
static
void getfields(char* buff) {
	char* tok;
	char* rest = buff;

	// TODO: missing null check
	if ('#' == *buff) // skip if commented out
    	return;

    while ((tok = strtok_r(buff, "\n", &rest))) {
    	buff = NULL; // reset after first
    	char * line = strdup(tok); // new line to scan

    	// add element to linked list
        push((void**)&fhead, sizeof(filed_t));

    	// start with ID
    	if ((tok = strtok(line, ";")))
			fhead->id = atoi(tok);

		int cnt = 0;
		// continue until end
		while ((tok = strtok(NULL, ";\n"))){
			switch (cnt) {
				case 0:
					fhead->title = strdup(tok);
					break;
				case 1:
					fhead->author = strdup(tok);
					break;
				case 2:
					fhead->year = strdup(tok);
					break;
				case 3:
					fhead->folder = strdup(tok);
					break;
				case 4:
					fhead->filen = strdup(tok);
					break;
				case 5:
					fhead->target = strdup(tok);
			}
			cnt++;
		}
		// delete element if incomplete or too big
		if (6 < cnt || 6 > cnt)
			pop((void**)&fhead);
    }
}

/*
 * readFile() : Read CSV containing the papers into memory
 *
 * Arguments : -
 *
 * Return : 0 on success, Error code otherwise
 */
static
int readFile () {
    FILE* stream;
    char fbuff [FBUFSZ];

    if ((stream = fopen(FILEDB, "r"))) {
		while (fgets(fbuff, FBUFSZ, stream))
		{
			// NOTE strtok clobbers tmp
			char* tmp = strdup(fbuff);
			getfields(tmp);
			free(tmp);
		}
		fclose(stream);

		return 0;
    }

    cgiOut("<h2>warn: file %s not found: %s </h2>\n", FILEDB, strerror(errno));

    return ENFILE;
}

/*
 * fileListName() : get the full file path of the entry no
 *
 * Arguments : - id or entry number
 *
 * Returns : Path on success, NULL on fail
 */
const char *
fileListName (int id) {

	if (!readFile())
		for (filed_t * cur = fhead; ((cur)); cur=cur->next){
			if (id == cur->id) {
				char* fn = malloc(strlen(cur->folder) + strlen(cur->filen) +2 + 10);
				// TOOD: fix and clean
				*fn = '\0';
				(void)strcat(fn, "../");
				(void)strcat(fn, cur->folder);
				(void)strcat(fn, "/");
				(void)strcat(fn, cur->filen);
				return fn;
			}
		}
	return NULL;
}

/*
 * listFiles() : print a list of documents in CSV file
 *
 * Arguments : -
 *
 * Return: 0 on success, error code otherwise
 */
int
listFiles () {
	// TODO: add session ID to force visiting site, or use hashes only
	int ret = readFile();

	cgiTag(tt_DIV, NULL, NULL, "padding-left:16px");
	cgiTag(tt_TABLE);

	if (!ret)
		for (filed_t * cur = fhead; ((cur)); cur=cur->next){
			// TODO: fix colspan... simplify (macro?)
			cgiTag(tt_TR);
			cgiTag(tt_TD, NULL);
			cgiOut("%s", cur->author);
			cgiTagClose(tt_TD);
			cgiTag(tt_TD, NULL);
			cgiOut("%s", cur->year);
			cgiTagClose(tt_TR);

			cgiTag(tt_TR);
			cgiTag(tt_TD, "2");
			char url[20];
			(void)sprintf(url, "display.cgi?id=%d", cur->id);
			cgiTag(tt_A, url);
			cgiOut("%s", cur->title);
			cgiTagClose(tt_TR);

			cgiTag(tt_TR);
			cgiTag(tt_TD, "2");
			cgiOut ("&nbsp;");
			cgiTagClose(tt_TR);
		}
	cgiTagClose(tt_DIV);

	return ret;
}

/*
 * includeFile() : reads and prints a file to out
 *
 * Arguments : - filename to include in output
 *
 * Return: 0 on success, error code otherwise
 */
int 
includeFile(char* fname){

	if (!fname)
		return -1;

	// build filename
	// TODO: add function, external append directory prefix + sanitize
	char * fn = malloc (strlen(fname) + 4); // + ../\0
	(void)strcpy(fn , "../");
	(void)strcat(fn, fname);

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(fn, "r");
    if (fp == NULL){
     	free(fn);
        return -2; // does not exist
	}
	
    while ((read = getline(&line, &len, fp)) != -1) 
		cgiOut("%s", line);
		
    fclose(fp);

    free(line);
	free (fn);
	return 0;    
}

