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
	long id;		// TODO: change id to 128 bit hash or use session ID
	char* title;
	char* author;
	char* year;
	char* folder;
	char* filen;
	char* target;
} filed_t;

static filed_t * fhead;

#define PRJDB "../res/prj.csv"

typedef struct prjdata {
	struct prjdata * next;
	long id;
	char* title;
	char* years;
	char* partner;
	char* link;
	char* linkd;
	char* descr;
} prjd_t;

static prjd_t * phead;

/*
 * getfields() : parse buffer and get entry fields for paper specification
 *
 * Arguments : - pointer to buffer containing a CSV line
 * 			   - pointer ** head of linked list
 * 			   - element size
 *
 * Return : -
 */
static
void getfields(char* buff, void **head, size_t elements) {
	char* tok;
	char* rest = buff;

	if ((!buff) || '#' == *buff) // skip if commented out
    	return;

	if ((tok = strtok_r(buff, "\n", &rest)))
		do {
			char * line = strdup(tok); // new line to scan

			// add element to linked list
			push(head, elements);

			char ** elmPos = ((char**)*head);
			elmPos++;

			// start with ID -> int
			if ((tok = strtok(line, ";"))){
				*elmPos = (void*) atol(tok);
				elmPos++;
			}

			int cnt = 0;

			// continue until end
			while ((tok = strtok(NULL, ";\n"))){
				// skip if out of scope
				if (((void*)*head)+elements > (void*)elmPos)
					*elmPos = strdup(tok);

				elmPos++;
				cnt++;
			}
			// delete element if incomplete or too big
			if (((void*)*head)+elements != (void*)elmPos)
				pop(head);
		}
		while ((tok = strtok_r(NULL, "\n", &rest)));
}

/*
 * readFile() : Read CSV containing the papers into memory
 *
 * Arguments : - CSV filename
 * 			   - pointer ** head of linked list
 * 			   - element size
 *
 * Return : 0 on success, Error code otherwise
 */
static
int readFile (const char * filename, void **head, size_t elements) {
    FILE* stream;
    char fbuff [FBUFSZ];

    if ((stream = fopen(filename, "r"))) {
		while (fgets(fbuff, FBUFSZ, stream))
		{
			// NOTE strtok clobbers tmp
			char* tmp = strdup(fbuff);
			getfields(tmp, head, elements);
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
fileListName (long id) {

	if (!readFile(FILEDB, (void**)&fhead, sizeof(filed_t)))
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
	int ret = readFile(FILEDB, (void**)&fhead, sizeof(filed_t));

	cgiTag(tt_DIV, NULL, NULL, "padding-left:16px");
	cgiTag(tt_TABLE);

	if (!ret)
		for (filed_t * cur = fhead; ((cur)); cur=cur->next){
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
			(void)sprintf(url, "display.cgi?id=%ld", cur->id);
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
 * listProjects() : print a list of projects in CSV file
 *
 * Arguments : -
 *
 * Return: 0 on success, error code otherwise
 */
int
listProjects () {
	// TODO: add session ID to force visiting site, or use hashes only
	int ret = readFile(PRJDB, (void**)&phead, sizeof(prjd_t));

	cgiTag(tt_DIV, NULL, NULL, "padding-left:16px");
	cgiTag(tt_TABLE);

	if (!ret)
		for (prjd_t * cur = phead; ((cur)); cur=cur->next){
			cgiTag(tt_TR);
			cgiTag(tt_TD, NULL);
			cgiOut("%s", cur->title);
			cgiTagClose(tt_TD);
			cgiTag(tt_TD, NULL);
			cgiOut("%s", cur->partner);
			cgiTagClose(tt_TR);

			cgiTag(tt_TR);
			cgiTag(tt_TD, "2");
			cgiTag(tt_A, cur->link);
			cgiOut("%s", cur->linkd);
			cgiTagClose(tt_TR);

			cgiTag(tt_TR);
			cgiTag(tt_TD, "2");
			cgiOut("%s", cur->descr);
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

