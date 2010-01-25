#ifndef _COLOR_H
#define  _COLOR_H

typedef unsigned char byte;

class Color {
   public:
      double r, g, b;
      Color() {
         r = g = b = 0.0;
      }
      Color(double r, double g, double b) {
         this->r = r;
         this->g = g;
         this->b = b;
      }
      Color operator+(const Color & other) {
         return Color(r + other.r, g + other.g, b + other.b);
      }
};

typedef struct byte_color_t {
   byte r, g, b;
} ByteColor;

#endif
