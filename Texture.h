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

        //Renders texture at given point
        void render( SDL_Renderer* renderer, int x, int y );

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
