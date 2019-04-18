/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 5 */
/* Codigo: Threads escrevem e leem uma variavel global: exemlo de programa com condicao de corrida */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 3

int x = 0; //variavel compartilhada entre as threads

//funcao executada pela thread 1
void *t1(void *threadid)
{
  int tid = *(int *)threadid;
  printf("Thread : %d esta executando...\n", tid);
  x = 1;
  printf("Thread : %d terminou!\n", tid);
  free(threadid);
  pthread_exit(NULL);
}

//funcao executada pela thread 2
void *t2(void *threadid)
{
  int tid = *(int *)threadid;
  printf("Thread : %d esta executando...\n", tid);
  x = 2;
  printf("Thread : %d terminou!\n", (int)tid);
  free(threadid);
  pthread_exit(NULL);
}

//funcao executada pela thread 3
void *t3(void *threadid)
{
  int y;
  int tid = *(int *)threadid;
  printf("Thread : %d esta executando...\n", tid);
  y = x;
  printf("Thread : %d terminou!\n", tid);
  printf("Valor de y = %d\n", y);
  free(threadid);
  pthread_exit(NULL);
}

void create_thread(int *t, int t_value, pthread_t *thread, void *t_func(void *threadid))
{
  t = malloc(sizeof(int));
  if (t == NULL)
  {
    printf("Couldnt create malloc\n");
    exit(-1);
  }
  *t = t_value;
  if (pthread_create(thread, NULL, t_func, (void *)t))
  {
    printf("error at pthread_create\n");
    exit(-1);
  }
}

//thread principal
int main(int argc, char *argv[])
{
  pthread_t tid[NTHREADS];
  int *t;

  create_thread(t, 3, &tid[2], t3);

  create_thread(t, 2, &tid[1], t2);

  create_thread(t, 1, &tid[0], t1);

  //cria as 3 threads
  // t = malloc(sizeof(int));
  // if (t == NULL)
  //   return -1;
  // *t = 1;
  // if (pthread_create(&tid[0], NULL, t1, (void *)t))
  // {
  //   printf("--ERRO: pthread_create()\n");
  //   exit(-1);
  // }

  // t = malloc(sizeof(int));
  // if (t == NULL)
  //   return -1;
  // *t = 2;
  // if (pthread_create(&tid[1], NULL, t2, (void *)t))
  // {
  //   printf("--ERRO: pthread_create()\n");
  //   exit(-1);
  // }

  // t = malloc(sizeof(int));
  // if (t == NULL)
  //   return -1;
  // *t = 3;
  // if (pthread_create(&tid[2], NULL, t3, (void *)t))
  // {
  //   printf("--ERRO: pthread_create()\n");
  //   exit(-1);
  // }

  pthread_exit(NULL);
  return 0;
}
