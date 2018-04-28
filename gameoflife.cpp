#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <array>
#include <unistd.h>
#include <time.h>

#define ARRAY_SIZE 320
#define CELL_SIZE 2

static constexpr auto _WIDTH = 640;
static constexpr auto _HEIGHT = 640;

template<typename T>
using matrix = std::array<std::array<T, ARRAY_SIZE>, ARRAY_SIZE>;

class GameOfLife {
	struct Cell {
		SDL_Rect rect;

		typedef enum {
			DEAD, ALIVE
		} STATE;
	
		STATE state;

		void init(int _x, int _y, int _w, int _h) { 
			state = DEAD;
			rect.x = CELL_SIZE * _x; rect.y = CELL_SIZE * _y;
			rect.w = _w; rect.h = _h;

		}
		void draw(SDL_Surface* surface) {
			if(state == DEAD)
				SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0xFF,0,0));
			else
				SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0,0xFF,0));

		}
	};

	matrix<Cell> t;
	matrix<Cell> t1;

	void checkSurroundings(int, int);
	void draw(SDL_Window*, SDL_Surface*);

	public:

	GameOfLife();
	void run(SDL_Window*, SDL_Surface*, SDL_Event);
};

GameOfLife::GameOfLife() {
	for(auto i = 0; i<ARRAY_SIZE; i++) {
		for(auto j=0; j<ARRAY_SIZE; j++) {
			t[i][j].init(i,j,CELL_SIZE,CELL_SIZE);
			t1[i][j].init(i,j,CELL_SIZE,CELL_SIZE);
		}
	}

	t[1][1].state = Cell::ALIVE;
	t[1][2].state = Cell::ALIVE;
	t[1][3].state = Cell::ALIVE;
	
	srand(time(NULL));
	int range = ARRAY_SIZE*(ARRAY_SIZE/2);
	for(auto i = 0; i<range; i++) {
		int a = rand()%ARRAY_SIZE;
		int b = rand()%ARRAY_SIZE;
		t[a][b].state = Cell::ALIVE;		
	}
}

