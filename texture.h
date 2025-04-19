#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <string>

#include <SOIL/SOIL.h>

#include <GL/glew.h>

//
// texture
//
class Texture
{
public:

	virtual ~Texture();

	Texture(GLuint ID)
	{
		_texID = ID;		
	}

	Texture(GLuint width, GLuint height, GLuint bits_per_pixel, GLenum format = GL_RGBA);

	void save_to_file(const std::string& filename)
	{
		unsigned char *pixels = (unsigned char *)malloc(_width*_height);

    	glBindTexture ( GL_TEXTURE_2D, _texID );

		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		// save that image as another type 
		int save_result = SOIL_save_image ( filename.c_str(), SOIL_SAVE_TYPE_TGA, _width, _height, 1, pixels);

		if (save_result == 0)
			printf ("SOIL save result %d\n", save_result);

		free(pixels);
	}

	void load(unsigned char* bits);
	void load(int x, int y, int w, int h, unsigned char* bits);
	void clamp();

	static Texture* load(const std::string& file_name);

	GLuint get_texID() {
		return _texID;
	}

private:

	GLuint _width;
	GLuint _height;
	GLuint _bpp;
	GLenum _format;

	GLuint _texID;
};


#endif //TEXTURE_H_
