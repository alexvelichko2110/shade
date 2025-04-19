#ifndef BUFFER_H_
#define BUFFER_H_


#include <GL/glew.h>

#include <string>
#include <map>

#include "glm_includes.h"

#include "vertex.h"


// struct vertex_attr_type
// {
// 	int _size;		// size of element of data - position or normal or color or others

// 	std::string _name;	// name of element - position, color, normal, texture,

// 	vertex_attr_type()
// 	{}

// 	vertex_attr_type(const std::string& name, int size)
// 		: _name(name), _size(size)
// 	{
// 		// std::cout << "vertex_attr_type.create " << _name << " , " << _size << std::endl;
// 	}
// };


//
// Буфер для предствления геометрии - множество вершин + набор индексов
//
class Buffer
{
public:

	// enum {
	// 	GL_POINTS,
	// 	GL_LINES,
	// 	GL_LINE_STRIP,
	// 	GL_TRIANGLES,
	// };

	virtual ~Buffer();

    Buffer(int type = GL_LINES);

	// api for working with geometry
	void reset();

	void add_point(const glm::vec3& p);
	void add_point(const glm::vec3& p, const glm::vec2& t);

	void add_vertex(const Vertex& vertex);

	void make_line();
	void make_rect();

	void make_triangle();
	void make_poly();

	// api for VAO, and others buffers

	void make_buffers();
	void load_to_gpu();

	void enable(bool state);

	int get_index_count() {
		return _index_count;
	}

	int get_element_type() {
		return _type;
	}

private:

	int _type;


	void elements_enable_all() {

		_attrs.insert( std::make_pair(0, vertex_attr_type("pos", 3)) );
		_attrs.insert( std::make_pair(1, vertex_attr_type("tex", 2)) );

	}


	int get_elements_count_all() {

		// compute
		int vertex_elements_count_all = 0;

		for (unsigned int i = 0; i < _attrs.size(); i++)
		{
			vertex_elements_count_all += _attrs[i]._size;
		}

		// std::cout << "vertex_elements_count_all " << vertex_elements_count_all << std::endl;

		return vertex_elements_count_all;
	}


	std::map<int, vertex_attr_type> _attrs;


	GLuint VAO, VBO, EBO;

	// step in memory to next data element
	int _data_stride;

	// data vertex and index
	int _vertex_count;
	unsigned char *_vertexlist;

	int _index_count;
	GLuint *_indexlist;

	int lastIndex;
	int lastVertx;
	int lastVertxStart;
};


#endif //BUFFER_H_