void GameOfLife::checkSurroundings(int i, int j) {
	int counter = 0;
	if(i==0 && j==0) {					// CORNERS
		if(t[1][0].state == Cell::ALIVE)
			counter++;
		if(t[1][1].state == Cell::ALIVE)
			counter++;
		if(t[0][1].state == Cell::ALIVE)
			counter++;
		if(t[ARRAY_SIZE-1][1].state == Cell::ALIVE)
			counter++;
		if(t[ARRAY_SIZE-1][0].state == Cell::ALIVE)
			counter++;
		if(t[ARRAY_SIZE-1][ARRAY_SIZE-1].state == Cell::ALIVE)
			counter++;
		if(t[0][ARRAY_SIZE-1].state == Cell::ALIVE)
			counter++;
		if(t[1][ARRAY_SIZE-1].state == Cell::ALIVE)
			counter++;
	} else if (i == ARRAY_SIZE-1 && j == 0) {
		if(t[ARRAY_SIZE-2][0].state == Cell::ALIVE)
			counter++;
		if(t[ARRAY_SIZE-2][1].state == Cell::ALIVE)
			counter++;
		if(t[ARRAY_SIZE-1][1].state == Cell::ALIVE)
			counter++;
		if(t[0][1].state == Cell::ALIVE)
			counter++;
		if(t[0][0].state == Cell::ALIVE)
			counter++;
		if(t[0][ARRAY_SIZE-1].state == Cell::ALIVE)
			counter++;
		if(t[ARRAY_SIZE-1][ARRAY_SIZE-1].state == Cell::ALIVE)
			counter++;
		if(t[ARRAY_SIZE-2][ARRAY_SIZE-1].state == Cell::ALIVE)
			counter++;
	} else if (i == ARRAY_SIZE-1 && j == ARRAY_SIZE-1) {
		if(t[ARRAY_SIZE-2][ARRAY_SIZE-1].state == Cell::ALIVE)
			counter++;
		if(t[ARRAY_SIZE-2][ARRAY_SIZE-2].state == Cell::ALIVE)
			counter++;
		if(t[ARRAY_SIZE-1][ARRAY_SIZE-2].state == Cell::ALIVE)
			counter++;
		if(t[0][ARRAY_SIZE-2].state == Cell::ALIVE)
			counter++;
		if(t[0][ARRAY_SIZE-1].state == Cell::ALIVE)
			counter++;
		if(t[0][0].state == Cell::ALIVE)
			counter++;
		if(t[ARRAY_SIZE-1][0].state == Cell::ALIVE)
			counter++;
		if(t[ARRAY_SIZE-2][0].state == Cell::ALIVE)
			counter++;
	} else if (i == 0 && j == ARRAY_SIZE-1) {
		if(t[1][ARRAY_SIZE-1].state == Cell::ALIVE)
			counter++;
		if(t[1][ARRAY_SIZE-2].state == Cell::ALIVE)
			counter++;
		if(t[0][ARRAY_SIZE-2].state == Cell::ALIVE)
			counter++;
		if(t[ARRAY_SIZE-1][ARRAY_SIZE-2].state == Cell::ALIVE)
			counter++;
		if(t[ARRAY_SIZE-1][ARRAY_SIZE-1].state == Cell::ALIVE)
			counter++;
		if(t[ARRAY_SIZE-1][0].state == Cell::ALIVE)
			counter++;
		if(t[0][0].state == Cell::ALIVE)
			counter++;
		if(t[1][0].state == Cell::ALIVE)
			counter++;
	} else if (i==0 && (j!=0 || j!=ARRAY_SIZE-1)) {		// BOUNDS
		if(t[0][j-1].state == Cell::ALIVE)
			counter++;
		if(t[1][j-1].state == Cell::ALIVE)
			counter++;
		if(t[1][j].state == Cell::ALIVE)
			counter++;
		if(t[1][j+1].state == Cell::ALIVE)
			counter++;
		if(t[0][j+1].state == Cell::ALIVE)
			counter++;
		if(t[ARRAY_SIZE-1][j+1].state == Cell::ALIVE)
			counter++;
		if(t[ARRAY_SIZE-1][j].state == Cell::ALIVE)
			counter++;
		if(t[ARRAY_SIZE-1][j-1].state == Cell::ALIVE)
			counter++;
	} else if (j==0 && (i!=0 || i!=ARRAY_SIZE-1)) {
		if(t[i-1][0].state == Cell::ALIVE)
			counter++;
		if(t[i-1][1].state == Cell::ALIVE)
			counter++;
		if(t[i][1].state == Cell::ALIVE)
			counter++;
		if(t[i+1][1].state == Cell::ALIVE)
			counter++;
		if(t[i+1][0].state == Cell::ALIVE)
			counter++;
		if(t[i+1][ARRAY_SIZE-1].state == Cell::ALIVE)
			counter++;
		if(t[i][ARRAY_SIZE-1].state == Cell::ALIVE)
			counter++;
		if(t[i-1][ARRAY_SIZE-1].state == Cell::ALIVE)
			counter++;
	} else if (i==ARRAY_SIZE-1 && (j!=0 || j!=ARRAY_SIZE-1)) {
		if(t[ARRAY_SIZE-1][j-1].state == Cell::ALIVE)
			counter++;
		if(t[ARRAY_SIZE-2][j-1].state == Cell::ALIVE)
			counter++;
		if(t[ARRAY_SIZE-2][j].state == Cell::ALIVE)
			counter++;
		if(t[ARRAY_SIZE-2][j+1].state == Cell::ALIVE)
			counter++;
		if(t[ARRAY_SIZE-1][j+1].state == Cell::ALIVE)
			counter++;
		if(t[0][j+1].state == Cell::ALIVE)
			counter++;
		if(t[0][j].state == Cell::ALIVE)
			counter++;
		if(t[0][j-1].state == Cell::ALIVE)
			counter++;
	} else if (j==ARRAY_SIZE-1 && (i!=0 || i!=ARRAY_SIZE-1)) {
		if(t[i-1][ARRAY_SIZE-1].state == Cell::ALIVE)
			counter++;
		if(t[i-1][ARRAY_SIZE-2].state == Cell::ALIVE)
			counter++;
		if(t[i][ARRAY_SIZE-2].state == Cell::ALIVE)
			counter++;
		if(t[i+1][ARRAY_SIZE-2].state == Cell::ALIVE)
			counter++;
		if(t[i+1][ARRAY_SIZE-1].state == Cell::ALIVE)
			counter++;
		if(t[i+1][0].state == Cell::ALIVE)
			counter++;
		if(t[i][0].state == Cell::ALIVE)
			counter++;
		if(t[i-1][0].state == Cell::ALIVE)
			counter++;
	} else {						// MIDDLE
		if(t[i-1][j-1].state == Cell::ALIVE)
			counter++;
		if(t[i][j-1].state == Cell::ALIVE)
			counter++;
		if(t[i+1][j-1].state == Cell::ALIVE)
			counter++;
		if(t[i+1][j].state == Cell::ALIVE)
			counter++;
		if(t[i+1][j+1].state == Cell::ALIVE)
			counter++;
		if(t[i][j+1].state == Cell::ALIVE)
			counter++;
		if(t[i-1][j+1].state == Cell::ALIVE)
			counter++;
		if(t[i-1][j].state == Cell::ALIVE)
			counter++;
	}

	// game of life rules
	if((t[i][j].state == Cell::DEAD) && counter==3)
		t1[i][j].state = Cell::ALIVE;
	else if ((t[i][j].state == Cell::ALIVE) && (counter==3||counter==2))
		t1[i][j].state = Cell::ALIVE;
	else if((t[i][j].state == Cell::ALIVE) && (counter>3||counter<2))
		t1[i][j].state = Cell::DEAD;
	else if((t[i][j].state == Cell::DEAD) && (counter != 3))
		t1[i][j].state = Cell::DEAD;
	else
		t1[i][j].state = Cell::DEAD;
}

