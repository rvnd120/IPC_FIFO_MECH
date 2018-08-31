#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <fcntl.h>
#include <pthread.h>

#define COUNT 3
#define SFILE "service.log"
#define LFILE "IPC_file.log"

pthread_t thread[COUNT];
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER; 

char *myfifo_1 = "/tmp/myfifo_1";
char *myfifo_2 = "/tmp/myfifo_2";
char *myfifo_3 = "/tmp/myfifo_3";
char **cp[COUNT]={&(myfifo_1), &(myfifo_2), &(myfifo_3)};


static void daemon2()
{
	pid_t pid;



	pid = fork();              //child process creation//


	if (pid < 0)
		exit(EXIT_FAILURE);


	if (pid > 0)              //Terminating parent process//
		exit(EXIT_SUCCESS);


	if (setsid() < 0)         //child process becoming the session leader//
		exit(EXIT_FAILURE);

	pid = fork();


	if (pid < 0)
		exit(EXIT_FAILURE);


	if (pid > 0)
		exit(EXIT_SUCCESS);


	umask(0);


	//chdir("/");             //changing to root directory//


}

void* fifo_mess(void* ptr)
{
	char *fifo_fi;
	fifo_fi = (char *) ptr;
	FILE *fp=NULL;
	int fd,cl_ret,seq_ret,rand_ret;
	int client_id;
	int sequenceNumber;
	int randomNumber;
	while(1)
	{

		fd = open(fifo_fi, O_RDONLY);
		if(fd > 0)

		{
			cl_ret = read(fd, &client_id, sizeof(client_id));
			seq_ret = read(fd, &sequenceNumber, sizeof(sequenceNumber));
			rand_ret = read(fd, &randomNumber, sizeof(randomNumber));

			if((cl_ret != -1) && (seq_ret != -1) && (rand_ret != -1))
			{
				
				fp=fopen(LFILE,"a+");
				if(fp != NULL)
				{	
					fprintf(fp,"<timestamp>: Received IPC request # %d from client '%d' w/ random number '%d'\n",sequenceNumber,client_id,randomNumber);
					fclose(fp);
				
			}
				else
				{
					
				}
			}
			else
			{	
				fp=fopen(LFILE,"a+");
				fprintf(fp,"can't read fifo file %d\n,%d\n,%d\n",cl_ret,seq_ret,rand_ret);
				fclose(fp);
				close(fd);
				unlink(fifo_fi);
				break;
			}
			close(fd);
			   
		}
		else
		{
			
			fp=fopen(LFILE,"a+");
			fprintf(fp,"%s\n","No my fifo");
			fclose(fp);
		

		}

		sleep(7);
	}

	
}
void create(void)
{
	int count;
	int iret[COUNT];
	for(count=0;count<COUNT; count++)
	{
		iret[count]=pthread_create(&thread[count], NULL, fifo_mess, (void *)(*(cp[count])));
		if(iret[count])
		{
			fprintf(stderr, "Error in creating thread %d and ret value is %d\n",count,iret[count]);
			exit(EXIT_FAILURE);
		}
	}
}	

int main(void)
{       
	daemon2();
	create();
	FILE *fp1;
	while (1)
	{

		fp1=fopen(SFILE,"a+");
		fprintf(fp1,"%s\n","service working");
		fflush(fp1);
		fclose(fp1);
		sleep(1);

	}

	pthread_join(thread[0],NULL);
	pthread_join(thread[1],NULL);
	pthread_join(thread[2],NULL);

	return 0;

}
