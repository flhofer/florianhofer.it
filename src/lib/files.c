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
			pop((void**)fhead);
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

	cgiOut("<div style=\"padding-left:16px\"><table>\n");

	if (!ret)
		for (filed_t * cur = fhead; ((cur)); cur=cur->next){
		cgiOut ("<tr><td>%s</td><td>%s</td></tr>\n", cur->author, cur->year);
		cgiOut ("<tr><td colspan=2><a href=\"display.cgi?id=%d\"> %s</a></td></tr>\n", cur->id, cur->title);
		cgiOut ("<tr><td colspan=2>&nbsp;</td></tr>\n");
		}
	cgiOut("</table></div>\n");

	return ret;
}
