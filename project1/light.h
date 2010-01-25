#ifndef _LIGHT_H
#define _LIGHT_H

#include <deque>
#include "vector3.h"
#include "parser.h"

using namespace std;

class Light {
   private:
      Point _location;
      Color _color;

   public:
      Light(const Point & location, const Color & color) {
         this->_location = Point(location);
         this->_color = color;
      }
      Light(deque<string> & tokens) {
         assert(!tokens.front().compare("light_source"));
         tokens.pop_front(); // light_source

         _location = Parser::parse_vector(tokens);
         _color = Parser::parse_color(tokens);
      }
};

#endif
