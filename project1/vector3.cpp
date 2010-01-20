#include "vector3.h"

   Vector3 * Vector3::add(Vector3 *other)
   {
      Vector3 *sum = new Vector3(*this);
      sum->x += other->x;
      sum->y += other->y;
      sum->z += other->z;

      return sum;
   }

   Vector3 * Vector3::cross(Vector3 *other)
   {
      Vector3 a;
      a.x = (y * other->z - z * other->y);
      a.y = (z * other->x - x * other->z);
      a.z = (x * other->y - y * other->x);

      return new Vector3(a);
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
   
   Vector3 * Vector3::normalize()
   {
      double magn = this->magnitude();

      if(magn == 0.0)
         return new Vector3(*this);

      double inv_mag = 1.0f / magn;

      return this->multiply(inv_mag);
   }

   double Vector3::magnitude()
   {
      return sqrt(abs(x*x + y*y + z*z));
   }

   Vector3 * Vector3::multiply(double k)
   {
      Vector3 * product = new Vector3(*this);
      product->x *= k;
      product->y *= k;
      product->z *= k;

      return product;
   }

   double Vector3::dot(Vector3 *const &other) {
      return other->x * x + other->y * y + other->z * z;
   }

void multVector3(const Vector3 * a, double k, Vector3 * result)
{
   result->x = a->x * k;
   result->y = a->y * k;
   result->z = a->z * k;
}
