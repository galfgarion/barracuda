#include "vector3.h"

typedef struct s_ray {
   Vector3 direction;
   Vector3 origin; 
} Ray;

class GeomObject {
   public:
      virtual double intersect(Ray&) = 0; // pure virtual fn
};

class Sphere: public GeomObject {
   public:
      Sphere(Vector3& center, double radius);
      double intersect(Ray&);

   private:
      Vector3 _center;
      double _radius;

};

Sphere::Sphere(Vector3 &center, double radius) {
   _center = Vector3(center);
   _radius = radius;
}

// returns closest non-negative intersection, or -1 if a non-negative intersection does not exist
double Sphere::intersect(Ray& ray) {
   
   Vector3 * centerToOrigin = ray.origin.subtract(&_center);
   double A = ray.direction.dot(&ray.direction);
   double B = 2 * ray.direction.dot(centerToOrigin);
   double C = centerToOrigin->dot(centerToOrigin) - _radius*_radius;

   double discriminant = B * B - 4 * A * C;

   if(discriminant < 0) {
      return -1;
   } else {
      double denom = 2 * A;
      double t1 = (-B + sqrt(discriminant)) / denom;
      double t2 = (-B - sqrt(discriminant)) / denom;

      if(t1 < 0 && t2 < 0) {
         return -1;
      }
      else if(t1 < 0) {
         return t2;
      } else if(t2 < 0) {
         return t1;
      } else {
         if(t1 < t2)
            return t1;
         else 
            return t2;
      }
   }
}
