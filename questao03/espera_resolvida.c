#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

int leitores = 0;
sem_t acesso;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *leitor(void *arg) {
  pthread_mutex_lock(&mutex);
  leitores++;
  if (leitores == 1) {
    sem_wait(&acesso); // Bloqueia o acesso dos escritores
  }
  printf("Leitor entrou na região crítica.\n");
  pthread_mutex_unlock(&mutex);

  // Leitura na região crítica

  pthread_mutex_lock(&mutex);
  leitores--;
  if (leitores == 0) {
    sem_post(&acesso); // Libera o acesso dos escritores
  }
  printf("Leitor saiu da região crítica.\n");
  pthread_mutex_unlock(&mutex);
}

void *escritor(void *arg) {
  sem_wait(&acesso); // Bloqueia o acesso dos escritores
  printf("Escritor entrou na região crítica.\n");

  // Escrita na região crítica

  printf("Escritor saiu da região crítica.\n");
  sem_post(&acesso); // Libera o acesso dos escritores
}

int main() {
  // Inicialização das threads
  pthread_t leitores[5];
  pthread_t escritores[2];

  // Inicialização do semáforo
  sem_init(&acesso, 0, 1);

  // Criação das threads
  for (int i = 0; i < 5; i++) {
    pthread_create(&leitores[i], NULL, leitor, NULL);
  }
  for (int i = 0; i < 2; i++) {
    pthread_create(&escritores[i], NULL, escritor, NULL);
  }

  // Espera as threads terminarem
  for (int i = 0; i < 5; i++) {
    pthread_join(leitores[i], NULL);
  }
  for (int i = 0; i < 2; i++) {
    pthread_join(escritores[i], NULL);
  }

  // Destroi o semáforo
  sem_destroy(&acesso);

  return 0;
}
