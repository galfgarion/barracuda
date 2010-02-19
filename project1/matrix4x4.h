#ifndef _MATRIX4X4
#define _MATRIX4X4
class Matrix4x4 {
   public:
      double m [4][4];
      Matrix4x4();
      Matrix4x4(double[4][4]);
      const char * c_str();
};


// scalar multiplication
extern Matrix4x4 operator*=(Matrix4x4& A, double r);
extern Matrix4x4 operator*(const Matrix4x4& A, double r);
extern Matrix4x4 operator*(const double b, const Matrix4x4& a);
extern Matrix4x4& operator+=(Matrix4x4& A, const Matrix4x4& B);
extern Matrix4x4 operator+(const Matrix4x4& A, const Matrix4x4& B);

#endif