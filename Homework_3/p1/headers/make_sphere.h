#pragma once
#include <iostream>
#include <cmath>

using namespace std;



/**
This computes the number of array elements necessary to store the coordinates for the sphere
@param rows - the number of rows for the sphere
@param segments - the number of segments for the sphere
@return the number of array elements. 
**/
int NumVec3ArrayElements(const int rows, const int segments);



/**
The function creates the vertices for a sphere. Note, the sphere must be rendered as a GL_TRIANGLE_STRIP to obtain 
a complete surface model. The outcome are two arrays with points. 

The number of elements you need for your array is
	 
	N = (segments * 2 + 1) * 6 * rows
	e.g. with 10 rows and 10 segments
	N = (10*2 + 1) * 6* 10 = 1260 elememts -> float vertices[1260]l float normals[1260];
	which results in 1260/3 = 420 vertices

@oaram rows - the number of vertex rows.
@param segments - the number of segments per row. 
@param center - a pointer to a array with 3 elements of the origin [x, y, z]
@param r - the radius of the sphere
@param spherePoints - pointer to a vector that can contain the vertices. The vector must already been initialized
@param normals - pointer to a vector that can contain the normals. The vector must already been initialized
**/
int Make_Sphere(const int rows, const int segments, const float *center, const float r, vector<float>& spherePoints, vector<float>& normals);
