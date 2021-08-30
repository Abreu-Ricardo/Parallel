/*  Trabalho 3
 *
 * Alunos: João Víctor Zárate, Julio Huang, Ricardo Abreu
 *----------------------------------------------------------------------------
 * Remove 0s de um vetor
 * Para compilar: mpicc remove0_par.c -o remove0_par -Wall
 * Para executar: mpirun -oversubscribe -np 10 a.out entrada.txt saida.txt
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>
#include <mpi.h>

#define MESTRE 0
#define TAG1 1
#define TAG2 2

// ----------------------------------------------------------------------------
void remove0(int n, int vIn[], int *m, int vOut[])
{
	int i;
	int c = 0;

	for (i = 0; i < n; i++)
	{
		if (vIn[i] != 0)
		{
			vOut[c] = vIn[i];
			c++;
		}
	}
	*m = c;
}

int *leitura( FILE *arqIn, int *n){
	
	int *vIn;
	
    if (arqIn == NULL)
	{
		printf("\nArquivo texto de entrada não encontrado\n") ;
		exit(1) ;
	}

	// Lê tamanho do vetor de entrada
	fscanf(arqIn, "%d", n);

	// Aloca vetores de entrada e saída
	vIn  = (int *) malloc(*n * sizeof(int));

	if ((vIn == NULL) /*|| (vOut == NULL)*/)
	{
		printf("\nErro na alocação de estruturas\n") ;
		exit(1) ;
	}

	// Lê vetor do arquivo de entrada
	for (int i = 0; i < *n; i++)
	{
		fscanf(arqIn, "%d", &(vIn[i]));
		//printf("na leitura %d\n", vIn[i]);

	}

	// Fecha arquivo de entrada
	fclose(arqIn);

    return vIn;
}


// ----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	int n,		// Número de elementos do vetor de entrada (com 0s)
		 //m,		// Número de elementos do vetor de saída (sem 0s)
		 *vIn,	// Vetor de entrada de n elementos
		 *vOut,	// Vetor de saída de no máximo n elementos
		 i;

	FILE *arqIn,	// Arquivo texto de entrada
		 *arqOut;	// Arquivo texto de saída

	struct timeval tIni, tFim;

	if(argc != 3)
	{
		printf("O programa foi executado com argumentos incorretos.\n") ;
		printf("Uso: ./remove0_seq arquivo_entrada arquivo_saida\n") ;
		exit(1) ;
	}

	// Abre arquivo de entrada
	arqIn = fopen(argv[1], "rt") ;
    //leitura(arqIn, vIn, vOut);

    /************ COMECO MPI ***********/
    int idProcesso, nProcessos;
    int numIt;
    int *aux;
    
	MPI_Status status;

    MPI_Init(&argc, &argv); // Inicializa ambiente de execução do MPI

	MPI_Comm_rank(MPI_COMM_WORLD, &idProcesso); // Obtém id do processo
	
	MPI_Comm_size(MPI_COMM_WORLD, &nProcessos); // Obtém número de processos

	vIn = leitura(arqIn, &n);
    numIt = n / nProcessos;
    aux = (int *) malloc(numIt * sizeof(int));

	
    if (idProcesso == 0){
		// n = 100;
	    //vIn  = (int *) malloc(n * sizeof(int));

		vOut  = (int *) malloc(n * sizeof(int));
		vOut == NULL ? perror("Erro na alocacao de vOut") : 1;

		/* if (n % nProcessor != 0)
				nProcessos = recebe o resto
		*/
		gettimeofday(&tIni, 0);

		MPI_Scatter(vIn, numIt, MPI_INT, 
					MPI_IN_PLACE, numIt, MPI_INT, MESTRE, MPI_COMM_WORLD);

		int m; // Número de elementos do vetor de saída (sem 0s)
		remove0(numIt, vIn, &m, vOut);
		int contTotal = 0;
		int *vetorFinal;

		vetorFinal = (int *) malloc(n * sizeof(int));

		for (int j = 0; j < m; j++)
        {
            vetorFinal[j] = vOut[ j ];
			//printf("%d ", vetorFinal[j]);    
        }
		contTotal += m; 

		// MESTRE RECEBE dos trabalhadores
        for( i=1; i < nProcessos; i++){

            MPI_Recv(&m, 1, MPI_INT, i, TAG1, MPI_COMM_WORLD, &status);
            MPI_Recv(aux, numIt, MPI_INT, i, TAG2, MPI_COMM_WORLD, &status);

			//contTotal += m;
            
            for (int j = 0; j < m; j++)
            {
                vetorFinal[ contTotal ] = aux[ j ]; 
				//printf("%d ", aux[j]);   
				contTotal++;
			}
			//printf("\n");
        }

		// printf("%d\n", contTotal);
		// for(int k = 0; k < contTotal; k++){
		// 	printf("%d ", vetorFinal[k]);
		// }
		// printf("\n");
		
		gettimeofday(&tFim, 0);

		//Tempo de execução em milissegundos
		long segundos = tFim.tv_sec - tIni.tv_sec;
		long microsegundos = tFim.tv_usec - tIni.tv_usec;
		double tempo = (segundos * 1e3) + (microsegundos * 1e-3);
		printf("Tempo=%.2fms\n", tempo);

        // // Cria arquivo de saída
        arqOut = fopen(argv[2], "wt");

        // // Escreve tamanho do vetor de saída
        fprintf(arqOut, "%d\n", contTotal) ;
		// printf("%d\n", m);

        // // Escreve vetor do arquivo de saída
        for (i = 0; i < contTotal; i++)
        {
            fprintf(arqOut, "%d ", vetorFinal[i]);
        }
		//Fecha arquivo de saída
		fclose(arqOut);
		//Libera vetores de entrada e saída
        free(vIn);
        free(vOut);
        // fprintf(arqOut, "\n");

    }
    else{
        // // vetor de saida
		int *dest;
		int *envio;
        dest = (int *) malloc(numIt * sizeof(int));

        MPI_Scatter(NULL, 0, MPI_INT, 
				   dest, numIt, MPI_INT, MESTRE, MPI_COMM_WORLD);

        
		//printf("Processo:%d valor dest: %d \n", idProcesso, dest[0]);

		// for (int i=0; i<numIt; i++){
		// 	printf("no else %d\n", dest[i]);
		// }

		int m;
		envio = (int *) malloc(numIt * sizeof(int));
		remove0(numIt, dest, &m, envio);
	
		MPI_Send(&m, 1, MPI_INT, MESTRE, TAG1, MPI_COMM_WORLD);
		MPI_Send(envio, m, MPI_INT, MESTRE, TAG2, MPI_COMM_WORLD);


		// printf("Processo: %d valor do m: %d\n", idProcesso, m);
		// for (int i = 0; i < m; i++)
		// {
		// 	printf("valor no vetor: %d\n", envio[i]);
		// }
    }
	MPI_Finalize();

	

    /*****************************************************/
	// struct timeval tIni, tFim;
	// gettimeofday(&tIni, 0);


	// gettimeofday(&tFim, 0);
	// // Tempo de execução em milissegundos
	// long segundos = tFim.tv_sec - tIni.tv_sec;
	// long microsegundos = tFim.tv_usec - tIni.tv_usec;
	// double tempo = (segundos * 1e3) + (microsegundos * 1e-3);
	// printf("Tempo=%.2fms\n", tempo);

	return 0;
}