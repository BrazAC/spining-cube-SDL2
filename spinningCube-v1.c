#include <SDL.h>
#include <stdio.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.141592
#endif

//gcc spinningCube-v1.c `sdl2-config --cflags --libs` -o out
/*
otographic projection logic applied
*/
void tornaPixelsBrancos(Uint32*, int, int);
void tornaPixelsPretos(Uint32*, int, int);

typedef struct estruturaVertice3d {
    float x;
    float y;
    float z;
} estruturaVertice3d;

typedef struct estruturaVertice2d {
    float x;
    float y;
} estruturaVertice2d;

typedef struct estruturaCubo3d {
    estruturaVertice3d vertices[8];
} estruturaCubo3d;

typedef struct estruturaCubo2d {
    estruturaVertice2d vertices[8];
} estruturaCubo2d;

int* multiplicaMatrizes(int*, int*);

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

    //-- Preparar pontos com weak perspective projection
    estruturaCubo3d cubo3d;
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

    float mtzVertices3d[8][3] = {
        {cubo3d.vertices[0].x, cubo3d.vertices[0].y, cubo3d.vertices[0].z},
        {cubo3d.vertices[1].x, cubo3d.vertices[1].y, cubo3d.vertices[1].z},
        {cubo3d.vertices[2].x, cubo3d.vertices[2].y, cubo3d.vertices[2].z},
        {cubo3d.vertices[3].x, cubo3d.vertices[3].y, cubo3d.vertices[3].z},
        {cubo3d.vertices[4].x, cubo3d.vertices[4].y, cubo3d.vertices[4].z},
        {cubo3d.vertices[5].x, cubo3d.vertices[5].y, cubo3d.vertices[5].z},
        {cubo3d.vertices[6].x, cubo3d.vertices[6].y, cubo3d.vertices[6].z},
        {cubo3d.vertices[7].x, cubo3d.vertices[7].y, cubo3d.vertices[7].z}
    };

    //Converter para duas dimensoes
    estruturaCubo2d cubo2d;
    int mtzVertices2d[8][2] = {
        {cubo2d.vertices[0].x, cubo2d.vertices[0].y},
        {cubo2d.vertices[1].x, cubo2d.vertices[1].y},
        {cubo2d.vertices[2].x, cubo2d.vertices[2].y},
        {cubo2d.vertices[3].x, cubo2d.vertices[3].y},
        {cubo2d.vertices[4].x, cubo2d.vertices[4].y},
        {cubo2d.vertices[5].x, cubo2d.vertices[5].y},
        {cubo2d.vertices[6].x, cubo2d.vertices[6].y},
        {cubo2d.vertices[7].x, cubo2d.vertices[7].y}
    };
    
    //Criar mtzTransformacao3d_2d
    //Calcular f (FOV VERTICAL)
    double graus_FOV = 90;
    double radian_FOV = graus_FOV * (M_PI / 180.0);
    double f = ALTURA_JANELA / (2 * tan(radian_FOV));

    //Criar matriz
    int mtzTransformacaoOrtografica[3][3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 0},
    };

    //Multiplicar cada vetor de mtzVertices3d pela mtzTransformacaoOrtografica
    for (int i = 0; i < 8; i ++) {
        int *mtzResultado;
        mtzResultado = multiplicaMatrizes(mtzVertices2d[i], mtzTransformacaoOrtografica);
        mtzVertices2d[i][0] = mtzResultado[0];
        mtzVertices2d[i][1] = mtzResultado[1];
    }

    //-- Mostrar os pontos com SDL
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

        //Desenhar linhas
        SDL_RenderDrawLine(renderizador, 100, 100, 100, 300);
        SDL_RenderDrawLine(renderizador, 100, 100, 300, 100);

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

int* multiplicaMatrizes(int* mtz0, int* mtz1){
    //2x1 * 3x3
    
}