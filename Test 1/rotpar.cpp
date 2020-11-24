/* Aluno: Ricardo Abreu de Oliveira
*  Comando: g++ rotpar.cpp -o rotpar -fopenmp
*  Execucao: ./rotpar < entrada.in > saida.out
*
*  Paralelizacao ocorre na fase de expansao e 
*  no arquivo componentesPar.h
*  Obs: Problema com o obstaculo 14 9 55 da entrada2
*/


#include "componentesPar.h"
using namespace std;


void caminhaMatriz(int **M, int n, int m, int i, int j, posicao cel, tfila *fila){

    while(  pertenceMatriz(M, n, m, cel.i, cel.j) == true ){
        
        // Anda somente em uma direcao
        cel.i += i;
        cel.j += j;
        
        // Se passar da matriz cancela o laco
        if (  ( (cel.i<0 || cel.i>=n) || (cel.j>=m || cel.j<0) ) ){
            break;
        }

        else{

            // Se posicao atual eh diferente de obstaculo
            if ( M[cel.i][cel.j] != -1 ){

                #pragma omp parallel sections
                {
                    #pragma omp section
                    andaColuna(M, n, m, cel, fila);
                    
                    #pragma omp section
                    andaLinha(M, n, m, cel, fila);
                    
                    #pragma omp section
                    preencheVizinhos(M,  n,  m, &cel, fila);
                }
            }

            else{                
                break;
            }
        }
    }
    return;
}


int main (int argc, char *argv[]){

    int numObstaculos;
    int **M, n, m;
    tfila fila, caminho;

  
    posicao cel, obstaculo, celulaInserida;
    double timeI, timeF, timeT;
    bool achou = false, valido;
    int vizinhos[4] = {1, -1, 0, 0};   // Posicao i recebe vizinho[k]
                                       // Posicao j recebe vizinho[3-k], assim verificando celulas vizinhas

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
    

    // Coloca obstaculos
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

    // Garantindo que nao vai comparar valores iguais
    flag.i = -2;
    flag.j = -2;
    

    iniciaFila(&fila);
    insereFila(&fila, Origem);

    M[Origem.i][Origem.j] = 0; 

    timeI = omp_get_wtime();    // Inicio da contagem

    // Fase de expansao
    /****************************************************************/

    cel.i = Origem.i;
    cel.j = Origem.j;
    while(  fila.cabeca != NULL and not achou ){
        removeFila(&fila);

        if ( flag.i == Destino.i and  flag.j == Destino.j){
            achou = true;
            break;
        }

        else{
            
            // Fazendo expansao sempre a partir da origem para garantir os niveis
            cel.i = Origem.i;
            cel.j = Origem.j;
            
            #pragma omp parallel for
            for(int k = 0; k < 4; k++){   
                valido = verifica(cel.i + vizinhos[k], cel.j + vizinhos[3-k], M, n, m );
                
                if(valido){
            
                    M[cel.i + vizinhos[k]][cel.j + vizinhos[3-k]] = M[cel.i][cel.j] + 1;
            
                    celulaInserida.i = cel.i + vizinhos[k];
                    celulaInserida.j = cel.j + vizinhos[3-k];

                    insereFila(&fila, celulaInserida);
                }

                else {
                    caminhaMatriz(M, n, m,  vizinhos[k] ,  vizinhos[3-k], cel, &fila);
                }
            }
        }           
    }

    /*****************************************************************/


    //Impressao da matriz
    // for (int i = 0; i < n; i++){
    //     for(int j = 0; j < m; j++){
    //         printf("%10d     ", M[i][j]);
    //     }
    //     cout<< endl;
    // }



    // Fase de backtrack
    /*******************************************************************/
    
    cel.i = Destino.i;
    cel.j = Destino.j;

    iniciaFila(&caminho);
    insereFila(&caminho, cel);

    if (achou){
        insereCaminho( &caminho, cel);
        achaCaminho(M, n, m, &caminho, &cel, Origem);

        // Menos 1 da origem
        cout<<"Total: "<<contCaminho-1<<endl;

    }

    timeF = omp_get_wtime();
    timeT = timeF - timeI;
    printf("\nTempo total--> %lf\n", timeT);


    
    /*******************************************************************/

    return 0;
}

