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
   finish_t() : ambient(0), diffuse(0), specular(0), roughness(0.05),
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
      Plane() {}
      Plane(Vector3& normal, double d);
      Plane(deque<string> & tokens);
      double intersect(const Ray & ray);
      Vector3 surfaceNormal(const Point &p);

   private:
      Vector3 _normal;
      Point p0;
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
   p0 = d * _normal;
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
   double d = Parser::parse_double(tokens.front());
   p0 = d * _normal;
   tokens.pop_front();

   cout << "parsed plane with normal " << _normal.c_str() << " and d " << d << endl;
   
   parseOptions(tokens);
}

double Plane::intersect(const Ray & ray) {
   double denom = ray.direction * _normal;

   if(denom == 0) {
      return -1.0;
   }

   double numerator = (p0 - ray.origin) * _normal;
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

#define ZMIN 0
#define ZMAX 1
#define XMIN 2
#define XMAX 3
#define YMAX 4
#define YMIN 5

class Box : public GeomObject {
   double xMin, xMax, yMin, yMax, zMin, zMax;
   public:
      Box(double, double, double, double, double, double);
      double intersect(const Ray& ray);
      Vector3 surfaceNormal(const Point& p);
};

Vector3 Box::surfaceNormal(const Point& p) {
   if(fabs(p.x - xMin) < EPSILON) return Vector3(-1, 0, 0);
   if(fabs(p.x - xMax) < EPSILON) return Vector3(1, 0, 0);
   if(fabs(p.y - yMin) < EPSILON) return Vector3(0, -1, 0);
   if(fabs(p.y - yMax) < EPSILON) return Vector3(0, 1, 0);
   if(fabs(p.z - zMin) < EPSILON) return Vector3(0, 0, -1);
   if(fabs(p.z - zMax) < EPSILON) return Vector3(0, 0, 1);

   assert(false); //error if reached
}

Box::Box(double xMin, double xMax, double yMin, double yMax, double zMin, 
   double zMax) {

   this->xMin = xMin;
   this->xMax = xMax;
   this->yMin = yMin;
   this->yMax = yMax;
   this->zMin = zMin;
   this->zMax = zMax;

   // defaults for bounding box draw
   Finish finish;
   finish.specular = 0.5;
   finish.diffuse = 0.5;
   finish.filter = 0.5;
   finish.refraction = 1;
   finish.ior = 1;

   this->color = Color(0, 1, 0);
   this->finish = finish;
}

void swap(double *a, double *b) {
   double tmp = *a;
   *a = *b;
   *b = tmp;
}
double Box::intersect(const Ray& ray) {
   Vector3 x = Vector3(1, 0, 0);
   Vector3 y = Vector3(0, 1, 0);
   Vector3 z = Vector3(0, 0, 1);
   Vector3 negx = Vector3(1, 0, 0);
   Vector3 negy = Vector3(0, 1, 0);
   Vector3 negz = Vector3(0, 0, 1);
   
   Plane planes[6];
   planes[ZMIN] = Plane(negz, zMin);
   planes[ZMAX] = Plane(z, zMax); // furthest away from viewer at origin
   planes[XMIN] = Plane(negx, xMin);
   planes[XMAX] = Plane(x, xMax);
   planes[YMIN] = Plane(negy, yMin);
   planes[YMAX] = Plane(y, yMax);

   double d[6];

   for(int i=0; i < 6; i++) {
      d[i] = planes[i].intersect(ray);
      if(d[i] > EPSILON) { 
         Point p = ray.origin + d[i]*ray.direction;

         if(i == ZMIN || i == ZMAX) {
            if(p.x < xMin || p.x > xMax || p.y < yMin || p.y > yMax)
               d[i] = -1;
         }
         else if(i == XMIN || i == XMAX) {
            if(p.y < yMin || p.y > yMax || p.z < zMin || p.z > zMax)
               d[i] = -1;
         }
         else if(i == YMIN || i == YMAX) {
            if(p.x < xMin || p.x > xMax || p.z < zMin || p.z > zMax) {
               d[i] = -1;
            }
         }
      }
   }

   bool hit = false;
   double t = numeric_limits<double>::max();

   // find the smallest positive d if it exists
   for(int i=0; i < 6; i++) {
      if(d[i] > EPSILON && d[i] < t) {
         t = d[i];
      }
   }

   return t;
}

#endif
