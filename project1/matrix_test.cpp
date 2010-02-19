#include "matrix4x4.h"
#include <iostream>
#include <assert.h>

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
   Matrix4x4 A = Matrix4x4(arr1);
   Matrix4x4 B = Matrix4x4(arr1);
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

}