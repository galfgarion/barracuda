#include <GL/glut.h>

float pixel_to_world_x(float x, int xPixels, int yPixels);
float pixel_to_world_y(float y, int yPixels);

/* convert a GLUT pixel coordinate to an image space coordinate */
float pixel_to_world_x(float x, int xPixels, int yPixels) {
   assert(yPixels > 0);

   float pixel_width = 2.0 / yPixels;
   float extent = pixel_width / 2;
   float screen_left = -(float)xPixels / yPixels;
   return screen_left + x * pixel_width + extent;
}

/* convert a GLUT pixel coordinate to an image space coordinate */
float pixel_to_world_y(float y, int yPixels) {
   float pixel_height = 2.0 / yPixels;
   float extent = pixel_height / 2;
   float screen_top = 1;
   return screen_top - y * pixel_height - extent;
}

GLfloat clamp(const GLfloat x, const GLfloat min, const GLfloat max)
{
   if(x < min)
      return min;
   else if(x > max)
      return max;
   else
      return x;
}