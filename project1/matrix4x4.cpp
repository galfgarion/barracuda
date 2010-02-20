#include "matrix4x4.h"
#include <string>
#include <sstream>
#include <assert.h>


#define ZERO 0
#define ONE 1
#define TWO 2
#define THREE 3

using namespace std;

Matrix4x4::Matrix4x4() {
   for(int i=0; i < 4; i++) {
      for(int j=0; j < 4; j++) {
         m[i][j] = 0;
      }
   }
}

Matrix4x4::Matrix4x4(double A[4][4]) {
   for(int i=0; i < 4; i++) {
      for(int j=0; j < 4; j++) {
         m[i][j] = A[i][j];
      }
   }
}

const char * Matrix4x4::c_str() {
   ostringstream o;
   o << "{";
      for(int i=0; i < 4; i++) {
         o << "{";
         for(int j=0; j < 4; j++) {
            o << m[i][j];
            if(j < 3) {
               o << " ";
            }
         }
         o << "}";
      }
   o << "}";
   return o.str().c_str();
}

double Matrix4x4::determinant() {
   return m[ZERO][ZERO]*m[ONE][ONE]*m[TWO][TWO]*m[THREE][THREE] + m[ZERO][ZERO]*m[ONE][TWO]*m[TWO][THREE]*m[THREE][ONE]
        + m[ZERO][ZERO]*m[ONE][THREE]*m[TWO][ONE]*m[THREE][TWO] + m[ZERO][ONE]*m[ONE][ZERO]*m[TWO][THREE]*m[THREE][TWO]
        + m[ZERO][ONE]*m[ONE][TWO]*m[TWO][ZERO]*m[THREE][THREE] + m[ZERO][ONE]*m[ONE][THREE]*m[TWO][TWO]*m[THREE][ZERO]
        + m[ZERO][TWO]*m[ONE][ZERO]*m[TWO][ONE]*m[THREE][THREE] + m[ZERO][TWO]*m[ONE][ONE]*m[TWO][THREE]*m[THREE][ZERO]
        + m[ZERO][TWO]*m[ONE][THREE]*m[TWO][ZERO]*m[THREE][ONE] + m[ZERO][THREE]*m[ONE][ZERO]*m[TWO][TWO]*m[THREE][ONE]
        + m[ZERO][THREE]*m[ONE][ONE]*m[TWO][ZERO]*m[THREE][TWO] + m[ZERO][THREE]*m[ONE][TWO]*m[TWO][ONE]*m[THREE][ZERO]
        - m[ZERO][ZERO]*m[ONE][ONE]*m[TWO][THREE]*m[THREE][TWO] - m[ZERO][ZERO]*m[ONE][TWO]*m[TWO][ONE]*m[THREE][THREE]
        - m[ZERO][ZERO]*m[ONE][THREE]*m[TWO][TWO]*m[THREE][ONE] - m[ZERO][ONE]*m[ONE][ZERO]*m[TWO][TWO]*m[THREE][THREE]
        - m[ZERO][ONE]*m[ONE][TWO]*m[TWO][THREE]*m[THREE][ZERO] - m[ZERO][ONE]*m[ONE][THREE]*m[TWO][ZERO]*m[THREE][TWO]
        - m[ZERO][TWO]*m[ONE][ZERO]*m[TWO][THREE]*m[THREE][ONE] - m[ZERO][TWO]*m[ONE][ONE]*m[TWO][ZERO]*m[THREE][THREE]
        - m[ZERO][TWO]*m[ONE][THREE]*m[TWO][ONE]*m[THREE][ZERO] - m[ZERO][THREE]*m[ONE][ZERO]*m[TWO][ONE]*m[THREE][TWO]
        - m[ZERO][THREE]*m[ONE][ONE]*m[TWO][TWO]*m[THREE][ZERO] - m[ZERO][THREE]*m[ONE][TWO]*m[TWO][ZERO]*m[THREE][ONE]
        ;
}

