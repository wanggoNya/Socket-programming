#include "hashTable.h"
__thread hash_table_t hash_table_arr[TBLSIZE];

int pidCheck(int pid)
{
		if (pid < 0)
		{
				perror("thread create error : \n");
				exit(0);
		}
		return 1;
}

int jsonReadThreadNum(void)
{
		JSON_Value *rootValue;
		JSON_Object *rootObject;
		JSON_Array *threadArray;

		rootValue = json_parse_file("jparser.json");
		rootObject = json_value_get_object(rootValue);

		int threadNum, threadArrayCount;

		threadNum = (int)json_object_get_number(rootObject, "thread_num");
		threadArray = json_object_get_array(rootObject, "thread");
		threadArrayCount = (int)json_array_get_count(threadArray);

		if(threadNum != threadArrayCount) return 0;

		json_value_free(rootValue);

		return threadNum;
}

void * jsonReadAlloc(void * json_struct)
{		
		json_struct_t * tj1 = (json_struct_t *)json_struct;	

		tj1->thread_flag = 1;

		strcpy(tj1->thread_status, "RUNNING\0"); 

		JSON_Value *rootValue;
		JSON_Object *rootObject;
		JSON_Object *threadObject;
		JSON_Array *threadArray;

		rootValue = json_parse_file("jparser.json");
		rootObject = json_value_get_object(rootValue);

		threadArray = json_object_get_array(rootObject, "thread");

		int threadNum, i, threadNameLen, hashFileNameLen;

		threadNum = (int)json_object_get_number(rootObject, "thread_num");

		for (i = 0 ; i < threadNum ; i++)
		{
				if (i == tj1->thread_num)
				{
						threadObject = json_array_get_object(threadArray, i);

						threadNameLen = strlen(json_object_get_string(threadObject, "name"));
						hashFileNameLen = strlen(json_object_get_string(threadObject, "hash_file"));

						tj1->thread_name = calloc(threadNameLen, sizeof(char));
						tj1->hash_file = calloc(hashFileNameLen, sizeof(char));

						strncpy(tj1->thread_name, json_object_get_string(threadObject, "name"), threadNameLen + 1);
						tj1->thread_option = (int)(json_object_get_number(threadObject, "thread_option"));
						strncpy(tj1->hash_file, json_object_get_string(threadObject, "hash_file"), hashFileNameLen + 1);
				}
		}

		json_value_free(rootValue);

		thread_routine(tj1);

		return NULL;
}

void * thread_routine(void * arg)
{
		json_struct_t * tj1 = (json_struct_t *)arg;	

		int i = 1;
		int num = (int)tj1->thread_name[strlen(tj1->thread_name)-1]-8;

		while(i <= tj1->thread_option){
				strcpy(tj1->thread_status, "SLEEPING\0"); 
				if(0) printf(" \033[%dm\t%s\033[0m running \033[31mtime : \033[0m%ds\n", num, tj1->thread_name, i);
				i+=1;
				sleep(1);
		}

		strcpy(tj1->thread_status, "RUNNING\0"); 
		sleep(1);
		whileFileRead(tj1);

		return NULL;
}

int whileFileRead(void * arg)
{
		json_struct_t * tj1 = (json_struct_t *)arg;	

		FILE *fp;
		char hash_buf[1024];
		char **hash;
		int hash_key, hash_index;

		linked_list_t *linked_list;	
		hash_table_t * hash_table;

		fp = fileOpen(tj1);

		strcpy(tj1->thread_status, "INIT\0"); 	
		hashTableArrClear(hash_table_arr + 0);
		strcpy(tj1->thread_status, "RUNNING\0"); 
		sleep(1);

		while (!feof(fp))
		{
				fgets(hash_buf, sizeof(hash_buf), fp);

				hash = hashFileRead(hash_buf);

				hash_buf[0] = '\0';

				if(!nullCheck(hash, "hash")) break;

				linked_list = linkedListAlloc(linked_list, hash);

				hash_key = hashKeyToAscii(hash[0]);
				hash_index = hashKeyToIndex(hash_key);	

				hash_table = hash_table_arr + hash_index;
				hash_table = hashTableIndex(hash_table, linked_list);

				if(!nullCheck(hash_table, "hash_table")) return 0;

				freeFunc(hash);
		}

		strcpy(tj1->thread_status, "DONE\0"); 
		sleep(1);
		strcpy(tj1->thread_status, "REST\0"); 
		sleep(1);
		fclose(fp);
		
		while (tj1->thread_flag) { };
		
		printf("%s  ", tj1->thread_name);
		printHashTable(hash_table_arr);

		return 0;
}

FILE * fileOpen(void * arg)
{
		json_struct_t * tj1 = (json_struct_t *)arg;	

		FILE *fp;

		fp = fopen(tj1->hash_file, "r");

		if (!fp)
		{
				printf("ERROR : file open\n");
				exit(0);			
		}

		return fp;
}

hash_table_t * hashTableArrClear(hash_table_t* hash_table_array)
{
		int i;
		for(i = 0 ; i < TBLSIZE ; i++)
		{
				(hash_table_array + i)->head = NULL;
				(hash_table_array + i)->tail = NULL;
				(hash_table_array + i)->list_entry = 0;
		}
		return hash_table_array;
}

