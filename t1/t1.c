#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct t1
{
    int R,G,B;
};
 Pixel
 

void converteCinza(struct Pixel **vetor, int numRows, int numCols) {
    for (int j = 0; j < numRows; j++) {
        for (int i = 0; i < numCols; i++) {
            Pixel pixel = vetor[j][i];
            int cinza = (pixel.R * 0.299) + (pixel.G * 0.587) + (pixel.B * 0.114);
            vetor[j][i].R = cinza;
            vetor[j][i].G = cinza;
            vetor[j][i].B = cinza;
        }
    }
}

void converteImgNegativa(struct Pixel **vetor, int numRows, int numCols){
    for (int j = 0; j < numRows; j++) {
        for (int i = 0; i < numCols; i++) { 
        {
            Pixel pixel = vetor[j][i];
            int R_novo = 255 - pixel.R;
            int G_novo = 255 - pixel.G;
            int B_novo = 255 - pixel.B;
            vetor[j][i].R = R_novo;
            vetor[j][i].G = G_novo;
            vetor[j][i].B = B_novo;
        }
    }

    }
}

void converteRaioX(struct Pixel **vetor, int numRows, int numCols)
{
    converteCinza(vetor,numRows,numCols);
    for (int j = 0; j < numRows; j++) {
        for (int i = 0; i < numCols; i++) {
            Pixel pixel = vetor[j][i];
            vetor[j][i].R = (int)pow(pixel.R, 1.5);
            vetor[j][i].G = (int)pow(pixel.G, 1.5);
            vetor[j][i].B = (int)pow(pixel.B, 1.5);

        }
    }
}

void converteEnvelhecida(struct Pixel **vetor, int numRows, int numCols)
{
    int fator = 0.1;
    for (int j = 0; j < numRows; j++) {
        for (int i = 0; i < numCols; i++) {
        
            Pixel pixel = vetor[j][i];

            //reduzir azul + reduzir tintura
            int B_reduzido = pixel.B * (1 - fator) - 10;
            //aumentar vermelho e verde + adicionar tintura
            int R_reduzido = pixel.R * (1 + fator) + 10;
            int G_reduzido = pixel.G * (1 + fator) + 10;
            
            vetor[j][i].R = R_reduzido;
            vetor[j][i].G = G_reduzido;
            vetor[j][i].B = B_reduzido;


        }
    }
}

void rotacionar(struct Pixel **vetor, int numRows, int numCols){
    for (int j = 0; j < numRows; j++) {
        for (int i = 0; i < numCols; i++) {
            
            //Transposição da matriz
            Pixel pixel1 = vetor[j][i];
            Pixel pixel2 = vetor[i][j];
            vetor[i][j] = pixel1;
            vetor[j][i] = pixel2;
        }
    }

}

int main() 
{
    //abertura do arquivo
    FILE *fp;
    int c;
    char tipoImg[3];
    int i, j, linhas, colunas, valor, r, g, b;
    
    fp = fopen("golden_novo.ppm","r");

    // Lê o cabeçalho do arquivo
    fscanf(fp, "%s", tipoImg); 					// lê o tipo de imagem P3 
    fscanf(fp, "%d %d", &colunas, &linhas); 	// lê o tamanho da matriz  
    fscanf(fp, "%d", &valor); 					// lê o valor máximo. 

      // Alocação da matriz de pixels RGB
    struct Pixel **matriz = (struct Pixel **)malloc(linhas * sizeof(struct Pixel *));
    for (i = 0; i < linhas; i++) 
    {
        matriz[i] = (struct Pixel *)malloc(colunas * sizeof(struct Pixel));    
    }
    
    // Leitura dos valores de pixel RGB
    for (i = 0; i < linhas; i++) 
    {
        for (j = 0; j < colunas; j++) 
        {
            fscanf(fp, "%d %d %d", &r, &g, &b);
            matriz[i][j].R = r;
            matriz[i][j].G = g;
            matriz[i][j].B = b;
        }
    }

    // Fecha o arquivo.    
    fclose(fp);
    //vetor[0][0].R = 130;
 	//vetor[0][0].G = 150;
 	//vetor[0][0].B = 120;
	//printf("%d - %d - %d", vetor[0][0].R, vetor[0][0].G, vetor[0][0].B);

    int escolha;
    printf("Escolha uma das seguintes opcões:");
    //\n1-Gerar uma imagem tons de cinza\n
    //2-Gerar uma imagem negativa\n
    //3-Gerar uma imagem raio-x\n
    //4-Rotacionar a imagem 90º\n
    //5-Gerar uma imagem envelhecida"); 

    scanf("%d",&escolha);

    if(escolha == 1){
        converteCinza(matriz, linhas,colunas);
    }
    if(escolha == 2){
        converteImgNegativa(matriz, linhas,colunas);
    }
    if(escolha == 3){
        converteRaioX(matriz, linhas,colunas);
    }
    if(escolha == 4){
        rotacionar(matriz, linhas,colunas);
    }
    if(escolha == 5){
        converteEnvelhecida(matriz, linhas,colunas);
    }

       // Criação do novo arquivo PPM
    FILE* fp_novo = fopen("golden_igual.ppm", "w");
    if (fp_novo == NULL) {
        printf("Erro ao criar o arquivo.\n");
        exit(1);
    }

    // Escreve o cabeçalho do arquivo PPM
    fprintf(fp_novo, "P3\n");
    fprintf(fp_novo, "%d %d\n", colunas, linhas);
    fprintf(fp_novo, "%d\n", valor);

    // Escreve os valores RGB da matriz no arquivo
    for (i = 0; i < linhas; i++) 
    {
        for (j = 0; j < colunas; j++) 
        {
            fprintf(fp_novo, "%d %d %d ", matriz[i][j].R, matriz[i][j].G, matriz[i][j].B);
        }
        fprintf(fp_novo, "\n");
    }

    // Fecha o arquivo    
    fclose(fp_novo);
  // Libera a memória alocada para a matriz de pixels RGB
    for (i = 0; i < linhas; i++) 
    {
        free(matriz[i]);
    }
    free(matriz);

    return 0;
}