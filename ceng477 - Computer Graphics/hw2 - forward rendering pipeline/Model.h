#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "Triangle.h"
#include "Vec3.h"
#include <iostream>

using namespace std;

class Model
{

public:
    int modelId;
    int type;
    int numberOfTransformations;
    vector<int> transformationIds;
    vector<char> transformationTypes;
    int numberOfTriangles;
    vector<Triangle> triangles;
    vector<Triangle> facedTriangles;
    vector<int> vIds;
    vector< Vec3* > modelTransformedVertices;
    vector< Vec3* > modelVertices;
    int numberOfFacedTriangles;

    Model();
    Model(int modelId, int type, int numberOfTransformations,
          vector<int> transformationIds,
          vector<char> transformationTypes,
          int numberOfTriangles,
          vector<Triangle> triangles);

    friend ostream &operator<<(ostream &os, const Model &m);
};

#endif