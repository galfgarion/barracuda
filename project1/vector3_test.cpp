#include "assert.h"
#include <iostream>
#include "vector3.h"


using namespace  std;
int main() {

   Vector3 x (1, 0, 0);
   Vector3 y (0, 1, 0);
   Vector3 z (0, 0, 1);

   double result = x.dot(&y);
   assert(result == 0);

   result = x.dot(&x);
   assert(result == 1);



   cout << "Success" << endl;
   

   return 0;
}
