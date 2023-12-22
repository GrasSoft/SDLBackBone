//
// Created by gras on 12/1/23.
//
//The sprite class represents a texture that can move based on the frame rate(for now) and/or have colissions
//with rectangles and circles. For now the collision is donw with AABBs, further on there will be collisions with OBBs
#pragma once
#ifndef SPRITE_H
#define SPRITE_H
#include "Texture.h"
#include "definitions.h"

class Scene;
class CircleSprite;
class RectangleSprite;

struct Circle {
    SDL_Point center;
    Uint32 radius;
};

enum ColisionType{
    NO_COLISION,
    CIRCLE_SPRITE,
    RECTANGLE_SPRITE
};

class Sprite {
public:
    //constructor
    Sprite();

    //getter for X and Y coordinates
    int get_x();

    int get_y();

    //render to the screen, needs camera postion to shift postion
    void render(SDL_Renderer* renderer, int camera_x, int camera_y);

    //set the collisions on or off
    void toggle_collision(bool can_collide);

    //set angle
    void set_angle(int angle);

    //set clip
    void set_clip(SDL_Rect* clip);

    //set texture
    void set_texture(Texture* texture, SDL_Rect* clip = NULL);

    //check collisions between rectangles and circles

    static bool collides(RectangleSprite* rect1, RectangleSprite* rect2);

    static bool collides(CircleSprite* circle1, CircleSprite* circle2);

    static bool collides(CircleSprite* circle, RectangleSprite* rect);

    static bool collides(Sprite* spr1, Sprite* spr2);

    // check collision with all the sprites in the scene
    bool collide_scene(Scene* scene);

    ColisionType colision_type;

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

    //set position
    void set_position(int x, int y);

    //move the texture and the collision boxes wether or not if the can_collide is set
    void move(int x, int y, Scene* scene = NULL);

    Circle collision_box;
};

class RectangleSprite : public Sprite {
public:
    RectangleSprite();

    //set the rectangle collision box, if left null the circle will fit the whole texure clip box
    void set_collision_box(SDL_Rect* rect = NULL);

    //set position
    void set_position(int x, int y);

    //move the texture and the collision boxes wether or not if the can_collide is set
    void move(int x, int y, Scene* scene = NULL);

    SDL_Rect collision_box;
};

#endif //SPRITE_H
