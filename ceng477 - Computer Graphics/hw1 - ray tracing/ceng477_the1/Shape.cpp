#include "Shape.h"
#include "Scene.h"
#include <cstdio>

Shape::Shape(void)
{
}

Shape::Shape(int id, int matIndex)
    : id(id), matIndex(matIndex)
{
}

Sphere::Sphere(void)
{}

/* Constructor for sphere. You will implement this. */
Sphere::Sphere(int id, int matIndex, int cIndex, float R, vector<Vector3f> *pVertices)
    : Shape(id, matIndex)
{
	this->cIndex = cIndex;
    this->R = R;
    this->pVertices = pVertices;
}

/* Sphere-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Sphere::intersect(const Ray & ray) const
{
	Vector3f center = (*pVertices)[cIndex-1];

    float A = ray.direction * ray.direction;
    float B = (ray.direction * (ray.origin - center)) * 2;
    float C = (ray.origin - center) * (ray.origin - center) - (this->R * this->R);

    float delta = B*B - 4*A*C;

    ReturnVal result;

    if(delta < 0){
        result.hit = false;
        return result;
    }
    else{
        float t1 = (-B - sqrt(delta))/(2*A);
        float t2 = (-B + sqrt(delta))/(2*A);
        if(t1 > pScene->intTestEps && t2 > pScene->intTestEps){
            float t = (t1<=t2) ? t1 : t2;

            result.hit = true;
            result.int_point = ray.getPoint(t);
            result.normal = (result.int_point - center) / R;
        }
        

        return result;
    }
}

Triangle::Triangle(void)
{}

/* Constructor for triangle. You will implement this. */
Triangle::Triangle(int id, int matIndex, int p1Index, int p2Index, int p3Index, vector<Vector3f> *pVertices)
    : Shape(id, matIndex)
{
	this->p1Index = p1Index;
    this->p2Index = p2Index;
    this->p3Index = p3Index;
    this->pVertices = pVertices;
}
float Triangle::determinant(const Vector3f &first, const Vector3f &second, const Vector3f &other)const{
            return (first.x * (second.y*other.z - other.y*second.z)
                + first.y * (other.x*second.z - second.x*other.z)
                + first.z * (second.x*other.y - second.y*other.x));
}
/* Triangle-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Triangle::intersect(const Ray & ray) const
{
    //printf("hallo\n");
	Vector3f a = (*pVertices)[p1Index-1];
    Vector3f b = (*pVertices)[p2Index-1];
    Vector3f c = (*pVertices)[p3Index-1];
    ReturnVal result;

    Vector3f a_b = a - b;
    Vector3f a_c = a - c;
    Vector3f a_o = a - ray.origin;

    float detA = determinant(a_b, a_c, ray.direction);
    //printf("%f\n", detA );
    if(detA == 0){
        result.hit = false;
        return result;
    } 
    //printf("deneme\n");
    float t = determinant(a_b, a_c, a_o)/detA;
    float beta = determinant(a_o, a_c, ray.direction)/detA;
    float gamma = determinant(a_b, a_o, ray.direction)/detA;
    float alpha = 1 - gamma - beta;
    //printf("%f---%f\n",t,beta );
    if(t<=pScene->intTestEps){
        result.hit = false;
        return result;
    }
    if(gamma < 0 || gamma > 1){
        result.hit = false;
        return result;
    } 
    if(0 > beta || beta > 1){
        result.hit = false;
        return result;
    }
     if(0 > alpha || alpha > 1){
        result.hit = false;
        return result;
    }

    result.hit = true;
    result.t = t;
    result.int_point = ray.getPoint(t);
    Vector3f b_a = b-a;
    Vector3f c_a = c-a;
    Vector3f normal = b_a.crossProduct(c_a);
    float normal_len = sqrt(normal * normal);
    normal = normal / normal_len;

    result.normal = normal;
    return result;
}

Mesh::Mesh()
{}

/* Constructor for mesh. You will implement this. */
Mesh::Mesh(int id, int matIndex, const vector<Triangle>& faces, vector<int> *pIndices, vector<Vector3f> *pVertices)
    : Shape(id, matIndex)
{
	this->faces = faces;
    this->pIndices = pIndices;
    this->pVertices = pVertices;
}

/* Mesh-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Mesh::intersect(const Ray & ray) const
{
	ReturnVal result;
    result.t = INT8_MAX;
    for(int i=0; i<faces.size(); i++){
        ReturnVal temp;
        temp = faces[i].intersect(ray);
        if(temp.hit == true){
            if(result.t > temp.t){
                result = temp;
            }
        }
    }
    return result;
}
