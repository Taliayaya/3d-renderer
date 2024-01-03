# include "../../include/math/utils.h"

float clampf(float value, float lower, float upper)
{
    return fmin(upper, fmax(lower, value));
}

float lerpf(float value, float lower, float upper)
{
    return (upper - lower) * value + lower;
}
