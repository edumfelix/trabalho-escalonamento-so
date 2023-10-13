#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

int leitores = 0;
int escritores = 0;
sem_t mutex;
sem_t escrevendo;

void *leitor(void *arg) {
  sem_wait(&mutex);
  leitores++;
  if (leitores == 1) {
    sem_wait(&escrevendo);
  }
  sem_post(&mutex);

  // Leitura na região crítica

  sem_wait(&mutex);
  leitores--;
  if (leitores == 0) {
    sem_post(&escrevendo);
  }
  sem_post(&mutex);

  printf("Leitor entrou na região crítica.\n");
  printf("Leitor saiu da região crítica.\n");
}

void *escritor(void *arg) {
  sem_wait(&escrevendo);
  escritores++;

  // Escrita na região crítica

  escritores--;
  sem_post(&escrevendo);

  if (escritores == 0) {
    sem_post(&mutex);
  } else {
    sem_post(&escrevendo);
  }

  printf("Escritor entrou na região crítica.\n");
  printf("Escritor saiu da região crítica.\n");
}

int main() {
  // Inicialização das threads
  pthread_t leitores_threads[5];
  pthread_t escritores_threads[2];

  // Inicialização dos semáforos
  sem_init(&mutex, 0, 1);
  sem_init(&escrevendo, 0, 1);

  // Criação das threads
  for (int i = 0; i < 5; i++) {
    pthread_create(&leitores_threads[i], NULL, leitor, NULL);
  }
  for (int i = 0; i < 2; i++) {
    pthread_create(&escritores_threads[i], NULL, escritor, NULL);
  }

  // Espera as threads terminarem
  for (int i = 0; i < 5; i++) {
    pthread_join(leitores_threads[i], NULL);
  }
  for (int i = 0; i < 2; i++) {
    pthread_join(escritores_threads[i], NULL);
  }

  // Destroi os semáforos
  sem_destroy(&mutex);
  sem_destroy(&escrevendo);

  return 0;
}
