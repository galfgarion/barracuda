#include <deque>
#include <iostream>
#include <assert.h>
#include <sstream>
#include "vector3.h"
#include "parser.h"
#include "color.h"
#include "matrix4x4.h"

#ifndef _GEOM_H
#define _GEOM_H

#define DEBUG false

#ifndef EPSILON
#define EPSILON 0.00001
#endif

using namespace std;

typedef Vector3 Point;

typedef struct s_ray {
   Vector3 direction;
   Vector3 origin; 
} Ray;

typedef struct finish_t {
   double ambient, diffuse, specular, roughness;
   double ior, reflection, refraction, filter;
   finish_t() : ambient(0), diffuse(0), specular(0), roughness(0),
      ior(1), reflection(0), refraction(0), filter(0) {}
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
      } else if(!tokens.front().compare("ior")) {
         tokens.pop_front();
         finish.ior = Parser::parse_double(tokens);
         cout << "ior = " << finish.ior << endl;
      } else if(!tokens.front().compare("reflection")) {
         tokens.pop_front();
         finish.reflection = Parser::parse_double(tokens);
         cout << "reflection = " << finish.reflection << endl;
      } else if(!tokens.front().compare("refraction")) {
         tokens.pop_front();
         finish.refraction = Parser::parse_double(tokens);
         cout << "refraction = " << finish.refraction << endl;
      } else break;
   }

   return finish;
}

class GeomObject {
   public:
      Color color;
      Finish finish;

      virtual double intersect(const Ray&) = 0; // pure virtual fn
      virtual Vector3 surfaceNormal(const Point & p) = 0;

      virtual void parseOptions(deque<string> & tokens) {
        double tmpFilter = 0.0;
        while(!tokens.empty()) {
            if(!tokens.front().compare("pigment")) {
               tokens.pop_front(); // pigment
               tokens.pop_front(); // color
               string colorType = tokens.front();
               tokens.push_front("color");
               color = Parser::parse_color(tokens);
               if(!colorType.compare("rgbf")) {
                  tmpFilter = Parser::parse_double(tokens);
               }
            } else if (!tokens.front().compare("finish")){
               this->finish = parse_finish(tokens);
            } else break;  
         }

         finish.filter = tmpFilter;
         cout << "filter = " << finish.filter << endl;
      }
};

class Sphere: public GeomObject {
   public:
      Sphere(Vector3& center, double radius);
      Sphere(deque<string> & tokens);
      double intersect(const Ray & ray);
      Vector3 surfaceNormal(const Point & p);

   private:
      Vector3 _center;
      double _radius;
};

class Plane: public GeomObject {
   public:
      Plane(Vector3& normal, double d);
      Plane(deque<string> & tokens);
      double intersect(const Ray & ray);
      Vector3 surfaceNormal(const Point &p);

   private:
      Vector3 _normal;
      double _d;
};

class Triangle: public GeomObject {
   Point v1, v2, v3;
   public:
   
   Triangle() {} 

   Triangle(deque<string> & tokens);
   Vector3 surfaceNormal(const Point &p);
   double intersect(const Ray & ray);

};

Vector3 Triangle::surfaceNormal(const Point &p) {
   Vector3 s1 = v2 - v1;
   Vector3 s2 = v3 - v1;
   return s1.cross(s2).normalize();
}

