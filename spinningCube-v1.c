#include <SDL.h>
#include <stdio.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.141592
#endif

//gcc spinningCube-v1.c `sdl2-config --cflags --libs` -o out -lm
/*
otographic projection logic applied
*/
void tornaPixelsBrancos(Uint32*, int, int);
void tornaPixelsPretos(Uint32*, int, int);
void multiplicaMatrizes(double*, double[3][3], double*);
void rotacionaVertices(double[8][3], char, double);
void converteVertices3d_2d(double[3][3], double[3][2], int, int);
void desenhaCuboPorVertices(double[8][2], SDL_Renderer*);

typedef struct estruturaVertice3d {
    double x;
    double y;
    double z;
} estruturaVertice3d;

typedef struct estruturaVertice2d {
    double x;
    double y;
} estruturaVertice2d;

typedef struct estruturaCubo3d {
    estruturaVertice3d vertices[8];
} estruturaCubo3d;

typedef struct estruturaCubo2d {
    estruturaVertice2d vertices[8];
} estruturaCubo2d;

int main(){
    //Inicializando SDL e indicando o uso do subsistema de video (tem varios)
    SDL_Init(SDL_INIT_VIDEO);

    //Obter largura e altura da tela
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    int LARGURA_JANELA = displayMode.w; 
    int ALTURA_JANELA = displayMode.h;


    //Declaracao de variaveis
    int fechar = 0;
    SDL_Event evento;

    //-- Preparar pontos com otographic projection
    estruturaCubo3d cubo3d;
    estruturaCubo2d cubo2d;

    //Pra y < 0
    cubo3d.vertices[0].x = -1;
    cubo3d.vertices[0].y = -1;
    cubo3d.vertices[0].z = 1;

    cubo3d.vertices[1].x = 1;
    cubo3d.vertices[1].y = -1;
    cubo3d.vertices[1].z = 1;

    cubo3d.vertices[2].x = -1;
    cubo3d.vertices[2].y = -1;
    cubo3d.vertices[2].z = -1;

    cubo3d.vertices[3].x = 1;
    cubo3d.vertices[3].y = -1;
    cubo3d.vertices[3].z = -1;
    //Pra y > 0
    cubo3d.vertices[4].x = -1;
    cubo3d.vertices[4].y = 1;
    cubo3d.vertices[4].z = 1;

    cubo3d.vertices[5].x = 1;
    cubo3d.vertices[5].y = 1;
    cubo3d.vertices[5].z = 1;

    cubo3d.vertices[6].x = -1;
    cubo3d.vertices[6].y = 1;
    cubo3d.vertices[6].z = -1;

    cubo3d.vertices[7].x = 1;
    cubo3d.vertices[7].y = 1;
    cubo3d.vertices[7].z = -1;

    double mtzVertices2d[8][2] = {
        {cubo2d.vertices[0].x, cubo2d.vertices[0].y},
        {cubo2d.vertices[1].x, cubo2d.vertices[1].y},
        {cubo2d.vertices[2].x, cubo2d.vertices[2].y},
        {cubo2d.vertices[3].x, cubo2d.vertices[3].y},
        {cubo2d.vertices[4].x, cubo2d.vertices[4].y},
        {cubo2d.vertices[5].x, cubo2d.vertices[5].y},
        {cubo2d.vertices[6].x, cubo2d.vertices[6].y},
        {cubo2d.vertices[7].x, cubo2d.vertices[7].y}
    };

    double mtzVertices3d[8][3] = {
        {cubo3d.vertices[0].x, cubo3d.vertices[0].y, cubo3d.vertices[0].z},
        {cubo3d.vertices[1].x, cubo3d.vertices[1].y, cubo3d.vertices[1].z},
        {cubo3d.vertices[2].x, cubo3d.vertices[2].y, cubo3d.vertices[2].z},
        {cubo3d.vertices[3].x, cubo3d.vertices[3].y, cubo3d.vertices[3].z},
        {cubo3d.vertices[4].x, cubo3d.vertices[4].y, cubo3d.vertices[4].z},
        {cubo3d.vertices[5].x, cubo3d.vertices[5].y, cubo3d.vertices[5].z},
        {cubo3d.vertices[6].x, cubo3d.vertices[6].y, cubo3d.vertices[6].z},
        {cubo3d.vertices[7].x, cubo3d.vertices[7].y, cubo3d.vertices[7].z}
    };

    //Escalonar coordenadas
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 3; j ++) {
            mtzVertices3d[i][j] *= 250; 
        }
    }

    //-- Desenhar os pontos com SDL
    //Criando janela
     SDL_Window *janela = SDL_CreateWindow(
        "Spining cube window 2000",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        LARGURA_JANELA,
        ALTURA_JANELA,
        SDL_WINDOW_FULLSCREEN
    );

    //Criar renderizador
    SDL_Renderer *renderizador = SDL_CreateRenderer(janela, -1, 0);

    //Criar texturas
    //Background
    SDL_Texture *texturaBackground = SDL_CreateTexture(
        renderizador, 
        SDL_PIXELFORMAT_ABGR8888,
        SDL_TEXTUREACCESS_STATIC,
        LARGURA_JANELA,
        ALTURA_JANELA
    );

    //Criar mapa de pixels
    Uint32 *pixels = (Uint32*)malloc((LARGURA_JANELA * ALTURA_JANELA) * sizeof(Uint32));
    //Colorir pixels 
    tornaPixelsBrancos(pixels, LARGURA_JANELA, ALTURA_JANELA);

    //- Main loop (detecao de eventos)
    while(!fechar){
        //Esperar a ocorrencia de um evento
        SDL_WaitEvent(&evento);

        //Se o evento for fechar, fechar a janela
        if (evento.type == SDL_QUIT) {
            fechar = 1;
            break;
        } 

        //- Configurando e mostrando textura do background
        //Carregando pixels na textura
        SDL_UpdateTexture(texturaBackground, NULL, pixels, LARGURA_JANELA * sizeof(Uint32));
        //Manda o renderizador limpar a tela (preenche com a cor configurada na declaracao)
        SDL_RenderClear(renderizador);
        //Copiar texturaBackground para o renderizador
        SDL_RenderCopy(renderizador, texturaBackground, NULL, NULL);

        //Converter vertices para 2d
        converteVertices3d_2d(mtzVertices3d, mtzVertices2d, LARGURA_JANELA, ALTURA_JANELA);

        //Desenhar cubo
        desenhaCuboPorVertices(mtzVertices2d, renderizador);
    
        //Rotacionar vertices 3d
        rotacionaVertices(mtzVertices3d, 'x', 1);
        rotacionaVertices(mtzVertices3d, 'y', 1);
        rotacionaVertices(mtzVertices3d, 'z', 1);

        //Carrega o back-buffer para o front-buffer
        SDL_RenderPresent(renderizador);
    }

    //Chamadas de limpeza
    free(pixels);
    SDL_DestroyRenderer(renderizador);
    SDL_DestroyTexture(texturaBackground);

    //Finalizando o SDL
    SDL_Quit();
    return 0;
}

