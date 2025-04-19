
#ifndef VERTEX_H_
#define VERTEX_H_

struct vertex_attr_type
{
public:

	vertex_attr_type(const std::string& name, int size)
		: _name(name), _size(size)
	{
		// std::cout << "vertex_attr_type.create " << _name << " , " << _size << std::endl;
	}

	// name of vertex element - position, color, normal, texture,
	std::string _name;

	int _size;		// size of vertex element's data - position or normal or color or others

	vertex_attr_type()
	{}

};


//
// Структура для описания вершин
//
struct Vertex
{
public:
	
	Vertex(const std::string& type = "line")
	{
		_type = type;

		velem_enable_all();

		// _size = get_size_in_bytes();
	}

	void velem_enable_all() {

		if (_type == "line")
		{
			_attrs.insert( std::make_pair(0, vertex_attr_type("pos", 3)) );
		}

		if (_type == "poly1")
		{
			_attrs.insert( std::make_pair(0, vertex_attr_type("pos", 3)) );
			_attrs.insert( std::make_pair(1, vertex_attr_type("tex", 2)) );
		}

		if (_type == "poly2")
		{
			_attrs.insert( std::make_pair(0, vertex_attr_type("pos", 3)) );
			_attrs.insert( std::make_pair(1, vertex_attr_type("col", 2)) );
		}

		if (_type == "poly3d")
		{
			_attrs.insert( std::make_pair(0, vertex_attr_type("pos", 3)) );
			_attrs.insert( std::make_pair(1, vertex_attr_type("norm", 3)) );
			_attrs.insert( std::make_pair(0, vertex_attr_type("col", 4)) );
			_attrs.insert( std::make_pair(1, vertex_attr_type("tex", 2)) );
		}
	}

	int get_velem_count_all() {

		// compute count of elem of this vertex
		int velem_count_all = 0;

		for (unsigned int i = 0; i < _attrs.size(); i++)
		{
			velem_count_all += _attrs[i]._size;
		}

		// std::cout << "vertex_elements_count_all " << velem_count_all << std::endl;

		return velem_count_all;
	}

	int get_size_in_bytes() {

		int size_of_vert = sizeof(GLfloat) * this->get_velem_count_all();

		return size_of_vert;
	}

	// Vertex(float x, float y, float z, float u, float v)
	// 	: _pos(glm::vec4(x, y, z, 1.0f)),
	// 	  _tex(glm::vec2(u, v))
	// {}

	// Vertex(float x, float y, float z)
	// 	: _pos(glm::vec4(x, y, z, 1.0f)),
	// 	  _tex(glm::vec2(0.0f, 0.0f))
	// {}

    // Vertex(float range, float angle)
	// {
	// 	_pos.x = range*sin(glm::radians(angle));
	// 	_pos.y = range*cos(glm::radians(angle));
	// 	_pos.z = 0.0f;
	// }

	std::map<int, vertex_attr_type> _attrs;


	std::string _type;	// "type1 : {pos, tex}", "others", ...

	unsigned int _size;		// stride one vertex to other vertex and it's likewise size of vertex in bytes yet

	glm::vec4 _pos;
	glm::vec4 _normal;
	glm::vec4 _color;
	glm::vec2 _tex;
};

#endif //VERTEX_H_
