#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Pixel {
    int R, G, B;
};

void cinza(int linhas, int colunas, struct Pixel matriz[linhas][colunas]){

       for (int j = 0; j < linhas; j++) {
        for (int i = 0; i < colunas; i++) {
            struct Pixel pixel = matriz[j][i];
            int cinza = (pixel.R * 0.299) + (pixel.G * 0.587) + (pixel.B * 0.114);
            matriz[j][i].R = cinza;
            matriz[j][i].G = cinza;
            matriz[j][i].B = cinza;
        }
    }
}

void imgNegativa(int linhas, int colunas, struct Pixel matriz[linhas][colunas]){
        for (int j = 0; j < linhas; j++) {
            for (int i = 0; i < colunas; i++) { 
            {
                struct Pixel pixel = matriz[j][i];
                int R_novo = 255 - pixel.R;
                int G_novo = 255 - pixel.G;
                int B_novo = 255 - pixel.B;
                matriz[j][i].R = R_novo;
                matriz[j][i].G = G_novo;
                matriz[j][i].B = B_novo;
            }
        }

    }

}

void raioX(int linhas, int colunas, struct Pixel matriz[linhas][colunas]){
    cinza(linhas,colunas,matriz);
    for (int j = 0; j < linhas; j++) {
        for (int i = 0; i < colunas; i++) {
            struct Pixel pixel = matriz[j][i];
            matriz[j][i].R = pixel.R * pixel.R;
            matriz[j][i].G = pixel.G * pixel.G;
            matriz[j][i].B = pixel.B * pixel.B;

        }
    }
}

void rotacionar(int linhas, int colunas, struct Pixel matriz[linhas][colunas]){
    // Transposição da matriz (trocar linhas por colunas)
    for (int i = 0; i < linhas; i++) {
        for (int j = i + 1; j < colunas; j++) {
            struct Pixel temp = matriz[i][j];
            matriz[i][j] = matriz[j][i];
            matriz[j][i] = temp;
        }
    }

    // Inverter a ordem das colunas (para rotacionar 90 graus)
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas / 2; j++) {
            struct Pixel temp = matriz[i][j];
            matriz[i][j] = matriz[i][colunas - j - 1];
            matriz[i][colunas - j - 1] = temp;
        }
    }

}


int main() {
    FILE *fp;
    char tipoImg[3];
    int i, j, linhas, colunas, valor, r, g, b;

    // Abre o arquivo
    fp = fopen("/home/lucas/Área de Trabalho/PUCRS/PBN/t1/Yuri-Alberto-desencanta-com-chegada-de-Antonio-Oliveira.ppm", "r");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }

    // Lê o cabeçalho do arquivo
    fscanf(fp, "%s", tipoImg);                 // lê o tipo de imagem P3
    fscanf(fp, "%d %d", &colunas, &linhas);    // lê o tamanho da matriz
    fscanf(fp, "%d", &valor);                  // lê o valor máximo.

    // Imprime as informações do cabeçalho para debug
    //printf("Tipo de Imagem: %s\n", tipoImg);
    //printf("Dimensões: %d x %d\n", colunas, linhas);
    //printf("Valor máximo: %d\n", valor);

    // Verifica se as dimensões da imagem são razoáveis
    if (linhas <= 0 || colunas <= 0) {
        printf("Dimensões inválidas.\n");
        fclose(fp);
        return 1;
    }

    // Alocação da matriz de pixels RGB em uma única chamada
    struct Pixel *matriz = (struct Pixel *)malloc(linhas * colunas * sizeof(struct Pixel));
    if (matriz == NULL) {
        printf("Erro ao alocar memória para a matriz.\n");
        fclose(fp);
        return 1;
    }

    // Leitura dos valores de pixel RGB
    for (i = 0; i < linhas; i++) {
        for (j = 0; j < colunas; j++) {
            if (fscanf(fp, "%d %d %d", &r, &g, &b) != 3) {
                printf("Erro ao ler os valores de pixel na posição [%d][%d].\n", i, j);
                free(matriz);
                fclose(fp);
                return 1;
            }
            matriz[i * colunas + j].R = r;
            matriz[i * colunas + j].G = g;
            matriz[i * colunas + j].B = b;
        }
    }

    // Fecha o arquivo de entrada.
    fclose(fp);

    // Interação com o usuário para escolha de opções
    int escolha;
    printf("Escolha uma das seguintes opções: ");
    scanf("%d", &escolha);

    if(escolha == 1){
        cinza(linhas,colunas,matriz);
    }
    if(escolha == 2){
        imgNegativa(linhas,colunas,matriz);
        
    }
    if(escolha == 3){
        raioX(linhas,colunas,matriz);
        
    }
    if(escolha == 4){
        rotacionar(linhas,colunas,matriz);
        
    }

    // Criação do novo arquivo PPM
    FILE* fp_novo = fopen("/home/lucas/Área de Trabalho/PUCRS/PBN/t1/Yuri-Alberto-desencanta-com-chegada-de-Antonio-Oliveira.ppm", "w");
    if (fp_novo == NULL) {
        printf("Erro ao criar o arquivo.\n");
        free(matriz);
        return 1;
    }

    // Escreve o cabeçalho do arquivo PPM
    fprintf(fp_novo, "P3\n");
    fprintf(fp_novo, "%d %d\n", colunas, linhas);
    fprintf(fp_novo, "%d\n", valor);

    // Escreve os valores RGB da matriz no arquivo
    for (i = 0; i < linhas; i++) {
        for (j = 0; j < colunas; j++) {
            fprintf(fp_novo, "%d %d %d ", matriz[i * colunas + j].R, matriz[i * colunas + j].G, matriz[i * colunas + j].B);
        }
        fprintf(fp_novo, "\n");
    }

    // Fecha o arquivo
    fclose(fp_novo);

    // Libera a memória alocada para a matriz de pixels RGB
    free(matriz);

    return 0;
}
