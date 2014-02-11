
/************************************************************************
 * aMath.cpp
 ************************************************************************/

#include <math.h>
#include "aMath.h"

// make a rotation matrix
// Param:  Matrix4x4 ret - the matrix being created
//         char axis - rotation axis
//         double angle - angle, in degrees
void MakeRotationMatrix(Matrix4x4 ret, char axis, double angle)
{
	InitMatrix(ret);

	// convert angles to radians
	angle = (angle/180.0f)*3.14159f;
	
	switch(axis) {
		case 'x':
			SET_ENTRY(ret, 1, 1, cos(angle) );
			SET_ENTRY(ret, 1, 2, -sin(angle) );
			SET_ENTRY(ret, 2, 1, sin(angle) );
			SET_ENTRY(ret, 2, 2, cos(angle) );
			break;
		case 'y':
			SET_ENTRY(ret, 0, 0, cos(angle) );
			SET_ENTRY(ret, 0, 2, sin(angle) );
			SET_ENTRY(ret, 2, 0, -sin(angle) );
			SET_ENTRY(ret, 2, 2, cos(angle) );
			break;
		case 'z':
			SET_ENTRY(ret, 0, 0, cos(angle) );
			SET_ENTRY(ret, 0, 1, -sin(angle) );
			SET_ENTRY(ret, 1, 0, sin(angle) );
			SET_ENTRY(ret, 1, 1, cos(angle) );
			break;
	}
}

// make a translation matrix
// Param:  Matrix4x4 ret - matrix to set
//         double x, y, z - translation in each direction
void MakeTranslationMatrix(Matrix4x4 ret, double x, double y, double z)
{
	InitMatrix(ret);

	SET_ENTRY(ret, 0, 3, x);
	SET_ENTRY(ret, 1, 3, y);
	SET_ENTRY(ret, 2, 3, z);

}

// make a scaling matrix
// Param: Matrix4x4 ret - matrix to set
//        double sx, sy, sz - scaling values
void MakeScalingMatrix(Matrix4x4 ret, double sx, double sy, double sz)
{
	InitMatrix(ret);

	SET_ENTRY(ret, 0, 0, sx);
	SET_ENTRY(ret, 1, 1, sy);
	SET_ENTRY(ret, 2, 2, sz);
}

// initialize a matrix to the identity matrix.
void InitMatrix(Matrix4x4 h)
{
  int i,j;
  
  for( i= 0 ; i< 4 ; i++ )
    for( j= 0 ; j< 4 ; j++ )
		SET_ENTRY(h, i, j, i == j);
}

//
// Multiplying mat1*mat2
// ret can be one of mat1 or mat2
//
void MulMatrix(Matrix4x4 ret, Matrix4x4 mat1, Matrix4x4 mat2)
{
  int i,j;
  Matrix4x4 r;

  for( i= 0 ; i< 4 ; i++ )
    for( j= 0 ; j< 4 ; j++ )
		SET_ENTRY(r, i, j, GET_ENTRY(mat1, i, 0)*GET_ENTRY(mat2, 0, j)+
				 GET_ENTRY(mat1, i, 1)*GET_ENTRY(mat2, 1, j)+
				 GET_ENTRY(mat1, i, 2)*GET_ENTRY(mat2, 2, j)+
				 GET_ENTRY(mat1, i, 3)*GET_ENTRY(mat2, 3, j) );

  for( i= 0 ; i< 4 ; i++ )
    for( j= 0 ; j< 4 ; j++ )
		SET_ENTRY(ret, i, j, GET_ENTRY(r, i, j) );
}

// transform a vector
// Param: Vector3D ret - result of the transformation
//        Matrix4x4 trans - transformation matrix
//        Vector3D vec - vector to transfrom
// ret can be the same as vec
void TransformVector(Vector3D ret, Matrix4x4 trans, Vector3D vec)
{
	Vector3D r;

	r[0] = vec[0]*GET_ENTRY(trans, 0, 0)+vec[1]*GET_ENTRY(trans, 0, 1)
		+vec[2]*GET_ENTRY(trans, 0, 2);
	r[1] = GET_ENTRY(trans, 1, 0)*vec[0]+GET_ENTRY(trans, 1, 1)*vec[1]
		+GET_ENTRY(trans, 1, 2)*vec[2];
	r[2] = GET_ENTRY(trans, 2, 0)*vec[0]+GET_ENTRY(trans, 2, 1)*vec[1]
		+GET_ENTRY(trans, 2, 2)*vec[2];


	ret[0] = r[0];
	ret[1] = r[1];
	ret[2] = r[2];
}

// transform a point
// Param:  Vector3D ret - result of transformation
//         Matrix4x4 trans - transformation matrix
//         Vector3D p - point to transform
void TransformPoint(Vector3D ret, Matrix4x4 trans, Vector3D p)
{
	Vector3D r;

	r[0] = GET_ENTRY(trans, 0, 0)*p[0]+GET_ENTRY(trans, 0, 1)*p[1]
		+GET_ENTRY(trans, 0, 2)*p[2]+GET_ENTRY(trans, 0, 3);
	r[1] = GET_ENTRY(trans, 1, 0)*p[0]+GET_ENTRY(trans, 1, 1)*p[1]
		+GET_ENTRY(trans, 1, 2)*p[2]+GET_ENTRY(trans, 1, 3);
	r[2] = GET_ENTRY(trans, 2, 0)*p[0]+GET_ENTRY(trans, 2, 1)*p[1]
		+GET_ENTRY(trans, 2, 2)*p[2]+GET_ENTRY(trans, 2, 3);

	ret[0] = r[0];
	ret[1] = r[1];
	ret[2] = r[2];
}

// calculate the cross product of v1 and v2, placing the result in ret
void CrossProduct(Vector3D ret, Vector3D v1, Vector3D v2)
{
	Vector3D r;

	r[0] = v1[1]*v2[2]-v1[2]*v2[1];
	r[1] = v1[2]*v2[0]-v1[0]*v2[2];
	r[2] = v1[0]*v2[1]-v1[1]*v2[0];

	ret[0] = r[0];
	ret[1] = r[1];
	ret[2] = r[2];
	ret[3] = 0.0f;
}

// calculate and return the dot product of v1 and v2
double DotProduct(Vector3D v1, Vector3D v2)
{
	return v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2];
}

// normalize vector vec
void VecNormalize(Vector3D vec)
{
	double len = sqrt(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]);

	vec[0] /= len;
	vec[1] /= len;
	vec[2] /= len;
}

// return the length of vec
double VecLen(Vector3D vec)
{
	return sqrt(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]);
}

// copy a matrix from src to dest
void CopyMatrix(Matrix4x4 dest, Matrix4x4 src)
{
	for(int n = 0; n < 16; n++)
		dest[n] = src[n];
}

// debug for matrix
void printMatrix(Matrix4x4 mat)
{
	printf("%.3f  %.3f  %.3f  %.3f\n", mat[0], mat[1], mat[2], mat[3]);
	printf("%.3f  %.3f  %.3f  %.3f\n", mat[4], mat[5], mat[6], mat[7]);
	printf("%.3f  %.3f  %.3f  %.3f\n", mat[8], mat[9], mat[10], mat[11]);
	printf("%.3f  %.3f  %.3f  %.3f\n", mat[12], mat[13], mat[14], mat[15]);
}