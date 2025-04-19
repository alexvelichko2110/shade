
#include <FreeImage.h>


#include "texture.h"

Texture::~Texture()
{
    glDeleteTextures (1, &_texID );
}

Texture::Texture(GLuint width, GLuint height, GLuint bits_per_pixel, GLenum format)
    : _width(width), _height(height), _bpp(bits_per_pixel), _format(format)
{
    // Use tightly packed data
    glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );

    // Generate a texture object
    glGenTextures ( 1, &_texID );

//		printf("texture TexID %d \n", (int)texID);

    // Bind the texture object
    glBindTexture ( GL_TEXTURE_2D, _texID );

    // Load the texture
    glTexImage2D ( GL_TEXTURE_2D, 0, _format, _width, _height, 0, _format, GL_UNSIGNED_BYTE, nullptr );

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Set the filtering mode
//		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
//		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
}

void Texture::load(unsigned char* bits) {

    glBindTexture ( GL_TEXTURE_2D, _texID );
    glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, _width, _height, _format, GL_UNSIGNED_BYTE, bits);
}

void Texture::load(int x, int y, int w, int h, unsigned char* bits) {
    glBindTexture ( GL_TEXTURE_2D, _texID );
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, _format, GL_UNSIGNED_BYTE, bits);
}

void Texture::clamp() {
    glBindTexture ( GL_TEXTURE_2D, _texID );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

// void Texture::bind() {

//     glActiveTexture ( GL_TEXTURE0 );
//     glBindTexture ( GL_TEXTURE_2D, _texID );
// }


Texture* Texture::load(const std::string& file_name)
{
    FIBITMAP* dibLoad;
    FIBITMAP* dib;

    // dibLoad = FreeImage_Load( FIF_TARGA, fileName.c_str() );
    dibLoad = FreeImage_Load( FIF_JPEG, file_name.c_str() );

    dib = FreeImage_ConvertTo32Bits(dibLoad);

    FreeImage_FlipVertical(dib);

    int width = FreeImage_GetWidth( dib );
    int height = FreeImage_GetHeight( dib );
//	    int pitch = FreeImage_GetPitch(dib);
    int bpp = FreeImage_GetBPP( dib );

    // printf(" loadTexture dib %d %d %d\n", width, height, bpp);

    // Calculate the number of bytes per pixel (3 for 24-bit or 4 for 32-bit)
    // int bytespp = FreeImage_GetLine(dib) / FreeImage_GetWidth(dib);
    //
    // for(unsigned y = 0; y < FreeImage_GetHeight(dib); y++) {
    //
    //     BYTE *bits = FreeImage_GetScanLine(dib, y);
    //
    //     for(unsigned x = 0; x < FreeImage_GetWidth(dib); x++) {
    //
    //         bits[FI_RGBA_RED] = 0;
    //         bits[FI_RGBA_GREEN] = 128;
    //         bits[FI_RGBA_BLUE] = 0;
    //         bits[FI_RGBA_ALPHA] = 255;
    //
    //         // jump to next pixel
    //         bits += bytespp;
    //     }
    // }

//    BYTE *bits = (BYTE*)FreeImage_GetBits(dib);
//
//    int x, y;
//
//    for(y = 0; y < height; y++) {
//		BYTE *pixel = (BYTE*)bits;
//		for(x = 0; x < width; x++) {
//			pixel[FI_RGBA_RED]	= 128;
//			pixel[FI_RGBA_GREEN] = 128;
//			pixel[FI_RGBA_BLUE] = 128;
//			pixel += 4;
//		}
//
//		// next line
//		bits += pitch;
//    }

    unsigned char* data = FreeImage_GetBits( dib );

    Texture *tex = new Texture(width, height, 32);
    tex->load(data);

    return tex;
}
