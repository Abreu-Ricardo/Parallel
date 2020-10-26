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

posicao aki;

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
            cout<< aux->pontos.i << aux->pontos.j <<" insere primeiro"<<endl;

        }

        else{
            // Caso em que soh tem um item na fila
            if (fila->cabeca == fila->cauda){
                fila->cabeca->prox = fila->cauda;
                fila->cauda->prox = aux;
                fila->cauda = aux;
                cout<<" insere qnd tem 1 item "<< "pontos "<< Origem.i <<" "<< Origem.j <<endl;
            }

            else{
                fila->cauda->prox = aux;
                fila->cauda = aux;
                //fila->cauda->ant = aux->ant;
                cout<<"insere qnd tem mais de 1 item "<< "pontos "<< Origem.i <<" "<< Origem.j <<endl;
            }
        }
    }
    //cout<<"fim de insere"<<endl;
    return;
}

// Remove na cabeca
void removeFila(tfila *fila){
    tnode *aux = new tnode;
    //cout<<"inicio remove fila"<<endl;

    if (aux != NULL && fila->cabeca != NULL){
        if(fila->cabeca == fila->cauda){
            cout<<"Cabeca == cauda\n";
            cout<< fila->cabeca->pontos.i << fila->cabeca->pontos.j<< "pontos removidos\n";
            delete(fila->cabeca);
            fila->cabeca = NULL;
            fila->cauda = NULL;
        }

        else{
            //cout<<"Cabeca != cauda\n";
           

            aux = fila->cabeca;            
            fila->cabeca = fila->cabeca->prox;
            //fila->cabeca->ant = NULL;

            cout<< aux->pontos.i << aux->pontos.j<< " pontos removidos\n";
            
        }
        //printf("Ponterio da cabeca i=%d j=%d", fila->cabeca->pontos.i, fila->cabeca->pontos.j);
    }

    else{
        cout << "FILA VAZIA" << endl;
    }

    delete(aux);
    
    
    cout<<"fim remove"<<endl;

    return;
}

bool verifica(int i,int j, int **M, int n, int m){

    return ( (i>=0 && i<n) && (j>=0 && j<m) && (M[i][j] == INT_MAX) ) ? true : false;
}

bool verificaBorda(int **M, int m, int n, posicao *cel){
    
    // if ( (verifica(cel->i-1, cel->j, M, n, m) || verifica(cel->i+1, cel->j, M, n, m)) || 
    //     (verifica(cel->i, cel->j-1, M, n, m) || verifica(cel->i, cel->j+1, M, n, m)) ){
    //     return true;
    // }

    // else
    //     return false;


    return (verifica(cel->i-1, cel->j, M, n, m) || verifica(cel->i+1, cel->j, M, n, m)) || 
           (verifica(cel->i, cel->j-1, M, n, m) || verifica(cel->i, cel->j+1, M, n, m)) ? true : false;
    
}


bool verificaRecursao(int **M, int n, int m, int i, int j, int nivel){

    return ( (i>=0 && i<n) && (j>=0 && j<m) && (M[i][j] != INT_MAX && M[i][j] > 0) ) ? true : false;
}


void preencheVizinhos(int **M, int n, int m, posicao *cel, tfila *fila){
    
    int vizinhos[4] = {1, -1, 0, 0};
    bool valido= false;
    posicao celulaInserida;

    for(int k = 0; k < 4; k++){   
        //Verifica se os vizinhos sao validos                 
        valido = verifica(cel->i + vizinhos[k], cel->j + vizinhos[3-k], M, n, m );
                
        if(valido){
            M[cel->i + vizinhos[k]][cel->j + vizinhos[3-k]] = M[cel->i][cel->j] + 1;
            
            celulaInserida.i = cel->i + vizinhos[k];
            celulaInserida.j = cel->j + vizinhos[3-k];
            insereFila(fila, celulaInserida);
        }  
    }
    
    return;
}

