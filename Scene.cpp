//
// Created by gras on 12/4/23.
//

#include "Scene.h"
#include "Sprite.h"

Scene::Scene() {

}

void Scene::add_sprite(Sprite* sprite) {
    sprite_vector.push_back(sprite);
}
