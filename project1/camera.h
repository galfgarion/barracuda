#ifndef _CAMERA_H
#define _CAMERA_H
class Camera {
   public:
      Camera() {}
      Camera(Vector3& eye, Vector3& up, Vector3& right, Vector3& lookAt,
         int xRes, int yRes); 
      Vector3 eye, up, right, lookAt;
      int xRes, yRes;
      Vector3 pixelToScreen(int x, int y);

      static Camera parse(deque<string> & tokens, int xRes, int yRes) {
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

         return Camera(eye, up, right, lookAt, xRes, yRes);
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
         Vector3 u = up.cross(w).normalize(); 
         Vector3 v = w.cross(u).normalize();

         cout << "u: " << u.c_str() << endl;
         cout << "v: " << v.c_str() << endl;
         cout << "w: " << w.c_str() << endl;

         assert(fabs(u * v) < 0.00000001);
         assert(fabs(w * v) < 0.00000001);
         assert(fabs(w * u) < 0.00000001);


         return Matrix4x4(
            u.x, v.x, w.x, 0,
            u.y, v.y, w.y, 0,
            u.z, v.z, w.z, 0,
            0, 0, 0, 1
         );
      }

      Matrix4x4 transform() {
         return  vectorTransform() * translate();
      }

      double pixelToScreenX(int x) {
         double screenWidth = right.magnitude();
         double left = -(right.magnitude() / 2.0);
         double wPixels = double(xRes);

         return left + screenWidth * (x + 0.5) / (wPixels - 1);
      }

      double pixelToScreenY(int y) {
         double hPixels = double(yRes);
         double screenHeight = up.magnitude();
         double top = screenHeight / 2.0;

         return top - screenHeight * (y + 0.5) / (hPixels - 1);
      }

};

Vector3 Camera::pixelToScreen(int x, int y) {
   return Vector3(pixelToScreenX(x), pixelToScreenY(y), -1);
}

Camera::Camera(Vector3 &eye, Vector3 &up, Vector3 &right, Vector3 &lookAt,
   int xRes, int yRes) {
   this->eye = eye;
   this->up = up;
   this ->right = right;
   this->lookAt = lookAt;
   this->xRes = xRes;
   this->yRes = yRes;
}
#endif
