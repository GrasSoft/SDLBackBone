//
// Created by gras on 12/1/23.
//

#include "Sprite.h"

Sprite::Sprite() {
    position = {0,0};
    texture = NULL;
    clip = {0,0,0,0};
    angle = 0;
    can_collide = false;
}

void Sprite::render(SDL_Renderer* renderer) {
    texture->render(renderer, position.x, position.y, &clip, angle);
}

void Sprite::set_angle(int angle) {
    this->angle = angle;
}

void Sprite::set_clip(SDL_Rect* clip) {
    this->clip = {clip->x, clip->y, clip->w, clip->h};
}

void Sprite::toggle_collision(bool can_collide) {
    this->can_collide = can_collide;
}

bool Sprite::collides(SDL_Rect* rect1, SDL_Rect* rect2) {
    if(rect1->x >= rect2->x + rect2->w) {
        if(rect1->y >= rect2->y + rect2->h) {
            return true;
        }
    }
    return false;

}

bool Sprite::collides(Circle* circle1, Circle* circle2) {
    //the sqare law
    if((circle1->center.x - circle2->center.x) * (circle1->center.x - circle2->center.x) + (circle1->center.y - circle2->center.y) * (circle1->center.y - circle2->center.y) <= (circle1->radius + circle2->radius) * (circle1->radius + circle2->radius)) {
        return true;
    }
    return false;
}

bool Sprite::collides(Circle* circle, SDL_Rect* rect) {
    if(circle->center.y - rect->y <= circle->radius || rect->y + rect->h - circle->center.y <= circle->radius) {
        if(circle->center.x - rect->x <= circle->radius || rect->x + rect->w - circle->center.x <= circle->radius) {
            return true;
        }
    }

    //inside the rectangle itself
    if(circle->center.x >= rect->x && circle->center.y <= rect->x + rect->w) {
        if(circle->center.y >= rect->y && circle->center.y <= rect->y + rect->h) {
            return true;
        }
    }
    return false;
}

CircleSprite::CircleSprite() {
    Sprite::Sprite();
    collision_box = {{0,0}, 0};
}

//TODO:: check collision as well
void CircleSprite::move(int x, int y) {
    if((position.x + x < SCREEN_WIDTH && position.x + x >= 0)) {
        position.x += x;
        collision_box.center.x +=x;
    }

    if((position.y + y < SCREEN_WIDTH && position.y + y >= 0)) {
        position.y += y;
        collision_box.center.y += y;
    }
}

void CircleSprite::set_collision_box(Circle* circle) {
    if(circle == NULL) {
        collision_box = {{clip.w/2, clip.h/2}, (Uint32)clip.w/2};
    }
    else {
        collision_box = {{circle->center.x, circle->center.y}, circle->radius};
    }
}

RectangleSprite::RectangleSprite() {
    Sprite::Sprite();
    collision_box = {0,0,0,0};
}


//TODO:: check with collision as well
void RectangleSprite::move(int x, int y) {
    if((position.x + x < SCREEN_WIDTH && position.x + x >= 0)) {
        position.x += x;
        collision_box.x +=x;
    }

    if((position.y + y < SCREEN_WIDTH && position.y + y >= 0)) {
        position.y += y;
        collision_box.y += y;
    }
}

void RectangleSprite::set_collision_box(SDL_Rect* rect) {
    if(rect == NULL) {
        collision_box = {0,0, clip.w, clip.h};
    }
    else {
        collision_box = {rect->x, rect->y, rect->w, rect->h};
    }
}


