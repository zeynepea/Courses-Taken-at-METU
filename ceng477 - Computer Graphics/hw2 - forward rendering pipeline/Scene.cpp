#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <cmath>

#include "Scene.h"
#include "Camera.h"
#include "Color.h"
#include "Model.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Triangle.h"
#include "Vec3.h"
#include "tinyxml2.h"
#include "Helpers.h"

using namespace tinyxml2;
using namespace std;


void Scene::translateVertex(int id, Vec3* vertex){
	Vec4 hVertex(vertex->x, vertex->y, vertex->z, 1.0, -1.0);
	//cout<<id<<endl;
	double val[4][4] = {{1.0, 0.0, 0.0, translations[id-1]->tx},
						{0.0, 1.0, 0.0, translations[id-1]->ty},
						{0.0, 0.0, 1.0, translations[id-1]->tz},
						{0.0, 0.0, 0.0, 1.0}};
	Matrix4 tMatrix(val);

	Vec4 result(multiplyMatrixWithVec4(tMatrix, hVertex));
	vertex->x = result.x;
	vertex->y = result.y;
	vertex->z = result.z;
}

void Scene::scaleVertex(int id, Vec3* vertex){
	Vec4 hVertex(vertex->x, vertex->y, vertex->z, 1.0, -1.0);
	double val[4][4] = {{scalings[id-1]->sx, 0.0, 0.0, 0.0},
						{0.0, scalings[id-1]->sy, 0.0, 0.0},
						{0.0, 0.0, scalings[id-1]->sz, 0.0},
						{0.0, 0.0, 0.0, 1.0}};
	Matrix4 sMatrix(val);

	Vec4 result(multiplyMatrixWithVec4(sMatrix, hVertex));
	vertex->x = result.x;
	vertex->y = result.y;
	vertex->z = result.z;
}

void Scene::rotateVertex(int id, Vec3* vertex){
	Vec4 hVertex(vertex->x, vertex->y, vertex->z, 1.0, -1.0);
	double pi = 3.14159265;
	double angle = rotations[id-1]->angle;
	Vec3 v;
	Vec3 uNotNorm(rotations[id-1]->ux,rotations[id-1]->uy,rotations[id-1]->uz, -1.0);
	Vec3 u(normalizeVec3(uNotNorm));

	if((ABS(u.y)>= ABS(u.x))&& (ABS(u.z)>= ABS(u.x))){
		v.x = 0.0;
		v.y = -(u.z);
		v.z = u.y;
	}
	else if((ABS(u.x)>= ABS(u.y))&& (ABS(u.z)>= ABS(u.y))){
		v.x = -(u.z);
		v.y = 0.0;
		v.z = u.x;
	}
	else if((ABS(u.y)>= ABS(u.z))&& (ABS(u.x)>= ABS(u.z))){
		v.x = -(u.y);
		v.y = u.x;
		v.z = 0.0;
	}

	Vec3 w(crossProductVec3(u, v));
	Vec3 vNorm(normalizeVec3(v));
	Vec3 wNorm(normalizeVec3(w));

	//m inverse
	double val1[4][4] = {{u.x, vNorm.x, wNorm.x, 0.0},
					     {u.y, vNorm.y, wNorm.y, 0.0},
					     {u.z, vNorm.z, wNorm.z, 0.0},
					     {0.0, 0.0, 0.0, 1.0}};
	Matrix4 mInverse(val1);

	double val2[4][4] = {{u.x, u.y, u.z, 0.0},
					     {vNorm.x, vNorm.y, vNorm.z, 0.0},
					     {wNorm.x, wNorm.y, wNorm.z, 0.0},
					     {0.0, 0.0, 0.0, 1.0}};
	Matrix4 m(val2);

	double val3[4][4] = {{1.0, 0.0, 0.0, 0.0},
					     {0.0, cos(angle * pi / 180.0), -1.0*sin(angle * pi / 180.0), 0.0},
					     {0.0, sin(angle * pi / 180.0), cos(angle * pi / 180.0), 0.0},
					     {0.0, 0.0, 0.0, 1.0}};

	Matrix4 rotationX(val3);

	Matrix4 first(multiplyMatrixWithMatrix(rotationX, m));
	Matrix4 rMatrix(multiplyMatrixWithMatrix(mInverse, first));

	Vec4 result(multiplyMatrixWithVec4(rMatrix, hVertex));
	vertex->x = result.x;
	vertex->y = result.y;
	vertex->z = result.z;

}
void Scene::transformations(int id, char type, Vec3* vertex){
	switch ( type )
      {
         case 't':
		    translateVertex(id, vertex);
            break;
         case 's':
		 	scaleVertex(id, vertex);
            break;
         case 'r':
		 	rotateVertex(id, vertex);
            break;
      }
}