char ** hashFileRead(char *hash_buf)
{
		char sep[] = ", \n";
		char *hash_key, *hash_data, *temp;
		char **hash; 

		temp = strtok(hash_buf, sep);

		if (!temp) return NULL;

		hash_key = calloc(strlen(temp) + 1, sizeof(char));

		strcpy(hash_key, temp);	

		if (!nullCheck(hash_key, "hash_key")) return NULL;

		temp = strtok(NULL, sep);

		hash_data = calloc(strlen(temp) + 1, sizeof(char));

		strcpy(hash_data, temp);	

		hash = calloc(strlen(hash_buf), sizeof(char *));

		hash[0] = hash_key;
		hash[0][strlen(hash_key)] = '\0';

		hash[1] = hash_data;
		hash[1][strlen(hash_data)] = '\0';

		return hash;
}

int nullCheck(void *string, char *name)
{
		if(!string)
		{
				if (strcmp(name, "hash")) printf("error! %s is empty\n", name);
				return 0;
		}
		return 1;
}

int hashKeyToAscii(char *hash_key)
{
		int i, hash_ascii = 0;	
		for (i = 0; i < (int)strlen(hash_key); i++)
		{
				hash_ascii += hash_key[i];		
		}

		return hash_ascii; 
}

int hashKeyToIndex(int hash_key)
{
		int index = 0;
		index = hash_key % TBLSIZE;

		return index; 
}

void * linkedListAlloc(linked_list_t *linked_list, char **hash)
{
		linked_list = malloc(sizeof(linked_list_t));

		linked_list->hash_key = calloc(strlen(hash[0]) + 1, sizeof(char));

		linked_list->data = calloc(strlen(hash[1]) + 1, sizeof(char));

		char *hash_key = linked_list->hash_key;
		char *data = linked_list->data;

		strncpy(hash_key, hash[0], strlen(hash[0]));
		strncpy(data, hash[1], strlen(hash[1]));

		linked_list->next = NULL;

		return linked_list;
}

void * hashTableIndex(hash_table_t * hash_table, linked_list_t * linked_list)
{
		hash_table->list_entry += 1;

		if (hash_table->list_entry == 1)
		{
				hash_table->head = linked_list; 
				hash_table->head->next = NULL;
				hash_table->tail = linked_list; 
				hash_table->tail->next = NULL;
		}
		else
		{
				hash_table->tail->next = linked_list;
				hash_table->tail = linked_list;
		}

		return hash_table;
}

void printHashTable(hash_table_t * hash_table)
{
		int i, j;
		linked_list_t * linked_list;
		char mark = '*';
		printf("\033[01m\033[4mHashTable\033[0m\n");
		for(i = 0 ; i < TBLSIZE ; i++)
		{

				if (!(hash_table + i)->head) mark = '#';
				else mark = '*';

				printf("%02d | %c |", i, mark); 

				if ((hash_table + i)->head)
				{
						linked_list = (hash_table + i)->head->next;
						if (!linked_list) mark = '#';
						printf(" -----> | %c |", mark);	
						printf("|\033[32m %d \033[0m|", hashKeyToAscii((hash_table + i)->head->hash_key));	
						printf("|\033[32m key : %s \033[0m|", (hash_table + i)->head->hash_key);	
						printf("|\033[31m data : %s \033[0m|", (hash_table + i)->head->data);	
						printf("\n");

						if (!linked_list)
						{
								printf("\t -----> ");	
								printf("|\033[31m NULL \033[0m");	
								printf("\n");
						}

						if ((hash_table + i)->head->next) 
						{
								for(j = 1 ; j < (hash_table + i)->list_entry ; j++)
								{
										if (j == (hash_table + i)->list_entry - 1) mark = '#';
										else mark = '*';
										printf("\t -----> | %c |", mark);	
										printf("|\033[32m %d \033[0m|", hashKeyToAscii(linked_list->hash_key));	
										printf("|\033[32m key : %s \033[0m|", linked_list->hash_key);	
										printf("|\033[31m data : %s \033[0m|", linked_list->data);	
										linked_list = linked_list->next;
										printf("\n");
								}
								if (!linked_list)
								{
										printf("\t -----> ");	
										printf("|\033[31m NULL \033[0m");	
										printf("\n");
								}
						}
				}

				printf("\n");
		}

		freeHash(hash_table_arr);		
}

void freeFunc(char **addr)
{
		if (addr[0])
		{
				free(addr[0]);
				addr[0] = '\0';
		}
		if (addr[1])
		{
				free(addr[1]);
				addr[1] = '\0';
		}
		free(addr);
}

void freeHash(hash_table_t * hash_table)
{
		int i, j;
		linked_list_t *linked_list;
		linked_list_t *temp;

		for(i = 0 ; i < TBLSIZE ; i++)
		{
				if (hash_table + i)
				{
						for(j = 1 ; j <= (hash_table+i)->list_entry ; j++)
						{
								if (j == 1)
								{
										linked_list = (hash_table + i)->head->next;
										freeList((hash_table +i)->head);
										freeAll((hash_table + i)->head);
										temp = linked_list;
								}	
								else
								{
										linked_list = temp;
										temp = linked_list->next;
										freeList(linked_list);
										freeAll(linked_list);	
								}
						}
				}
		}
}

void freeList(linked_list_t * addr)
{
		if(addr->hash_key)
		{
				freeAll(addr->hash_key);
		}
		if(addr->data)
		{
				freeAll(addr->data);
		}
}

void freeThreadJson(json_struct_t * addr, int threadNum)
{
		int i;
		for (i = 0 ; i < threadNum ; i++)
		{
				freeAll((addr[i]).thread_name);
				freeAll((addr[i]).hash_file);
		}
		freeAll(addr);
}

void freeAll(void * addr)
{
		if (addr) {
				free(addr);
				addr = NULL;
		}
}

