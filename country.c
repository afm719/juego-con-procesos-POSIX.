
#include <stdio.h>
#include "datos.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main (int argc, char *argv[]) {

    char c[5] = {*argv[1]};
    srand(time(NULL));
    //Creacion de un fichero binario
    FILE *fichero_binario = fopen(strcat(c,".bin"),"w");
    //Comprobacion de si se ha creado correctamente el fichero binario 
    if (fichero_binario == NULL) {
        fprintf (stderr, "Error en la creacion del fichero\n");
    }
    int mapa[N][N];
    for(int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            mapa[i][j]= rand() % 3;
             //escritura fichero binario
            fwrite(&mapa[i][j], sizeof(N*N*4), N, fichero_binario);
        }
        
    }

    return 0;
}