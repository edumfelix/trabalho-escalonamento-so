#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PHILOS 5
#define DELAY 5000
#define FOOD 50

void *filosofo(void *id);
void pegar_garfo(int, int, char *);
void devolver_garfos(int, int);
int comida_na_mesa();

sem_t garfo[PHILOS];
pthread_t filosofos[PHILOS];
sem_t trava_comida;
int segundos_de_sono = 0;

int main(int argc, char **argv) {
  int i;
  if (argc == 2)
    segundos_de_sono = atoi(argv[1]);

  sem_init(&trava_comida, 0, 1);

  for (i = 0; i < PHILOS; i++)
    sem_init(&garfo[i], 0, 1);

  for (i = 0; i < PHILOS; i++)
    pthread_create(&filosofos[i], NULL, filosofo, (void *)i);

  for (i = 0; i < PHILOS; i++)
    pthread_join(filosofos[i], NULL);

  return 0;
}

void *filosofo(void *num) {
  int id;
  int i, garfo_esquerdo, garfo_direito, f;
  id = num;

  printf("Filósofo %d terminou de pensar e está pronto para comer.\n", id);

  garfo_direito = id;
  garfo_esquerdo = (id + 1) % PHILOS;

  while ((f = comida_na_mesa())) {
    pegar_garfo(id, garfo_direito, "direito ");
    pegar_garfo(id, garfo_esquerdo, "esquerdo ");
    printf("Filósofo %d: comendo.\n", id);
    usleep(DELAY * (FOOD - f + 1));
    devolver_garfos(garfo_esquerdo, garfo_direito);
  }
}

int comida_na_mesa() {
  static int comida = FOOD;
  int minha_comida;
  sem_wait(&trava_comida); // Bloqueia o acesso à variável de comida

  if (comida > 0) {
    comida--;
  }

  minha_comida = comida;
  sem_post(&trava_comida); // Libera o acesso à variável de comida
  return minha_comida;
}

void pegar_garfo(int fil, int g, char *lado) {
  if (sem_trywait(&garfo[g]) == 0) { // Tenta bloquear o garfo
    printf("Filósofo %d: pegou o garfo %s%d.\n", fil, lado, g);
  } else {
    printf("Filósofo %d: não conseguiu pegar o garfo %s%d.\n", fil, lado, g);
  }
}

void devolver_garfos(int g1, int g2) {
  sem_post(&garfo[g1]); // Libera o garfo 1
  sem_post(&garfo[g2]); // Libera o garfo 2
}