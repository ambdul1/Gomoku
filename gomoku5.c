#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_stdinc.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdlib.h>

#define TITRE "Mon App"
#define LARGEUR 800
#define HAUTEUR 600
#define GRILLE 9
#define CELLULE 70


typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
}Game;


// Fonction pour Initialiser
bool initialize(Game *game);
void clean(Game *game, int exit_status);
void tracer_grille(Game *game);
void dessiner_piece(Game *game, int x, int y, int joueur);
void dessiner_croix(Game *game, int x, int y, Uint32 color);

int main(){
	Game game;
	if(initialize(&game)){
		clean(&game, EXIT_FAILURE);
	}
	/*printf("bien init \n : ");*/
	/*fflush(stdout);*/
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

		//Efface l ecran
		SDL_SetRenderDrawColor(game.renderer, 0,0,0,255);	
		SDL_RenderClear(game.renderer);

		//Dessine la grille
		tracer_grille(&game);
		/*printf("avant les cercles : \n");*/
		/*fflush(stdout);*/

		for(int i = 0; i < GRILLE; i++){
			for(int j = 0; j < GRILLE; j++){
				if(board[i][j] == 1){
					dessiner_piece(&game, i, j, 1);
				} else if(board[i][j] == 2){	
					dessiner_piece(&game,i, j, 2);	
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
    SDL_SetRenderDrawColor(game->renderer, 0xF1, 0x00, 0x00, 0xFF);  // Noir pour la grille

    // Dessiner les lignes horizontales
    for(int i = 0; i <= GRILLE; i++){
        SDL_RenderDrawLine(game->renderer, 0, i * CELLULE, GRILLE * CELLULE, i * CELLULE);
        SDL_RenderDrawLine(game->renderer, i * CELLULE, 0, i * CELLULE, GRILLE * CELLULE);
    }
}

void dessiner_croix(Game *game, int x, int y, Uint32 color){
	int cx = x * CELLULE + CELLULE / 2;
	int cy = y * CELLULE + CELLULE / 2;
	int taille_croix = CELLULE / 3;
	int epaisseur = 3;

	// Premiere diagonale
	thickLineColor(game->renderer, cx - taille_croix, cy - taille_croix, cx + taille_croix, cy + taille_croix, epaisseur, color);	

	// Seconde diagonale
	thickLineColor(game->renderer, cx - taille_croix, cy + taille_croix, cx + taille_croix, cy - taille_croix, epaisseur,color);
}
void dessiner_piece(Game *game, int x, int y, int joueur){
	if(joueur == 1){
		dessiner_croix(game, x, y, 0xFF0000FF);//croix rouge 
	}else if(joueur == 2){	
		int cx = x * CELLULE + CELLULE / 2;
		int cy = y * CELLULE + CELLULE / 2;
		int radius = CELLULE / 3;
		int radius_interne = radius - 4;
		filledCircleColor(game->renderer, cx, cy, radius, 0xFFFF0000);// bleu
		filledCircleColor(game->renderer, cx, cy, radius_interne, 0xFF000000);// noir  
	}
}