void tornaPixelsBrancos(Uint32 *pixels, int LARGURA_JANELA, int ALTURA_JANELA){
    for(int i = 0; i < (LARGURA_JANELA * ALTURA_JANELA); i ++){
        pixels[i] = 0xFFFFFFFF;
    }
}

void tornaPixelsPretos(Uint32 *pixels, int LARGURA_JANELA, int ALTURA_JANELA){
    for(int i = 0; i < (LARGURA_JANELA * ALTURA_JANELA); i ++){
        pixels[i] = 0;
    }
}

void multiplicaMatrizes(double *mtz3x1, double mtz3x3[3][3], double *mtzResultado){
    //3x1 * 3x3
    double somaLinha;
    for (int i = 0; i < 3; i ++) {
        somaLinha = 0;
        for (int j = 0; j < 3; j ++) {
            somaLinha += mtz3x3[i][j] * mtz3x1[j];
        }
        mtzResultado[i] = somaLinha;
    }
}

void rotacionaVertices(double mtzVertices3d[8][3], char eixo, double angulo){
    //Converter angulo para radiano
    double anguloRad = angulo * (M_PI / 180);

    double mtzResultado[3] = {0, 0, 0};

    if (eixo == 'x') {
        double mtzRotacao[3][3] = {
            {1, 0, 0},
            {0, cos(anguloRad), -1 * sin(anguloRad)},
            {0, sin(anguloRad), cos(anguloRad)}
        };
        //Multiplicar cada linha de mtzVertices3d por mtzRotacaoX, atualizar a linha
        for (int i = 0; i < 8; i++) {
            //Multiplicar vertice pela matriz de rotacao, armazenar em mtzResultado
            multiplicaMatrizes(mtzVertices3d[i], mtzRotacao, mtzResultado);
            //Atualizar o vertice rotacionado na mtzVertice3d
            for (int j = 0; j < 3; j++) {
                mtzVertices3d[i][j] = mtzResultado[j];
            }
        }
    }
    else if (eixo == 'y') {
        double mtzRotacao[3][3] = {
            {cos(anguloRad), 0, sin(anguloRad)},
            {0, 1, 0},
            {-1 * sin(anguloRad), 0, cos(anguloRad)}
        };
        //Multiplicar cada linha de mtzVertices3d por mtzRotacaoX, atualizar a linha
        for (int i = 0; i < 8; i++) {
            //Multiplicar vertice pela matriz de rotacao, armazenar em mtzResultado
            multiplicaMatrizes(mtzVertices3d[i], mtzRotacao, mtzResultado);
            //Atualizar o vertice rotacionado na mtzVertice3d
            for (int j = 0; j < 3; j++) {
                mtzVertices3d[i][j] = mtzResultado[j];
            }
        }
    }
    else if (eixo == 'z') {
        double mtzRotacao[3][3] = {
            {cos(anguloRad), -1 * sin(anguloRad), 0},
            {sin(anguloRad), cos(anguloRad), 0},
            {0, 0, 1}
        };
        //Multiplicar cada linha de mtzVertices3d por mtzRotacaoX, atualizar a linha
        for (int i = 0; i < 8; i++) {
            //Multiplicar vertice pela matriz de rotacao, armazenar em mtzResultado
            multiplicaMatrizes(mtzVertices3d[i], mtzRotacao, mtzResultado);
            //Atualizar o vertice rotacionado na mtzVertice3d
            for (int j = 0; j < 3; j++) {
                mtzVertices3d[i][j] = mtzResultado[j];
            }
        }
    }
}

