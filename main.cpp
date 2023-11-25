
//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <cstring>
#include <filesystem>

#include "Texture.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//The window we'll be rendering to
SDL_Window* window = NULL;

//The surface contained by the window
SDL_Surface* screen_surface = NULL;

//The surface of the loaded media
SDL_Surface* optimized_surface = NULL;

//The renderer of the screen
SDL_Renderer* renderer = NULL;

Texture texture_obj;

SDL_Surface* load_media() {
	bool success = true;

	char path[] = "/home/gras/CLionProjects/SDLBonanza/resources/cock.png";
	SDL_Surface* load_surface = NULL;
	const char* extention = std::filesystem::path(path).extension().c_str();
	if(!strcmp(extention, ".png"))
		load_surface = IMG_Load(path);
	else if(!strcmp(extention, ".bmp"))
			load_surface = SDL_LoadBMP(path);

	if(load_surface == NULL) {
		printf("Unable to load image: %s with error msg: %s", path, IMG_GetError());
		success = false;
	}
	//optimize image on load to remove conversion when blitting
	SDL_Surface* optimized = SDL_ConvertSurface(load_surface, screen_surface->format, 0);

	if(optimized == NULL) {
		printf("The optimised image could not be computed with error: %s", SDL_GetError());
		success = false;
	}
	SDL_FreeSurface(load_surface);

	return optimized;

}


bool init() {
	bool success = true;

	//initialize sdl
	if( SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize with error code: %s", SDL_GetError());
		success = false;
	}
	else {
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if(window == NULL) {
			printf("The window could not be created with error: %s", SDL_GetError());
			success = false;
		}
		else {
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if(renderer == NULL) {
				printf("The renderer could not be created with error: %s\n", SDL_GetError());
				success = false;
			}
			else {

					int pngFlag = IMG_INIT_PNG;
					if(! (IMG_Init(pngFlag) & pngFlag)) {
						printf("The img init flag from SDL2_IMAGE could not be initialised with error: %s", IMG_GetError());
						success = false;
					}
					else {
							//This section can not be used in combination with the hardware renderer
							//screen_surface = SDL_GetWindowSurface(window);
							//if(screen_surface == NULL) {
							//	printf("The screen surface could not be created with error: %s", SDL_GetError());
							//	success = false;
							//}
					}
			}
		}

	}
	return success;
}

void close() {
	//dealocate surface
	SDL_FreeSurface(screen_surface);
	screen_surface = NULL;

	//dealocate window
	SDL_DestroyWindow(window);
	window = NULL;

	//dealocate renderer
	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	//quit sdl subsystem and image library
	IMG_Quit();
	SDL_Quit();
}

//this method uses the CPU to render images to the window
void software_render() {
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = SCREEN_WIDTH;
	rect.h = SCREEN_HEIGHT;
	//the dstrect does not take into account the width and height, only postition, RTFM
	SDL_BlitSurface(optimized_surface, NULL, screen_surface, &rect);

	//Update the surface
	SDL_UpdateWindowSurface( window );
}

//this method uses hardware to render images to the window
void hardware_render() {
	texture_obj.render(renderer, 0, 0);
	SDL_RenderPresent(renderer);
}

//this function creates a second viewport to render to
void viewport_rendering() {
	SDL_Rect topright_viewport = {0,0,SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
	SDL_RenderSetViewport(renderer, &topright_viewport);
	texture_obj.render(renderer, 0, 0);
}

//this method uses hardware to render shapes
void hardware_draw_shapes() {
	SDL_Rect rect = {SCREEN_WIDTH/4, SCREEN_HEIGHT/4, SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(renderer, &rect);
	SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0xFF, 0xFF);
	SDL_RenderDrawRect(renderer, &rect);
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
	for(int i=SCREEN_HEIGHT/4; i<SCREEN_HEIGHT*3/4; i+=4) {
		SDL_RenderDrawPoint(renderer,SCREEN_WIDTH/2, i);
	}
	viewport_rendering();
}

int main( int argc, char* args[] )
{
	if(!init()) {
		printf("The initialization failed!");
	}
	else {
		//This section can not be used in combination with the hardware renderer
		//optimized_surface = load_media();
		//if(optimized_surface == NULL) {
		//	printf("The media could not be loaded!");
		//}
		//else
		{
			std::string path = "/home/gras/CLionProjects/SDLBonanza/resources/cock.png";
			texture_obj.loadFromFile(renderer, path);

			{
				//event handlers
				SDL_Event e;
				bool quit = false;
				while( quit == false ) {
					//set the viewport to the whole screen
					SDL_Rect wholescreen_viewport = {0,0,SCREEN_WIDTH, SCREEN_HEIGHT};
					SDL_RenderSetViewport(renderer, &wholescreen_viewport);
					//set default render colour to while
					SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
					//clear the screen so we can draw another frame
					SDL_RenderClear(renderer);
					//this draws images using the hardware renderer
					//hardware_render();
					//this draws shapes like dots, lines and rectangles
					hardware_draw_shapes();
					SDL_RenderPresent(renderer);

					while( SDL_PollEvent( &e ) ) {
						if( e.type == SDL_QUIT )
							quit = true;
						if( e.type == SDL_KEYDOWN) {
							switch(e.key.keysym.sym) {
								case(SDLK_UP):
									break;
								default:
									break;
							}
						}

					}
				}
			}
		}
	}

	//free and destroy window
	close();

	return 0;
}