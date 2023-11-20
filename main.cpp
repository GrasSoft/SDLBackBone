
//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <cstring>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//The window we'll be rendering to
SDL_Window* window = NULL;

//The surface contained by the window
SDL_Surface* screenSurface = NULL;

//The surface of the loaded media
SDL_Surface* optimizedSurface = NULL;

bool loadMediaPNG() {
	bool success = true;

	char path[] = "/home/gras/CLionProjects/SDLBonanza/resources/cock.png";


	SDL_Surface* loadSurface = IMG_Load(path);

	if(loadSurface == NULL) {
		printf("Unable to load image: %s with error msg: %s", path, IMG_GetError());
		success = false;
	}
	//optimize image on load to remove conversion when blitting
	optimizedSurface = SDL_ConvertSurface(loadSurface, screenSurface->format, NULL);

	if(optimizedSurface == NULL) {
		printf("The optimised image could not be computed with error: %s", SDL_GetError());
		success = false;
	}
	SDL_FreeSurface(loadSurface);

	return success;
}


bool loadMediaBMP() {
	bool success = true;

	char path[] = "/home/gras/CLionProjects/SDLBonanza/resources/cock.bmp";


	SDL_Surface* loadSurface = SDL_LoadBMP(path);

	if(loadSurface == NULL) {
		printf("Unable to load image: %s with error msg: %s", path, SDL_GetError());
		success = false;
	}
	//optimize image on load to remove conversion when blitting
	optimizedSurface = SDL_ConvertSurface(loadSurface, screenSurface->format, NULL);

	if(optimizedSurface == NULL) {
		printf("The optimised image could not be computed with error: %s", SDL_GetError());
		success = false;
	}
	SDL_FreeSurface(loadSurface);

	return success;
}

bool init() {
	bool success = true;

	//initialize sdl
	if( SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize with error code: %s", SDL_GetError());
		success = false;
	}
	else {
		int pngFlag = IMG_INIT_PNG;
		if(! (IMG_Init(pngFlag) & pngFlag)) {
			printf("The img init flag from SDL2_IMAGE could not be initialised with error: %s", IMG_GetError());
			success = false;
		}
			else {
				window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

				if(window == NULL) {
					printf("The window could not be created with error: %s", SDL_GetError());
					success = false;
				}
				else {
					screenSurface = SDL_GetWindowSurface(window);
					if(screenSurface == NULL) {
						printf("The screen surface could not be created with error: %s", SDL_GetError());
						success = false;
				}
			}
		}

	}
	return success;
}

void close() {
	//dealocate surface
	SDL_FreeSurface(screenSurface);
	screenSurface = NULL;

	//dealocate window
	SDL_DestroyWindow(window);
	window = NULL;

	//quit sdl subsystem
	SDL_Quit();
}

void negative() {
	for(int i=0; i<optimizedSurface->h; i++) {
		for(int j=0; j<optimizedSurface->w; j++) {
			Uint8* pixel = ((Uint8*)optimizedSurface->pixels + i * optimizedSurface->pitch + j * optimizedSurface->format->BytesPerPixel);
			for(int k=0; k< optimizedSurface->format->BytesPerPixel; k++) {
				*(pixel+k) = 0xFF - *(pixel+k);
			}
		}
	}
}

int main( int argc, char* args[] )
{
	if(!init()) {
		printf("The initialization failed!");
	}
	else
		if(!loadMediaPNG()) {
			printf("The media could not be loaded!");
		}
		else
		{
			//Update the surface
			SDL_UpdateWindowSurface( window );

			//Hack to get window to stay up
			SDL_Event e;
			bool quit = false;
			while( quit == false ) {
				while( SDL_PollEvent( &e ) ) {
					if( e.type == SDL_QUIT )
						quit = true;
					if( e.type == SDL_KEYDOWN) {
						switch(e.key.keysym.sym) {
							case(SDLK_UP):
								negative();

								SDL_Rect rect;
								rect.x = 0;
								rect.y = 0;
								rect.w = SCREEN_WIDTH;
								rect.h = SCREEN_HEIGHT;
								//the dstrect does not take into account the width and height, only postition, RTFM
								SDL_BlitSurface(optimizedSurface, NULL, screenSurface, &rect);

								//Update the surface
								SDL_UpdateWindowSurface( window );

								break;
							default:
								break;
						}
					}

				}
			}
		}

	//free and destroy window
	close();

	return 0;
}