void converteVertices3d_2d(double mtzVertices3d[3][3], double mtzVertices2d[3][2], int LARGURA_JANELA, int ALTURA_JANELA){
    //Converter para duas dimensoes
    //Criar mtzTransformacaoOrtografica
    double mtzTransformacaoOrtografica[3][3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 0},
    };

    //Multiplicar cada vetor de mtzVertices3d pela mtzTransformacaoOrtografica
    double mtzResultado[3] = {0, 0, 0};

    for (int i = 0; i < 8; i ++) {
        //Converter vertice atual
        multiplicaMatrizes(mtzVertices3d[i], mtzTransformacaoOrtografica, mtzResultado);
        
        //Armazenar vertice convertido
        for (int j = 0; j < 2; j ++) {
            mtzVertices2d[i][j] = mtzResultado[j]; 
        }
        
    }

    //Mostrando pontos
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 3; j ++) {
            j == 2 ? printf("%f\n", mtzVertices3d[i][j]) : printf("%f", mtzVertices3d[i][j]);
        }
    }
    printf("\n");
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 2; j ++) {
            j == 1 ? printf("%f\n", mtzVertices2d[i][j]) : printf("%f", mtzVertices2d[i][j]);
        }
    }

    //Ajustar intervalo dos valores (transformacao de janela para viewport)
    //Dados
    double xJanelaMax = 1920, xJanelaMin = 0;
    double yJanelaMax = 1080, yJanelaMin = 0;

    //Para cada vertice de mtzVertices2d
    for (int i = 0; i < 8; i ++) {
        //Calcular projecoes
        //Calculo projecao em x
        double proporcaoX = (mtzVertices2d[i][0] - xJanelaMin)/(xJanelaMax - xJanelaMin);
        //Calculo projecao em y
        double proporcaoY = (mtzVertices2d[i][1] - yJanelaMin)/(yJanelaMax - yJanelaMin);
        printf("Proporcao x: %f, Proporcao y: %f\n", proporcaoX, proporcaoY);
        //Calcular coordenadas na viewport
        //Calculo coordenada x
        mtzVertices2d[i][0] = (proporcaoX * LARGURA_JANELA + xJanelaMin) + (LARGURA_JANELA / 2);
        //Calculo coordenada y
        mtzVertices2d[i][1] = (proporcaoY * ALTURA_JANELA + yJanelaMin) + (ALTURA_JANELA / 2);
    }

    //Mostrar matriz com coordenadas para viewport
    printf("\nCoordenadas para viewport:\n");
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 2; j ++) {
            j == 1 ? printf("%f\n", mtzVertices2d[i][j]) : printf("%f", mtzVertices2d[i][j]);
        }
    }

    //TODO Atualizar cubo2d (jogar dados da mtz pra estrutura de dados)
}

