#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#define N 10  // Número de invitados
#define M 5   // Número de mozos

sem_t semaforo_manzana, semaforo_servir, semaforo_respuesta, semaforo_levantarse;
int comensales_sentados = 0;
int platos_servidos = 0;

void* funcion_manzana(void* arg) {
    sem_wait(&semaforo_manzana);  // Espera a que todos los comensales se sienten
    printf("Manucho se sienta.\n");
    sem_post(&semaforo_servir); // Permite a los mozos empezar a servir

    printf("Manucho está comiendo.\n");
    printf("Manucho pregunta sobre los favoritos al mundial.\n");
    sem_post(&semaforo_respuesta);  // Permite a un invitado responder

    printf("Manucho se enoja y se levanta.\n");
    sem_post(&semaforo_levantarse);  // Permite a los invitados levantarse
    pthread_exit(NULL);
}

void* funcion_invitado(void* arg) {
    int num = *(int*) arg;
    printf("Invitado %d se sienta.\n", num);
    comensales_sentados++;
    if (comensales_sentados == N) {
        sem_post(&semaforo_manzana);  // El último invitado permite a Manucho sentarse
    }

    sem_wait(&semaforo_servir);
    sem_post(&semaforo_servir);  // Permite que el siguiente invitado proceda a servirse
    printf("Invitado %d está comiendo.\n", num);

    sem_wait(&semaforo_respuesta);
    printf("Invitado %d responde a la pregunta de Manucho.\n", num);
    sem_post(&semaforo_respuesta); // Permite que otro invitado pueda responder si fuera necesario

    sem_wait(&semaforo_levantarse);
    printf("Invitado %d se levanta.\n", num);
    sem_post(&semaforo_levantarse);  // Permite que el siguiente invitado se levante
    pthread_exit(NULL);
}

void* funcion_mozo(void* arg) {
    int num = *(int*) arg;
    while (platos_servidos < N + 1) {
        sem_wait(&semaforo_servir);
        platos_servidos++;
        printf("Mozo %d sirve un plato. Total platos servidos: %d\n", num, platos_servidos);
        sem_post(&semaforo_servir);
        if (platos_servidos >= N + 1) {
            break;
        }
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t manucho, mozos[M], invitados[N];
    int i, id_invitados[N], id_mozos[M];

    sem_init(&semaforo_manzana, 0, 0);
    sem_init(&semaforo_servir, 0, 0);
    sem_init(&semaforo_respuesta, 0, 0);
    sem_init(&semaforo_levantarse, 0, 0);

    pthread_create(&manucho, NULL, funcion_manzana, NULL);
    for (i = 0; i < M; i++) {
        id_mozos[i] = i + 1;
        pthread_create(&mozos[i], NULL, funcion_mozo, (void*)&id_mozos[i]);
    }
    for (i = 0; i < N; i++) {
        id_invitados[i] = i + 1;
        pthread_create(&invitados[i], NULL, funcion_invitado, (void*)&id_invitados[i]);
    }

    pthread_join(manucho, NULL);
    for (i = 0; i < M; i++) {
        pthread_join(mozos[i], NULL);
    }
    for (i = 0; i < N; i++) {
        pthread_join(invitados[i], NULL);
    }

    sem_destroy(&semaforo_manzana);
    sem_destroy(&semaforo_servir);
    sem_destroy(&semaforo_respuesta);
    sem_destroy(&semaforo_levantarse);

    return 0;
}
