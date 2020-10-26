/* Aluno: Ricardo Abreu de Oliveira
*  Comando: g++ rotseq.cpp -o rotseq -Wall -fopenmp
*/


#include "componentes.h"
#include <omp.h>
using namespace std;


int main (int argc, char *argv[]){

    int numObstaculos;
    int **M, n, m;
    tfila fila, caminho;

    posicao Origem;
    posicao Destino;
    posicao cel;
    posicao obstaculo;
    bool achou = false;


    cin >> n>>m; // Dimen da matriz
    M = new int *[n];
    
    // Alocacao da matriz
    for(int i=0; i<n; i++)
        *(M+i) = new int[m];

    for (int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            M[i][j] = INT_MAX;
        }
    }

    cin >> Origem.i >> Origem.j;
    cin >> Destino.i >> Destino.j;
    cin >> numObstaculos;
    

    for (int i =0; i < numObstaculos; i++){
        cin >> obstaculo.i >> obstaculo.j;
        
        int lin, col; // Ler a qtd de linhas e colunas do obstaculo
        cin >> lin >> col;

        for (int j = 0; j < lin; j++){ // Laco que permanece na linhas
            for( int k = 0; k < col; k++ ){ // Laco que preenche a coluna
                M[obstaculo.i + j][obstaculo.j + k] = -1;
            }
        }
    }
    

    iniciaFila(&fila);
    insereFila(&fila, Origem);

    M[Origem.i][Origem.j] = 0; // na primeira interacao da funcao vericaVizinho ja troca para 0

    #pragma omp parallel

    // Fase de expansao
    /****************************************************************/
    cel.i = Origem.i;
    cel.j = Origem.j;
    while(fila.cabeca != NULL and not achou){
        removeFila(&fila);

        if (/*cel.i*/ aki.i == Destino.i and /*cel.j*/ aki.j == Destino.j){
            printf("AQUI A POSICAO %d %d\n", aki.i, aki.j);
            achou = true;
            break;
        }

        else{
            cel.i = Origem.i;
            cel.j = Origem.j;
            

            recursivo(M, n, m, &cel, &fila, &Destino);            

             for (int i = 0; i < n; i++){
                for(int j = 0; j < m; j++){
                    printf("%10d     ", M[i][j]);
                }
                cout<< endl;
            }
           
        }
                        
    }
    /*****************************************************************/


    printf("valor de   var achou--> %d\n\n\n\n", achou);

    for (int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            printf("%10d     ", M[i][j]);
        }
        cout<< endl;
    }



    // Fase de backtraking
    /*******************************************************************/
    
    cel.i = Destino.i;
    cel.j = Destino.j;

    iniciaFila(&caminho);
    insereFila(&caminho, cel);

    // Nao precisa desse laco
    //while(cel.i != Origem.i && cel.j != Origem.j){
        achaCaminho(M, n, m, &caminho, &cel, Origem);

        printf("Backtrack: i=%d  j=%d\n\n", cel.i, cel.j);
    //}

    
    /*******************************************************************/

    return 0;
}