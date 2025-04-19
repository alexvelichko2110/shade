#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include "texture.h"

class FrameBuffer
{
public:

    ~FrameBuffer()
    {
        delete _tex;
    }

    FrameBuffer(int width, int height);

    unsigned int get_framebuffer_ID() {
        return _framebuffer;
    }

    unsigned int get_texture_ID() {
        return _texture;
    }

    int get_width()
    {
        return _width;
    }

    int get_height()
    {
        return _height;
    }

    Texture* get_texture()
    {
        return _tex;
    }

    int _width;
    int _height;

    unsigned int _framebuffer;
    unsigned int _texture;
    unsigned int _rbo;

    Texture* _tex;
};

#endif //FRAMEBUFFER_H_
