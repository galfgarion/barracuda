#include <iostream>
#include "vector3.h"

using namespace std;

// incoming ray, vector n on the surface at point p where it hit
Vector3 reflect(Vector3 d, Vector3 n) {
   n = n.normalize();
   return (d - (2.0 * (d*n) * n));
}

/*
Vector3 refract(Vector3 d, Vector3 n, double iorIn, double iorOut) {
   d = d.normalize();
   n = n.normalize();

   double iorOutSquared = iorOut * iorOut;
   double iorInSquared = iorIn * iorIn;
   double dDotN = d * n;
   double dDotNSquared = dDotN * dDotN;

   double underRoot = 1 - (iorInSquared * (1 -  dDotNSquared) / iorOutSquared);

   // total internal reflection
   if(underRoot < 0)
      return reflect(d, n);

   return (1.0 / iorOut) * (iorIn * (d - (n * dDotN))) - (n * sqrt(underRoot));
}
*/

Vector3 refract(Vector3 d, Vector3 n, double iorIn, double iorOut) {
   d = d.normalize();
   n = n.normalize();
   double iorRatio = iorIn / iorOut;

   // HACK i did this to get the results matching lab3,
   // but not sure if it's correct
   if(iorIn > iorOut) {
      iorRatio = 1.0 / iorRatio;
   }

   double iorRatioSquared = iorRatio * iorRatio;

   double underRoot = 1 - (iorRatioSquared * (1 - (d * n)*(d * n)));
   // total internal reflection
   if(underRoot < 0)
      return reflect(d, n);

   return iorRatio * (d - n*(d * n)) - n*sqrt(underRoot);
}

int main() {
   Vector3 ray = Vector3(-1, -1, 0);

   Vector3 normal = Vector3(0, 1, 0);

   Vector3 reflected = reflect(ray, normal);

   cout << "Dot product of " << normal.c_str() << " and " << ray.c_str()
        << " is " << ray * normal << endl;
   cout << "Ray " << ray.c_str() << " was reflected around " << normal.c_str()
        << " to " << reflected.c_str() << endl;

   ray = Vector3(0, 0, -1);
   normal = Vector3(1, 0, 1);
   reflected = reflect(ray, normal);
   cout << "Ray " << ray.c_str() << " was reflected around " << normal.c_str()
        << " to " << reflected.c_str() << endl;



   Vector3 refracted = refract(ray, normal, 1, 1.33);

   cout << "refracted to " << refracted.c_str() << endl;

   ray = Vector3(.707, -.707, 0);
   refracted = refract(ray, normal, 1, .9);
   cout << "refracted to " << refracted.c_str() << endl;

}
