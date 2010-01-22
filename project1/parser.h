#ifndef _PARSER_H
#define _PARSER_H

#include "color.h"

class Parser {
   public:
      static double parse_double(string token) {
         stringstream ss;
         double x;
         ss << token;
         ss >> x;
         assert(ss);
         return x;
      }
      static Vector3 parse_vector(deque<string> & tokens) {
         double x, y, z;
         stringstream ss;

         x = Parser::parse_double(tokens.front());
         tokens.pop_front();

         y = Parser::parse_double(tokens.front());
         tokens.pop_front();

         z = Parser::parse_double(tokens.front());
         tokens.pop_front();
            
         return Vector3(x, y, z);
      }
      static Color parse_color(deque<string> & tokens) {
         tokens.pop_front(); // pigment
         tokens.pop_front(); // color
         tokens.pop_front(); // rgb
         Vector3 &vector = parse_vector(tokens);
         Color color;
         color.r = (byte) (255 * vector.x);
         color.g = (byte) (255 * vector.y);
         color.b = (byte) (255 * vector.z);
         cout << "parsed color: " << vector.c_str() << " converted to <" 
              << (int) color.r << ", " << (int) color.g << ", " << (int) color.b << ">" << endl;
         return color;
      }

};

#endif // _PARSER_H