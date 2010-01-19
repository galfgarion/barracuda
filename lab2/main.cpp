/* Nate Black
 * Z Wood: CPE 473
 * Lab1
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <boost/spirit.hpp>
#include <iostream>

using namespace std;
using namespace boost::spirit;

typedef unsigned char byte;

typedef struct color_t {
   byte r, g, b;
} Color;

typedef struct image_t {
} Image;

int gWidth = -1;
int gHeight = -1;
char inputFileName [10];

int parse_int(const char *arg);
void parse_args(int argc, const char **argv);
void draw_image(vector< vector<Color> > image); 

class GeomObject {
};

/*
class Box : GeomObject {
};

class Sphere : GeomObject {
};

class Cone : GeomObject {
};

class PlanThe function push accepts an iterator str that points to the first matching character, as well as an iterator end pointing to one past the last valid character. We want to attach this function to the integer production rule: e : GeomObject {
};
*/

class Triangle : GeomObject {

};

void parse_camera(char const * str, char const * end) {
   cout << "Parsed a camera object" << endl;
}

Triangle * parse_triangle(char const * str, char const * end) {
   cout << "Parsed a triangle object:"<< endl << str << endl;
   //parse_vector3(str, vector3_p);
   return new Triangle();
}

int main(int argc, char **argv) {
   using qi::double_;

   parse_args(argc - 1, (const char **)(argv + 1));

   if(gWidth <= 0 || gHeight <= 0) {
      printf("Usage: %s +W<int> +H<int>\n", argv[0]);
      exit(-1);
   }

   printf("Valid args accepted\n");
   printf("Width: %d\n", gWidth);
   printf("Height: %d\n", gHeight);

   /* allocate the image */
   vector< vector<Color> > image(gHeight, vector<Color>(gWidth));
   //Color ** image = (Color **) malloc(gWidth * gHeight * sizeof(Color));
   //
   /*
   if(NULL == image) {
      fprintf(stderr, "Failed to allocate memory for the image\n");
      exit(-1);
   }
   */

   rule<> vector3_test = ch_p('<') >> double_ >> ',' >> double_ >> ',' 
      >> double_ >> '>';

   parse("<1,2,3>", vector3_test, space_p);

   /* experiment with some parser stuff */
   rule<> vector3_p = ch_p('<') >> *space_p >> real_p 
      >> ',' >> *space_p >> real_p >> *space_p
      >> ',' >> *space_p >> real_p >> *space_p >> ch_p('>');


   rule<> whitespace_p = *space_p;
   rule<> ws_p = whitespace_p;

   rule<> anything_p = *(anychar_p) ;

   rule<> camera_p = str_p("camera") >> whitespace_p >> '{' 
      >> anything_p >> '}';

   rule<> triangle_opts = *space_p >> vector3_p >> *space_p
      ;

   rule<> triangle_p = str_p("triangle") >> *space_p >> '{'
      >> triangle_opts >> '}'
      ;


   rule<> identifier = str_p("triangle") |  "light_source" | "sphere" | "plane";

   rule<> triangle_opts =  | 
   rule<> opt_p = *space_p;

   rule<> object_p = identifier >> *space_p >> '{' >> opt_p >> '}' ;

   //parse_info pinfo = parse("aksdkd <1, 2, 3.5> dkdkas\n", vector_p);
   //
   std::string triangle_str = "triangle  { djdjsj }\ncamera\n{ }";

   if(parse("triangle\n\t {\n\n\t  }", object_p[&parse_triangle]).full) {
   //if(parse("<1, 2, 3>", vector3_p).full) {
      cout << "Success" << endl;
   }
   else {
      cout << "Fail" << endl;
   }
   //
   std::string test_str = "triangle  { djdjsj }\ncamera\n{ }";

   if(parse("light_source {\n\t }", object_p[&parse_triangle]).full) {
   //if(parse("<1, 2, 3>", vector3_p).full) {
      cout << "Success" << endl;
   }
   else {
      cout << "Fail" << endl;
   }

   //*anychar_p ch_p('\n');
   //

   bool oddcol = false;
   bool oddrow = false;

   int lineHeight = gHeight / 8;
   int lineWidth = gWidth / 8;

   /* set the colors to draw lines */
   for(int y = 0; y < gHeight; y++) {
      if(y % lineHeight == 0) {
        oddcol = !oddcol;
      }
 
      for(int x=0; x < gWidth; x++) {
         if(x % lineWidth == 0) {
	    oddrow = !oddrow;
	 }
         if(oddcol && oddrow || !oddcol && !oddrow) {
            image[y][x].r = 255;
            image[y][x].g = 255;
            image[y][x].b = 255;
         }
      }
  }

   draw_image(image);
   
   return 0;
}

void draw_image(vector< vector<Color> > image) {
   /* determine the output file name */
   // TODO this is a fake
   char outputFileName[] = "output.ppm";

   /* open file */
   FILE * outputFile = fopen(outputFileName, "w");
   if(NULL == outputFile) {
      perror("draw_image()");
      exit(-1);
   }

   /* write header */
   fputs("P6\n", outputFile);
   fprintf(outputFile, "%d %d \n", gWidth, gHeight);
   fputs("255\n", outputFile); // max color val


   /* write pixel info */
   for(int y = 0; y < gHeight; y++) {
      for(int x = 0; x < gWidth; x++) {
         fputc(image[y][x].r, outputFile);
         fputc(image[y][x].g, outputFile);
         fputc(image[y][x].b, outputFile);
      }
      //fputs("\n", outputFile);
   }

   /* close file */
   fclose(outputFile);
}

void parse_args(int argc, const char **argv) {
   if(argc < 1) {
      return;
   }

   if(strncmp(argv[0], "+H", 2) == 0) {
      gHeight = parse_int(argv[0]);
   }
   else if(strncmp(argv[0], "+W", 2) == 0) {
      gWidth = parse_int(argv[0]);
   }
   else {
      printf("Unknown argument: %s", argv[0]);
      exit(-1);
   }

   parse_args(argc - 1, argv + 1);

   return;
}

int parse_int(const char * arg) {
   printf("Int portion: %s\n", arg + 2);

   return atoi(arg + 2);
}
