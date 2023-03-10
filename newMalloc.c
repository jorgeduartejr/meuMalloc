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
    void *enderecoMemoria; // Inicio de onde a memoria foi alocada

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
        block -> enderecoMemoria = memEnder + BLOCK_SIZE;
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
            novoBloco ->enderecoMemoria = memEnder + BLOCK_SIZE;
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
        printf("é livre = %d, size %d, endereço de memoria = %x, atual = %x, prox nó = %x\n", atual->ehlivre, atual->size,atual->enderecoMemoria,atual,atual->prox);
        atual = atual -> prox;
    }
}

int main(){

    // bloco *testeMemBloco = NULL;
    // alocarProxMemBloco(1,&testeMemBloco);
    // alocarProxMemBloco(2,&testeMemBloco);
    // alocarProxMemBloco(3,&testeMemBloco);

    // imprimeMemBlock(testeMemBloco);

    // printf("\n Depois de liberar o segundo nó\n");
    // liberaMemBloco(&(testeMemBloco->prox));
    // imprimeMemBlock(testeMemBloco);

    printf("teste1:aloca todos os numeros de 0 a 9, caso seje par o teste libera a memoriaque deveria ser armazenada. \n") ;
    printf("My Malloc:\n");
    for (int i = 0; i < 10; i++)
    {
        void *p=alocaMemBloco(i);
        if(i%2==0){
            liberaMemBloco(p);
        }else{
            printf("memoria:%p, armazeno:%d\n",p,i);
        }
    }
    printf("Malloc Original:\n");
    for (int i = 0; i < 10; i++)
    {
        void *p=malloc(i);
        if(i%2==0){
            free(p);
        }
        else{
            printf("memoria:%p, armazeno:%d\n",p,i);
        }
    }

    printf("--------------------------------\n");
    printf("teste2:compara quantos espacos em bytes ele e separado em um alocador 0\n");
    printf("Malloc Original\n");
    void *p1=malloc(sizeof (int));
    void *p2=malloc(sizeof (int));
    int diferenca=p2-p1;
    printf("diferenca: %d\n",diferenca);
    free(p1);
    free(p2);   

    
   printf("My Malloc\n");
    void *p3=alocaMemBloco(sizeof (int));
    void *p4=alocaMemBloco(sizeof (int));
    
    diferenca=p4-p3;
    printf("diferenca: %d\n",diferenca);
    liberaMemBloco(p3);
    liberaMemBloco(p4);

    printf("------------ Teste de fragmentação -----------------------\n");
    printf("My Malloc:\n");
    void *ponteir[20];
    int quantidadeArmazenada=0;
    int quantidadeFragementada=0;
    for (int i = 0; i < 20; i++)
    {
        int diferenca=0;
        ponteir[i] = alocaMemBloco(i);
        if (i>1)
        {
            diferenca=ponteir[i]-ponteir[i-2];
            
        }
        
       
        if(i%2==0){
            liberaMemBloco(ponteir[i]);
        }else{
           i==1? printf("memoria:%p, armazeno:%d\n",ponteir[i],i):printf("memoria:%p, armazeno:%d, diferenca ultimo: %d\n",ponteir[i],i,diferenca);
            quantidadeArmazenada=i+quantidadeArmazenada;
            quantidadeFragementada=diferenca-i+quantidadeFragementada;
        }
        
        
    }
    printf("quantidade de armazenamentos:%d,quantidade Fragementada: %d\n",quantidadeArmazenada,quantidadeFragementada);
    printf("Malloc Original:\n");
    quantidadeArmazenada=0;
    quantidadeFragementada=0;
    for (int i = 0; i < 20; i++)
    {
        int diferenca=0;
        ponteir[i]=malloc(i);
        if (i>1){
            diferenca=ponteir[i]-ponteir[i-2];
            
        }
        
       
        if(i%2==0){
            free(ponteir[i]);
        }else{
           i==1? printf("memoria:%p, armazeno:%d\n",ponteir[i],i):printf("memoria:%p, armazeno:%d, diferenca ultimo: %d\n",ponteir[i],i,diferenca);
            quantidadeArmazenada=i+quantidadeArmazenada;
            quantidadeFragementada=diferenca-i+quantidadeFragementada;
        }
        
        
    }

    printf("quantidade de armazenamentos:%d,quantidade Fragementada: %d\n",quantidadeArmazenada,quantidadeFragementada);

    return 0;


}
