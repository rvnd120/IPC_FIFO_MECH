#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

// This API functions to establish an IPC communication with daemon process through// 
// FIFO mechanism. In particular it writes fifo buffer with clientID, SequenceNUmber, Random NUmber//
// which is being fetched by the daemon process running in background which creates a log file to //
// to display the timestamp//

#define ECLIENT2 "error_client_2.log"
extern pthread_mutex_t mutex1;
extern char *myfifo_2;

void* client_2 (void)
{

	FILE *er=NULL;
	FILE *fp=NULL;
   
        
	int mkfi_ret;
	int fd_2;
	int cl_ret_2, seq_ret_2, rand_ret_2;
	int pid_2=0;
	int client_2_id= 5678;
	unsigned int seed_2 ;
	int randomNumber_2=0;
	int sequenceNumber=-1;
	//Random Number Creation//
	//      pthread_mutex_lock( &mutex2);
	int fret;


	while(1)
	{
		seed_2 = time(NULL);
		seed_2=seed_2 % 9999999;
		randomNumber_2=rand_r(&seed_2);
		printf("waiting for readers...\n");
		printf("got a daemon\n");
		pid_2=getpid();
		printf("clientId is:%d \n",client_2_id);
		printf("ProcessID is %d \n",pid_2);
		fd_2 = open(myfifo_2, O_WRONLY);
		if( fd_2>0 )
		{


			

			pthread_mutex_lock( &mutex1);
			sequenceNumber=sequenceNumber+1;
			pthread_mutex_unlock( &mutex1);
			
			cl_ret_2 = write(fd_2,&client_2_id,sizeof(client_2_id));
			seq_ret_2 = write(fd_2,&sequenceNumber,sizeof(sequenceNumber));
			rand_ret_2 = write(fd_2,&randomNumber_2,sizeof(randomNumber_2));
			

			if((cl_ret_2 == -1) && (seq_ret_2 == -1) && (rand_ret_2 == -1))
			{
				printf("!Not able to write client_id, sequence number, random number values into fifo pipe\n");
				close(fd_2);
				unlink(myfifo_2);
				return 0;
			}
			close(fd_2);
		}

		else

		{       er=fopen(ECLIENT2,"a+");
			fprintf(er,"%s\n","Can't open the fifo pipe");
			fclose(er);	
			return 0;
		}
		fflush(stdout);
		sleep(7);
	}





}

