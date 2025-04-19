#include <iostream>

#include "buffer.h"

const int max_vertex_count = 10000;
const int max_index_count = 5000;

Buffer::~Buffer()
{
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    free(_vertexlist);
    free(_indexlist);
}

Buffer::Buffer(int type)
{
    _type = type;

    elements_enable_all();
}

void Buffer::make_buffers()
{
    // make cpu memory buffers

    int vertex_elements_count_all = this->get_elements_count_all();


    _data_stride = sizeof(GLfloat)*vertex_elements_count_all;

    _vertexlist = (unsigned char *)malloc(_data_stride*max_vertex_count);
    _indexlist = (GLuint *)malloc(sizeof(GLuint)*max_index_count);


    // make gpu memory buffers

    // GLuint VAO;
    glGenVertexArrays(1, &VAO);

    // GLuint VBO;
    glGenBuffers(1, &VBO);


    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, _data_stride*max_vertex_count, nullptr, GL_STATIC_DRAW);



    // shift of data from starting position of vertex buffer
    // it needs for set address for start position of next vertex attribute

    int vshift = 0;

    for (unsigned int loc = 0; loc < _attrs.size(); loc++)
    {
        int vsize = _attrs[loc]._size;

        glVertexAttribPointer(loc, vsize, GL_FLOAT, GL_FALSE, _data_stride, (GLvoid*)(vshift *sizeof(GLfloat) ) );
        glEnableVertexAttribArray(loc);

        vshift += vsize;
    }


    // // Position attribute
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, _data_stride, (GLvoid*)0);
    // glEnableVertexAttribArray(0);
    //
    // // Position attribute
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, _data_stride, (GLvoid*)(3 * sizeof(GLfloat)));
    // glEnableVertexAttribArray(1);




    // GLuint EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, max_index_count*sizeof(GLuint), nullptr, GL_STATIC_DRAW);

    glBindVertexArray(0); // Unbind VAO

}

void Buffer::enable(bool state)
{
    if (state) {
        glBindVertexArray(VAO);
    }
    else {
        glBindVertexArray(0);
    }
}


void Buffer::reset()
{
    lastIndex = 0;
    lastVertx = 0;
    lastVertxStart = 0;
}

void Buffer::add_point(const glm::vec3& p)
{
    int ind = lastVertx;

    // compute memory position of next free vertex
    GLfloat *vlist = (GLfloat *)(_vertexlist + ind*_data_stride);

    vlist[0] = p.x;
    vlist[1] = p.y;
    vlist[2] = p.z;
    vlist[3] = 0.0;
    vlist[4] = 0.0;

    lastVertx++;
}

void Buffer::add_point(const glm::vec3& p, const glm::vec2& t)
{
    int ind = lastVertx;

    // compute memory position of next free vertex
    GLfloat *vlist = (GLfloat *)(_vertexlist + ind*_data_stride);

    vlist[0] = p.x;
    vlist[1] = p.y;
    vlist[2] = p.z;
    vlist[3] = t.x;
    vlist[4] = t.y;

    lastVertx++;
}

void Buffer::add_vertex(const Vertex& vertex)
{
    int ind = lastVertx;

    // compute memory position of next free vertex
    GLfloat *vlist = (GLfloat *)(_vertexlist + ind*_data_stride);

    vlist[0] = vertex._pos.x;
    vlist[1] = vertex._pos.y;
    vlist[2] = vertex._pos.z;
    vlist[3] = vertex._tex.x;
    vlist[4] = vertex._tex.y;

    lastVertx++;

}



void Buffer::make_line()
{
    if (lastVertx < 1)
        return;

    // add line to index buffer

    if (_type == GL_LINE_STRIP)
    {
        for (int ii = 0; ii < lastVertx; ii++ )
        {
            _indexlist[ii] = ii;
        }

        lastIndex = lastVertx;
    }

    if (_type == GL_LINES)
    {
        int vertx = lastVertxStart;
        int index = lastIndex;

        while (vertx < lastVertx)
        {
            _indexlist[index+0] = vertx;
            _indexlist[index+1] = vertx + 1;

            index += 2;
            vertx += 2;
        }

        lastIndex = index;
        lastVertxStart = lastVertx;
    }

    _vertex_count = lastVertx;
    _index_count = lastIndex;
}

void Buffer::make_rect()
{
    int ii = lastIndex;

    int vertx = lastVertxStart;

    _indexlist[ii+0] = vertx + 0;
    _indexlist[ii+1] = vertx + 1;
    _indexlist[ii+2] = vertx + 2;

    _indexlist[ii+3] = vertx + 2;
    _indexlist[ii+4] = vertx + 3;
    _indexlist[ii+5] = vertx + 0;

    lastVertxStart = lastVertx;

    lastIndex += 6;

    // std::cout << " lastIndex " << lastIndex << " lastVert " << lastVertx << std::endl;

    _vertex_count = lastVertx;
    _index_count = lastIndex;
}

void Buffer::make_triangle()
{
    if (lastVertx < 3)
        return;

    int index = lastIndex;
    int vertx = lastVertxStart;

    _indexlist[index+0] = vertx + 0;
    _indexlist[index+1] = vertx + 1;
    _indexlist[index+2] = vertx + 2;

    lastVertxStart = lastVertx;
    lastIndex += 3;

    _vertex_count = lastVertx;
    _index_count = lastIndex;
}

void Buffer::make_poly()
{
    int ii = lastIndex;

    int vertx = lastVertxStart;

    _indexlist[ii+0] = vertx + 0;
    _indexlist[ii+1] = vertx + 1;
    _indexlist[ii+2] = vertx + 2;

    lastVertxStart = lastVertx;

    lastIndex += 3;

    _vertex_count = lastVertx;
    _index_count = lastIndex;
}


void Buffer::load_to_gpu()
{
    // glBufferSubData( 	GLenum target,
  	// GLintptr offset,
  	// GLsizeiptr size,
  	// const void * data);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, _vertex_count*_data_stride, _vertexlist);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, _index_count*sizeof(GLuint), _indexlist);

	// printf("buffer load_to_gpu %d %d\n", _vertex_count, _index_count);
}
