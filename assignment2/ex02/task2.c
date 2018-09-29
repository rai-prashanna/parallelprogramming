#include <stdio.h>

/* Returns the index of the next prime (non-zero, since we "knock out" non-primes by setting their value to zero) */
int getNextPrime(int i, int x[]) {
  while (x[i] == 0) {
    i++;
  }
  return i;
}

int main(int argc, char* argv[]) {
  int* primes;
  int i, j, NPRIMES;

  NPRIMES = 24;
  primes = (int*)malloc(NPRIMES * sizeof(int));

  /* we start at 2 because it is the smallest prime */
  /*list all numbers from 2 to max */
  for(i=2; i < NPRIMES; i++) {
    primes[i] = i;
    printf("%d\n", i);
  }

/*make iterations from 2 to NPRIMES and update counter with next prime number*/
  for(i=2; i < NPRIMES; i = getNextPrime(i+1, primes)) {
   /*find multiple i.e j=i*2 and j=j+i and marked that number by setting array[index]=0 */
    for(j = (i*2); j < NPRIMES; j += i) {
      printf("i: %d, j: %d\n", i, j);
      primes[j] = 0;

    }

 }

  for(i=2; i < NPRIMES; i++) {
    if ( primes[i] != 0 ) {
      printf("Prime number: %d\n", primes[i]);
    }
  }

  return 0;
}
