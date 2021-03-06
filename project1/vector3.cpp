#include "vector3.h"
#include <string>
#include <sstream>
#include <cstring>
#include <math.h>

   Vector3 * Vector3::add(Vector3 *other)
   {
      Vector3 *sum = new Vector3(*this);
      sum->x += other->x;
      sum->y += other->y;
      sum->z += other->z;

      return sum;
   }

   Vector3 Vector3::cross(const Vector3 & other)
   {
      double x1 = (y * other.z - z * other.y);
      double y1 = (z * other.x - x * other.z);
      double z1 = (x * other.y - y * other.x);

      return Vector3(x1, y1, z1);
   }

   bool Vector3::equals(Vector3 *other)
   {
      return(x == other->x && y == other->y && z == other->z);
   }

   Vector3 * Vector3::inverse()
   {
      return new Vector3(-x, -y, -z);
   }
  
   Vector3 * Vector3::subtract(Vector3 * const & other)
   {
      Vector3 *diff = new Vector3(*this);
      diff->x -= other->x;
      diff->y -= other->y;
      diff->z -= other->z;

      return diff;
   }
   
   Vector3 Vector3::normalize() const
   {
      double magn = this->magnitude();

      if(magn == 0.0)
         return Vector3(*this);

      double inv_mag = 1.0 / magn;

      return *this * inv_mag;
   }

   double Vector3::magnitude() const
   {
      return sqrt(*this * *this);
   }

   /*
   Vector3 * Vector3::multiply(double k)
   {
      Vector3 * product = new Vector3(*this);
      product->x *= k;
      product->y *= k;
      product->z *= k;

      return product;
   }
   */

   double Vector3::dot(Vector3 *const &other) {
      return other->x * x + other->y * y + other->z * z;
   }

   const char * Vector3::c_str() {
      std::ostringstream o;
      o << "<" << x << ", " << y << ", " << z << ">";
      std::string str = o.str();
      char * cstr = new char[str.size() + 1];
      strcpy(cstr, str.c_str());
      return cstr;
   }

   Vector3 Vector3::operator-() const {
      return Vector3(-x, -y, -z);
   }

   // For the following overloaded operator functions, see Stroustrup p. 231

   Vector3 operator+(const Vector3& a, const Vector3& b) {
      return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
   }
   
   Vector3 operator-(const Vector3& a, const Vector3& b) {
      return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
   }

   double operator*(const Vector3& a, const Vector3& b) {
      return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
   }

   Vector3 operator*(const Vector3& a, const double b) {
      return(Vector3(a.x * b, a.y * b, a.z * b));
   }

   Vector3 operator*(const double b, const Vector3& a) {
      return a * b;
   }

   bool operator==(const Vector3& a, const Vector3&b) {
      return(a.x == b.x && a.y == b.y && a.z == b.z);
   }
   
