#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

class Texture
{
public:
    Texture(SDL_Texture *texture, SDL_Rect rect);
    // ~Texture();

    void render(SDL_Renderer *renderer);
    void setPosition(float x, float y);

private:
    SDL_Texture *mTexture = NULL;
    SDL_Rect mRect;
};