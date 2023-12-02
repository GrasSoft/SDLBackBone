//
// Created by gras on 11/27/23.
//

//This class is used in combination to the texure, it adds the ability for mouse events to interact and change the
//current displayed texture by providing a parent texture and an array of SDL_Rect that represent where to clip the
//texture. There are 4 posible mouse positions, mouse out, mouse over, mouse down and mouse up. The clips must be
//provided in order but can skip the last ones depending on the needs of the button(e.g. can skip the mouse up).

#ifndef BUTTON_H
#define BUTTON_H
#include <SDL_events.h>

#include "Texture.h"

enum ButtonSprite
{
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
    BUTTON_SPRITE_MOUSE_DOWN = 2,
    BUTTON_SPRITE_MOUSE_UP = 3,
    BUTTON_SPRITE_TOTAL = 4
};

class Button {
public:
    //Initialize internal variables
    Button();

    //Sets position of the button
    void set_position(int x, int y);

    //Sets the positions of the clips to be used from the
    void set_clips(uint size, SDL_Rect* clips);

    //Sets the pointer for the texture
    void set_texture(Texture* t);

    //Handles mouse events
    void handle_event( SDL_Event* e );

    //shows sprite
    void render(SDL_Renderer* renderer);

    //set function pointer
    void set_function_pointer(void (*ptr)());

private:
    //position of the button
    SDL_Point position;

    //size of the array of clips
    uint size_clips;

    //array of clips from the texture
    SDL_Rect clips[BUTTON_SPRITE_TOTAL];

    Texture* texture;

    //Currently used global sprite
    ButtonSprite current_texture;

    //the interupt function pointer
    void (*fun_ptr)();
};



#endif //BUTTON_H
