#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "client.c"
#include "client2.c"
#include "client3.c"

char *myfifo_1 = "/tmp/myfifo_1";
char *myfifo_2 = "/tmp/myfifo_2";
char *myfifo_3 = "/tmp/myfifo_3";

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

int main(void)
{
	printf("From functional unit test\n");

	int mkfi_ret_1;
	int mkfi_ret_2;
	int mkfi_ret_3;
	
	pthread_t thread1, thread2, thread3;
        int iret1, iret2, iret3;

	mkfi_ret_1 = mkfifo(myfifo_1, 0666);
	mkfi_ret_2 = mkfifo(myfifo_2, 0666);
	mkfi_ret_3 = mkfifo(myfifo_3, 0666);	
	 
	printf("%d\n",mkfi_ret_1);
	printf("%d\n",mkfi_ret_2);
	printf("%d\n",mkfi_ret_3);

	if((mkfi_ret_1 == 0) && (mkfi_ret_2 == 0) && (mkfi_ret_3 == 0))

	{

	
		iret1 = pthread_create( &thread1, NULL, (void *)client_1, NULL);
		if(iret1)
		{
			fprintf(stderr, "Error in creating thread %d\n",iret1);
			exit(EXIT_FAILURE);
		}	   

		iret2 = pthread_create( &thread2, NULL, (void *)client_2, NULL);
		if(iret2)
		{
			fprintf(stderr, "Error in creating thread %d\n",iret2);
			exit(EXIT_FAILURE);
		}

		iret3 = pthread_create( &thread3, NULL, (void *)client_3, NULL);
		if(iret3)
		{
			fprintf(stderr, "Error in creating thread %d\n",iret3);
			exit(EXIT_FAILURE);
		}

	}

	else

	{
		exit(EXIT_FAILURE);
	}		


	pthread_join( thread1, NULL);
	pthread_join( thread2, NULL);
	pthread_join( thread3, NULL);  

	exit(EXIT_SUCCESS);

}
