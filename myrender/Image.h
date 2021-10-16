#ifndef IMAGE_H
#define IMAGE_H

#include "vecmath/include/vecmath.h"
#include <string>

class Image
{
public:
    Image(int w, int h);

    
    ~Image();
    const Vector3f& getPixel(int x, int y) const;
    void setPixel(int x, int y, const Vector3f& color);
    int SaveBMP(const char *filename);
private:
    int width, height;
    Vector3f* data;
};

#endif