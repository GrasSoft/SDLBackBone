//
// Created by gras on 11/25/23.
//

#ifndef TEXTURE_H
#define TEXTURE_H
#include <SDL_render.h>
#include <string>


class Texture {
    public:
        //Initializes variables
        Texture();

        //Deallocates memory
        ~Texture();

        //Loads image at specified path
        bool loadFromFile(SDL_Renderer* renderer, std::string& path);

        //Deallocates texture
        void free();

        //Collour modulation
        void set_color( Uint8 red, Uint8 green, Uint8 blue);

        //Renders texture at given point
        void render( SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

        //enable blending
        void set_blend_mode( SDL_BlendMode blending);

        //set the transparency
        void set_alpha( Uint8 alpha );

        //Get image dimensions
        int get_width();
        int get_height();

    private:
        //The actual hardware texture
        SDL_Texture* texture;

        //Image dimensions
        int width;
        int height;
};



#endif //TEXTURE_H