void Scene::cameraTransformation(Camera *camera, Vec3* vertex){

	Vec4 hVertex(vertex->x, vertex->y, vertex->z, 1.0, -1.0);
	Vec3 uNorm(normalizeVec3(camera->u));
	Vec3 vNorm(normalizeVec3(camera->v));
	Vec3 wNorm(normalizeVec3(camera->w));
	Vec3 e(camera->pos);

	double ct[4][4] = {{uNorm.x, uNorm.y, uNorm.z, -(uNorm.x*e.x + uNorm.y*e.y + uNorm.z*e.z)},
					     {vNorm.x, vNorm.y, vNorm.z, -(vNorm.x*e.x + vNorm.y*e.y + vNorm.z*e.z)},
					     {wNorm.x, wNorm.y, wNorm.z, -(wNorm.x*e.x + wNorm.y*e.y + wNorm.z*e.z)},
					     {0.0, 0.0, 0.0, 1.0}};

	Matrix4 camTrans(ct);

	Vec4 result(multiplyMatrixWithVec4(camTrans, hVertex));
	vertex->x = result.x;
	vertex->y = result.y;
	vertex->z = result.z;

}

void Scene::orthographicProjection(Camera *camera, Vec3* vertex){
	Vec4 hVertex(vertex->x, vertex->y, vertex->z, 1.0, -1.0);
	double op[4][4] = {{2.0/(camera->right-camera->left), 0.0, 0.0, -((camera->right+camera->left)/(camera->right-camera->left))},
					     {0.0, 2.0/(camera->top-camera->bottom), 0.0, -((camera->top+camera->bottom)/(camera->top-camera->bottom))},
					     {0.0, 0.0, -2.0/(camera->far-camera->near), -((camera->far+camera->near)/(camera->far-camera->near))},
					     {0.0, 0.0, 0.0, 1.0}};

	Matrix4 opMatrix(op);

	Vec4 result(multiplyMatrixWithVec4(opMatrix, hVertex));
	vertex->x = result.x;
	vertex->y = result.y;
	vertex->z = result.z;
	/*
	cout<<"--------"<<endl;
	cout<<vertex->x<<endl;
	cout<<vertex->y<<endl;
	cout<<vertex->z<<endl;*/
}

void Scene::perspectiveProjection(Camera *camera, Vec3* vertex,int i){
	Vec4 hVertex(vertex->x, vertex->y, vertex->z, 1.0, -1.0);
	double pp[4][4] = {{2.0*camera->near/(camera->right-camera->left), 0.0, ((camera->right+camera->left)/(camera->right-camera->left)), 0.0},
					     {0.0, 2.0*camera->near/(camera->top-camera->bottom), ((camera->top+camera->bottom)/(camera->top-camera->bottom)), 0.0},
					     {0.0, 0.0, -((camera->far+camera->near)/(camera->far-camera->near)), -2.0*camera->far*camera->near/(camera->far-camera->near)},
					     {0.0, 0.0, -1.0, 0.0}};

	Matrix4 ppMatrix(pp);

	Vec4 result(multiplyMatrixWithVec4(ppMatrix, hVertex));
	vertex->x = result.x;
	vertex->y = result.y;
	vertex->z = result.z;

	//CLIPPING SHOULD BE DONE HERE
	
	//perspective divide
	vertex->x = result.x/result.t;
	vertex->y = result.y/result.t;
	vertex->z = result.z/result.t;
	
	
	/*
	if(vertex->x>1 || vertex->x<-1||vertex->y<-1||vertex->y>1){
		cout<<"vertex id: "<<i<<endl;
		cout<<"--------"<<endl;
		cout<<vertex->x<<endl;
		cout<<vertex->y<<endl;
		cout<<vertex->z<<endl;
	}*/
	
}

void Scene::viewportTransformation(Camera *camera, Vec3* vertex){
	Vec4 hVertex(vertex->x, vertex->y, vertex->z, 1.0, -1.0);
	double vp[4][4] = {{(double)(camera->horRes)/2.0, 0.0, 0.0, ((double)(camera->horRes)-1.0)/2.0},
					     {0.0, (double)(camera->verRes)/2.0, 0.0, ((double)(camera->verRes)-1.0)/2.0},
					     {0.0, 0.0, 1.0/2.0, 1.0/2.0},
					 	 {0.0, 0.0, 0.0, 1.0}};

	Matrix4 vpMatrix(vp);

	Vec4 result(multiplyMatrixWithVec4(vpMatrix, hVertex));
	vertex->x = round(result.x);
	vertex->y = round(result.y);
	vertex->z = round(result.z);
	
	/*
	cout<<camera->horRes<<"****"<<camera->verRes<<endl;
	cout<<"--------"<<endl;
	cout<<vertex->x<<endl;
	cout<<vertex->y<<endl;
	cout<<vertex->z<<endl;
	cout<<result.t<<endl;*/
}