Matrix4x4 Matrix4x4::inverse() {
   double b [4][4];
   b[ZERO][ZERO] = m[ONE][ONE]*m[TWO][TWO]*m[THREE][THREE] + m[ONE][TWO]*m[TWO][THREE]*m[THREE][ONE] + m[ONE][THREE]*m[TWO][ONE]*m[THREE][TWO] - m[ONE][ONE]*m[TWO][THREE]*m[THREE][TWO] - m[ONE][TWO]*m[TWO][ONE]*m[THREE][THREE] - m[ONE][THREE]*m[TWO][TWO]*m[THREE][ONE];

   b[ZERO][ONE] = m[ZERO][ONE]*m[TWO][THREE]*m[THREE][TWO] + m[ZERO][TWO]*m[TWO][ONE]*m[THREE][THREE] + m[ZERO][THREE]*m[TWO][TWO]*m[THREE][ONE] - m[ZERO][ONE]*m[TWO][TWO]*m[THREE][THREE] - m[ZERO][TWO]*m[TWO][THREE]*m[THREE][ONE] - m[ZERO][THREE]*m[TWO][ONE]*m[THREE][TWO];
 
   b[ZERO][TWO] = m[ZERO][ONE]*m[ONE][TWO]*m[THREE][THREE] + m[ZERO][TWO]*m[ONE][THREE]*m[THREE][ONE] + m[ZERO][THREE]*m[ONE][ONE]*m[THREE][TWO] - m[ZERO][ONE]*m[ONE][THREE]*m[THREE][TWO] - m[ZERO][TWO]*m[ONE][ONE]*m[THREE][THREE] - m[ZERO][THREE]*m[ONE][TWO]*m[THREE][ONE];

   b[ZERO][THREE] = m[ZERO][ONE]*m[ONE][THREE]*m[TWO][TWO] + m[ZERO][TWO]*m[ONE][ONE]*m[TWO][THREE] + m[ZERO][THREE]*m[ONE][TWO]*m[TWO][ONE] - m[ZERO][ONE]*m[ONE][TWO]*m[TWO][THREE] - m[ZERO][TWO]*m[ONE][THREE]*m[TWO][ONE] - m[ZERO][THREE]*m[ONE][ONE]*m[TWO][TWO];

   b[ONE][ZERO] = m[ONE][ZERO]*m[TWO][THREE]*m[THREE][TWO] + m[ONE][TWO]*m[TWO][ZERO]*m[THREE][THREE] + m[ONE][THREE]*m[TWO][TWO]*m[THREE][ZERO] - m[ONE][ZERO]*m[TWO][TWO]*m[THREE][THREE] - m[ONE][TWO]*m[TWO][THREE]*m[THREE][ZERO] - m[ONE][THREE]*m[TWO][ZERO]*m[THREE][TWO];

   b[ONE][ONE] = m[ZERO][ZERO]*m[TWO][TWO]*m[THREE][THREE] + m[ZERO][TWO]*m[TWO][THREE]*m[THREE][ZERO] + m[ZERO][THREE]*m[TWO][ZERO]*m[THREE][TWO] - m[ZERO][ZERO]*m[TWO][THREE]*m[THREE][TWO] - m[ZERO][TWO]*m[TWO][ZERO]*m[THREE][THREE] - m[ZERO][THREE]*m[TWO][TWO]*m[THREE][ZERO];

   b[ONE][TWO] = m[ZERO][ZERO]*m[ONE][THREE]*m[THREE][TWO] + m[ZERO][TWO]*m[ONE][ZERO]*m[THREE][THREE] + m[ZERO][THREE]*m[ONE][TWO]*m[THREE][ZERO] - m[ZERO][ZERO]*m[ONE][TWO]*m[THREE][THREE] - m[ZERO][TWO]*m[ONE][THREE]*m[THREE][ZERO] - m[ZERO][THREE]*m[ONE][ZERO]*m[THREE][TWO];

   b[ONE][THREE] = m[ZERO][ZERO]*m[ONE][TWO]*m[TWO][THREE] + m[ZERO][TWO]*m[ONE][THREE]*m[TWO][ZERO] + m[ZERO][THREE]*m[ONE][ZERO]*m[TWO][TWO] - m[ZERO][ZERO]*m[ONE][THREE]*m[TWO][TWO] - m[ZERO][TWO]*m[ONE][ZERO]*m[TWO][THREE] - m[ZERO][THREE]*m[ONE][TWO]*m[TWO][ZERO];

   b[TWO][ZERO] = m[ONE][ZERO]*m[TWO][ONE]*m[THREE][THREE] + m[ONE][ONE]*m[TWO][THREE]*m[THREE][ZERO] + m[ONE][THREE]*m[TWO][ZERO]*m[THREE][ONE] - m[ONE][ZERO]*m[TWO][THREE]*m[THREE][ONE] - m[ONE][ONE]*m[TWO][ZERO]*m[THREE][THREE] - m[ONE][THREE]*m[TWO][ONE]*m[THREE][ZERO];

   b[TWO][ONE] = m[ZERO][ZERO]*m[TWO][THREE]*m[THREE][ONE] + m[ZERO][ONE]*m[TWO][ZERO]*m[THREE][THREE] + m[ZERO][THREE]*m[TWO][ONE]*m[THREE][ZERO] - m[ZERO][ZERO]*m[TWO][ONE]*m[THREE][THREE] - m[ZERO][ONE]*m[TWO][THREE]*m[THREE][ZERO] - m[ZERO][THREE]*m[TWO][ZERO]*m[THREE][ONE];

   b[TWO][TWO] = m[ZERO][ZERO]*m[ONE][ONE]*m[THREE][THREE] + m[ZERO][ONE]*m[ONE][THREE]*m[THREE][ZERO] + m[ZERO][THREE]*m[ONE][ZERO]*m[THREE][ONE] - m[ZERO][ZERO]*m[ONE][THREE]*m[THREE][ONE] - m[ZERO][ONE]*m[ONE][ZERO]*m[THREE][THREE] - m[ZERO][THREE]*m[ONE][ONE]*m[THREE][ZERO];

   b[TWO][THREE] = m[ZERO][ZERO]*m[ONE][THREE]*m[TWO][ONE] + m[ZERO][ONE]*m[ONE][ZERO]*m[TWO][THREE] + m[ZERO][THREE]*m[ONE][ONE]*m[TWO][ZERO] - m[ZERO][ZERO]*m[ONE][ONE]*m[TWO][THREE] - m[ZERO][ONE]*m[ONE][THREE]*m[TWO][ZERO] - m[ZERO][THREE]*m[ONE][ZERO]*m[TWO][ONE];

   b[THREE][ZERO] = m[ONE][ZERO]*m[TWO][TWO]*m[THREE][ONE] + m[ONE][ONE]*m[TWO][ZERO]*m[THREE][TWO] + m[ONE][TWO]*m[TWO][ONE]*m[THREE][ZERO] - m[ONE][ZERO]*m[TWO][ONE]*m[THREE][TWO] - m[ONE][ONE]*m[TWO][TWO]*m[THREE][ZERO] - m[ONE][TWO]*m[TWO][ZERO]*m[THREE][ONE];

   b[THREE][ONE] = m[ZERO][ZERO]*m[TWO][ONE]*m[THREE][TWO] + m[ZERO][ONE]*m[TWO][TWO]*m[THREE][ZERO] + m[ZERO][TWO]*m[TWO][ZERO]*m[THREE][ONE] - m[ZERO][ZERO]*m[TWO][TWO]*m[THREE][ONE] - m[ZERO][ONE]*m[TWO][ZERO]*m[THREE][TWO] - m[ZERO][TWO]*m[TWO][ONE]*m[THREE][ZERO];

   b[THREE][TWO] = m[ZERO][ZERO]*m[ONE][TWO]*m[THREE][ONE] + m[ZERO][ONE]*m[ONE][ZERO]*m[THREE][TWO] + m[ZERO][TWO]*m[ONE][ONE]*m[THREE][ZERO] - m[ZERO][ZERO]*m[ONE][ONE]*m[THREE][TWO] - m[ZERO][ONE]*m[ONE][TWO]*m[THREE][ZERO] - m[ZERO][TWO]*m[ONE][ZERO]*m[THREE][ONE];

   b[THREE][THREE] = m[ZERO][ZERO]*m[ONE][ONE]*m[TWO][TWO] + m[ZERO][ONE]*m[ONE][TWO]*m[TWO][ZERO] + m[ZERO][TWO]*m[ONE][ZERO]*m[TWO][ONE] - m[ZERO][ZERO]*m[ONE][TWO]*m[TWO][ONE] - m[ZERO][ONE]*m[ONE][ZERO]*m[TWO][TWO] - m[ZERO][TWO]*m[ONE][ONE]*m[TWO][ZERO];

   Matrix4x4 B = Matrix4x4(b);
   double det = this->determinant();
   assert(det != 0);
   B *= (1 / det);
   return B;
}

