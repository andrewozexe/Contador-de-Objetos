#include <stdio.h>
#include <stdlib.h>

typedef struct Pixel{
    int fundo, visitado;

}Pixel;

typedef struct No {
    int x,y;
    struct No *prox;
} No;

typedef struct {
    No *topo;
    int tam;
}Pilha;

void initPilha(Pilha *pilha);
void push(Pilha *pilha, int x, int y);
No pop(Pilha *pilha);
void leVizinhos(Pilha *pilha, Pixel **matriz, int x, int y, int linhas, int colunas);
void printPilha(Pilha *pilha);

int main(){
    char arq[] = "..\\Imagens\\18.ppm";
    FILE *fp;

    fp = fopen(arq, "rb");
    if (fp == NULL) {
        printf("Erro na abertura do arquivo <%s>", arq);
        exit(EXIT_FAILURE);
    }

    char id[3];
    int colunas, linhas, maximo;
    unsigned char r, g, b;
    unsigned char rf, gf, bf;
    int i, j, k;
    int posicao;

    fp = fopen(arq, "rb");
    if (fp == NULL) {
        printf("Erro na abertura do arquivo <%s>", arq);
        exit(EXIT_FAILURE);
    }

    fscanf (fp, "%s", id);
    fscanf (fp, "%d", &colunas);
    fscanf (fp, "%d", &linhas);
    fscanf (fp, "%d", &maximo);
    fscanf (fp, "%c", &r);

    printf("Tipo do arquivo <%s>: %s\n", arq, id);
    printf("Maximo: %d\n", maximo);
    printf("Imagem tem %d x %d pixels\n", linhas, colunas);

    posicao = ftell(fp);
    fscanf (fp, "%c%c%c", &rf, &gf, &bf);
    fseek(fp, posicao, SEEK_SET);
    Pixel **imagem = calloc(linhas, sizeof(Pixel*));
    for (int l = 0; l < linhas; ++l) {
        imagem[l] = calloc(colunas, sizeof(Pixel));
    }

    for (i=0; i<linhas; i++) {
        for (j=0; j<colunas; j++) {
            fscanf (fp, "%c%c%c", &r, &g, &b);
            if ((r!=rf) || (g!=gf)  || (b!=bf)  ){
                imagem[i][j].fundo = 1;
                //printf("x");
            } else {
                //printf("_");
            }
        }
        //printf("\n");
    }

    Pilha pilha;
    initPilha(&pilha);
    int objetos = 0;
    for (int linha = 0; linha < linhas; linha++) {
        for (int coluna = 0; coluna < colunas; coluna++) {
            if(imagem[linha][coluna].fundo != 0 && imagem[linha][coluna].visitado != 1){
                push(&pilha,linha,coluna);
                imagem[linha][coluna].visitado = 1;
                while (pilha.topo != NULL){
                    No temp = pop(&pilha);
                    leVizinhos(&pilha,imagem,temp.x,temp.y,linhas,colunas);
                }
                objetos++;
            }
        }
    }

    printf("Objetos contados = %d", objetos);
    fclose(fp);
    return 0;
}


void initPilha(Pilha *pilha){
    pilha->topo = NULL;
    pilha->tam = 0;
}
void push(Pilha *pilha, int x, int y){
    No *novo = (No*) malloc(sizeof(No));
    novo->x = x;
    novo->y = y;
    if (pilha->topo == NULL){
        novo->prox = NULL;
        pilha->topo = novo;
    } else {
        novo->prox = pilha->topo;
        pilha->topo = novo;
    }
    pilha->tam++;
}
No pop(Pilha *pilha){
    No no;
    if (pilha->topo != NULL){
        No *temp = pilha->topo; // Copia o no do topo para temp
        pilha->topo = pilha->topo->prox; // define o topo da pilha como o de baixo
        no = *temp;
        free(temp);
        pilha->tam--;
        return no;
    } else {
        no.prox = NULL;
        return no;
    }
}
void leVizinhos(Pilha *pilha, Pixel **matriz, int x, int y, int linhas, int colunas){
    if(x-1 >= 0){
        if(matriz[x-1][y].visitado == 0 && matriz[x-1][y].fundo != 0){
            push(pilha, x-1, y);
            matriz[x-1][y].visitado = 1;
        }
    }
    if(x + 1 < linhas){
        if(matriz[x+1][y].visitado == 0 && matriz[x+1][y].fundo != 0){
            push(pilha, x+1, y);
            matriz[x+1][y].visitado = 1;
        }
    }
    if(y - 1 >= 0){
        if(matriz[x][y-1].visitado == 0 &&matriz[x][y-1].fundo != 0){
            push(pilha, x,y-1);
            matriz[x][y-1].visitado = 1;
        }
    }
    if(y + 1 < colunas){
        if(matriz[x][y+1].visitado == 0 &&matriz[x][y+1].fundo != 0){
            push(pilha, x,y+1);
            matriz[x][y+1].visitado = 1;
        }
    }
}
void printPilha(Pilha *pilha){
    No *aux = pilha->topo;
    if (aux == NULL)
        printf("vazia.");
    else
        while(aux!=NULL){
            printf(" %d %d", aux->x, aux->y);
            aux= aux->prox;
            printf("\n");
        };
    printf("\n");
}