void Scene::viewingTransformation(Camera *camera, Vec3* vertex,int i){
	cameraTransformation(camera, vertex);

	if(projectionType == 0){
		orthographicProjection(camera, vertex);
	}
	else if(projectionType == 1){
		perspectiveProjection(camera, vertex,i);
	}

	

}

void Scene::culling(Camera *camera, Model *model){
	//int newNumber =0;

	for(int i=0;i<model->numberOfTriangles;i++){
		Vec3 edge01(subtractVec3(*(model->modelTransformedVertices[model->triangles[i].getSecondVertexId()-1]), *(model->modelTransformedVertices[model->triangles[i].getFirstVertexId()-1])));
		Vec3 edge02(subtractVec3(*(model->modelTransformedVertices[model->triangles[i].getThirdVertexId()-1]), *(model->modelTransformedVertices[model->triangles[i].getFirstVertexId()-1])));
		Vec3 normal(crossProductVec3(edge01, edge02));
		Vec3 etp(subtractVec3(*model->modelTransformedVertices[model->triangles[i].getFirstVertexId()-1], camera->pos));
		/*if(dotProductVec3(normal, etp)<0.0){
			
			cout<<"before: "<< model->numberOfTriangles<<endl;
			//model->numberOfTriangles=(model->numberOfTriangles)-1;
			cout<<"after: "<< model->numberOfTriangles<<endl;
			//model->triangles[i].visible = false;
		}*/
		if(dotProductVec3(normal, etp)<0.0){
			//newNumber++;
			model->facedTriangles.push_back(model->triangles[i]);
		}
	}
	
	model->numberOfFacedTriangles = model->facedTriangles.size();
	//cout<<"after: "<< newNumber<<endl;
}
void Scene::slopeRange(double m, Vec3 p1, Vec3 p2){
	
	if(m>0.0 && m<=1.0){
		//cout<<"only one: "<<m<<endl;
		int y = p1.y;
		int d = 2*(p1.y-p2.y) + (p2.x-p1.x);
		//Color c(colorsOfVertices[p1.colorId-1]->r,colorsOfVertices[p1.colorId-1]->g,colorsOfVertices[p1.colorId-1]->b); //color of first point
		Color c(*colorsOfVertices[p1.colorId-1]);
		double r = (colorsOfVertices[p2.colorId-1]->r- c.r) / ((double)(p2.x-p1.x));
		double g = (colorsOfVertices[p2.colorId-1]->g- c.g) / ((double)(p2.x-p1.x));
		double b = (colorsOfVertices[p2.colorId-1]->b- c.b) / ((double)(p2.x-p1.x));
		//cout<<"red: "<<r<<" green: "<<g<<" blue: "<<b<<endl;
		//cout<<"dist "<<p2.x-p1.x<<endl;
		//r = makeBetweenZeroAnd255(r);
		//g = makeBetweenZeroAnd255(g);
		//b = makeBetweenZeroAnd255(b);
		Color dc(r, g, b);
		//cout<<"dc val: **** "<<dc<<endl;
		for(int x=p1.x; x<p2.x; x++){
			if(x>image.size()-1||y>image[0].size()-1){
				if(d<0){
					y = y+1;
					d += 2*(p1.y - p2.y + p2.x - p1.x);
				}
				else{
					d += 2*(p1.y - p2.y);
				}
				c.r += dc.r;
				c.g += dc.g;
				c.b += dc.b;
				continue;
			}
			if(x<0||y<0){
				if(d<0){
					y = y+1;
					d += 2*(p1.y - p2.y + p2.x - p1.x);
				}
				else{
					d += 2*(p1.y - p2.y);
				}
				c.r += dc.r;
				c.g += dc.g;
				c.b += dc.b;
				continue;
			}
			//cout<<"	X: " << image.size()<< "  "<<x<<"	y: "<< image[0].size()<< "  " << y<<endl;
			image[x][y].r = round(c.r);
			image[x][y].g = round(c.g);
			image[x][y].b = round(c.b);
			if(d<0){
				y = y+1;
				d += 2*(p1.y - p2.y + p2.x - p1.x);
			}
			else{
				d += 2*(p1.y - p2.y);
			}
			c.r += dc.r;
			c.g += dc.g;
			c.b += dc.b;
		}

	}


	else if(m<0.0 && m>=-1.0){
		//cout<<"only one: "<<m<<endl;
		int y = p1.y;
		int d = -2*(p1.y-p2.y) + (p2.x-p1.x);
		//Color c(colorsOfVertices[p1.colorId-1]->r,colorsOfVertices[p1.colorId-1]->g,colorsOfVertices[p1.colorId-1]->b); //color of first point
		Color c(*colorsOfVertices[p1.colorId-1]);
		double r = (colorsOfVertices[p2.colorId-1]->r- c.r) / ((double)(p2.x-p1.x));
		double g = (colorsOfVertices[p2.colorId-1]->g- c.g) / ((double)(p2.x-p1.x));
		double b = (colorsOfVertices[p2.colorId-1]->b- c.b) / ((double)(p2.x-p1.x));
		//cout<<"red: "<<r<<" green: "<<g<<" blue: "<<b<<endl;
		//cout<<"dist "<<p2.x-p1.x<<endl;
		//r = makeBetweenZeroAnd255(r);
		//g = makeBetweenZeroAnd255(g);
		//b = makeBetweenZeroAnd255(b);
		Color dc(r, g, b);
		//cout<<"dc val: **** "<<dc<<endl;
		for(int x=p1.x; x<p2.x; x++){
			if(x>image.size()-1){
				if(d<0){
					y = y-1;
					d += 2*(p2.y - p1.y + p2.x - p1.x);
				}
				else{
					d += 2*(p2.y - p1.y);
				}
				c.r += dc.r;
				c.g += dc.g;
				c.b += dc.b;
				continue;
			}
			if(y<0){
				if(d<0){
					y = y-1;
					d += 2*(p2.y - p1.y + p2.x - p1.x);
				}
				else{
					d += 2*(p2.y - p1.y);
				}
				c.r += dc.r;
				c.g += dc.g;
				c.b += dc.b;
				continue;
			}
			if(x<0){
				if(d<0){
					y = y-1;
					d += 2*(p2.y - p1.y + p2.x - p1.x);
				}
				else{
					d += 2*(p2.y - p1.y);
				}
				c.r += dc.r;
				c.g += dc.g;
				c.b += dc.b;
				continue;
			}
			image[x][y].r = round(c.r);
			image[x][y].g = round(c.g);
			image[x][y].b = round(c.b);
			if(d<0){
				y = y-1;
				d += 2*(p2.y - p1.y + p2.x - p1.x);
			}
			else{
				d += 2*(p2.y - p1.y);
			}
			c.r += dc.r;
			c.g += dc.g;
			c.b += dc.b;
		}

	}
	

	else if(m>1.0){
		//cout<<"only one: "<<m<<endl;
		int x = p1.x;
		int d = 2*(p1.x-p2.x) + (p2.y-p1.y);
		//Color c(colorsOfVertices[p1.colorId-1]->r,colorsOfVertices[p1.colorId-1]->g,colorsOfVertices[p1.colorId-1]->b); //color of first point
		Color c(*colorsOfVertices[p1.colorId-1]);
		double r = (colorsOfVertices[p2.colorId-1]->r- c.r) / ((double)(p2.y-p1.y));
		double g = (colorsOfVertices[p2.colorId-1]->g- c.g) / ((double)(p2.y-p1.y));
		double b = (colorsOfVertices[p2.colorId-1]->b- c.b) / ((double)(p2.y-p1.y));
		//cout<<"red: "<<r<<" green: "<<g<<" blue: "<<b<<endl;
		//cout<<"dist "<<p2.x-p1.x<<endl;
		//r = makeBetweenZeroAnd255(r);
		//g = makeBetweenZeroAnd255(g);
		//b = makeBetweenZeroAnd255(b);
		Color dc(r, g, b);
		//cout<<"dc val: **** "<<dc<<endl;
		for(int y=p1.y; y<p2.y; y++){
			if(x>image.size()-1||y>image[0].size()-1){
				if(d<0){
					x = x+1;
					d += 2*(p1.x - p2.x + p2.y - p1.y);
				}
				else{
					d += 2*(p1.x - p2.x);
				}
				c.r += dc.r;
				c.g += dc.g;
				c.b += dc.b;
				continue;
			}
			if(x<0||y<0){
				if(d<0){
					x = x+1;
					d += 2*(p1.x - p2.x + p2.y - p1.y);
				}
				else{
					d += 2*(p1.x - p2.x);
				}
				c.r += dc.r;
				c.g += dc.g;
				c.b += dc.b;
				continue;
			}
			image[x][y].r = round(c.r);
			image[x][y].g = round(c.g);
			image[x][y].b = round(c.b);
			if(d<0){
				x = x+1;
				d += 2*(p1.x - p2.x + p2.y - p1.y);
			}
			else{
				d += 2*(p1.x - p2.x);
			}
			c.r += dc.r;
			c.g += dc.g;
			c.b += dc.b;
		}

	}
	
	
	else if(m<-1.0){
		//cout<<"only one: "<<m<<endl;
		int x = p1.x;
		int d = -2*(p1.x-p2.x) + (p2.y-p1.y);
		//Color c(colorsOfVertices[p1.colorId-1]->r,colorsOfVertices[p1.colorId-1]->g,colorsOfVertices[p1.colorId-1]->b); //color of first point
		Color c(*colorsOfVertices[p1.colorId-1]);
		double r = (colorsOfVertices[p2.colorId-1]->r- c.r) / ((double)(p1.y-p2.y));
		double g = (colorsOfVertices[p2.colorId-1]->g- c.g) / ((double)(p1.y-p2.y));
		double b = (colorsOfVertices[p2.colorId-1]->b- c.b) / ((double)(p1.y-p2.y));
		//cout<<"red: "<<r<<" green: "<<g<<" blue: "<<b<<endl;
		//cout<<"dist "<<p2.x-p1.x<<endl;
		//r = makeBetweenZeroAnd255(r);
		//g = makeBetweenZeroAnd255(g);
		//b = makeBetweenZeroAnd255(b);
		Color dc(r, g, b);
		//cout<<"dc val: **** "<<dc<<endl;
		for(int y=p1.y; y>p2.y; y--){
			if(x>image.size()-1){
				if(d>0){
					x = x+1;
					d += 2*(p2.x - p1.x + p2.y - p1.y);
				}
				else{
					d += 2*(p2.x - p1.x);
				}
				c.r += dc.r;
				c.g += dc.g;
				c.b += dc.b;
				continue;
			}
			if(y<0){
				if(d>0){
					x = x+1;
					d += 2*(p2.x - p1.x + p2.y - p1.y);
				}
				else{
					d += 2*(p2.x - p1.x);
				}
				c.r += dc.r;
				c.g += dc.g;
				c.b += dc.b;
				continue;
			}
			if(x<0){
				if(d>0){
					x = x+1;
					d += 2*(p2.x - p1.x + p2.y - p1.y);
				}
				else{
					d += 2*(p2.x - p1.x);
				}
				c.r += dc.r;
				c.g += dc.g;
				c.b += dc.b;
				continue;
			}
			image[x][y].r = round(c.r);
			image[x][y].g = round(c.g);
			image[x][y].b = round(c.b);
			if(d>0){
				x = x+1;
				d += 2*(p2.x - p1.x + p2.y - p1.y);
			}
			else{
				d += 2*(p2.x - p1.x);
			}
			c.r += dc.r;
			c.g += dc.g;
			c.b += dc.b;
		}

	}
	
	


}

