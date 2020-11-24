/* Programador: Ricardo Abreu de Oliveira
 * Paralelizacao ocorre em preencheVizinho, acahaCaminho, andaLinha, andaColuna*/

#ifndef __componentes__
#define __componentes__

#include <iostream>
#include <omp.h>
#include <limits.h> // INT_MAX
#include <stdbool.h>
#include <stdlib.h>
using namespace std;



// Posicoes da origem
struct posicao   
{
    int i=0;
    int j=0;
};

int contCaminho = 0;
posicao flag;
posicao Destino;
posicao Origem;



// Construcao dos nohs
typedef struct node{
    struct node *prox = NULL;
    struct node *ant = NULL;
    posicao pontos;
}tnode;


// Construcao da fila com cabeca e cauda
typedef struct{
    tnode *cabeca;
    tnode *cauda;
}tfila;

void iniciaFila(tfila *fila){
    fila->cabeca = NULL;
    fila->cauda = NULL;
    return;
}


// Insere na cauda
void insereFila(tfila *fila, posicao Origem){
    tnode *aux = new tnode;
    aux->pontos.i = Origem.i;
    aux->pontos.j = Origem.j;

        // Caso nao tenha alocado ele nao entra no if
        if (aux != NULL){

            // Se for o primeiro item a ser inserido
            if (fila->cabeca == NULL && NULL == fila->cauda){
                fila->cabeca = fila->cauda =aux;
                fila->cauda->ant = fila->cabeca;

            }

            else{
                // Caso em que soh tem um item na fila
                if (fila->cabeca == fila->cauda){
                    fila->cabeca->prox = fila->cauda;
                    fila->cauda->prox = aux;
                    fila->cauda = aux;
                }

                else{
                    fila->cauda->prox = aux;
                    fila->cauda = aux;
                    
                }
            }
        }
    return;
}


void insereCaminho(tfila *fila, posicao Origem){
    tnode *aux = new tnode;
    aux->pontos.i = Origem.i;
    aux->pontos.j = Origem.j;

         // Caso nao tenha alocado ele nao entra no if
        if (aux != NULL){

            // Se for o primeiro item a ser inserido
            if (fila->cabeca == NULL && NULL == fila->cauda){
                fila->cabeca = fila->cauda =aux;
                fila->cauda->ant = fila->cabeca;
                contCaminho++;
                cout << Origem.i << " "<< Origem.j<<endl;

            }

            else{
                // Caso em que soh tem um item na fila
                if (fila->cabeca == fila->cauda){
                    fila->cabeca->prox = fila->cauda;
                    fila->cauda->prox = aux;
                    fila->cauda = aux;
                    contCaminho++;
                    cout << Origem.i << " "<< Origem.j<<endl;

                }

                else{
                    fila->cauda->prox = aux;
                    fila->cauda = aux;
                    contCaminho++;
                    cout << Origem.i << " "<< Origem.j<<endl;


                }
            }
        }
    return;
}



// Remove na cabeca
void removeFila(tfila *fila){
    tnode *aux = new tnode;
    //cout<<"inicio remove fila"<<endl;

    if (aux != NULL && fila->cabeca != NULL){
        if(fila->cabeca == fila->cauda){
            delete(fila->cabeca);
            fila->cabeca = NULL;
            fila->cauda = NULL;
        }

        else{
            aux = fila->cabeca;            
            fila->cabeca = fila->cabeca->prox;            
        }
    }

    delete(aux);
    return;
}

// Verifica se os vizinhos nunca foram visitados
bool verifica(int i, int j, int **M, int n, int m){

    return ( ((i>=0 && i<n) && (j>=0 && j<m)) && ((M[i][j] == INT_MAX) )) ? true : false;
}


// Verifica se algum dos vizinhos eh infinito
bool verificaBorda(int **M, int n, int m, posicao *cel){
    
    return (verifica(cel->i-1, cel->j, M, n, m) || verifica(cel->i+1, cel->j, M, n, m)) || 
           (verifica(cel->i, cel->j-1, M, n, m) || verifica(cel->i, cel->j+1, M, n, m)) ? true : false;
    
}

// Verifica se ja foi visitado 
bool verificaRecursao(int **M, int n, int m, int i, int j){

    return ( ((i>=0 && i<n) && (j>=0 && j<m)) && (M[i][j] != INT_MAX && M[i][j] >= 0) ) ? true : false;
}

// Verifica se eh maior ou nao que a matriz
bool pertenceMatriz(int **M, int n, int m, int i, int j){
    
    if ( (((i < 0) || (i >= n)) || ( j<0 || j>=m )) ){
        
        return false;
    }

    else{
        return true;
    }
    
}

