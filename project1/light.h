#ifndef _LIGHT_H
#define _LIGHT_H

#include <deque>
#include "vector3.h"
#include "parser.h"

using namespace std;

class Light {
   public:
      Point location;
      Color color;
      Light(const Point & location, const Color & color) {
         this->location = Point(location);
         this->color = color;
      }
      Light(deque<string> & tokens) {
         assert(!tokens.front().compare("light_source"));
         tokens.pop_front(); // light_source

         location = Parser::parse_vector(tokens);
         color = Parser::parse_color(tokens);
      }
};

#endif