void Scene::midpoint(Model* model){
	for(int i=0; i<model->numberOfFacedTriangles; i++){
		//cout<<"sizefaced: "<<model->facedTriangles.size()<<"   i:"<< i<<endl;
		Vec3 v0(*(model->modelTransformedVertices[model->facedTriangles[i].getFirstVertexId()-1]));
		Vec3 v1(*(model->modelTransformedVertices[model->facedTriangles[i].getSecondVertexId()-1]));
		Vec3 v2(*(model->modelTransformedVertices[model->facedTriangles[i].getThirdVertexId()-1]));

		//v0-1
		if(v0.x<=v1.x){
			double m01 = (double)(v1.y-v0.y)/(double)(v1.x-v0.x);
			//cout<<"m01 is: "<<m01<<endl;
			slopeRange(m01, v0, v1);
		}
		else if(v0.x>v1.x){
			double m10 = (double)(v0.y-v1.y)/(double)(v0.x-v1.x);
			//cout<<"m10 is: "<<m10<<endl;
			slopeRange(m10, v1, v0);
		}
		//cout<<"m01 is: "<<m01<<endl;
		

		//v0-2
		if(v0.x<v2.x){
			double m02 = (double)(v2.y-v0.y)/(double)(v2.x-v0.x);
			//cout<<"m02 is: "<<m02<<endl;
			slopeRange(m02, v0, v2);
		}
		else if(v0.x>v2.x){
			double m20 = (double)(v0.y-v2.y)/(double)(v0.x-v2.x);
			//cout<<"m02 is: "<<m02<<endl;
			slopeRange(m20, v2, v0);
		}
		

		//v1-2
		if(v1.x<v2.x){
			double m12 = (double)(v2.y-v1.y)/(double)(v2.x-v1.x);
			//cout<<"m12 is: "<<m12<<endl;
			slopeRange(m12, v1, v2);
		}
		else if(v1.x>v2.x){
			double m21 = (double)(v1.y-v2.y)/(double)(v1.x-v2.x);
			//cout<<"m12 is: "<<m12<<endl;
			slopeRange(m21, v2, v1);
		}
		

	}
	

	
}

