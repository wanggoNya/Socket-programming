#include "hashTable.h"

int main(void) 
{
		int threadNum;
		json_struct_t * tjArray;

		threadNum = jsonReadThreadNum();
		tjArray = calloc(threadNum, sizeof(json_struct_t));
		pthread_t * thread = (pthread_t *)malloc(threadNum * sizeof(pthread_t));

		int i, pid, status;

		for (i = 0 ; i < threadNum ; i++)
		{
				tjArray[i].thread_num = i;	
				pid = pthread_create(&(thread[i]), NULL, jsonReadAlloc, (void *)&(tjArray[i]));
				pidCheck(pid);
				strcpy(tjArray[i].thread_status, "INIT\0");
		}

		int num, cnt = 0, j = 1;
		char * thread_status;

		printf("Building .... \n");
		sleep(1);

		while(j)
		{ 
				for(i = 0 ; i < threadNum ; i++ )
				{
						num = (int)(tjArray[i].thread_name[strlen(tjArray[i].thread_name) - 1]) - 8;
						printf(" \033[%dm\t%s\033[0m \033[31m%s\033[0m\n", 
										num, tjArray[i].thread_name, tjArray[i].thread_status);

						thread_status = tjArray[i].thread_status;
						if (!strcmp(thread_status, "DONE") || !strcmp(tjArray[i].thread_status, "REST"))
						{
								cnt ++; 
						}
						else cnt = 0;
				}

				if (cnt == threadNum)
				{
						j = 0;
				}
				printf("\n");
				cnt = 0;
				sleep(1);
		}

		printf("끝났다..........\n");
		sleep(2);

		for(i = 0 ; i < threadNum ; i++ )
		{
				tjArray[i].thread_flag = 0;
		}

		for (i = 0 ; i < threadNum ; i++)
		{
				pthread_join(thread[i], (void **)&status);
		}

		free(thread);
		freeThreadJson(tjArray, threadNum);

		printf("수고하셨습니다...........\n");
		return 0;
}
