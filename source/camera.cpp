#include "camera.h"

Camera::Camera(int w, int h)
{
    width = w;
    height = h;
    position = b2Vec2(0, 0);
}

Camera::~Camera()
{
    followTarget = NULL;
}

void Camera::update()
{
    position.Set(followTarget->x - width / 2, followTarget->y - height / 2);
}

void Camera::setCameraTarget(b2Vec2 *target)
{
    followTarget = target;
}

b2Vec2 Camera::transformPoint(b2Vec2 *point)
{
    if (followTarget == point)
    {
        return b2Vec2(width / 2, height / 2);
    }
    // return b2Vec2(0, 0);
    return b2Vec2(point->y - position.x, point->y - position.y);
}