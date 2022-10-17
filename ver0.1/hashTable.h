#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "./parson/parson.h"
#include <unistd.h>
#include <signal.h>
#define TBLSIZE 5 

typedef struct linked_list 
{
		struct linked_list *next;
		char *hash_key;
		char *data;
} linked_list_t;

typedef struct hash_table 
{
		struct linked_list *head;
		struct linked_list *tail;
		int list_entry;
} hash_table_t;

typedef struct json_struct
{
		int thread_num;
		char *thread_name;
		int thread_option;
		char *hash_file;	
		//int thread_id;
		int thread_flag;
		char thread_status[9];
} json_struct_t;

int pidCheck(int pid);

int jsonReadThreadNum(void);
void * jsonReadAlloc(void * json_struct);
void * thread_routine(void * arg);

int whileFileRead(void * arg);

FILE * fileOpen(void * arg);
hash_table_t * hashTableArrClear(hash_table_t* hash_table_array);
char ** hashFileRead(char *hash_buf);

int nullCheck(void *string, char *name);

int hashKeyToAscii(char *hash_key);
int hashKeyToIndex(int hash_key);

void * linkedListAlloc(linked_list_t *linked_list, char **hash);
void * hashTableIndex(hash_table_t * hash_table, linked_list_t * linked_list);
void printHashTable(hash_table_t * hash_table);

void freeFunc(char **addr);
void freeHash(hash_table_t * hash_table);
void freeList(linked_list_t * addr);
void freeThreadJson(json_struct_t * addr, int threadNum);
void freeAll(void *addr);

