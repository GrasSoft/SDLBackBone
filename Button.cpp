//
// Created by gras on 11/27/23.
//

#include "Button.h"

Button::Button() {
    position.x = 0;
    position.y = 0;

    sprite = NULL;

    fun_ptr = NULL;

    //initialise all the clips
    for(int i=0; i< BUTTON_SPRITE_TOTAL; i++) {
        clips[i]= {0,0,0,0 };
    }

    current_sprite = BUTTON_SPRITE_MOUSE_OUT;
}

void Button::set_position(int x, int y) {
    position.x = x;
    position.y = y;
}

void Button::set_clips(uint size, SDL_Rect* clips) {
    size_clips = size;

    for(int i=0; i< size; i++) {
        this->clips[i] = {clips[i].x,clips[i].y,clips[i].w,clips[i].h};
    }
}

void Button::render(SDL_Renderer* renderer) {
    sprite->render(renderer, position.x, position.y, &clips[current_sprite]);
}

void Button::set_texture(Texture* t) {
    sprite = t;
}

void Button::set_function_pointer(void (*ptr)()) {
    fun_ptr = ptr;
}


//By using this order, we ensure that
void Button::handle_event(SDL_Event* e) {
    int x, y;
    bool inside = true;
    SDL_GetMouseState( &x, &y );

    if( x < position.x || x > clips[current_sprite].w + position.x)
        inside = false;
    if( y < position.y || y > clips[current_sprite].h + position.y)
        inside = false;

    if(!inside)
        current_sprite = BUTTON_SPRITE_MOUSE_OUT;
    else
        switch(e->type) {
            case(SDL_MOUSEMOTION):
                if(size_clips > 1)
                    current_sprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                break;

            case(SDL_MOUSEBUTTONDOWN):
                if(size_clips > 2 ) {
                    current_sprite = BUTTON_SPRITE_MOUSE_DOWN;
                    if(fun_ptr != NULL) {
                        (*fun_ptr)();
                    }
                }
                break;
            //no break because if this step does not exist at least it returns to mouse over
            case(SDL_MOUSEBUTTONUP):
                if(size_clips > 3)
                    current_sprite = BUTTON_SPRITE_MOUSE_UP;
            default:
                if(size_clips > 1)
                    current_sprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                break;
        }
}