int Scene::min3(int a, int b, int c){
	return min(min(a,b),c);
}

int Scene::max3(int a, int b, int c){
	return max(max(a,b),c);
}

int Scene::f01(int x, int y, int x0, int y0, int x1, int y1){
	return (x*(y0-y1) + y*(x1-x0) + x0*y1 - y0*x1);
}
int Scene::f12(int x, int y, int x1, int y1, int x2, int y2){
	return (x*(y1-y2) + y*(x2-x1) + x1*y2 - y1*x2);
}
int Scene::f20(int x, int y, int x0, int y0, int x2, int y2){
	return (x*(y2-y0) + y*(x0-x2) + x2*y0 - y2*x0);
}

void Scene::rasterization(Model* model){
	//cout<<"RASTAR"<<endl;
	for(int i=0; i<model->numberOfFacedTriangles; i++){
		
		int x0 = model->modelTransformedVertices[model->facedTriangles[i].getFirstVertexId()-1]->x; 
		int y0 = model->modelTransformedVertices[model->facedTriangles[i].getFirstVertexId()-1]->y;
		Color c0(*colorsOfVertices[model->modelTransformedVertices[model->facedTriangles[i].getFirstVertexId()-1]->colorId-1]);

		int x1 = model->modelTransformedVertices[model->facedTriangles[i].getSecondVertexId()-1]->x; 
		int y1 = model->modelTransformedVertices[model->facedTriangles[i].getSecondVertexId()-1]->y;
		Color c1(*colorsOfVertices[model->modelTransformedVertices[model->facedTriangles[i].getSecondVertexId()-1]->colorId-1]);

		int x2 = model->modelTransformedVertices[model->facedTriangles[i].getThirdVertexId()-1]->x; 
		int y2 = model->modelTransformedVertices[model->facedTriangles[i].getThirdVertexId()-1]->y;
		Color c2(*colorsOfVertices[model->modelTransformedVertices[model->facedTriangles[i].getThirdVertexId()-1]->colorId-1]);

		int xMax = max3(x0, x1, x2);
		int xMin = min3(x0, x1, x2);
		int yMax = max3(y0, y1, y2);
		int yMin = min3(y0, y1, y2); 

		for(int y=yMin; y<=yMax; y++){
			for(int x=xMin; x<=xMax; x++){
				double alpha = (double)f12(x, y, x1, y1, x2, y2) / (double)f12(x0, y0, x1, y1, x2, y2);
				double beta = (double)f20(x, y, x0, y0, x2, y2) / (double)f20(x1, y1, x0, y0, x2, y2);
				double gama = (double)f01(x, y, x0, y0, x1, y1) / (double)f01(x2, y2, x0, y0, x1, y1);
				/*if(isnan(alpha)||isnan(beta)||isnan(gama)){
					cout<<"YOUR NAN"<<endl;
					if(x>image.size()-1||y>image[0].size()-1)
						continue;
					if(x<0||y<0)
						continue;
					image[x][y].r = c0.r+c1.r+c2.r;
					image[x][y].g = c0.g+c1.g+c2.g;
					image[x][y].b = c0.b+c1.b+c2.b;
				}*/
				if((alpha>=0) && (beta>=0) && (gama>=0)){
					if(x>image.size()-1||y>image[0].size()-1)
						continue;
					if(x<0||y<0)
						continue;
					image[x][y].r = round(alpha*c0.r + beta*c1.r + gama*c2.r);
					image[x][y].g = round(alpha*c0.g + beta*c1.g + gama*c2.g);
					image[x][y].b = round(alpha*c0.b + beta*c1.b + gama*c2.b);
				}
			}
		}
	}
}

