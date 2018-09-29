#include <stdio.h>
#include <pthread.h>

int x = 10;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
void *func1(void *arg)
{
    int *n = arg;
    pthread_mutex_lock(&lock);
    (*n)++;
    pthread_mutex_unlock(&lock);
    return NULL;
}
void *func2(void *arg)
{
    int *n = arg;
    pthread_mutex_lock(&lock);
    (*n)--;
    pthread_mutex_unlock(&lock);
    return NULL;
}


int main()
{
    pthread_t thread1, thread2;
    if (pthread_create(&thread1, NULL, func1, &x))
    {
        fprintf(stderr, "Error creating thread1.\n");
        return -1;
    }
    if (pthread_create(&thread2, NULL, func2, &x))
    {
        fprintf(stderr, "Error creating thread2.\n");
        return -1;
    }
    pthread_join(thread1, NULL);
    printf("x = %d\n", x);
    return 0;
}

