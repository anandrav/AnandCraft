#include "RaycastEvent.h"

#include <limits>

RaycastEvent::RaycastEvent(const Ray& ray, bool left_click)
    : ray(ray)
    , left_click(left_click)
    , closest(glm::vec3(FLT_MAX))
{
}