#ifndef FONT_H_
#define FONT_H_

#include <string>
#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>


// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

#include "glm_includes.h"

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;    // Horizontal offset to advance to next glyph

    GLfloat tx;
    GLfloat ty;
};


class Font {
public:

	virtual ~Font();

	Font()
	{}

	void load_from_file(const std::string& filename);
	void save_texture_to_file(const std::string& filename);

	GLuint get_texture_ID() {
		return tex;
	}

	Character& get_glyph(int index) {
		return Characters[index];
	}

	unsigned int get_width()
	{
		return w;
	}

	unsigned int get_height()
	{
		return h;
	}

private:
	
	void build(FT_Face face, int height);

	// texture object
	GLuint tex;

	unsigned int w;	// width of texture in pixels
	unsigned int h; // height of texture in pixels

	Character Characters[256];
};

#endif // FONT_H_
