#ifndef NODE_H_
#define NODE_H_

#include <list>

#include "glm_includes.h"


//
//  class for setuping and manipulating with local systeam of coords into world space
//
class Node
{
public:

	typedef std::list<Node*> NodeList;

	Node();

public:

	void set_parent(Node *parent_node) {
		_parent = parent_node;
	}

	void set_position (const glm::vec3& pos) {
		_pos = pos;
	}

	glm::vec3 get_position() {
		return _pos;
	}


	glm::mat4 get_mat() {
		return _mat;
	}

	glm::mat4 get_mat_inverse() {

		glm::mat4 mat = glm::inverse(_mat);

		return mat;
	}

	glm::mat4 get_world_mat() {
		return _mat_world;
	}

	glm::mat4 get_world_mat_inverse() {

		glm::mat4 mat = glm::inverse(_mat_world);

		return mat;
	}




	// Euler Angles trunsfomations

	void set_roll(float roll) {
		_roll = roll;
	}

	void set_yaw(float yaw) {
		_yaw = yaw;
	}

	void set_pitch(float pitch) {
		_pitch = pitch;
	}

	void euler();


	// transformation op
	void translate(const glm::vec3& vec);
	void rotate(float angle, const glm::vec3& vec );


	// look at point from another point
	void look_at(glm::vec3& eye, glm::vec3& target, glm::vec3& up);
	void build_this();


	// void build_root()
	// {
	//     // u up vector, v right , n normal to plane
	//     _u = glm::vec3(0.0, 1.0, 0.0);
	//     _v = glm::vec3(1.0, 0.0, 0.0);
	//     _n = glm::vec3(0.0, 0.0, 1.0);
	
	//     _pos = glm::vec3(0.0, 0.0, 0.0);
	// }


	// traversig and calculate all node hierarchy
	void build_all();

	void add_child(Node *child_node);
	void delete_child(Node *child_node);

private:

	// perent node
	Node *_parent;


	// list of children node
	NodeList _children;


	// transform matrix
	glm::mat4 _mat;

	glm::mat4 _mat_world;



	// position
	glm::vec3 _pos;

	// target point of view
	glm::vec3 _target;

	// node orientation
	glm::vec3 _u; // uo;
	glm::vec3 _v; //_right;
	glm::vec3 _n; //_forward;

	// angle of Euler
	float _roll;
	float _yaw;
	float _pitch;
};


#endif //NODE_H_
