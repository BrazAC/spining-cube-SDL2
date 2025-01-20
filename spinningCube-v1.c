#include <SDL.h>
#include <stdio.h>

//gcc spinningCube-v1.c `sdl2-config --cflags --libs` -o out

void tornaPixelsBrancos(Uint32*, int, int);
void tornaPixelsPretos(Uint32*, int, int);

int main(){
    //Declaracao de variaveis
    int fechar = 0;
    SDL_Event evento;

    //Inicializando SDL e indicando o uso do subsistema de video (tem varios)
    SDL_Init(SDL_INIT_VIDEO);

    //Obter largura e altura da tela
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    int LARGURA_JANELA = displayMode.w; 
    int ALTURA_JANELA = displayMode.h;

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