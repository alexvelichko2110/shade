
#include "node.h"

	Node::Node()
	{
		_parent = nullptr;

		_target = glm::vec4(0.0, 0.0, 0.0, 1.0);


		// angle of Euler
		_roll = 0;
		_yaw = 0.0;
		_pitch = 0.0;


		// bazis
		_pos = glm::vec3(0.0, 0.0, 0.0);

		// u up vector, v right , n normal to plane
	    _u = glm::vec3(0.0, 1.0, 0.0);
	    _v = glm::vec3(1.0, 0.0, 0.0);
	    _n = glm::vec3(0.0, 0.0, 1.0);

	}

	void Node::euler()
	{
		// Calculate the new Front vector
		glm::vec3 n;

		n.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
		n.y = sin(glm::radians(_pitch));
		n.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));


		glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);


		n = glm::normalize(n);// make unit length


		glm::vec3  v = glm::cross(up, n);

		v = glm::normalize(v);


		glm::vec3 u = glm::cross(n, v);

		u = glm::normalize(u);


	    _u = u;
	    _v = v;
	    _n = n;
	}


	// transformation op
	void Node::translate(const glm::vec3& vec)
    {

		glm::mat4 T = glm::mat4(1.0);

		T = glm::translate(T, vec);

		_mat = _mat * T;
	}

	void Node::rotate(float angle, const glm::vec3& vec )
    {

		glm::mat4 M = glm::mat4(1.0);

		M = glm::rotate(M, angle, vec);

		_mat = _mat * M;

	}


	// look at point from another point
	void Node::look_at(glm::vec3& eye, glm::vec3& target, glm::vec3& up)
	{
		// compute the forward vector from target to eye
		glm::vec3 n = eye - target;

		n = glm::normalize(n);


		glm::vec3  v = glm::cross(up, n);

		v = glm::normalize(v);


		glm::vec3 u = glm::cross(n, v);

		u = glm::normalize(u);



		_pos = eye;

	    _u = u;
	    _v = v;
	    _n = n;
	}


	void Node::build_this()
	{
		// init 4x4 matrix
		glm::mat4 mat(1.0);

		// set rotation part, inverse rotation matrix: M^-1 = M^T for Euclidean transform
		mat[0][0] = _v.x;
		mat[1][0] = _v.y;
		mat[2][0] = _v.z;

		mat[0][1] = _u.x;
		mat[1][1] = _u.y;
		mat[2][1] = _u.z;

		mat[0][2] = _n.x;
		mat[1][2] = _n.y;
		mat[2][2] = _n.z;


		mat[3][0] = -glm::dot(_v, _pos);
		mat[3][1] = -glm::dot(_u, _pos);
		mat[3][2] = -glm::dot(_n, _pos);

		_mat = mat;

		if (_parent != nullptr) {
			_mat_world = _parent->get_world_mat() * mat;
		}
	}


void Node::build_all()
{
    if (_children.size() == 0)
        return;

    this->build_this();


    NodeList::iterator it = _children.begin(), end = _children.end();

    for (; it != end; ++it)
    {
        (*it)->build_all();
    }
}

void Node::add_child(Node *child_node)
{
    child_node->set_parent(this);

    _children.push_back(child_node);
}

void Node::delete_child(Node *child_node)
{
    child_node->set_parent(nullptr);

    NodeList::iterator i = _children.begin(), e = _children.end();

    for (; i != e; ++i)
    {
        if ((*i) == child_node)
        {
            _children.erase(i);
            break;
        }
    }
}
