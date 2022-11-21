#include "header.h"

void jobArgs(jobstruct *job, int k, char *ch, char *command) {
    if (job != NULL) {
        if (k != 0) {           
            int str_cmd = strlen("command");
            int str_start = strlen("starttime");
            int str_stop = strlen("endtime");
            int index = 0;
            int len_cmd = strlen(job[index].job_cmd);
            int len_status = strlen(job[index].job_status);
            int len_date = strlen(timeDateStr(NULL));            
            index++;

            while (index < k) {
                int next_len_cmd = strlen(job[index].job_cmd);
                int next_len_status = strlen(job[index].job_status);                            
                if (len_cmd < next_len_cmd)
                    len_cmd = next_len_cmd;
                if (len_status < next_len_status)
                    len_status = next_len_status;       
                index++;                               
            }

            if (strcmp(command, "showjobs") == 0) {
                printf("jobid\tcommand%*s\tstatus\n", len_cmd-str_cmd, "");                                
                int i=0;
                while(i < k) {
                    if (strcmp(job[i].job_status, "success") != 0) {
                        int space = len_cmd-strlen(job[i].job_cmd);
                        printf("%d\t%s%*s\t%s\n",job[i].job_id+1,job[i].job_cmd, space, "", job[i].job_status);
                    }
                    i++;
                }
            } else if (strcmp(command, "submithistory") == 0) {                
                printf("jobid\tcommand%*s\tstarttime%*s\tendtime%*s\tstatus\n", len_cmd-str_cmd, "", len_date-str_start, "", len_date-str_stop, "");
                int i = 0;
                while(i < k) {
                    if (strcmp(job[i].job_status, "success") == 0) {
                        int space = len_cmd-strlen(job[i].job_cmd);
                        printf("%d\t%s%*s\t%s\t%s\t%s\n",job[i].job_id+1,job[i].job_cmd, space, "", job[i].start_time, job[i].end_time, job[i].job_status);
                    }
                    i++;
                }
            }
        }
    }
}

int log_descriptor(char *filename, int file_descriptor) {
    file_descriptor = open(filename, O_CREAT | O_APPEND | O_WRONLY, 755);
    
    if (file_descriptor == -1) {
        fprintf(stderr, "Error: faile to open the file. Check permissions \"%s\"\n", filename);
        perror("open");
        exit(1);
    }

    return file_descriptor;
}

void *done_status(void *arg) {
    pid_t pid;   
    char **args; 
    jobstruct *jop_pointer;

    jop_pointer = (jobstruct *)arg;

    running++;
    jop_pointer->job_status = "running";
    jop_pointer->start_time = timeDateStr(NULL);

    pid = fork();
    if (pid == 0) {
        dup2(log_descriptor(jop_pointer->fdout,0), 1); 
        dup2(log_descriptor(jop_pointer->fderr,0), 2); 
        args = arguments(jop_pointer->job_cmd);
        execvp(args[0], args); 
        fprintf(stderr, "Error: faile to execute the command: \"%s\"\n", args[0]);
        perror("execvp");
        exit(1);
    } else if (pid > 0) {
        waitpid(pid, &jop_pointer->exit_status, WUNTRACED);        
        jop_pointer->job_status = "success";        
        jop_pointer->end_time = timeDateStr(NULL);

        if (!WIFEXITED(jop_pointer->exit_status)) 
            fprintf(stderr, "Child process %d did not terminate normally!\n", pid);
    } else {
        fprintf(stderr, "Error: faile to fork process.\n");
        perror("fork");
        exit(1);
    }

    running--;

    return NULL;
}

void *success_status(void *args) {
    jobstruct *jop_pointer;
    running = 0;
    while (true) {
        if (qWaiting->count > 0) {
            if (jobs > running) {
                jop_pointer = qDelete(qWaiting);
                pthread_create(&jop_pointer->thread_id, NULL, done_status, jop_pointer);
                pthread_detach(jop_pointer->thread_id);
            }
        }
        sleep(1); 
    }

    return NULL;
}

int main(int argc, char **argv) {
    printf("To quit: Press 'CTRL+\\'\n");
    int i = 0;              
    char inputLine[1000]; 
    char *n, *c=NULL;           
    char *job_cmd;

    char *fderr;  
    pthread_t thread_id; 

    if (argc != 2) {
        printf("Usage: %s <number>\n", argv[0]);
        exit(0);
    }

    jobs = atoi(argv[1]);
    jobs < 1?jobs = 1:jobs;
    jobs > 8?jobs = 8:jobs;

    fderr = malloc(sizeof(char) * (strlen(argv[0]) + 7));
    sprintf(fderr, "%s.err", argv[0]);
    dup2(log_descriptor(fderr,0), 2);

    qWaiting = qInit(100);

    pthread_create(&thread_id, NULL, success_status, NULL);

    while (printf("Enter command> ") && inputs(1000, inputLine, 0) != -1) {
        if ((n = strtok(cpyInputStr(inputLine, -1), " \x0b\x0c\t\n\r")) != NULL) {
            if (strcmp(n, "submit") == 0) {
                if (i >= 1000) {
                    printf("Too much jobs..\n");
                } else if (qWaiting->count >= qWaiting->size) {
                    printf("Max queues..\n");
                } else {
                    job_cmd = charPointer1((strstr(inputLine, "submit") + 6), 0);                    
                    jobArr[i] = my_job(job_cmd, i);
                    qInsert(qWaiting, jobArr + i);
                    printf("job %d added to the queue\n", (i++)+1);
                }
            } else if (strcmp(n, "showjobs") == 0 || strcmp(n, "submithistory") == 0) {
                jobArgs(jobArr, i, c, n);            
            }
        }
    }

    kill(0, 2); 

    exit(0);
}

