#ifndef VECTOR3_H_
#define VECTOR3_H_
#include <stdlib.h>
#include <math.h>
#include <limits>
//very simple data structure to store 3d points
class Vector3
{
   public:

   float x;
   float y;
   float z;
   Vector3(float in_x, float in_y, float in_z) : x(in_x), y(in_y), z(in_z) {}
   Vector3() {}

   Vector3 * add(Vector3 *other);
   Vector3 * cross(Vector3 *other);
   bool equals(Vector3 *other);
   Vector3 * inverse();
   Vector3 * subtract(Vector3 *other);
   Vector3 * normalize();
   float magnitude();
   Vector3 * multiply(float k);

};

#endif
