#ifndef VECTOR3_H_
#define VECTOR3_H_
#include <stdlib.h>
#include <math.h>
#include <limits>
#include <string>

using namespace std;
//very simple data structure to store 3d points
class Vector3
{
   public:

   double x;
   double y;
   double z;
   Vector3(double in_x, double in_y, double in_z) : x(in_x), y(in_y), z(in_z) {}
   Vector3() {}

   Vector3 * add(Vector3 *other);
   Vector3 cross(const Vector3 & other);
   bool equals(Vector3 *other);
   Vector3 * inverse();
   Vector3 * subtract(Vector3 * const & other);
   Vector3 normalize();
   double magnitude();
   Vector3 * multiply(double k);
   double dot(Vector3 *const & other);
   const char * c_str();

};

extern Vector3 operator+(const Vector3& a, const Vector3& b);
extern Vector3 operator-(const Vector3& a, const Vector3& b);
extern double operator*(const Vector3& a, const Vector3& b);
extern Vector3 operator*(const Vector3& a, double b);
extern Vector3 operator*(const double b, const Vector3& a);
/*
   ostream& operator<<(ostream& out, const Vector3& v) {
      out << "<" << v.x << ", " << v.y << ", " << v.z << ">";
      return out;
   }
   */

#endif
