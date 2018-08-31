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

#define ECLIENT1 "error_client_1.log"

extern pthread_mutex_t mutex1;
extern char *myfifo_1;

void* client_1 (void)
{
        
	FILE *er=NULL;
	FILE *fp=NULL;
	int mkfi_ret;
	int fd_1;
	int cl_ret_1, seq_ret_1, rand_ret_1;
	int pid_1=0;
	int client_1_id= 1234;
	unsigned int seed_1 ;
	int randomNumber_1=0;
	int sequenceNumber=-1;
		
	while(1)
	{	

		seed_1 = time(NULL);
		seed_1=seed_1 % 9999999;
		randomNumber_1=rand_r(&seed_1);
		printf("waiting for readers...\n");
		printf("got a daemon\n");
		pid_1=getpid();
		printf("clientId is:%d \n",client_1_id);
		printf("ProcessID is %d \n",pid_1);				
		fd_1 = open(myfifo_1, O_WRONLY);
		if( fd_1>0 )
		{

			pthread_mutex_lock( &mutex1 );	
			sequenceNumber=sequenceNumber+1;
			pthread_mutex_unlock( &mutex1 );
			
			cl_ret_1 = write(fd_1,&client_1_id,sizeof(client_1_id));              
			seq_ret_1 = write(fd_1,&sequenceNumber,sizeof(sequenceNumber));   
			rand_ret_1 = write(fd_1,&randomNumber_1,sizeof(randomNumber_1));       					       
			

			if((cl_ret_1 == -1) && (seq_ret_1 == -1) && (rand_ret_1 == -1))
			{
				printf("!Not able to write client_id, sequence number, random number values into fifo pipe\n");
				close(fd_1);
				unlink(myfifo_1);
				return 0;
			} 
			close(fd_1);
		}

		else

		{	er=fopen(ECLIENT1,"a+");
			fprintf(er,"%s\n","Can't open the fifo pipe");
			fclose(er);	
			return 0;
		}
	fflush(stdout);		 
	sleep(5);			
	}		


}
