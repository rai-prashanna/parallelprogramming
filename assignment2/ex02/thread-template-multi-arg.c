#include <stdio.h>
#include <pthread.h>

void *function_to_run(void *arg) {
int n = *(int *) arg;
printf("I'm the new thread! I got %d passed to me.\n", n);
return NULL;
}

int main() {
pthread_t mythread;
int n = 5;
if (pthread_create(&mythread, NULL, function_to_run, &n)) {
fprintf(stderr, "Error creating new thread.\n");
return -1;
}
printf("I'm the old thread!\n");
pthread_join(mythread, NULL);
return 0;
}

