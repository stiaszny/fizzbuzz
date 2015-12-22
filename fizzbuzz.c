#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

static void usage(const char *name)
{
fprintf(stderr, "Usage: %s <n>, where n is a non-negative integer"
          " less than 2^32.\n",
        name);
}

/* except for 2 and 3, any prime can be rewritten in the form 6k +- 1;
   thus we can check (6k +- 1) <= sqrt(n) and save ourselves some time
   over checking every integer <= sqrt(n).  Low-hanging fruit. */
static bool isPrime(uint32_t n)
{
    float limit;
    uint32_t k;
    bool isPrime = true;

    limit = sqrtf(n);

    if ( n == 1 )
        isPrime = false;
    if ( n % 2 == 0 )
        isPrime = false;
    if ( n % 3 == 0 )
        isPrime = false;

    for (k = 1; k <= limit/6 + 1; k++) {
        if (n % (6 * k - 1) == 0 && ( n / (6 * k - 1) != 1))
            isPrime = false;
        if (n % (6 * k + 1) == 0 && ( n / (6 * k + 1) != 1))
            isPrime = false;
    }

    if ( n == 2 || n == 3 )
        isPrime = true;

    return isPrime;
}

/* output tested by visual inspection against reference */
static uint32_t nextFib(bool reset)
{
    static int32_t prev = -1;
    static int32_t prevPrev = -1;
    uint32_t fib;

    if (prevPrev == -1) {
        fib = 1;
        prevPrev = fib;
    } else if ( prev == -1 ) {
        fib = 1;
        prev = fib;
    } else {
        fib = prev + prevPrev;
        prevPrev = prev;
        prev = fib;
    }

    if (reset == true) {
        prev = -1;
        prevPrev = -1;
    }

    return fib;
}

/* in production, I would use something like cpputest or other
   unit testing framework. For this excercise, visual inspection
   is fine. */
static void testIsPrime(int n)
{
    int i;
    int count = 0;

    printf("Testing isPrime() up to %d\n", n);
    for (i = 0; i < n; i++){
        if (isPrime(i)) {
            if (count++ % 10 == 0){
                printf("\n");
            }
            printf("%u ", i);
        }
    }
    printf("\n\n");
}

static void testNextFib(int n)
{
    int i;
    printf("Testing nextFib() up to %d...\n", n);

    for (i = 0; i < n; i++){
        printf("%u ", nextFib(false));
    }
    printf("\n\n");
}

/* Sieve of Eratosthenes is great if you have lots of memory and n is
   small-ish.  However, if this is done with an embedded mindset,
   memory is probably very tight, so I will use a simple primality
   test instead. */
int main(int argc, char **argv)
{
    uint32_t i;
    uint32_t n;
    bool printed = false;

    if (argc != 2) {
        usage(argv[0]);
        exit(1);
    }

    /* could make error handling on bad arguement better, but I have a
     plane to catch! */
    n = atol(argv[1]);
    if (n < 0) {
        usage(argv[0]);
        exit(2);
    }

    testIsPrime(200);
    testNextFib(n);
    nextFib(true);              /* reset the sequence after the test! */

    for (i = 0; i < n; i++) {
        uint32_t fib;
        fib = nextFib(false);
        if ( fib % 3 == 0 ) {
            printf("Buzz");
            printed = true;
        }
        if ( fib % 5 == 0 ) {
            printf("Fizz");
            printed = true;
        }
        if ( isPrime(fib) ) {
            printf("BuzzFizz");
            printed = true;
        }
        /* spec says "otherwise", so only print values that haven't
         been fizzbuzzed in some way */
        if (!printed) {
            printf("%u ", fib);
        }
        printf("\n");
        printed = false;
    }
    exit(0);
}
