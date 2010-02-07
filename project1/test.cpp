#include <iostream>
#include "vector3.h"

using namespace std;

// incoming ray, vector n on the surface at point p where it hit
Vector3 reflect(Vector3 d, Vector3 n) {
   return (d - (2.0 * (d*n) * n));
}


int main() {
   Vector3 ray = Vector3(-1, -1, 0);

   Vector3 normal = Vector3(0, 1, 0);

   Vector3 reflected = reflect(ray, normal);

   cout << "Dot product of " << normal.c_str() << " and " << ray.c_str()
        << " is " << ray * normal << endl;
   cout << "Ray " << ray.c_str() << " was reflected around " << normal.c_str()
        << " to " << reflected.c_str() << endl;
}
