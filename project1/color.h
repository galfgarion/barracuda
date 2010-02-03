#ifndef _COLOR_H
#define  _COLOR_H
#include <math.h>

typedef unsigned char byte;

class Color {
   public:
      double r, g, b;
      Color() {
         r = g = b = 0.0;
      }
      Color(double r, double g, double b) {
         //this->r = min(r, 1.0);
         //this->g = min(g, 1.0);
         //this->b = min(b, 1.0);
         this->r = r;
         this->g = g;
         this->b = b;
      }
      Color operator+(const Color & other) {
         //return Color(min(1.0, r + other.r), min(1.0, g + other.g), min(1.0, b + other.b));
		  return Color(r + other.r, g + other.g, b + other.b);
      }
      Color operator*(double x) {
         //return Color(min(r * x, 1.0), min(g * x, 1.0), min(b * x, 1.0));
		  return Color(r * x, g * x, b * x);
      }
};

typedef struct byte_color_t {
   byte r, g, b;
} ByteColor;

#endif
