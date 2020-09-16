// Filipi de Luca Valim dos Santos 
// RA 22216027-7
// codigo retirado do link: reference: https://c-program-example.com/2012/02/c-program-to-solve-dining-philosophers-problem.html
#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include <sys/time.h> 
 
#define N 5 // qtd de filosofos
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define LEFT (ph_num+4)%N
#define RIGHT (ph_num+1)%N // para adicioanar mais um garfo
 
sem_t mutex;
sem_t S[N];
 
void * philospher(void *num);
void take_fork(int);
void put_fork(int);
void test(int);
 
int state[N];
int phil_num[N]={0,1,2,3,4}; // alterar a qtd de filosofos
 
 
typedef unsigned long long timestamp_t;

    static timestamp_t
    get_timestamp ()
    {
      struct timeval now;
      gettimeofday (&now, NULL);
      return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
    }
 
 
int main()
{
    int i;
    pthread_t thread_id[N];
    sem_init(&mutex,0,1);
    for(i=0;i<N;i++)
        sem_init(&S[i],0,0);
    for(i=0;i<N;i++)
    {
        pthread_create(&thread_id[i],NULL,philospher,&phil_num[i]);
        printf("\n");
        printf("Philosopher %d is thinkingn",i+1);
    }
    for(i=0;i<N;i++)
        pthread_join(thread_id[i],NULL);
}
 
void *philospher(void *num)
{
    while(1)
    {
        int *i = num;
        sleep(1);
        take_fork(*i);
        sleep(0);
        put_fork(*i); // para dar deadlock
    }
}
 
void take_fork(int ph_num)
{
    sem_wait(&mutex);
    state[ph_num] = HUNGRY;
    printf("\n");
    printf("Philosopher %d is Hungryn",ph_num+1);
    test(ph_num);
    sem_post(&mutex);
    sem_wait(&S[ph_num]);
    sleep(1);
}
 
void test(int ph_num)
{

    if (state[ph_num] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
    {
    
        state[ph_num] = EATING;
        sleep(2);
        printf("\n");
        printf("Philosopher %d takes fork %d and %d",ph_num+1,LEFT+1,ph_num+1);
        printf("\n");
        printf("Philosopher %d is Eatingn",ph_num+1);
        sem_post(&S[ph_num]);
   
    }

	

}
 
void put_fork(int ph_num)
{
    sem_wait(&mutex);
    state[ph_num] = THINKING;
    printf("Philosopher %d putting fork %d and %d downn",ph_num+1,LEFT+1,ph_num+1);
    printf("\n");
    printf("Philosopher %d is thinkingn",ph_num+1);
    printf("\n");
    test(LEFT);
    test(RIGHT);
    sem_post(&mutex);
}
