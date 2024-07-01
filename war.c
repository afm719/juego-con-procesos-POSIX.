#include <stdio.h>
#include "datos.h"
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define CIVIL 1
#define MILITAR 2

void hijo (int senal) ;

int main(void) {
	// declaracion de variables
	int descriptor[2][2], descriptor2[2][2];
	pid_t creado, creado2, creado3, creado4;
	int var, var2;
	char buffer1[20], buffer2[20];
	int pos1, pos2;
	char caracter;
	int puntuacion, valor, num_civil, num_militar;

	// plantar la semilla
	srand(time(NULL));

	// se abre la pipe de comunicacion
	// con los hijos
	for (int i = 0; i < 2; i++) {
		if (pipe(descriptor[i]) < 0) {
			// da error
			perror("Error al crear la pipe\n");
			exit(-1);
		}
	}

	// se abre la pip2 de comunicacion
	// con los hijos
	for (int i = 0; i < 2; i++) {
		if (pipe(descriptor2[i]) < 0) {
			// da error
			perror("Error al crear la pipe\n");
			exit(-1);
		}
	}
	creado3 = fork();
	//creacion de 1 proceso para cada hijo
	if (creado3 == 0) {
		var = execlp("./country", "country","A", (char *)0);
		exit(0);
	}
	// creacion de los hijos
	// hijo 1/

	switch (creado = fork()) {
	case -1:
		perror("El fork no ha funcionado\n");
		exit(-1);
		break;
	case 0:
		// cerrar canal de escritura
		signal(SIGUSR1, hijo);
		//pid del pais contrario
		pid_t pid_contrario;

		//cierro canal de escritura y leo
		close(descriptor[0][1]);
		read(descriptor[0][0], buffer1, 40);
		usleep(100);
		read(descriptor[0][0], &pid_contrario, 4);

		//leo el fichero donde se encuentra el mapa
		FILE *fichero = fopen(buffer1, "rb");
		int mapa[N*N];
		//escribo en una matriz el fichero
		fread(mapa, 4, (N * N), fichero);
		fclose(fichero);

		//calculo de las posiciones que ataca
		for (int i = 0; i < M; i++) {
			usleep(100); 
			pos1 = rand() % N;
			pos2 = rand() % N;
			//posicion donde ha caido
			valor = mapa[pos1*N + pos2];

			//calculo de infrastructuras golpeadas
			switch (valor) {
				case CIVIL:
					num_civil++;
					break;
				case MILITAR:
					num_militar++;
					break;
			}

			kill(pid_contrario, SIGUSR1);
			pause();
	}

		//calculo de la puntuacion
		puntuacion = (num_civil * (-2)) + num_militar;

		// escribir en la pipe
		close(descriptor[1][0]);
		write(descriptor[1][1], &puntuacion, sizeof(puntuacion));

		if (num_civil > num_militar) {
			exit(1);
		}
		else if (num_militar > num_civil) {
			exit(2);
		}
		exit(3);
		break;
	}
	creado4 = fork();
	if (creado4 == 0) {
		var2 = execlp("./country", "country","B", (char *)0);
		exit(0);
	}
		
	
		// hijo 2/
	switch (creado2 = fork()) {
	case -1:
		perror("El fork no ha funcionado\n");
		exit(-1);
		break;
	case 0:
		// cerrar canal de escritura
		signal(SIGUSR2, hijo);
		pid_t pid_contrario;

		close(descriptor2[0][1]);
		read(descriptor2[0][0], buffer2, 40);
		read(descriptor2[0][0], &pid_contrario, 4);

		FILE *fichero = fopen(buffer2, "rb");
		int mapa[N*N];
		fread(mapa, 4, (N * N), fichero);
		fclose(fichero);

		for (int i = 0; i < M; i++) {
			pause(); 
			pos1 = rand() % N;
			pos2 = rand() % N;
			valor = mapa[pos1*N + pos2];

			switch (valor) {
				case CIVIL:
					num_civil++;
					break;
				case MILITAR:
					num_militar++;
					break;
			}

			usleep(100); //el hijo hace pause
			kill(pid_contrario, SIGUSR1); //aviso al hijo
			
		}

		puntuacion = (num_civil * (-2)) + num_militar;
		// escribir en la pipe/
		close(descriptor2[1][0]);

		write(descriptor2[1][1], &puntuacion, sizeof(puntuacion));
		
		if (num_civil > num_militar) {
			exit(1);
		}
		else if (num_militar > num_civil) {
			exit(2);
		}
		exit(3);
		break;
	}

	//pipe de un pais escribir
	close(descriptor[0][0]);
	write(descriptor[0][1], "B.bin", 40);
	write(descriptor[0][1], &creado2, 4);
	
	//pipe de otro pais escribir
	close(descriptor2[0][0]);
	write(descriptor2[0][1], "A.bin", 40);
	write(descriptor2[0][1], &creado, 4);

	//espera 1 segundo
	sleep(1);
	printf ("Comienza la Guerra\n");

	//despierta con la senal de los dos procesos pais
	kill(creado, SIGUSR1);
	int paisA, paisB;
	waitpid(creado, &paisA, 0);
	waitpid(creado2, &paisB, 0);
	paisA = WEXITSTATUS(paisA);
	paisB = WEXITSTATUS(paisB);

	//lectura de la puntuacion a traves de las pipes
	int puntosA, puntosB;
	close(descriptor[1][1]);
	read(descriptor[1][0], &puntosA, sizeof(int));
	close(descriptor2[1][1]);
	read(descriptor2[1][0], &puntosB, sizeof(int));
	

	printf ("Puntos de A: %d, Puntos de B %d\n", puntosA, puntosB);

	if (paisA == 1) {
		printf("El país A ha cometido crímenes de guerra\n");
	}
	if (paisB == 1) {
		printf("El país B  ha cometido crímenes de guerra\n");
	}

	//comprobacion del ganador de la partida
	if (puntosA > puntosB) {
		return 1;
	} else if (puntosB > puntosA) {
		return 2;
	} 

	return 0;
}

/* funcion de manejo de la signal para el hijo */
void hijo (int senal) {
	/* de nuevo para recibir la siguiente signal */
	return ;	/* pone SIGUSR1 para el hijo */
}