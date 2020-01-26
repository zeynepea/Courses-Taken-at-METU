#ifndef _DEFS_H_
#define _DEFS_H_
#include <iostream>
#include <string.h>
#include <cmath>
using namespace std;
class Scene;



/* 3 dimensional vector holding floating point numbers.
Used for both coordinates and color.
Use x, y, z for coordinate computations, and use r, g, b for color computations.
Note that you do not have to use this one if you use any vector computation library like Eigen. */
class Vector3f
{
	public:
		union
	{
		float x;
		float r;
	};
	union
	{
		float y;
		float g;
	};
	union
	{
		float z;
		float b;
	};

		Vector3f(const Vector3f &vectorToBeCopy):
			x(vectorToBeCopy.x),
			y(vectorToBeCopy.y),
			z(vectorToBeCopy.z){}
		
		Vector3f(float m_x =0.0, float m_y = 0.0, float m_z=0.0):
			x(m_x),
			y(m_y),
			z(m_z) {}
		

		Vector3f operator- () const{

    		return Vector3f(-x, -y, -z);
		}

		Vector3f operator- (const Vector3f &m_vector) const{

    		return Vector3f(x-m_vector.x, y-m_vector.y, z-m_vector.z);
		}

		Vector3f operator+ (const Vector3f &m_vector) const{

    		return Vector3f(m_vector.x+x, m_vector.y+y, m_vector.z+z);
		}

		Vector3f operator* (float m) const{

    		return Vector3f(m*x, m*y, m*z);
		}

		Vector3f operator/ (float m) const{

    		return Vector3f(x/m, y/m, z/m);
		}

		//DOT PRODUCT
		float operator* (const Vector3f &m_vector) const{

    		return m_vector.x*x + m_vector.y*y + m_vector.z*z;
		}

		//CROSS PRODUCT
		Vector3f crossProduct(const Vector3f &m_vector) const{

			return Vector3f(
				y*m_vector.z - z*m_vector.y,
				z*m_vector.x - x*m_vector.z,
				x*m_vector.y - y*m_vector.x
			);
		}

		Vector3f& operator= (const Vector3f &m_vector){
		
			if (this == &m_vector)
				return *this;

			x = m_vector.x; 
			y = m_vector.y;
			z = m_vector.z; 
	
			return *this;
		}
		

};


/* Structure to hold return value from ray intersection routine.
This should hold information related to the intersection point,
for example, coordinate of the intersection point, surface normal at the intersection point etc.
Think about the variables you will need for this purpose and declare them here inside of this structure. */
typedef struct ReturnVal
{

	bool hit = false;
	Vector3f int_point;
	Vector3f normal;
	float t;

} ReturnVal;

//
// The global variable through which you can access the scene data
//
extern Scene* pScene;

#endif
