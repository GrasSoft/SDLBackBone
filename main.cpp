
//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <cstring>
#include <filesystem>

#include "Button.h"
#include "Texture.h"
#include "Timer.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


//time between fps in ms, roughly 60 fps
const int TIME_BETWEEN_FRAMES = 16;

//The angle to which to render the image
double degrees = 0.0;

//Music from SDL_mixer
Mix_Music* music = NULL;

//The window we'll be rendering to
SDL_Window* window = NULL;

//The surface contained by the window
SDL_Surface* screen_surface = NULL;

//The surface of the loaded media
SDL_Surface* optimized_surface = NULL;

//The renderer of the screen
SDL_Renderer* renderer = NULL;

Texture texture_obj;

Button button;

bool init() {
	bool success = true;

	//initialize sdl
	if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		printf("SDL could not initialize with error code: %s\n", SDL_GetError());
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
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
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
					//Initialize SDL_mixer
					if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
					{
						printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
						success = false;
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

//this function creates a second viewport to render to
void viewport_rendering() {
	SDL_Rect topright_viewport = {0,0,SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
	SDL_RenderSetViewport(renderer, &topright_viewport);
	//texture_obj.render(renderer, 0, 0, NULL, degrees);
	button.render(renderer);
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

void play_music() {
	music = Mix_LoadMUS( "/home/gras/CLionProjects/SDLBonanza/resources/medium.wav" );
	if( music == NULL )
	{
		printf( "Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
	}
	else {
		//If there is no music playing
		if( Mix_PlayingMusic() == 0 )
		{
			//Play the music
			Mix_PlayMusic( music, -1 );
		}
		//If music is being played
		else
		{
			//If the music is paused
			if( Mix_PausedMusic() == 1 )
			{
				//Resume the music
				Mix_ResumeMusic();
			}
			//If the music is playing
			else
			{
				//Pause the music
				Mix_PauseMusic();
			}
		}
	}
}

int main( int argc, char* args[] )
{
	if(!init()) {
		printf("The initialization failed!");
	}
	else {
		{
			std::string path = "/home/gras/CLionProjects/SDLBonanza/resources/pulapizda.png";
			if(!texture_obj.loadFromFile(renderer, path)) {
				close();
				return 0;
			}
			texture_obj.set_blend_mode(SDL_BLENDMODE_BLEND);

			SDL_Rect clips[] = {{0,0,64,64}, {64,0, 64, 64}, {128, 0,64,64} };
			button.set_texture(&texture_obj);
			button.set_clips(3, clips);
			button.set_function_pointer(&play_music);
			{
				//event handlers
				SDL_Event e;

				//timer to cap the fps of the application
				Timer fps_timer;

				bool quit = false;
				while( quit == false ) {
					fps_timer.start();

					//set the viewport to the whole screen
					SDL_Rect wholescreen_viewport = {0,0,SCREEN_WIDTH, SCREEN_HEIGHT};
					SDL_RenderSetViewport(renderer, &wholescreen_viewport);
					//set default render colour to while
					SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
					//clear the screen so we can draw another frame
					SDL_RenderClear(renderer);
					//this draws shapes like dots, lines and rectangles
					hardware_draw_shapes();
					SDL_RenderPresent(renderer);


					while( SDL_PollEvent( &e ) ) {
						if( e.type == SDL_QUIT )
							quit = true;

						button.handle_event(&e);

						if( e.type == SDL_KEYDOWN) {
							switch(e.key.keysym.sym) {
								case(SDLK_UP):
									texture_obj.set_color(0, 128, 0);
									texture_obj.set_alpha(128);
									break;
								case(SDLK_LEFT):
									degrees += 10;
									break;
								case (SDLK_RIGHT):
									degrees -= 10;
									break;
								default:
									break;
							}
						}

					}

					if(fps_timer.get_ticks() < TIME_BETWEEN_FRAMES ) {
						//time to wait between frames
						SDL_Delay(TIME_BETWEEN_FRAMES - fps_timer.get_ticks());
					}
				}
			}
		}
	}

	//free and destroy window
	close();

	return 0;
}