void recursivo(int **M, int n, int m, posicao *cel, tfila *fila, int nivel, posicao *Destino){
    
    printf("Caminho que esta passando i=%d j=%d \n\n", cel->i, cel->j);
    printf("Valor da celula eh igual ao nivel M[cel->i][cel->j]%d == nivel %d\n\n", M[cel->i][cel->j], nivel);


    if ( verificaBorda(M, m, n, cel) || (cel->i == Destino->i && cel->j == Destino->j)){
        if (cel->i == Destino->i && cel->j == Destino->j){
            aki.i = cel->i;
            aki.j = cel->j;
        }
       preencheVizinhos(M,  n,  m, cel, fila);
       return ;
    }

    if(verificaRecursao(M, n, m, cel->i-1 , cel->j, nivel) && M[cel->i][cel->j] < M[cel->i-1][cel->j]){
        cel->i = cel->i -1;
        printf("Linha de cima\n");
        
        recursivo(M, n, m, cel, fila, nivel, Destino);
        cel->i = cel->i +1;

    }
    if(verificaRecursao(M, n, m, cel->i+1 , cel->j, nivel) && M[cel->i][cel->j] < M[cel->i+1][cel->j]){
        cel->i = cel->i +1;
        printf("Linha de baixo: i=%d j=%d\n", cel->i,cel->j);
        
        recursivo(M, n, m, cel, fila, nivel, Destino);
        cel->i = cel->i -1;

    }

    if(verificaRecursao(M, n, m, cel->i , cel->j+1, nivel) && M[cel->i][cel->j] < M[cel->i][cel->j+1]){
        cel->j = cel->j +1;
        printf("Coluna da frente\n: i=%d j=%d\n", cel->i,cel->j);
        
        recursivo(M, n, m, cel, fila, nivel, Destino);
        cel->j = cel->j -1;

    }

    if(verificaRecursao(M, n, m, cel->i , cel->j-1, nivel) && M[cel->i][cel->j] < M[cel->i][cel->j-1]){
        cel->j = cel->j -1;
        printf("Coluna de tras: i=%d j=%d\n", cel->i,cel->j);
                
        recursivo(M, n, m, cel, fila, nivel, Destino);
        cel->j = cel->j +1;

    }


    else{
        printf("N ENTROU EM NENHUM IF posicao i=%d j=%d \n", cel->i, cel->j);
    }

    //nivel+=1;
    //return;
}


int main (int argc, char *argv[]){

    int numObstaculos;
    int **M, n, m;
    tfila fila, caminho;
    posicao Origem;
    posicao Destino;
    posicao cel;
    posicao obstaculo;
    bool achou = false;
    bool valido = false;
    int k;

    posicao celulaInserida;

    int nivel = 0;
    int vizinhos[4] = {1,-1,0,0};
   

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

        //cout << obstaculo.i <<" "<<obstaculo.j << " inicio dos obstaculo\n";
        
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
    //M[Destino.i][Destino.j] = 888; // Sooh para vizualizar RETIRAR

    for (int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            printf("%10d    ", M[i][j]);
        }
        cout<< endl;
    }


    cel.i = Origem.i;
    cel.j = Origem.j;

    // Fase de expansao
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
            

            recursivo(M, n, m, &cel, &fila, nivel, &Destino);
            nivel++;
            

             for (int i = 0; i < n; i++){
                for(int j = 0; j < m; j++){
                    printf("%10d     ", M[i][j]);
                }
                cout<< endl;
            }
           
        }
                        
    }
    printf("valor de nivel: %d  var achou--> %d\n\n\n\n", nivel, achou);

    for (int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            printf("%10d     ", M[i][j]);
        }
        cout<< endl;
    }

    //cout<<&aki->i<<endl;

    // // Fase backtraking
    // iniciaFila(&caminho);

    // if (achou){
    //     cel.i = Destino.i;
    //     cel.j = Destino.j;

    //     while(cel.i != Origem.i or cel.j != Origem.j){
    //          // Se permanece dentro da matriz e eh "infinito"
    //             if (M[cel.i-1][cel.j] == (M[cel.i-1][cel.j] - 1)  && (cel.i-1 < n && cel.i-1 >= 0)){
    //                 cel.i = cel.i-1;
    //                 cel.j = cel.j;
    //                 insereFila(&fila, cel);
    //             }

    //             // Se permanece dentro da matriz e eh "infinito"
    //             if (M[cel.i+1][cel.j] == (M[cel.i-1][cel.j] - 1) && (cel.i+1 < n && cel.i+1 >= 0)){
    //                 cel.i = cel.i+1;
    //                 cel.j = cel.j;
    //                 insereFila(&fila, cel);
    //             }

    //             // Se permanece dentro da matriz e eh "infinito"
    //             if (M[cel.i][cel.j+1] == (M[cel.i-1][cel.j] - 1) && (cel.j+1 < m && cel.j+1 >= 0)){
    //                 cel.i = cel.i;
    //                 cel.j = cel.j+1;
    //                 insereFila(&fila, cel);
    //             }

    //             // Se permanece dentro da matriz e eh "infinito"
    //             if (M[cel.i][cel.j-1] == (M[cel.i-1][cel.j] - 1) && (cel.j-1 < m && cel.j-1 >= 0)){
    //                 cel.i = cel.i;
    //                 cel.j = cel.j-1;
    //                 insereFila(&fila, cel);
    //             }
    //     }
    // }





    return 0;
}