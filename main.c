/* 
 * File:   main.c
 * Author: matheus
 *
 * Created on 24 de Junho de 2019, 08:10
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int tamanho_nodes_internos = 256;

typedef struct node{
   char caractere;
   struct node* nodes_internos[256];
   int dados;
   int termina;
   int profundidade;
} Node;

int verbose=0;

int checa_string(char *str, int profundidade, Node* node){
    if(profundidade < strlen(str)-1){
        if(node->nodes_internos[str[profundidade]]->caractere != '\0'){
            node = node->nodes_internos[str[profundidade]];
            return checa_string(str, profundidade+1, node);
        }else{
            return 0;
        }
    }else if(node->nodes_internos[str[profundidade]]->caractere != '\0'){
        if(node->nodes_internos[str[profundidade]]->termina)
            return 1;
    }
}

int insere_string(char *str, int profundidade, Node* alvo, int dados){
    if(profundidade < strlen(str)){
        if(verbose){
            if(profundidade == 0){
                printf("PALAVRA: [%s]\n", str);
            }
            printf("Inserir [%c], profundidade=[%d]\n", str[profundidade], profundidade);
            
        }
        Node* atual = alvo; //Alvo é o node escolhido, se inicia na raiz e desce até o fim.
        int novo = 1;
        if(atual->nodes_internos[str[profundidade]]->caractere != '\0'){ //A letra existe, não é necessário inserir nada, desce para o próximo nível
            if(profundidade < strlen(str)-1){
                if(verbose){
                    printf("Existe [%c]\n", str[profundidade]);
                }
            }else{
               if(!atual->nodes_internos[str[profundidade]]->termina){ //É uma letra existente mas é uma folha, portanto uma palavra nova
                   atual->nodes_internos[str[profundidade]]->termina = 1;
                    if(verbose){
                       printf("Existe [%c], mas eh uma palavra nova\n", str[profundidade]);
                    }
                }else{
                    if(verbose){
                       printf("[%c] faz parte da palavra\n", str[profundidade]);
                    }
                }
            }

            insere_string(str, profundidade+1, atual->nodes_internos[str[profundidade]], dados); //O node atual existe, insere o próximo dentro dele
            novo = 0;
            return 0;
        }else{ //A letra não existe, é necessário inserir
            
            //Encontra um node vago dentro do node *Usando a letra como índice por performance
            if(verbose)
                printf("Inserindo [%c]\n", str[profundidade]);
            
            atual = atual->nodes_internos[str[profundidade]];

            if(profundidade < strlen(str)-1){ //Não é uma folha, portanto não é uma palavra
                atual->caractere = str[profundidade];
                atual->termina=0;
                atual->profundidade=profundidade;
                for(int i = 0; i < tamanho_nodes_internos; i++){ //Preenche os espaços dentro do node inserido
                    Node* n = malloc(sizeof(Node));
                    n->caractere = '\0';
                    atual->nodes_internos[i] = n;
                }
                insere_string(str, profundidade+1, atual, dados); //Desce mais um nível
            }else{ //se é uma folha então forma uma palavra
                atual->caractere = str[profundidade];
                atual->termina=1;
                atual->dados=dados;
                atual->profundidade = profundidade;
                if(verbose)
                    printf("Nova palavra! => %d\n", dados);
                for(int i = 0; i < tamanho_nodes_internos; i++){ //Preenche o node
                    Node* n = malloc(sizeof(Node));
                    n->caractere = '\0';
                    atual->nodes_internos[i] = n;
                }
            }
        }
    }
    return 1;
}

int main(int argc, char** argv) {
    if(argc > 1){
        if(strcmp(argv[1], "--v") == 0){
            verbose=1;
        }
    }
    //Cria o node inicial e aloca seus nodes internos
    Node root;
    for(int i = 0; i < tamanho_nodes_internos; i++){
        root.nodes_internos[i] = malloc(sizeof(Node));
    }
   
    
    char input[1000] = "Ana";
    int score;
    while(scanf(" %[^\n]s", input) != EOF){
        scanf("%d", &score);
        insere_string(input, 0, &root, score);
        if(verbose)
            printf("\n");
    }
    char str[] = "A";
    while (scanf(" %[^\n]s", str) != EOF){
        printf("se %s existe: [%d]\n", str, checa_string(str, 0, &root));
    }
    return (EXIT_SUCCESS);
}