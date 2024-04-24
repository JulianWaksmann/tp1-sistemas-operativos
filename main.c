#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#define N 10  // Número de invitados
#define M 5   // Número de mozos

sem_t semaforo_manucho, semaforo_servir, semaforo_respuesta, semaforo_levantarse;

void sentarse() {
    printf("se sienta: ");
}

void comer(int num) {
    printf("Comensal %d está comiendo.\n", num);
}


void enojarse() {
    printf("Manucho se enoja.\n");
}

void levantarse(){
	printf("Se levanta: ");
}

void Lanzar_pregunta_mundialista(){
	printf("manucho hace pregunta mundialista. \n");
}

void Lanzar_respuesta_mundialista(){
	printf("responde pregunta mundialista: ");
}

//------------funcion invitados------------------

void* funcion_invitados(void* arg) {
    int num = *(int*) arg;
    sentarse();
    printf("%d\n", num);

    sem_wait(&semaforo_comer);
    comer(num);
    sem_post(&semaforo_servir);

    sem_wait(&semaforo_respuesta);
	
	//la consigna dice que solo un invitado puede responder, por lo que es necesario el condicionallo
    if (!pregunta_respondida) {
        Lanzar_respuesta_mundialista(num);
        pregunta_respondida = true;  // Marca que la pregunta ha sido respondida
		
		//abre paso a que se enoje manucho
		sem_post(&semaforo_enojarse)
    }
    sem_post(&semaforo_respuesta);
	
	//espera a que manucho se levante
    sem_wait(&semaforo_levantarse);
    levantarse();
    printf("%d\n", num);
    sem_post(&semaforo_levantarse);
    pthread_exit(NULL);
}

//------------funcion manucho------------------

void* funcion_manucho(void* arg) {
    sem_wait(&semaforo_manucho);
    sentarse();
    printf("Manucho\n");
	
	sem_wait(&semaforo_comer);
    comer(0);
	sem_post(&semaforo_servir);
	
    //una vez termina de comer manucho, pregunta
    Lanzar_pregunta_mundialista();
    sem_post(&semaforo_respuesta);	//habilitamos que se pueda responder
	
	//semaforo que me indica si se respondio la pregunta
	sem_wait(&semaforo_enojarse)
    enojarse();
    levantarse();		//una vez se responde, se enoja y se levanta
    printf("Manucho\n");
	
	//se habilita a que los demas se puedan levantar
    sem_post(&semaforo_levantarse);
    pthread_exit(NULL);
}

//------------funcion mozo------------------

void* funcion_mozo(void* arg) {
    int num = *(int*) arg;
    while (comensales > 0) {  // Un ciclo infinito hasta que se sirvan todos los platos necesarios
        
		//genero mutex para que solo lo pueda modificar la variable global donde se acumulan los platos servidos uno a la vez
		sem_wait(&semaforo_variable_global)
		
		//espera para servir la orden del invitado o manucho
		sem_wait(&semaforo_servir);
		
		//se le sirve a un comensal
		comensales-= 1;
        printf("Mozo %d sirve un plato.\n", num);
        
		//libero para que ya pueda comer el comensal (manucho o el invitado)
		sem_post(&semaforo_comer);
        
		sem_post(&semaforo_variable_global) //cierro para que otro mozo pueda operar con la variable global
    }
    pthread_exit(NULL);
}


//------------funcion main------------------

int main() {
    pthread_t manucho, mozos[M], invitados[N];
    int i, id_invitados[N], id_mozos[M];
	int comensales= N + 1; //todos los invitados + manucho para controlar que no se sirva de mas.
	
	//inicio semaforos
    sem_init(&semaforo_manucho, 0, 0);
    sem_init(&semaforo_servir, 0, 1);
    sem_init(&semaforo_respuesta, 0, 0);
    sem_init(&semaforo_levantarse, 0, 0);
	sem_init(&semaforo_comer, 0, 0);
	sem_init(&semaforo_enojarse, 0, 0);
	sem_init(&semaforo_variable_global, 0, 1);
	
	//Creacion del hilo de manucho
    pthread_create(&manucho, NULL, funcion_manucho, NULL);
	
	//Creacion de los hilos de los mozos
    for (i = 0; i < M; i++) {
        id_mozos[i] = i + 1;
        pthread_create(&mozos[i], NULL, funcion_mozo, (void*)&id_mozos[i], &comensales);
    }
	
	//Creacion de los hilos de los invitados
	bool pregunta_respondida= false;
    for (i = 0; i < N; i++) {
        id_invitados[i] = i + 1;
        pthread_create(&invitados[i], NULL, funcion_invitados, (void*)&id_invitados[i], &pregunta_respondida);
    }

    // Esperamos a que todos los invitados se sienten
    for (i = 0; i < N; i++) {
        pthread_join(invitados[i], NULL);
    }
	
	//una vez todos los invitados sentados, damos el sem_post para que se siente manucho
    sem_post(&semaforo_manucho);  // Manucho puede sentarse ahora que todos los invitados están listos

	// Espera a que el thread de Manucho termine su ejecución antes de continuar.
    pthread_join(manucho, NULL);
	
	// Itera sobre cada mozo y espera a que cada uno de ellos termine su ejecución.
	// Esto asegura que todos los mozos hayan completado su tarea de servir antes de que el programa principal continúe o finalice.
    for (i = 0; i < M; i++) {
        pthread_join(mozos[i], NULL);
    }
	
	//destruimos los semaforos
    sem_destroy(&semaforo_manucho);
    sem_destroy(&semaforo_servir);
    sem_destroy(&semaforo_respuesta);
    sem_destroy(&semaforo_levantarse);

    return 0;
}
