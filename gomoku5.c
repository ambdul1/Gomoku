#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define TITRE "Mon App"
#define LARGEUR 800
#define HAUTEUR 600
#define GRILLE 12
#define CELLULE 50


typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
}Game;


// Fonction pour Initialiser
bool initialize(Game *game);
void clean(Game *game, int exit_status);
void tracer_grille(Game *game);
void dessiner_piece(Game *game, int x, int y, Uint32 color);
void drawcircle(Game *game, int cx, int cy, int radius, int thickness, SDL_Color color);

int main(){
	Game game;
	if(initialize(&game)){
		clean(&game, EXIT_FAILURE);
	}
	printf("bien init \n : ");
	fflush(stdout);
	int board[GRILLE][GRILLE] = {0};
	bool isBlackTurn = true;
	bool running = true;
	while(running){
		SDL_Event event;
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				clean(&game, EXIT_SUCCESS);
			} else if(event.type == SDL_KEYDOWN){
				SDL_Keycode key = event.key.keysym.sym;
				if(key == SDLK_q){
					clean(&game, EXIT_SUCCESS);
				}

			}else if(event.type == SDL_MOUSEBUTTONDOWN){
				int x = event.button.x / CELLULE;
				int y = event.button.y / CELLULE;
				if(x < GRILLE && y < GRILLE && board[x][y] == 0){
					board[x][y] = isBlackTurn ? 1 : 2;
					isBlackTurn = !isBlackTurn;
				}
			}
		}
		SDL_Color color = {255,0,0,255};
		SDL_SetRenderDrawColor(game.renderer, 0,0,0,255);	
		SDL_RenderClear(game.renderer);
		SDL_SetRenderDrawColor(game.renderer, 255,255,255,255);
		tracer_grille(&game);
		printf("avant les cercles : \n");
		fflush(stdout);

		for(int i = 0; i < GRILLE; i++){
			for(int j = 0; j < GRILLE; j++){
				if(board[i][j] == 1){
					dessiner_piece(&game, i, j, 0xFF00FFFF);
				} else if(board[i][j] == 2){
					dessiner_piece(&game, i, j, 0xFFFFFFFF);
				}
			}
		}

		SDL_RenderPresent(game.renderer);
	
	}	


	clean(&game, EXIT_SUCCESS);
	return 0;
}

void clean(Game *game, int exit_status){
	SDL_DestroyRenderer(game->renderer);
	SDL_DestroyWindow(game->window);
	SDL_Quit();
	exit(exit_status);
}

bool initialize(Game *game){
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
		SDL_Log("Erreur lors de l'Initialisation de la SDL : %s\n", SDL_GetError());
		return true;
	}
	game->window = SDL_CreateWindow(TITRE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GRILLE * CELLULE, GRILLE * CELLULE, SDL_WINDOW_SHOWN);
	if(!game->window){
		SDL_Log("Erreur lors de la cretion de la fenetre : %s\n", SDL_GetError());
		return true;
	}
	game->renderer = SDL_CreateRenderer(game->window, -1 , SDL_RENDERER_ACCELERATED);
	if(!game->renderer){
		SDL_Log("Erreur lors de la cretion du rendu graphique : %s\n", SDL_GetError());
		return true;
	}
	return false;
}
void tracer_grille(Game *game){
    SDL_SetRenderDrawColor(game->renderer, 0, 255, 0, 255);  // Noir pour la grille

    // Dessiner les lignes horizontales
    for(int i = 0; i <= GRILLE; i++){
        SDL_RenderDrawLine(game->renderer, 0, i * CELLULE, GRILLE * CELLULE, i * CELLULE);
        SDL_RenderDrawLine(game->renderer, i * CELLULE, 0, i * CELLULE, GRILLE * CELLULE);
    }
}

void dessiner_piece(Game *game, int x, int y, Uint32 color){
	int cx = x * CELLULE + CELLULE / 2;
	int cy = y * CELLULE + CELLULE / 2;
	int radius = CELLULE / 3;
	filledCircleColor(game->renderer, cx, cy, radius, color);
}


