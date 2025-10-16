#ifndef SOURCE_HPP
#define SOURCE_HPP

#include <iostream>
#include <math.h>
#include <stdlib.h>

#include <opencv2/opencv.hpp>
//distice color pallet 
//https://sashamaps.net/docs/resources/20-colors/
const int colors[][3] = {{230, 25, 75}, {60, 180, 75}, {255, 255, 25},
                   {0, 130, 200}, {245, 130, 48}, {145, 30, 180},
                   {70, 240, 240}, {240, 50, 230}, {210, 245, 60},
                   {250, 190, 212}, {0, 128, 128}, {220, 190, 255},
                   {170, 110, 40}, {255, 250, 200}, {128, 0, 0},
                   {170, 255, 195}, {128, 128, 0}, {255, 215, 180},
                   {0, 0, 128}, {245, 130, 128}, {128,128,128}
                };

typedef struct
{
    int x;
    int y;
}Point;

#endif