#include "textureManager.h"

TextureManager::TextureManager()
{
    mList = std::map<std::string, SDL_Texture *>();
}

TextureManager::~TextureManager()
{
    std::map<std::string, SDL_Texture *>::iterator itr;
    for (itr = mList.begin(); itr != mList.end(); ++itr)
    {
        printf("DESTROYING: %s\n", itr->first.c_str());

        SDL_DestroyTexture(itr->second);
        itr->second = NULL;
        mList.erase(itr->first);
    }

    mList.clear();
}

void TextureManager::clear()
{
    mList.clear();
}

bool TextureManager::addTexture(std::string key, std::string path, SDL_Renderer *renderer)
{
    bool success = true;
    SDL_Texture *newTexture = textureLoad(path, renderer);
    if (newTexture == NULL)
        success = false;
    else
        mList.insert(std::pair<std::string, SDL_Texture *>(key, newTexture));

    return success;
}

SDL_Texture *TextureManager::getTexture(std::string key)
{
    SDL_Texture *texture = NULL;
    try
    {
        texture = mList.at(key);
    }
    catch (int e)
    {
        printf("No texture for key: %s!\n", key);
    }
    return texture;
}