double Triangle::intersect(const Ray & ray) {
   // TODO
   double a, b, c, d, e, f, g, h, i, j, k, l;

   a = v1.x - v2.x;
   b = v1.y - v2.y;
   c = v1.z - v2.z;

   d = v1.x - v3.x;
   e = v1.y - v3.y;
   f = v1.z - v3.z;

   g = ray.direction.x;
   h = ray.direction.y;
   i = ray.direction.z;

   j = v1.x - ray.origin.x;
   k = v1.y - ray.origin.y;
   l = v1.z - ray.origin.z;

   double ei_minus_hf = e*i - h*f;
   double gf_minus_di = g*f - d*i;
   double dh_minus_eg = d*h - e*g;
   double ak_minus_jb = a*k - j*b;
   double jc_minus_al = j*c - a*l;
   double bl_minus_kc = b*l - k*c;

   double M = a * ei_minus_hf + b * gf_minus_di + c * dh_minus_eg;

   if(M == 0) {
      return -1;
   }

   double t = -(f * ak_minus_jb + e * jc_minus_al + d * bl_minus_kc) / M;

   // TODO we could do an additional shortcut if we restrict the time interval
   if(t < EPSILON) {
      //if(DEBUG) cout << "missed triangle due to t" << endl;
      return -1;
   }

   double gamma = (i * ak_minus_jb + h * jc_minus_al + g * bl_minus_kc) / M;

   if(gamma < 0 || gamma > 1) {
      //if(DEBUG) cout << "missed triangle due to gamma" << endl;
      return -1;
   }

   double beta = (j * ei_minus_hf + k * gf_minus_di + l * dh_minus_eg) / M;

   if(beta < 0 || beta > 1 - gamma) {
      //if(DEBUG) cout << "missed triangle due to beta" << endl;
      return -1;
   }

   //cout << "hit triangle at t=" << t << endl;

   //return -1;

   Vector3 hitRay = ray.direction.normalize();
   hitRay = hitRay * t;
   double distance = hitRay.magnitude();
   if(DEBUG) cout << "triangle hit at distance: " << distance << endl;
   return hitRay.magnitude();
}

Vector3 Plane::surfaceNormal(const Point &p) {
   return Vector3(this->_normal).normalize();
}

Plane::Plane(Vector3& normal, double d) {
   _normal = normal;
   _d = d;
}

Triangle::Triangle(deque<string> & tokens) {
   // there has to be a better way to do initialization
   finish.specular = 0.0;
   finish.diffuse = 0.0;
   finish.ambient = 0.0;

   assert(!tokens.front().compare("triangle"));
   tokens.pop_front();

   v1 = Parser::parse_vector(tokens);
   v2 = Parser::parse_vector(tokens);
   v3 = Parser::parse_vector(tokens);

   //Vector3 s1 = v2 - v1;
   //Vector3 s2 = v3 - v1;

   /*
   if(s1 * s2 < 0) {
      Vector3 tmp = v1;
      v1 = v2;
      v2 = tmp;
   }
   */

   cout << "parsed triangle with vertices " << v1.c_str() << ", "
        << v2.c_str() << ", " << v3.c_str() << endl;

   parseOptions(tokens);
}

Plane::Plane(deque<string> & tokens) {

   // there has to be a better way to do initialization
   finish.specular = 0.0;
   finish.diffuse = 0.0;
   finish.ambient = 0.0;
   finish.reflection = 0.0;
   finish.refraction = 0.0;

   assert(!tokens.front().compare("plane"));
   tokens.pop_front();

   _normal = Parser::parse_vector(tokens);
   _d = Parser::parse_double(tokens.front());
   tokens.pop_front();

   cout << "parsed plane with normal " << _normal.c_str() << " and d " << _d << endl;
   
   parseOptions(tokens);
}

double Plane::intersect(const Ray & ray) {
   Vector3 p1 = _normal * _d;
   double denom = ray.direction * _normal;

   if(denom == 0) {
      return -1.0;
   }

   double numerator = (p1 - ray.origin) * _normal;
   double dist = numerator / denom;
   if(dist < EPSILON) {
      return -1;
   }
   return dist;
}

Sphere::Sphere(Vector3 &center, double radius) {
   _center = Vector3(center);
   _radius = radius;
}

Sphere::Sphere(deque<string> & tokens) {
   finish.specular = finish.diffuse = finish.ambient = finish.filter = 0.0;

   assert(!tokens.front().compare("sphere"));
   tokens.pop_front();
   _center = Parser::parse_vector(tokens);
   _radius = Parser::parse_double(tokens.front());
   tokens.pop_front();

   parseOptions(tokens);

   cout << "parsed sphere with center " << _center.c_str() << " and radius " << _radius << endl;
}

Vector3 Sphere::surfaceNormal(const Point & p) {
   return (p - this->_center).normalize();
}

// returns closest non-negative intersection, or -1 if a non-negative intersection does not exist
double Sphere::intersect(const Ray & ray) {
   
   Vector3 centerToOrigin = ray.origin - _center;
   double A = ray.direction * ray.direction;
   double B = 2 * (ray.direction * centerToOrigin);
   double C = centerToOrigin * centerToOrigin - _radius * _radius;

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

      if(t1 < EPSILON && t2 < EPSILON) {
         return -1;
      }

      else if(t1 < EPSILON) {
         return t2;
      } else if(t2 < EPSILON) {
         return t1;
      } else {
         return min(t1, t2);
      }
   }
}


