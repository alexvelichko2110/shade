#include <SOIL/SOIL.h>

#include "font.h"

void Font::build(FT_Face face, int height)
{
    FT_Set_Pixel_Sizes(face, 0, height);

//		Characters.clear();
//		Characters.reserve(256);

    unsigned int roww = 0;
    unsigned int rowh = 0;

    for (unsigned int c = 32; c < 256; c++) {

        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            fprintf(stderr, "Loading character %c failed!\n", c);
            continue;
        }

        FT_GlyphSlot g = face->glyph;

//			fprintf(stderr, "Generated w %d h %d texture atlas\n", w, h);

        rowh = std::max(rowh, g->bitmap.rows);
        roww = std::max(roww, g->bitmap.width);
    }

    w = 16*roww;
    h = 16*rowh;

    // fprintf(stderr, "Generated w %d h %d texture atlas\n", w, h);

    /* Create a texture that will be used to hold all ASCII glyphs */
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    // void *pixels = malloc(w*h);
    
    // memset(pixels, 0, w*h);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, pixels);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

    // free(pixels);
    
    /* We require 1 byte alignment when uploading texture data */
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    /* Clamping to edges is important to prevent artifacts when scaling */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    /* Linear filtering usually looks best for text */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    FT_GlyphSlot g = face->glyph;

    /* Paste all glyph bitmaps into the texture, remembering the offset */

    for (unsigned int c = 32; c < 256; c++) {
        int iy = c/16;
        int ix = c%16;

//			printf("с %d ix %d iy %d \n", c, ix, iy);

        int oy = iy*rowh;
        int ox = ix*roww;

        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            fprintf(stderr, "Loading character %c failed!\n", c);
            continue;
        }

        glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy,
                g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

        GLfloat tx = ox / (GLfloat)w;
        GLfloat ty = oy / (GLfloat)h;

        // Now store character for later use
        Character character = {
            glm::ivec2(g->bitmap.width, g->bitmap.rows),
            glm::ivec2(g->bitmap_left, g->bitmap_top),
            (GLuint)g->advance.x,
            tx, ty };

        Characters[c] = character;
    }
}

void Font::load_from_file(const std::string& filename)
{
    // FreeType
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    // Load font as face
    FT_Face face;
    if (FT_New_Face(ft, filename.c_str(), 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    build(face, 40);

    // // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void Font::save_texture_to_file(const std::string& filename)
{
    unsigned char *pixels = (unsigned char*)malloc(w*h);

    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, pixels);

    /* save that image as another type */
    int save_result = SOIL_save_image ( filename.c_str(),
            SOIL_SAVE_TYPE_TGA,
            w, h, 1, pixels);

    if (save_result == 0)
        printf ("SOIL save result %d\n", save_result);

    free(pixels);
}

Font::~Font() {
    glDeleteTextures(1, &tex);
}
