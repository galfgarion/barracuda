#include "matrix4x4.h"
#include <iostream>
#include <assert.h>
#include <math.h>

using namespace std;

int main(void) {
   double arr1[4][4] =  { {1, 2, 3, 4},
                         {5, 6, 7, 8},
                         {9, 10, 11, 12},
                         {13, 14, 15, 16}
                       };
   double arr2[4][4] = { {1, 2, 3, 4},
                         {5, 6, 7, 8},
                         {9, 10, 11, 12},
                         {13, 14, 15, 16}
                       };
   double arr3[4][4] = { {1, 0, 0, 0},
                         {0, 1, 0, 0},
                         {0, 0, 1, 0},
                         {0, 0, 0, 1}
                       };
   Matrix4x4 A = Matrix4x4(arr1);
   Matrix4x4 B = Matrix4x4(arr1);
   Matrix4x4 I = Matrix4x4(arr3);
   cout << "Matrix A: " << A.c_str() << endl;
   assert(A.m[3][3] == 16);
   A *= 2;
   cout << "Matrix A: " << A.c_str() << endl;
   assert(A.m[3][3] == 32);
   A = A * 0.5;
   cout << "Matrix A: " << A.c_str() << endl;
   assert(A.m[3][3] == 16);
   cout << "Matrix B: " << B.c_str() << endl;
   A += B;
   cout << "A += B: " << A.c_str() << endl;
   assert(A.m[3][3] == 32);
   B = B + A;
   cout << "B = B + A: " <<  B.c_str() << endl;
   assert(B.m[3][3] == 48);
   B = B * I;
   cout << "B * I: " << B.c_str() << endl;
   assert(B.m[3][3] == 48);
   A = Matrix4x4(arr1);
   cout << "Matrix A: " << A.c_str() << endl;
   A = A * A;
   cout << "A * A: " << A.c_str() << endl;
   assert(A.m[0][0] == 90);
   assert(A.m[3][3] == 600);

   Matrix4x4 C = I.inverse();
   cout << "C = I.inverse(): " << C.c_str() << endl;

   C = I * C;
   cout << "I * C: " << C.c_str() << endl; 

   double scaleVec [4][4] = { {2, 0, 0, 0},
                              {0, 0.5, 0, 0},
                              {0, 0, 3, 0},
                              {0, 0, 0, 1}
                            };
   Matrix4x4 scaleMatrix = Matrix4x4(scaleVec);
   cout << "Scale matrix: " << scaleMatrix.c_str() << endl;
   cout << "Inverse scale matrix: " << scaleMatrix.inverse().c_str() << endl;

   Matrix4x4 transMatrix = Matrix4x4( 1, 0, 0, 1,
                                      0, 1, 0, -3,
                                      0, 0, 1, 2,
                                      0, 0, 0, 1 );

   cout << "Translation matrix: " << transMatrix.c_str() << endl;
   cout << "Inverse trans matrix: " << transMatrix.inverse().c_str() << endl;

   // rotate 90 degrees around y axis
   double rotVec [4][4] = { 
                              {0, 0, 1, 0},
                              {0, 1, 0, 0},
                              {-1, 0, 0, 0},
                              {0, 0, 0, 1}
                          };

   Matrix4x4 rotMatrix = Matrix4x4(rotVec);
   cout << "Rotation matrix: " << rotMatrix.c_str() << endl;
   cout << "Inverse rotation matrix: " << rotMatrix.inverse().c_str() << endl;


   // composit matrix
   Matrix4x4 M = (transMatrix * rotMatrix) * scaleMatrix;
   cout << "Composite matrix: " << M.c_str() << endl;
   cout << "Inverse composite matrix: " << M.inverse().c_str() << endl;
   cout << "Composite times its inverse: " << (M * M.inverse()).c_str() << endl;

   Vector3 vector = Vector3(-1, -1, -1);
   cout << "Vector: " << vector.c_str() << endl;
   cout << "Rot matrix * vector: " << (rotMatrix * vector).c_str() << endl;
   cout << "Trans matrix * vector: " << (transMatrix * vector).c_str() << endl;
   cout << "Scale matrix * vector: " << (scaleMatrix * vector).c_str() << endl;

   cout << "Composite matrix * vector: " << (M * vector).c_str() << endl;
   cout << "Composite matrix inv * ANS: " <<  (M.inverse() * (M * vector)).c_str() << endl;

}
