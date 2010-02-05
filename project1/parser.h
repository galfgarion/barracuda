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
      static double parse_double(deque<string> & tokens) {
         assert(!tokens.empty());
         string s = tokens.front();
         double d = parse_double(s);
         tokens.pop_front();
         return d;
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
         //tokens.pop_front(); // pigment
         tokens.pop_front(); // color
         tokens.pop_front(); // rgb
         //Vector3 &vector = parse_vector(tokens); //g++ doesn't like this but VS thinks it's ok
         Vector3 v = parse_vector(tokens);
         Color color = Color(v.x, v.y, v.z);
         cout << "parsed color: " 
              << "<" << color.r << ", " << color.g << ", " <<  color.b << ">" << endl;

         double maxval = max(color.r, color.g);
         maxval = max(maxval, color.b);
         /*
         if(maxval > 1.0) {
            double scaleFactor = 1.0 / maxval;
            color.r *= scaleFactor;
            color.g *= scaleFactor;
            color.b *= scaleFactor;
            cout << "scaled color to: " 
                 << "<" << color.r << ", " << color.g << ", " <<  color.b << ">" << endl;
         }
         */
         return color;
      }

};

#endif // _PARSER_H
