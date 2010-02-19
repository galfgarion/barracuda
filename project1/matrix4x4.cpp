#include "matrix4x4.h"
#include <string>
#include <sstream>

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

Matrix4x4 operator*=(Matrix4x4& A, double r) {
   for(int i=0; i < 4; i++) {
      for(int j=0; j < 4; j++) {
         A.m[i][j] *= r;
      }
   }
}

Matrix4x4 operator*(const Matrix4x4& A, double r) {
   Matrix4x4 result = Matrix4x4(A);
   result *= r;
   return result;
}

Matrix4x4 operator*(const double r, const Matrix4x4& A) {
   return A * r;
}