// Preenche todos os vizinhos nao visitados
void preencheVizinhos(int **M, int n, int m, posicao *cel, tfila *fila){
    
    int vizinhos[4] = {1, -1, 0, 0};
    bool valido= false;
    posicao celulaInserida;

   if (pertenceMatriz( M,  n,  m,  cel->i,  cel->j)){
       if (M[cel->i][cel->j] == -1){
            return;
       }

        #pragma omp parallel
        {
            #pragma omp for 
            for(int k = 0; k < 4; k++){  

                //Verifica se os vizinhos sao validos                 
                valido = verifica(cel->i + vizinhos[k], cel->j + vizinhos[3-k], M, n, m );
                        
                if(valido){
                    M[cel->i + vizinhos[k]][cel->j + vizinhos[3-k]] = M[cel->i][cel->j] + 1;
                    
                    celulaInserida.i = cel->i + vizinhos[k];
                    celulaInserida.j = cel->j + vizinhos[3-k];
                    
                    #pragma omp critical
                    insereFila(fila, celulaInserida);
                }  
            }
        }
    }
    
    return;
}


void achaCaminho(int **M, int n, int m, tfila *caminho,posicao *cel, posicao Origem){

    int valor = INT_MAX;              // Var para sempre pegar o menor caminho
    posicao menorCaminho;             // Posicao salva do menor caminho
    int vizinhos[4] = {1, -1, 0, 0};  // Vetor que verifica as posicoes vizinhas
    bool valido= false;

    #pragma omp parallel
    {
        #pragma omp for  
        for(int k = 0; k < 4; k++){   

            //Verifica se os vizinhos sao validos                 
            valido = verificaRecursao(M, n, m, cel->i + vizinhos[k], cel->j + vizinhos[3-k]);
            
            if(valido ){
                if (  M[cel->i + vizinhos[k]][cel->j + vizinhos[3-k]] < valor  || M[cel->i + vizinhos[k]][cel->j + vizinhos[3-k]] == 0){
                    
                    valor = M[cel->i + vizinhos[k]][cel->j + vizinhos[3-k]];
                    menorCaminho.i = cel->i + vizinhos[k];
                    menorCaminho.j = cel->j + vizinhos[3-k];

                }
            }  
        }
    }

    
    cel->i = menorCaminho.i;
    cel->j = menorCaminho.j;
    insereCaminho(caminho, *cel);
    

    if(cel->i == Origem.i && cel->j == Origem.j)
        return;


    achaCaminho(M,  n,  m, caminho, cel, Origem);
    
}


void andaLinha(int **M, int n, int m, posicao cel, tfila *fila){
    
    int i, j, i1, i2;
    j = cel.j;
    i = i1 = i2 = cel.i;

    while( pertenceMatriz(M, n, m, cel.i, cel.j) == true ){
        
        i1++;
        i2--;

        
        // Se passar da matriz cancela, pode-se ter uma coluna 
        // passando e outra nao da matriz
        if (  ((i1 < 0 || i1 >= n) && (i2 >= n || i2 < 0))  ){
            break;
        }

       
        #pragma omp parallel sections
        {
        #pragma omp section
        if(  !verifica( i1, j,  M,  n,  m)){
            
            cel.i = i1;
            preencheVizinhos(M,  n,  m, &cel, fila);            
        }

        #pragma omp section
        if ( !verifica( i2, j,  M,  n,  m)  ){

            cel.i = i2;
            preencheVizinhos(M,  n,  m, &cel, fila);
        }

        }
        // Se as posicoes batem com o Destino
        if ( (i1 == Destino.i || Destino.i == i2)  && Destino.j == cel.j){
            flag.i = (i1 == Destino.i) ? i1 : i2;  
            flag.j = cel.j;
            preencheVizinhos(M,  n,  m, &cel, fila);
        }
        

        cel.i = i;
        cel.j = j;
      
    }
}


// Para cada posicao da da linha andado verifica sua coluna
void andaColuna(int **M, int n, int m, posicao cel, tfila *fila){
    
    int i, j, j1, j2;
    i = cel.i;
    j = j1 = j2 = cel.j;

    while(  pertenceMatriz(M, n, m, cel.i, cel.j) == true ){
        
        j1++;
        j2--;

        // Se passar da matriz cancela, pode-se ter uma coluna 
        // passando e outra nao da matriz
        if (  ((j1 < 0 || j1 >= m) && (j2 >= m || j2 < 0))  ){
            break;
        }

        #pragma omp parallel sections
        {
            #pragma omp section
            if( !verifica( i, j1,  M,  n,  m)  ){
                
                cel.j = j1;

                // Para cada celula da coluna andada, preenche a linha 
                andaLinha(M, n, m, cel, fila);         
                preencheVizinhos(M,  n,  m, &cel, fila); 
            }

            #pragma omp section
            if ( !verifica( i, j2,  M,  n,  m)  ){

                cel.j = j2;

                // Para cada celula da coluna anda, preenche a linha 
                andaLinha(M, n, m, cel, fila);         
                preencheVizinhos(M,  n,  m, &cel, fila);
            }
        }

        cel.i = i;
        cel.j = j;
    }

    return;
}



#endif