#ifndef HEADER_H_
#define HEADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

typedef struct jobstruct
{
    char *job_cmd;
    char *job_status;   
    char *start_time; 
    char *end_time;  
    char fdout[25]; 
    char fderr[25];
    int job_id; 
    int exit_status;       
    pthread_t thread_id;
} jobstruct;

typedef struct queue
{
    jobstruct **buffer; 
    int size;    
    int start;  
    int end;    
    int count;  
} queue;

queue *qWaiting;
queue *qInit(int k);
jobstruct jobArr[5000]; 
jobstruct my_job(char *job_cmd, int job_id);
jobstruct *qDelete(queue *q);

void jobArgs(jobstruct *job, int k, char *ch, char *command);
void qDestroy(queue *q);

bool nullChar(char chr);
char *cpyInputStr(char *str, int i);
char *charPointer1(char *str, int i);
char *cpyNewStr(char *str, int i);
char *timeDateStr(char *nsc);
char **arguments(char *inputLine);
int qInsert(queue *q, jobstruct *job_pointer);
int inputs(int k, char *str, int w);
int log_descriptor(char *filename, int file_descriptor);
int jobs;      
int running;    

#endif