void GameOfLife::draw(SDL_Window* window, SDL_Surface* surf) {
	for(int i=0; i<ARRAY_SIZE; i++) {
		for(int j=0; j<ARRAY_SIZE; j++) {
			t[i][j].draw(surf);
		}
	}
	SDL_UpdateWindowSurface(window);
}

void GameOfLife::run(SDL_Window* window, SDL_Surface* surface, SDL_Event e) {
	bool quit, pause, candraw;
	quit = pause = candraw = false;
	int xm, ym;
	do {
		if(SDL_PollEvent(&e) != 0) {
			if(e.type == SDL_QUIT) {
				quit = true;
			} else if(e.type == SDL_KEYDOWN) {
				if(e.key.keysym.sym == SDLK_p) {
					if(!pause) printf("pause\n");
					else printf("unpause\n");
					pause = !pause;
				} else if(pause && e.key.keysym.sym == SDLK_o) {
					if(!candraw) printf("can draw\n");
					else printf("cant draw\n");
					candraw = !candraw;
				}
			} else if(candraw && e.type == SDL_MOUSEMOTION) {
				SDL_GetMouseState(&xm,&ym);
				if(xm>=0 && xm <= ARRAY_SIZE * CELL_SIZE)
					if(ym>=0 && ym <= ARRAY_SIZE * CELL_SIZE)
						t[xm/CELL_SIZE][ym/CELL_SIZE].state = Cell::ALIVE;
			}
		} else {	
			draw(window, surface);
			if(!pause){
				SDL_Delay(100);
				for(int i=0; i<ARRAY_SIZE; i++)
					for(int j=0; j<ARRAY_SIZE; j++)
						checkSurroundings(i,j);
	
				t = t1;
			}
		}
	} while(!quit);
}

int main() {

	SDL_Window* window = nullptr;
	SDL_Surface* surface = nullptr;
	SDL_Event event;

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Error:%s",SDL_GetError());
	} else {
		window = SDL_CreateWindow("GameOfLife",
					SDL_WINDOWPOS_UNDEFINED,
					SDL_WINDOWPOS_UNDEFINED,
					_WIDTH,
					_HEIGHT,
					SDL_WINDOW_SHOWN);
		if(!window) {
			printf("Error:%s", SDL_GetError());
		} else {
			surface = SDL_GetWindowSurface(window);
			SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xFF,0xFF,0xFF));
			SDL_UpdateWindowSurface(window);
			SDL_Delay(500);

		}
	}

	GameOfLife * gol = new GameOfLife();
	gol->run(window, surface, event);

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
