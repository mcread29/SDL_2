#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <map>

#include "utils.h"

class TextureManager
{
public:
    TextureManager();
    ~TextureManager();

    bool addTexture(std::string key, std::string path, SDL_Renderer *renderer);
    SDL_Texture *getTexture(std::string key);
    void clear();

private:
    std::map<std::string, SDL_Texture *> mList;
};