void Scene::modelingTransformation(){
	cout<<"modeling"<<endl;
	//for every models
	
	for(int i=0; i<models.size(); i++){
		//for every triangle in models
		for(int j =0; j<=vertices.size(); j++){
			models[i]->vIds.push_back(0);
		}
		models[i]->modelVertices.clear();
		for(int j =0; j<vertices.size(); j++){
			Vec3* vec = new Vec3(*vertices[j]);
			models[i]->modelVertices.push_back(vec);
		}
		for(int j=0; j<models[i]->numberOfTriangles; j++){
			//for every vertice of triangle
			for(int k=0; k<3; k++){
				//for every transformations in models
				models[i]->vIds.at(models[i]->triangles[j].vertexIds[k])++;
				
				if(models[i]->vIds.at(models[i]->triangles[j].vertexIds[k]) == 1){
					for(int l=0; l<models[i]->numberOfTransformations;l++){
						transformations(models[i]->transformationIds[l], models[i]->transformationTypes[l], models[i]->modelVertices[(models[i]->triangles[j].vertexIds[k])-1]);
					}
				}
			}	
		}
	}	
}


/*
	Transformations, clipping, culling, rasterization are done here.
	You can define helper functions inside Scene class implementation.
*/
void Scene::forwardRenderingPipeline(Camera *camera)
{
	/***********Modelling Transformation***********************/
	
	
	for(int i=0; i<models.size(); i++){
		models[i]->modelTransformedVertices.clear();
		for(int j =0; j<models[i]->modelVertices.size(); j++){
			Vec3* vec = new Vec3(*(models[i]->modelVertices[j]));
			models[i]->modelTransformedVertices.push_back(vec);
		}
	}
    
    /***********Culling****************************************/
	for(int i=0; i<models.size(); i++){

		models[i]->facedTriangles.clear();
		if(cullingEnabled == 1)
			culling(camera, models[i]);
		else{
			for(int j = 0; j<models[i]->numberOfTriangles; j++){
				models[i]->facedTriangles.push_back(models[i]->triangles[j]);
			}
			models[i]->numberOfFacedTriangles = models[i]->facedTriangles.size();
		}
	}

	/************Viewing ***************************************/
	for(int k=0; k<models.size(); k++){
	for(int i=0; i<models[k]->modelTransformedVertices.size(); i++){
		
		viewingTransformation(camera, models[k]->modelTransformedVertices[i],i+1);
		viewportTransformation(camera, models[k]->modelTransformedVertices[i]);
	}}

	
    
	/************Clipping ***************************************/
	/*for(int i=0; i<models.size(); i++){
		if(models[i]->type == 0){
			clipping(models[i]);
		}	
	}*/
	
	
	/************Viewport***************************************/
	/*for(int i=0; i<vertices.size(); i++){
		viewportTransformation(camera, transformedVertices[i]);
	}*/
	
	for(int i=0; i<models.size(); i++){
		if(models[i]->type == 0)
			midpoint(models[i]);
		if(models[i]->type == 1)//solid mode
			rasterization(models[i]);
	}
	
}

