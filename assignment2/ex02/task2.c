#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

/* Returns the index of the next prime (non-zero, since we "knock out" non-primes by setting their value to zero) */
int getNextPrime(int i, int x[]) {
  while (x[i] == 0) {
    i++;
  }
  return i;
}

void displayPrimeNumbers(int primes[],int NPRIMES)
{
  for(int i=2; i < NPRIMES; i++) {
    if ( primes[i] != 0 ) {
      printf("Prime number: %d\n", primes[i]);
    }
  }
}

int *init(int primes[],int NPRIMES)
{
  /* we start at 2 because it is the smallest prime */
  /*list all numbers from 2 to max */
  for(int i=2; i < NPRIMES; i++) {
    primes[i] = i;
    printf("%d\n", i);
  }
return primes;
}

int *filterPrimes(int startingindex,int primes[],int maxlimit){
/*make iterations from 2 to NPRIMES and update counter with next prime number*/
  for(int i=startingindex; i < maxlimit; i = getNextPrime(i+1, primes)) {
   /*find multiple i.e j=i*2 and j=j+i and marked that number by setting array[index]=0 */
    for(int j = (i*2); j < maxlimit; j += i) {
      printf("i: %d, j: %d\n", i, j);
      primes[j] = 0;

    }

 }
return primes;
}

int main(int argc, char* argv[]) {
  int* primes;
  int NPRIMES;

  NPRIMES = 24;
  primes = (int*)malloc(NPRIMES * sizeof(int));
unsigned int sqrt_num = (int) ceil(sqrt((double) NPRIMES));

primes=init(primes,NPRIMES);
primes=filterPrimes(2,primes,sqrt_num);
displayPrimeNumbers(primes,NPRIMES);


  return 0;
}
