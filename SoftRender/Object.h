#ifndef OBJECT_H
#define OBJECT_H

#include "Mesh.h"
#include "Material.h"

class Object {

public:
	Mesh mesh;
	Material material;

	Object() = default;
	~Object() = default;
	Object(const Object &obj) {
		mesh = obj.mesh;
		material = obj.material;
	}
	Object(const Mesh &m,const Material &mat) {
		mesh = m;
		material = mat;
	}
	Object & operator= (const Object &obj) {
		if (&obj == this)
			return *this;
		mesh = obj.mesh;
		material = obj.material;
		return *this;
	}
};


#endif