/*
	Parses XML file
*/
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLElement *pElement;

	xmlDoc.LoadFile(xmlPath);

	XMLNode *pRoot = xmlDoc.FirstChild();

	// read background color
	pElement = pRoot->FirstChildElement("BackgroundColor");
	str = pElement->GetText();
	sscanf(str, "%lf %lf %lf", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	// read culling
	pElement = pRoot->FirstChildElement("Culling");
	if (pElement != NULL)
		pElement->QueryBoolText(&cullingEnabled);

	// read projection type
	pElement = pRoot->FirstChildElement("ProjectionType");
	if (pElement != NULL)
		pElement->QueryIntText(&projectionType);

	// read cameras
	pElement = pRoot->FirstChildElement("Cameras");
	XMLElement *pCamera = pElement->FirstChildElement("Camera");
	XMLElement *camElement;
	while (pCamera != NULL)
	{
		Camera *cam = new Camera();

		pCamera->QueryIntAttribute("id", &cam->cameraId);

		camElement = pCamera->FirstChildElement("Position");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->pos.x, &cam->pos.y, &cam->pos.z);

		camElement = pCamera->FirstChildElement("Gaze");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->gaze.x, &cam->gaze.y, &cam->gaze.z);

		camElement = pCamera->FirstChildElement("Up");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->v.x, &cam->v.y, &cam->v.z);

		cam->gaze = normalizeVec3(cam->gaze);
		cam->u = crossProductVec3(cam->gaze, cam->v);
		cam->u = normalizeVec3(cam->u);

		cam->w = inverseVec3(cam->gaze);
		cam->v = crossProductVec3(cam->u, cam->gaze);
		cam->v = normalizeVec3(cam->v);

		camElement = pCamera->FirstChildElement("ImagePlane");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf %lf %lf %lf %d %d",
			   &cam->left, &cam->right, &cam->bottom, &cam->top,
			   &cam->near, &cam->far, &cam->horRes, &cam->verRes);

		camElement = pCamera->FirstChildElement("OutputName");
		str = camElement->GetText();
		cam->outputFileName = string(str);

		cameras.push_back(cam);

		pCamera = pCamera->NextSiblingElement("Camera");
	}

	// read vertices
	pElement = pRoot->FirstChildElement("Vertices");
	XMLElement *pVertex = pElement->FirstChildElement("Vertex");
	int vertexId = 1;

	while (pVertex != NULL)
	{
		Vec3 *vertex = new Vec3();
		Color *color = new Color();

		vertex->colorId = vertexId;

		str = pVertex->Attribute("position");
		sscanf(str, "%lf %lf %lf", &vertex->x, &vertex->y, &vertex->z);

		str = pVertex->Attribute("color");
		sscanf(str, "%lf %lf %lf", &color->r, &color->g, &color->b);

		vertices.push_back(vertex);
		colorsOfVertices.push_back(color);

		pVertex = pVertex->NextSiblingElement("Vertex");

		vertexId++;
	}

	// read translations
	pElement = pRoot->FirstChildElement("Translations");
	XMLElement *pTranslation = pElement->FirstChildElement("Translation");
	while (pTranslation != NULL)
	{
		Translation *translation = new Translation();

		pTranslation->QueryIntAttribute("id", &translation->translationId);

		str = pTranslation->Attribute("value");
		sscanf(str, "%lf %lf %lf", &translation->tx, &translation->ty, &translation->tz);

		translations.push_back(translation);

		pTranslation = pTranslation->NextSiblingElement("Translation");
	}

	// read scalings
	pElement = pRoot->FirstChildElement("Scalings");
	XMLElement *pScaling = pElement->FirstChildElement("Scaling");
	while (pScaling != NULL)
	{
		Scaling *scaling = new Scaling();

		pScaling->QueryIntAttribute("id", &scaling->scalingId);
		str = pScaling->Attribute("value");
		sscanf(str, "%lf %lf %lf", &scaling->sx, &scaling->sy, &scaling->sz);

		scalings.push_back(scaling);

		pScaling = pScaling->NextSiblingElement("Scaling");
	}

	// read rotations
	pElement = pRoot->FirstChildElement("Rotations");
	XMLElement *pRotation = pElement->FirstChildElement("Rotation");
	while (pRotation != NULL)
	{
		Rotation *rotation = new Rotation();

		pRotation->QueryIntAttribute("id", &rotation->rotationId);
		str = pRotation->Attribute("value");
		sscanf(str, "%lf %lf %lf %lf", &rotation->angle, &rotation->ux, &rotation->uy, &rotation->uz);

		rotations.push_back(rotation);

		pRotation = pRotation->NextSiblingElement("Rotation");
	}

	// read models
	pElement = pRoot->FirstChildElement("Models");

	XMLElement *pModel = pElement->FirstChildElement("Model");
	XMLElement *modelElement;
	while (pModel != NULL)
	{
		Model *model = new Model();

		pModel->QueryIntAttribute("id", &model->modelId);
		pModel->QueryIntAttribute("type", &model->type);

		// read model transformations
		XMLElement *pTransformations = pModel->FirstChildElement("Transformations");
		XMLElement *pTransformation = pTransformations->FirstChildElement("Transformation");

		pTransformations->QueryIntAttribute("count", &model->numberOfTransformations);

		while (pTransformation != NULL)
		{
			char transformationType;
			int transformationId;

			str = pTransformation->GetText();
			sscanf(str, "%c %d", &transformationType, &transformationId);

			model->transformationTypes.push_back(transformationType);
			model->transformationIds.push_back(transformationId);

			pTransformation = pTransformation->NextSiblingElement("Transformation");
		}

		// read model triangles
		XMLElement *pTriangles = pModel->FirstChildElement("Triangles");
		XMLElement *pTriangle = pTriangles->FirstChildElement("Triangle");

		pTriangles->QueryIntAttribute("count", &model->numberOfTriangles);

		while (pTriangle != NULL)
		{
			int v1, v2, v3;

			str = pTriangle->GetText();
			sscanf(str, "%d %d %d", &v1, &v2, &v3);

			model->triangles.push_back(Triangle(v1, v2, v3));

			pTriangle = pTriangle->NextSiblingElement("Triangle");
		}

		models.push_back(model);

		pModel = pModel->NextSiblingElement("Model");
	}
}

