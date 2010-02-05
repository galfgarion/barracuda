#include <deque>
#include <iostream>
#include <assert.h>
#include <sstream>
#include "vector3.h"
#include "parser.h"
#include "color.h"

#ifndef _GEOM_H
#define _GEOM_H

using namespace std;

typedef Vector3 Point;

typedef struct s_ray {
   Vector3 direction;
   Vector3 origin; 
} Ray;

typedef struct finish_t {
   double ambient, diffuse, specular, roughness;
} Finish;

Finish parse_finish (deque<string> & tokens) {
   Finish finish;
   assert(!tokens.front().compare("finish"));

   cout << "parsing finish" << endl;

   tokens.pop_front(); // finish

   while(!tokens.empty()) {
      if(!tokens.front().compare("ambient")) {
         tokens.pop_front();
         finish.ambient = Parser::parse_double(tokens);
         cout << "ambient = " << finish.ambient << endl;
      } else if(!tokens.front().compare("diffuse")) {
         tokens.pop_front();
         finish.diffuse = Parser::parse_double(tokens);
         cout << "diffuse = " << finish.diffuse << endl;
      } else if(!tokens.front().compare("specular")) {
         tokens.pop_front();
         finish.specular = Parser::parse_double(tokens);
         cout << "specular = " << finish.specular << endl;
      } else if(!tokens.front().compare("roughness")) {
         tokens.pop_front();
         finish.roughness = Parser::parse_double(tokens);
         cout << "roughness = " << finish.roughness << endl;
      } else break;
   }

   return finish;
}

class GeomObject {
   public:
      virtual double intersect(Ray&) = 0; // pure virtual fn
      virtual Vector3 surfaceNormal(const Point & p) = 0;
      Color color;
      Finish finish;
};

class Sphere: public GeomObject {
   public:
      Sphere(Vector3& center, double radius);
      Sphere(deque<string> & tokens);
      double intersect(Ray& ray);
      Vector3 surfaceNormal(const Point & p);

   private:
      Vector3 _center;
      double _radius;
};

class Plane: public GeomObject {
   public:
      Plane(Vector3& normal, double d);
      Plane(deque<string> & tokens);
      double intersect(Ray& ray);
      Vector3 surfaceNormal(const Point &p);

   private:
      Vector3 _normal;
      double _d;
};

Vector3 Plane::surfaceNormal(const Point &p) {
   return Vector3(this->_normal).normalize();
}

Plane::Plane(Vector3& normal, double d) {
   _normal = normal;
   _d = d;
}

Plane::Plane(deque<string> & tokens) {

   // there has to be a better way to do initialization
   finish.specular = 0.0;
   finish.diffuse = 0.0;
   finish.ambient = 0.0;

   assert(!tokens.front().compare("plane"));
   tokens.pop_front();

   _normal = Parser::parse_vector(tokens);
   _d = Parser::parse_double(tokens.front());
   tokens.pop_front();

   cout << "parsed plane with normal " << _normal.c_str() << " and d " << _d << endl;
   
   while(!tokens.empty()) {
      if(!tokens.front().compare("pigment")) {
         tokens.pop_front(); // pigment
         color = Parser::parse_color(tokens);
      } else if (!tokens.front().compare("finish")){
         this->finish = parse_finish(tokens);
      } else break;  
   }
}

double Plane::intersect(Ray& ray) {
   Vector3 p1 = *_normal.multiply(_d);
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

Sphere::Sphere(deque<string> & tokens) {
   finish.specular = finish.diffuse = finish.ambient = 0.0;

   assert(!tokens.front().compare("sphere"));
   tokens.pop_front();
   _center = Parser::parse_vector(tokens);
   _radius = Parser::parse_double(tokens.front());
   tokens.pop_front();

   while(!tokens.empty()) {
      if(!tokens.front().compare("pigment")) {
         tokens.pop_front(); // pigment
         this->color = Parser::parse_color(tokens);
      } else if(!tokens.front().compare("finish")) {
         this->finish = parse_finish(tokens);
      } else break;
   }
   cout << "parsed sphere with center " << _center.c_str() << " and radius " << _radius << endl;
}

Vector3 Sphere::surfaceNormal(const Point & p) {
   return (p - this->_center).normalize();
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
      Camera() {}
      Camera(Vector3& eye, Vector3& up, Vector3& right, Vector3& lookAt); 
      Vector3 eye, up, right, lookAt;

      static Camera parse(deque<string> & tokens) {
         unsigned int tokensLeft = 17; // expected num of tokens in camera

         Vector3 eye, up, right, lookAt;

         assert(tokens.size() >= tokensLeft);
         string token = tokens.front();
         assert(token.compare("camera") == 0);
         tokens.pop_front();

         for(;;) {
            token = tokens.front();
            tokens.pop_front();
            if(token.compare("location") == 0) {
               eye = Parser::parse_vector(tokens);
               cout << "set camera \"location\" (eye) to " << eye.c_str() << endl;
            } else if(token.compare("up") == 0) {
               up = Parser::parse_vector(tokens);
               cout << "set camera \"up\" to " << up.c_str() << endl;
            } else if(token.compare("right") == 0) {
               right = Parser::parse_vector(tokens);
               cout << "set camera \"right\" to " << right.c_str() << endl;
            } else if(token.compare("look_at") == 0) {
               lookAt = Parser::parse_vector(tokens);
               cout << "set camera \"look_at\" to " << eye.c_str() << endl;
            } else {
               tokens.push_front(token); //return the unused token
               break;
            }
         }

         return Camera(eye, up, right, lookAt);
      }
};

Camera::Camera(Vector3 &eye, Vector3 &up, Vector3 &right, Vector3 &lookAt) {
   this->eye = eye;
   this->up = up;
   this ->right = right;
   this->lookAt = lookAt;
}

#endif
