#include <deque>
#include <iostream>
#include <assert.h>
#include "vector3.h"

using namespace std;

typedef struct s_ray {
   Vector3 direction;
   Vector3 origin; 
} Ray;

typedef unsigned char byte;

typedef struct color_t {
   byte r, g, b;
} Color;

class GeomObject {
   public:
      virtual double intersect(Ray&) = 0; // pure virtual fn
      Color color;
};

class Sphere: public GeomObject {
   public:
      Sphere(Vector3& center, double radius);
      double intersect(Ray& ray);

   private:
      Vector3 _center;
      double _radius;

};

class Plane: public GeomObject {
   public:
      Plane(Vector3& normal, double d);
      double intersect(Ray& ray);

   private:
      Vector3 _normal;
      double _d;
};

Plane::Plane(Vector3& normal, double d) {
   _normal = normal;
   _d = d;
}

double Plane::intersect(Ray& ray) {
   Vector3 p1 = *_normal.multiply(-_d);
   double denom = ray.direction.dot(&_normal);

   if(denom == 0) {
      return -1.0;
   }

   double numerator = p1.subtract(&ray.origin)->dot(&_normal);
   return numerator / denom;
}

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

class Camera {
   public:
      Vector3 eye, up, right, lookAt;
      static Camera parse(deque<string> tokens) {
         Camera camera;

         unsigned int tokensLeft = 17; // expected num of tokens in camera

         assert(tokens.size() >= tokensLeft);
         string token = tokens.front();
         assert(token.compare("camera") == 0);
         tokens.pop_front();
         tokensLeft--;

         for(; tokensLeft > 0; tokens.pop_back(), tokensLeft--) {
            token = tokens.front();
            //TODO finish this
         }
         
         return camera;
      }
};
