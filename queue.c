#include "header.h"

jobstruct my_job(char *job_cmd, int job_id) {
    jobstruct myJobs;
    myJobs.job_id = job_id;
    myJobs.exit_status = -1;
    myJobs.job_cmd = cpyInputStr(job_cmd, -1);
    myJobs.start_time = myJobs.end_time = NULL;
    myJobs.job_status = "waiting";
    sprintf(myJobs.fdout, "%d.out", myJobs.job_id);
    sprintf(myJobs.fderr, "%d.err", myJobs.job_id);

    return myJobs;
}

queue *qInit(int size) {
    queue *q = malloc(sizeof(queue));
    q->buffer = malloc(sizeof(jobstruct *) * size);
    q->size = size;
    q->end = 0;
    q->start = 0;
    q->count = 0;

    return q;
}

int qInsert(queue *q, jobstruct *job_pointer) {
    if ((q == NULL) || (q->count == q->size))
        return -1;

    q->buffer[q->end % q->size] = job_pointer;
    q->end = (q->end + 1) % q->size;
    q->count++;

    return q->count;
}

jobstruct *qDelete(queue *q) {
    if ((q == NULL) || (q->count == 0))
        return (jobstruct *)-1;

    jobstruct *myJobs = q->buffer[q->start];
    q->start = (q->start + 1) % q->size;
    q->count--;

    return myJobs;
}

void q_history(queue *q) {
    free(q->buffer);
    free(q);
}

