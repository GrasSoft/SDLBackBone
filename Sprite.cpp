//
// Created by gras on 12/1/23.
//

#include "Sprite.h"
#include "Scene.h"

#include <iostream>

Sprite::Sprite() {
    position = {0,0};
    texture = NULL;
    angle = 0;
    can_collide = false;
    colision_type = NO_COLISION;
}

void Sprite::render(SDL_Renderer* renderer) {
    texture->render(renderer, position.x, position.y, NULL, angle);
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

void Sprite::set_texture(Texture* texture, SDL_Rect* clip) {
    if(clip == NULL)
        this->clip = {0,0, texture->get_width(), texture->get_height()};
    else
        this->clip = {clip->x, clip->y, clip->w, clip->h};
    this->texture = texture;
}

bool Sprite::collides(Sprite* spr1, Sprite* spr2) {
    if(spr1->colision_type == CIRCLE_SPRITE) {
        if(spr2->colision_type == CIRCLE_SPRITE) {
            return Sprite::collides(static_cast<CircleSprite*>(spr1), static_cast<CircleSprite*>(spr2));
        }
        if(spr2->colision_type == RECTANGLE_SPRITE) {
            return Sprite::collides(static_cast<CircleSprite*>(spr1), static_cast<RectangleSprite*>(spr2));
        }
    }

    if(spr1->colision_type == RECTANGLE_SPRITE) {
        if(spr2->colision_type == CIRCLE_SPRITE) {
            return Sprite::collides(static_cast<CircleSprite*>(spr2), static_cast<RectangleSprite*>(spr1));
        }
        if(spr2->colision_type == RECTANGLE_SPRITE) {
            return Sprite::collides(static_cast<RectangleSprite*>(spr1), static_cast<RectangleSprite*>(spr2));
        }
    }

    return false;
}



bool Sprite::collides(RectangleSprite* rect1, RectangleSprite* rect2) {
    int x1 = rect1->collision_box.x;
    int y1 = rect1->collision_box.y;
    int w1 = rect1->collision_box.w;
    int h1 = rect1->collision_box.h;

    int x2 = rect2->collision_box.x;
    int y2 = rect2->collision_box.y;
    int w2 = rect2->collision_box.w;
    int h2 = rect2->collision_box.h;

    if(x1 + w1 < x2 || x1 > x2 + w2 || y1 + h1 < y2 || y1 > y2 + h2 )
        return false;

    return true;

}

bool Sprite::collides(CircleSprite* circle1, CircleSprite* circle2) {
    //the sqare law
    if((circle1->collision_box.center.x - circle2->collision_box.center.x) * (circle1->collision_box.center.x - circle2->collision_box.center.x) + (circle1->collision_box.center.y - circle2->collision_box.center.y) * (circle1->collision_box.center.y - circle2->collision_box.center.y) <= (circle1->collision_box.radius + circle2->collision_box.radius) * (circle1->collision_box.radius + circle2->collision_box.radius)) {
        return true;
    }
    return false;
}

bool Sprite::collides(CircleSprite* circle, RectangleSprite* rect) {
    int y_circle = circle->collision_box.center.y;
    int x_circle = circle->collision_box.center.x;
    int radius = circle->collision_box.radius;

    int x_rect = rect->collision_box.x;
    int y_rect = rect->collision_box.y;
    int w_rect = rect->collision_box.w;
    int h_rect = rect->collision_box.h;

    if(abs(y_rect - y_circle) <= radius || abs( y_rect + h_rect - y_circle) <= radius) {
        if(abs(x_rect - x_circle) <= radius || abs(x_rect + w_rect - x_circle) <= radius) {
            return true;
        }
    }

    //inside the rectangle itself
    if(x_circle >= x_rect && x_circle <= x_rect + w_rect) {
        if(y_circle >= y_rect && y_circle <= y_rect + h_rect) {
            return true;
        }
    }
    return false;
}


bool Sprite::collide_scene(Scene* scene) {
    if(scene == NULL)
        return false;

    for(auto & spr : scene->sprite_vector) {
        if(spr != this) {
            if(Sprite::collides(spr, this)) {
                return true;
            }
        }
    }
    return false;
}

CircleSprite::CircleSprite() {
    collision_box = {{0,0}, 0};
    colision_type = CIRCLE_SPRITE;
}

void CircleSprite::move(int x, int y, Scene* scene) {
    position.x += x;
    collision_box.center.x += x;
    if((position.x >= (SCREEN_WIDTH - clip.w) || position.x <= 0) || (can_collide && collide_scene(scene))) {
        position.x -= x;
        collision_box.center.x -= x;
    }

    position.y += y;
    collision_box.center.y += y;
    if((position.y >= (SCREEN_HEIGHT - clip.h) || position.y <= 0) || (can_collide && collide_scene(scene))) {
        position.y -= y;
        collision_box.center.y -= y;
    }
}
void CircleSprite::set_position(int x, int y) {
    position.x = x;
    position.y = y;

    collision_box.center = {x + clip.w/2, y + clip.h/2};
}

void CircleSprite::set_collision_box(Circle* circle) {
    if(circle == NULL) {
        collision_box = {{position.x + clip.w/2, position.y + clip.h/2}, (Uint32)clip.w/2};
    }
    else {
        collision_box = {{circle->center.x, circle->center.y}, circle->radius};
    }
}

RectangleSprite::RectangleSprite() {
    collision_box = {0,0,0,0};
    colision_type = RECTANGLE_SPRITE;
}

void RectangleSprite::move(int x, int y, Scene* scene) {
    position.x += x;
    collision_box.x += x;
    if((position.x >= (SCREEN_WIDTH - clip.w) || position.x <= 0) || (can_collide && collide_scene(scene))) {
        position.x -= x;
        collision_box.x -= x;
    }

    position.y += y;
    collision_box.y += y;
    if((position.y >= (SCREEN_HEIGHT - clip.h) || position.y <= 0) || (can_collide && collide_scene(scene))) {
        position.y -= y;
        collision_box.y -= y;
    }
}

void RectangleSprite::set_position(int x, int y) {
    position.x = x;
    position.y = y;

    collision_box.x = x;
    collision_box.y = y;
}


void RectangleSprite::set_collision_box(SDL_Rect* rect) {
    if(rect == NULL) {
        collision_box = {position.x,position.y, clip.w, clip.h};
    }
    else {
        collision_box = {rect->x, rect->y, rect->w, rect->h};
    }
}


