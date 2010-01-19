/* Nate Black
 * Z Wood: CPE 473
 * Lab1
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>

#include "vector3.h"

using namespace std;

typedef unsigned char byte;

typedef struct color_t {
   byte r, g, b;
} Color;

typedef struct image_t {
} Image;

// globals
int gPixelWidth = -1;
int gPixelHeight = -1;
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

   parse_args(argc - 1, (const char **)(argv + 1));

   if(gPixelWidth <= 0 || gPixelHeight <= 0) {
      printf("Usage: %s +W<int> +H<int>\n", argv[0]);
      exit(-1);
   }

   printf("Valid args accepted\n");
   printf("Width: %d\n", gPixelWidth);
   printf("Height: %d\n", gPixelHeight);

   /* allocate the image */
   vector< vector<Color> > image(gPixelWidth, vector<Color>(gPixelHeight));
   
   bool oddcol = false;
   bool oddrow = false;

   int lineHeight = gPixelHeight / 8;
   int lineWidth = gPixelWidth / 8;

   /* set the colors to draw lines */
   for(int x = 0; x < gPixelWidth; x++) {
      if(x % lineWidth == 0) {
        oddcol = !oddcol;
      }
 
      for(int y=0; y < gPixelHeight; y++) {
         if(y % lineHeight == 0) {
             oddrow = !oddrow;
		 }
         if(oddcol && oddrow || !oddcol && !oddrow) {
            image[x][y].r = 255;
            image[x][y].g = 255;
            image[x][y].b = 255;
         }
         else {
            image[x][y].r = 0;
            image[x][y].g = 0;
            image[x][y].b = 0;
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

   /* o	pen file */
   FILE * outputFile = fopen(outputFileName, "w");
   if(NULL == outputFile) {
      perror("draw_image()");
      exit(-1);
   }

   /* write header */
   fputs("P6\n", outputFile);
   fprintf(outputFile, "%d %d \n", gPixelWidth, gPixelHeight);
   fputs("255\n", outputFile); // max color val


   /* write pixel info */
   for(int y = 0; y < gPixelHeight; y++) {
      for(int x = 0; x < gPixelWidth; x++) {
         fputc(image[x][y].r, outputFile);
         fputc(image[x][y].g, outputFile);
         fputc(image[x][y].b, outputFile);
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
      gPixelHeight = parse_int(argv[0]);
   }
   else if(strncmp(argv[0], "+W", 2) == 0) {
      gPixelWidth = parse_int(argv[0]);
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
