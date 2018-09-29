#include <stdio.h>
#include <pthread.h>

void *function_to_run(void *arg) {
printf("I'm the new thread!\n");
return NULL;
}

int main(int argc, char* argv[]) {
pthread_t mythread;
if (pthread_create(&mythread, NULL, function_to_run, NULL)) {
fprintf(stderr, "Error creating new thread.\n");
return -1;
}

printf("I'm the old thread!\n");
pthread_join(mythread, NULL);
return 0;


}


