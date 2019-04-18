#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <stdlib.h>
#include "timer.h"

#define Nthreads 10

pthread_mutex_t mutex;
long long int N;
long long int mainNumber = 1;
long long int concPrimesQnt = 0, seqPrimeQnt = 0;
double concTime, seqTime;
int threadQnt;

int ehPrimo(long long int n)
{
    int i;
    if (n <= 1)
        return 0;
    if (n == 2)
        return 1;
    if (n % 2 == 0)
        return 0;
    for (i = 3; i < sqrt(n) + 1; i += 2)
        if (n % i == 0)
            return 0;
    return 1;
}

void sequentialVersion(void)
{
    double start, end;
    long long int i;

    GET_TIME(start);

    for (i = 1; i <= N; i++)
        if (ehPrimo(i) == 1)
            seqPrimeQnt++;

    GET_TIME(end);

    seqTime = end - start;

    printf("sequential version time: %fs\n", seqTime);
    printf("sequential version primes quantity: %lld\n", seqPrimeQnt);
}

void *incrementIfIsPrime(void *pointer)
{
    long long int temp;

    pthread_mutex_lock(&mutex);
    temp = mainNumber++;
    pthread_mutex_unlock(&mutex);

    while (temp <= N)
    {
        if (ehPrimo(temp) == 1)
        {
            pthread_mutex_lock(&mutex);
            concPrimesQnt++;
            pthread_mutex_unlock(&mutex);
        }

        pthread_mutex_lock(&mutex);
        temp = mainNumber++;
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

void concurrentVersion(void)
{

    double start, end;
    int i;

    pthread_t threadId[threadQnt];

    GET_TIME(start);

    pthread_mutex_init(&mutex, NULL);

    for (i = 0; i < threadQnt; i++)
    {
        if (pthread_create(&threadId[i], NULL, incrementIfIsPrime, NULL))
        {
            printf("Error at create thread function\n");
            exit(-1);
        }
    }

    for (int t = 0; t < threadQnt; t++)
    {
        if (pthread_join(threadId[t], NULL))
        {
            printf("--ERRO: pthread_join() \n");
            exit(-1);
        }
    }

    pthread_mutex_destroy(&mutex);

    GET_TIME(end);

    concTime = end - start;

    printf("concurrent version time = %lf\n", concTime);
    printf("concurrent version primes quantity: %lld\n", concPrimesQnt);
}

int main()
{
    double dif, perfomance;

    printf("Enter N: ");
    scanf("%lld", &N);

    printf("Enter threadQnt: ");
    scanf("%d", &threadQnt);

    sequentialVersion();
    concurrentVersion();

    if (concTime < seqTime)
    {
        dif = seqTime - concTime;
        perfomance = 100 * (dif / seqTime);
        printf("performance gain: %lf%%\n", perfomance);
    }
    else
    {
        dif = concTime - seqTime;
        perfomance = 100 * (dif / seqTime);
        printf("loss of performance: %lf%%\n", perfomance);
    }
}