// -------------------------------------------
// operator overloads

Matrix4x4& operator*=(Matrix4x4& A, double r) {
   for(int i=0; i < 4; i++) {
      for(int j=0; j < 4; j++) {
         A.m[i][j] *= r;
      }
   }
}

Matrix4x4& operator+=(Matrix4x4& A, const Matrix4x4& B) {
   for(int i=0; i < 4; i++) {
      for(int j=0; j < 4; j++) {
         A.m[i][j] += B.m[i][j];
      }
   }
   return A;
}

Matrix4x4 operator+(const Matrix4x4& A, const Matrix4x4& B) {
   Matrix4x4 result = Matrix4x4(A);
   result += B;
   return result;
}

Matrix4x4 operator*(const Matrix4x4& A, double r) {
   Matrix4x4 result = Matrix4x4(A);
   result *= r;
   return result;
}

Matrix4x4 operator*(const double r, const Matrix4x4& A) {
   return A * r;
}

// dot the ith row of A with the jth column of B
Matrix4x4 operator * (const Matrix4x4& A, const Matrix4x4& B) {
   Matrix4x4 result = Matrix4x4();
   for(int i=0; i < 4; i++) {
      for(int j=0; j < 4; j++) {
         result.m[i][j] = 
              A.m[i][0] * B.m[0][j]
            + A.m[i][1] * B.m[1][j]
            + A.m[i][2] * B.m[2][j]
            + A.m[i][3] * B.m[3][j];
      }
   }

   return result;
}
