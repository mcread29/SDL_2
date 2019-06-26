#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

#include "Box2D/Box2D.h"

class Camera
{
public:
    Camera(int width, int height);
    ~Camera();

    void update();
    void setCameraTarget(b2Vec2 *target);
    b2Vec2 transformPoint(b2Vec2 *point);
    b2Vec2 *followTarget = NULL;
    int width;
    int height;
    // b2Vec2 &followTarget() const { return *_followTarget; }

private:
    b2Vec2 position;
};