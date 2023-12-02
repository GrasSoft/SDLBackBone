//
// Created by gras on 12/1/23.
//
//The sprite class represents a texture that can move based on the frame rate(for now) and/or have colissions
//with rectangles and circles. For now the collision is donw with AABBs, further on there will be collisions with OBBs
#ifndef SPRITE_H
#define SPRITE_H
#include "Texture.h"
#include "definitions.h"

struct Circle {
    SDL_Point center;
    Uint32 radius;
};

class Sprite {
public:
    //constructor
    Sprite();

    //render to the screen
    void render(SDL_Renderer* renderer);

    //set the collisions on or off
    void toggle_collision(bool can_collide);

    //set angle
    void set_angle(int angle);

    //set clip
    void set_clip(SDL_Rect* clip);

    //check collisions between rectangles and circles
    static bool collides(SDL_Rect* rect1, SDL_Rect* rect2);

    static bool collides(Circle* circle1, Circle* circle2);

    static bool collides(Circle* circle, SDL_Rect* rect);

protected:
    SDL_Point position;

    int angle;

    Texture* texture;

    SDL_Rect clip;

    bool can_collide;
};

class CircleSprite : public Sprite {
public:
    CircleSprite();

    //set the circle collision box, if left null the circle will fit the whole texure clip box
    void set_collision_box(Circle* circle = NULL);

    //move the texture and the collision boxes wether or not if the can_collide is set
    void move(int x, int y);

private:

    Circle collision_box;
};

class RectangleSprite : public Sprite {
public:
    RectangleSprite();

    //set the rectangle collision box, if left null the circle will fit the whole texure clip box
    void set_collision_box(SDL_Rect* rect = NULL);

    //move the texture and the collision boxes wether or not if the can_collide is set
    void move(int x, int y);

private:

    SDL_Rect collision_box;
};

#endif //SPRITE_H
