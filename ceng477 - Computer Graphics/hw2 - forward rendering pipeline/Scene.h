#ifndef _SCENE_H_
#define _SCENE_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "Camera.h"
#include "Color.h"
#include "Model.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Triangle.h"
#include "Vec3.h"
#include "Vec4.h"

using namespace std;

class Scene
{
public:
	Color backgroundColor;
	bool cullingEnabled;
	int projectionType;
	int flag=0;

	vector< vector<Color> > image;
	vector< Camera* > cameras;
	vector< Vec3* > vertices;
	vector< Vec3* > transformedVertices;
	vector< Vec3* > cVertices;
	vector< Color* > colorsOfVertices;
	vector< Scaling* > scalings;
	vector< Rotation* > rotations;
	vector< Translation* > translations;
	vector< Model* > models;

	Scene(const char *xmlPath);

	void initializeImage(Camera* camera);
	void forwardRenderingPipeline(Camera* camera);
	int makeBetweenZeroAnd255(double value);
	void writeImageToPPMFile(Camera* camera);
	void convertPPMToPNG(string ppmFileName, int osType);
	//our helper functions
	void modelingTransformation();
	void translateVertex(int id, Vec3* vertex);
	void scaleVertex(int id, Vec3* vertex);
	void rotateVertex(int id, Vec3* vertex);
	void transformations(int id, char type, Vec3* vertex);
	void viewingTransformation(Camera *camera, Vec3* vertex,int i);
	void viewportTransformation(Camera *camera, Vec3* vertex);
	void cameraTransformation(Camera *camera,Vec3* vertex);
	void orthographicProjection(Camera *camera, Vec3* vertex);
	void perspectiveProjection(Camera *camera, Vec3* vertex,int i);
	void clipping(Model *model);
	int  clipEdge(Vec3 *A, Vec3 *B);
	void culling(Camera *camera, Model *model);
	void midpoint(Model* model);
	void slopeRange(double m, Vec3 x, Vec3 y);
	void rasterization(Model* model);
	int min3(int a, int b, int c);
	int max3(int a, int b, int c);
	int f01(int x, int y, int x0, int y0, int x1, int y1);
	int f12(int x, int y, int x1, int y1, int x2, int y2);
	int f20(int x, int y, int x0, int y0, int x2, int y2);
};

#endif