
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
#include "definitions.h"
#include "Sprite.h"
#include "Scene.h"

//Music from SDL_mixer
Mix_Music* music = NULL;

//The window we'll be rendering to
SDL_Window* window = NULL;

//The renderer of the screen
SDL_Renderer* renderer = NULL;

//camera to show the screen
SDL_Rect camera = {0,0, SCREEN_WIDTH, SCREEN_HEIGHT};

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
	//dealocate window
	SDL_DestroyWindow(window);
	window = NULL;

	//dealocate renderer
	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	//quit sdl subsystem and image library
	IMG_Quit();
	Mix_Quit();
	SDL_Quit();
}

//this function creates a second viewport to render to
void viewport_rendering() {
	SDL_Rect topright_viewport = {0,0,SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
	SDL_RenderSetViewport(renderer, &topright_viewport);
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
	//viewport_rendering();
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
	printf("hello");
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

				Scene scene;

				//timer to cap the fps of the application
				Timer fps_timer;

				//the moving sprite
				RectangleSprite player;

				//the wall
				RectangleSprite wall;

				//texture player
				Texture texture_player;

				//texture wall
				Texture texture_wall;

				//bg texture
				Texture bg;

				path = "/home/gras/CLionProjects/SDLBonanza/resources/player.png";
				if(!texture_player.loadFromFile(renderer, path)) {
					close();
					return 0;
				}
				path = "/home/gras/CLionProjects/SDLBonanza/resources/wall.png";
				if(!texture_wall.loadFromFile(renderer, path)) {
					close();
					return 0;
				}

				path = "/home/gras/CLionProjects/SDLBonanza/resources/bg.png";
				if(!bg.loadFromFile(renderer, path)) {
					close();
					return 0;
				}


				player.set_texture(&texture_player);
				player.set_position(LEVEL_WIDTH - 32, LEVEL_HEIGHT - 32);

				wall.set_texture(&texture_wall);
				wall.set_position(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);

				scene.add_sprite(&player);
				scene.add_sprite(&wall);

				player.toggle_collision(true);
				wall.toggle_collision(true);

				player.set_collision_box();
				wall.set_collision_box();


				bool quit = false;
				while( quit == false ) {
					fps_timer.start();

					//set the viewport to the whole screen
					SDL_Rect wholescreen_viewport = {0,0,SCREEN_WIDTH, SCREEN_HEIGHT};
					//SDL_RenderSetViewport(renderer, &wholescreen_viewport);
					//set default render colour to while
					SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
					//clear the screen so we can draw another frame
					SDL_RenderClear(renderer);
					//this draws shapes like dots, lines and rectangles
					//hardware_draw_shapes();



					while( SDL_PollEvent( &e ) ) {
						if( e.type == SDL_QUIT )
							quit = true;

						button.handle_event(&e);

						if( e.type == SDL_KEYDOWN) {
							switch(e.key.keysym.sym) {
								case(SDLK_DOWN):
									player.move(0, 10, &scene);
									break;
								case(SDLK_UP):
									player.move(0,-10, &scene);
									break;
								case(SDLK_LEFT):
									player.move(-10, 0, &scene);
									break;
								case (SDLK_RIGHT):
									player.move(10, 0, &scene);
									break;
								default:
									break;
							}
						}

					}

					//change the coordinates of the camera
					camera.x = player.get_x() - SCREEN_WIDTH/2;
					camera.y = player.get_y() - SCREEN_HEIGHT/2;

					if(camera.x < 0) {
						camera.x = 0;
					}
					if(camera.x > LEVEL_WIDTH - camera.w) {
						camera.x = LEVEL_WIDTH - camera.w;
					}
					if(camera.y < 0) {
						camera.y = 0;
					}
					if(camera.y > LEVEL_HEIGHT - camera.h) {
						camera.y = LEVEL_HEIGHT - camera.h;
					}

					bg.render(renderer, 0, 0, &camera);
					player.render(renderer, camera.x, camera.y);
					wall.render(renderer, camera.x, camera.y);

					SDL_RenderPresent(renderer);

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