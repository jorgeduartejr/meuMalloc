#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#define BLOCK_SIZE sizeof(bloco) // tamanho da estrutura


struct newMalloc
{
    struct newMalloc *prox; //ptr que aponta para o proximo pedaço da memória
    int ehlivre; // verifica se o bloco esta livre ou não
    size_t size; // tamanho que será alocado na memoria
    void *enderçoMemoria; // Inicio de onde a memoria foi alocada

}; typedef struct newMalloc bloco;


// Recebe quanta memória quer alocar e adiciona um novo nó na linked list
bloco *alocaMemBloco(size_t size){
    bloco *block = (bloco*) sbrk(0);
    void *memEnder = (void*) sbrk(0); // guarda o sbrk no ponto da base p/ que possa começar do início
    void *alocaMem = (void*) sbrk(BLOCK_SIZE + size); // altera a localização para a quantidade de memoria que queremos alocar

    if(alocaMem == (void*)-1){ // da um erro, então retorna NULL
        return NULL;
    } else{
        block -> prox = NULL;
        block -> ehlivre = false;
        block -> size = size;
        block -> enderçoMemoria = memEnder + BLOCK_SIZE;
        return block;
    }

}

void alocarProxMemBloco(size_t size, bloco ** head){
    bloco *atual = *head;
    void *alocaMem = NULL;
    void *memEnder = (void*) sbrk(0);

    if (atual == NULL)
    {
        *head = alocaMemBloco(size);
    } else{
        while (atual -> prox != NULL)
        {
            atual = atual -> prox;
        }
        bloco *novoBloco = sbrk(0);
        alocaMem = (void*) sbrk(BLOCK_SIZE + size);
        if (alocaMem == (void*) - 1){}
        else{
            novoBloco -> prox = NULL;
            novoBloco -> ehlivre = false;
            novoBloco -> size = size;
            novoBloco ->enderçoMemoria = memEnder + BLOCK_SIZE;
            atual -> prox = novoBloco;
        }
    }
}

void liberaMemBloco(bloco **head){ // libera o bloco de memória
    if(*head == NULL){}
    else{
        (*head) -> ehlivre = true;
    }
}

void imprimeMemBlock(bloco *atual){ // imprime se o bloco está livre, o endereço atual e o proximo nó
    while(atual != NULL){
        printf("é livre = %d, size %d, endereço de memoria = %x, atual = %x, prox nó = %x\n", atual->ehlivre, atual->size,atual->enderçoMemoria,atual,atual->prox);
        atual = atual -> prox;
    }
}

int main(){


    bloco *testeMemBloco = NULL;
    alocarProxMemBloco(1,&testeMemBloco);
    alocarProxMemBloco(2,&testeMemBloco);
    alocarProxMemBloco(3,&testeMemBloco);

    imprimeMemBlock(testeMemBloco);

    printf("\n Depois de liberar o segundo nó\n");
    liberaMemBloco(&(testeMemBloco->prox));
    imprimeMemBlock(testeMemBloco);

    int *x;
    x = malloc(sizeof(int));
    printf("Valor de x: %x\n",x);

    return 0;


}