class Camera {
   public:
      Camera() {}
      Camera(Vector3& eye, Vector3& up, Vector3& right, Vector3& lookAt); 
      Vector3 eye, up, right, lookAt;
      Vector3 pixelToScreen(int x, int y);

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
               cout << "set camera \"look_at\" to " << lookAt.c_str() << endl;
            } else {
               tokens.push_front(token); //return the unused token
               break;
            }
         }

         return Camera(eye, up, right, lookAt);
      }

      Matrix4x4 translate() {
         return Matrix4x4(
            1, 0, 0, eye.x,
            0, 1, 0, eye.y,
            0, 0, 1, eye.z,
            0, 0, 0, 1
         );
      }

      Matrix4x4 vectorTransform() {

         Vector3 x(1, 0, 0);
         Vector3 y(0, 1, 0);
         Vector3 z(0, 0, 1);

         Vector3 w = (eye - lookAt).normalize();
         cout << "w: " << w.c_str() << endl;
         Vector3 u = up.cross(w).normalize(); 
         Vector3 v = w.cross(u);
         cout << "v: " << v.c_str() << endl;

         cout << "u: " << v.cross(w).c_str() << endl;

         // project v onto x-y plane
         // rotate such that v is in the y-z plane
         Vector3 vPrime = Vector3(v.x, v.y, 0).normalize();
         double cosThetaZ = vPrime * y;
         double sinThetaZ = sqrt(1 - cosThetaZ*cosThetaZ);
         if(v.x > 0) {
            sinThetaZ = -sinThetaZ;
         }

         Matrix4x4 RotZ = Matrix4x4 (
            cosThetaZ, -sinThetaZ, 0, 0,
            sinThetaZ, cosThetaZ, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
         );

         cout << "RotZ: " << RotZ.c_str() << endl;
         assert(RotZ * z == z);

         Matrix4x4 RotZInv = RotZ.inverse();

         // rotate such that vPrime is in x-y
         vPrime = RotZInv * v;
         double cosThetaX = vPrime * y;
         double sinThetaX = sqrt(1 - cosThetaX*cosThetaX);
         if(vPrime.z < 0) {
            sinThetaX = -sinThetaX;
         }

         Matrix4x4 RotX = Matrix4x4 (
            1, 0, 0, 0,
            0, cosThetaX, -sinThetaX, 0,
            0, sinThetaX, cosThetaX, 0,
            0, 0, 0, 1
         );

         cout << "RotX: " << RotX.c_str() << endl;
         assert(RotX * x == x);

         Matrix4x4 RotXInv = RotX.inverse();

         Vector3 wPrime = (RotXInv * (RotZInv * w));
         double cosThetaY = wPrime * z;
         double sinThetaY = sqrt(1 - cosThetaY*cosThetaY);
         if(wPrime.x < 0) {
            sinThetaY = -sinThetaY;
         }


         Matrix4x4 RotY = Matrix4x4 (
            cosThetaY, 0, sinThetaY, 0,
            0, 1, 0, 0,
            -sinThetaY, 0, cosThetaY, 0,
            0, 0, 0, 1
         );

         cout << "RotY: " << RotY.c_str() << endl;
         assert(RotY * y == y);

         return RotZ * (RotX * RotY);
      }

      Matrix4x4 transform() {
         return  vectorTransform() * translate();
      }

      double pixelToScreenX(int x) {
         double screenWidth = right.magnitude();
         double left = -(right.magnitude() / 2.0);
         //TODO HACK assume 640 x 480
         double wPixels = 640;

         return left + screenWidth * (x + 0.5) / (wPixels - 1);
      }

      double pixelToScreenY(int y) {
         //TODO HACK assume 640 x 480
         double hPixels = 480;
         double screenHeight = up.magnitude();
         double top = screenHeight / 2.0;

         return top - screenHeight * (y + 0.5) / (hPixels - 1);
      }

};

Vector3 Camera::pixelToScreen(int x, int y) {
   return Vector3(pixelToScreenX(x), pixelToScreenY(y), -1);
}

Camera::Camera(Vector3 &eye, Vector3 &up, Vector3 &right, Vector3 &lookAt) {
   this->eye = eye;
   this->up = up;
   this ->right = right;
   this->lookAt = lookAt;
}



#endif
