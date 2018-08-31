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

#define ECLIENT3 "error_client_3.log"
extern pthread_mutex_t mutex1;
extern char *myfifo_3;

void* client_3 (void)
{

	FILE *er=NULL;
	FILE *fp=NULL;
	int mkfi_ret;
	int fd_3;
	int cl_ret_3, seq_ret_3, rand_ret_3;
	int pid_3=0;
	int client_3_id= 9012;
	unsigned int seed_3 ;
	int randomNumber_3=0;
	int sequenceNumber=-1;
	//Random Number Creation//
	//      pthread_mutex_lock( &mutex2);
	int fret;

	while(1)
	{
		seed_3 = time(NULL);
		seed_3=seed_3 % 9999999;
		randomNumber_3=rand_r(&seed_3);
		printf("waiting for readers...\n");
		printf("got a daemon\n");
		pid_3=getpid();
		printf("clientId is:%d \n",client_3_id);
		printf("ProcessID is %d \n",pid_3);
		fd_3 = open(myfifo_3, O_WRONLY);
		if( fd_3>0 )
		{


	

			pthread_mutex_lock( &mutex1 );
			sequenceNumber=sequenceNumber+1;
			pthread_mutex_unlock( &mutex1);
			cl_ret_3 = write(fd_3,&client_3_id,sizeof(client_3_id));
			seq_ret_3 = write(fd_3,&sequenceNumber,sizeof(sequenceNumber));
			rand_ret_3 = write(fd_3,&randomNumber_3,sizeof(randomNumber_3));
			

			if((cl_ret_3 == -1) && (seq_ret_3 == -1) && (rand_ret_3 == -1))
			{
				printf("!Not able to write client_id, sequence number, random number values into fifo pipe\n");
				close(fd_3);
				unlink(myfifo_3);
				return 0;
			}
			close(fd_3);
		}

		else

		{       er=fopen(ECLIENT3,"a+");
			fprintf(er,"%s\n","Can't open the fifo pipe");
			fclose(er);	
			return 0;
		}
		fflush(stdout);
		sleep(10);
	}




}

