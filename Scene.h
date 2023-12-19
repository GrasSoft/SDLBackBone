//
// Created by gras on 12/4/23.
//
#pragma once
#ifndef SCENE_H
#define SCENE_H
#include <SDL_render.h>
#include <vector>
#include "Texture.h"
class Sprite;

class Scene {
public:
    //constructor
    Scene();

    //render the whole scene
    //void render(SDL_Renderer* renderer);

    //add existing sprite to sprite vector
    void add_sprite(Sprite* sprite);

    //array of sprites to check collsion against and/or move
    std::vector<Sprite*> sprite_vector;
};



#endif //SCENE_H