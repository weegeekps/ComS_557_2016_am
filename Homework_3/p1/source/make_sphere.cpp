#include <iostream>
#include <cmath>
#include <vector>

#include "../headers/make_sphere.h"

using namespace std;

/**
This computes the number of array elements necessary to store the coordinates for the sphere
@param rows - the number of rows for the sphere
@param segments - the number of segments for the sphere
@return the number of array elements. 
**/
int NumVec3ArrayElements(const int rows, const int segments)
{
	return  ((segments * 2) + 1) * 6* rows;
}

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
int Make_Sphere(const int rows, const int segments, const float *center, const float r, vector<float>& spherePoints, vector<float>& normals)
{
    /* I had to change this to use vectors because the NumVec3ArrayElements seems to be unreliable!!! */

    const float PI = 3.141592653589793238462643383279502884197;
    
    int current_size = 0;
    for (float theta = 0.; theta < PI; theta += PI/float(rows)) // Elevation [0, PI]
    {
        //double theta = 1.57;
        float theta2 = theta + PI/float(rows);
        int count = 0;
        int count_row = 0;
        
        // Iterate through phi, theta then convert r,theta,phi to  XYZ
        for (float phi = 0.; phi < 2*PI + PI/float(segments) ; phi += PI/float(segments)) // Azimuth [0, 2PI]
        {
            int index = current_size + count;

            spherePoints.push_back(r * cos(phi) * sin(theta) + center[0]);
            spherePoints.push_back(r * sin(phi) * sin(theta) + center[1]);
            spherePoints.push_back(r            * cos(theta) + center[2]);
			count+=3;

            spherePoints.push_back(r * cos(phi) * sin(theta2) + center[0]);
            spherePoints.push_back(r * sin(phi) * sin(theta2) + center[1]);
            spherePoints.push_back(r            * cos(theta2) + center[2]);
            count+=3;
            
            normals.push_back(cos(phi) * sin(theta));
            normals.push_back(sin(phi) * sin(theta));
            normals.push_back(cos(theta));

            normals.push_back(cos(phi) * sin(theta2));
            normals.push_back(sin(phi) * sin(theta2));
            normals.push_back(cos(theta2));

        }
        if(count_row == 0) count_row = count;

		current_size += count;
    }
    return current_size/3;
}