void desenhaCuboPorVertices(double mtzVertices2d[8][2], SDL_Renderer *renderizador){
        //TODO - Colocar isso em um FOR
        //Vertices 0 -> 3
        SDL_RenderDrawLine(renderizador, 
        mtzVertices2d[0][0], mtzVertices2d[0][1], 
        mtzVertices2d[1][0], mtzVertices2d[1][1]);

        SDL_RenderDrawLine(renderizador, 
        mtzVertices2d[1][0], mtzVertices2d[1][1], 
        mtzVertices2d[3][0], mtzVertices2d[3][1]);

        SDL_RenderDrawLine(renderizador, 
        mtzVertices2d[3][0], mtzVertices2d[3][1], 
        mtzVertices2d[2][0], mtzVertices2d[2][1]);

        SDL_RenderDrawLine(renderizador, 
        mtzVertices2d[2][0], mtzVertices2d[2][1], 
        mtzVertices2d[0][0], mtzVertices2d[0][1]);
        //Vertices 4 -> 7
        SDL_RenderDrawLine(renderizador, 
        mtzVertices2d[4][0], mtzVertices2d[4][1], 
        mtzVertices2d[5][0], mtzVertices2d[5][1]);

        SDL_RenderDrawLine(renderizador, 
        mtzVertices2d[5][0], mtzVertices2d[5][1], 
        mtzVertices2d[7][0], mtzVertices2d[7][1]);

        SDL_RenderDrawLine(renderizador, 
        mtzVertices2d[7][0], mtzVertices2d[7][1], 
        mtzVertices2d[6][0], mtzVertices2d[6][1]);

        SDL_RenderDrawLine(renderizador, 
        mtzVertices2d[6][0], mtzVertices2d[6][1], 
        mtzVertices2d[4][0], mtzVertices2d[4][1]);
        //Conectar faces
        SDL_RenderDrawLine(renderizador, 
        mtzVertices2d[1][0], mtzVertices2d[1][1], 
        mtzVertices2d[5][0], mtzVertices2d[5][1]);

        SDL_RenderDrawLine(renderizador, 
        mtzVertices2d[0][0], mtzVertices2d[0][1], 
        mtzVertices2d[4][0], mtzVertices2d[4][1]);

        SDL_RenderDrawLine(renderizador, 
        mtzVertices2d[2][0], mtzVertices2d[2][1], 
        mtzVertices2d[6][0], mtzVertices2d[6][1]);

        SDL_RenderDrawLine(renderizador, 
        mtzVertices2d[3][0], mtzVertices2d[3][1], 
        mtzVertices2d[7][0], mtzVertices2d[7][1]);
}