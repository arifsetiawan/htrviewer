
/************************************************************************
 * Amy Project - part of vision based human motion capture project
 * 
 * Nurul Arif Setiawan
 * Intelligent Image Media Laboratory
 * Dept. of Computer and Engineering
 * Chonnam National University, Gwangju, South Korea
 *
 * aMath.h
 * Some math function for fixed size matrix and vector
 * 
 * History : - May 19 2005 - Using definition from Matthew Thorne from his work
 *							 on Inverse Kinematic System
 *							 http://www.cs.ubc.ca/~mthorne/cpsc533project.html
 ************************************************************************/

#ifndef _AMY_MATH_H_
#define _AMY_MATH_H_

#include <stdio.h>

/************************************************************************
 *  matrix and vector
 ************************************************************************/
typedef double Matrix4x4[16];
typedef double Vector3D[4];


/************************************************************************
 *  useful handy macros
 ************************************************************************/
#define SET_ENTRY(m, r, c, e) (m)[((c)<<2)+(r)]=(e)
#define GET_ENTRY(m, r, c) (m)[((c)<<2)+(r)]

#define CLAMP(v, min, max) (v)=((v)<(min)?(min):((v)>(max)?(max):(v)))

#define SAFE_DELETE(mem) if((mem)!=NULL){delete mem;mem=NULL;}
#define SAFE_DELETE_ARRAY(a) if((a)!=NULL){delete [] a;a=NULL;}

#define X 0
#define Y 1
#define Z 2

#define makeVector3D(v, x, y, z) (v)[0]=(x);(v)[1]=(y);(v)[2]=(z);(v)[3]=0.0f;
#define makePoint3D(p, x, y, z) (p)[0]=(x);(p)[1]=(y);(p)[2]=(z);(p)[3]=1.0f;

#define COPY_VECTOR(v, c) (v)[0]=(c)[0];(v)[1]=(c)[1];(v)[2]=(c)[2];(v)[3]=(c)[3];
#define SUB_VECTOR(r, v1, v2) (r)[0]=(v1)[0]-(v2)[0];(r)[1]=(v1)[1]-(v2)[1];(r)[2]=(v1)[2]-(v2)[2];(r)[3]=0.0f;
#define ADD_VECTOR(r, v1, v2) (r)[0]=(v1)[0]+(v2)[0];(r)[1]=(v1)[1]+(v2)[1];(r)[2]=(v1)[2]+(v2)[2];(r)[3]=0.0f;


/************************************************************************
 *  some handy function on matrix and vector
 ************************************************************************/
void InitMatrix(Matrix4x4 h);
void MulMatrix(Matrix4x4 ret, Matrix4x4 mat1, Matrix4x4 mat2);
void MakeRotationMatrix(Matrix4x4 ret, char axis, double angle);
void MakeTranslationMatrix(Matrix4x4 ret, double x, double y, double z);
void MakeScalingMatrix(Matrix4x4 ret, double sx, double sy, double sz);
void CopyMatrix(Matrix4x4 dest, Matrix4x4 src);
void TransformVector(Vector3D ret, Matrix4x4 trans, Vector3D vec);
void TransformPoint(Vector3D ret, Matrix4x4 trans, Vector3D p);
void CrossProduct(Vector3D ret, Vector3D v1, Vector3D v2);
double DotProduct(Vector3D v1, Vector3D v2);
void VecNormalize(Vector3D vec);
double VecLen(Vector3D vec);

void printMatrix(Matrix4x4 mat);

#endif