/*
	Initializes image with background color
*/
void Scene::initializeImage(Camera *camera)
{
	if (this->image.empty())
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			vector<Color> rowOfColors;

			for (int j = 0; j < camera->verRes; j++)
			{
				rowOfColors.push_back(this->backgroundColor);
			}

			this->image.push_back(rowOfColors);
		}
	}
	// if image is filled before, just change color rgb values with the background color
	else
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			for (int j = 0; j < camera->verRes; j++)
			{
				this->image[i][j].r = this->backgroundColor.r;
				this->image[i][j].g = this->backgroundColor.g;
				this->image[i][j].b = this->backgroundColor.b;
			}
		}
	}
}

/*
	If given value is less than 0, converts value to 0.
	If given value is more than 255, converts value to 255.
	Otherwise returns value itself.
*/
int Scene::makeBetweenZeroAnd255(double value)
{
	if (value >= 255.0)
		return 255;
	if (value <= 0.0)
		return 0;
	return (int)(value);
}

/*
	Writes contents of image (Color**) into a PPM file.
*/
void Scene::writeImageToPPMFile(Camera *camera)
{
	ofstream fout;

	fout.open(camera->outputFileName.c_str());

	fout << "P3" << endl;
	fout << "# " << camera->outputFileName << endl;
	fout << camera->horRes << " " << camera->verRes << endl;
	fout << "255" << endl;

	for (int j = camera->verRes - 1; j >= 0; j--)
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			fout << makeBetweenZeroAnd255(this->image[i][j].r) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].g) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].b) << " ";
		}
		fout << endl;
	}
	fout.close();
}

/*
	Converts PPM image in given path to PNG file, by calling ImageMagick's 'convert' command.
	os_type == 1 		-> Ubuntu
	os_type == 2 		-> Windows
	os_type == other	-> No conversion
*/
void Scene::convertPPMToPNG(string ppmFileName, int osType)
{
	string command;

	// call command on Ubuntu
	if (osType == 1)
	{
		command = "convert " + ppmFileName + " " + ppmFileName + ".png";
		system(command.c_str());
	}

	// call command on Windows
	else if (osType == 2)
	{
		command = "magick convert " + ppmFileName + " " + ppmFileName + ".png";
		system(command.c_str());
	}

	// default action - don't do conversion
	else
	{
	}
}