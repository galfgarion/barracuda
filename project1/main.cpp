/* Nate Black
 * Z Wood: CPE 473
 * Project 1
 */
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <vector>
#include <iostream>
#include <limits>

#include "vector3.h"
#include "screen.h"
#include "geom.h"

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
string gInputFileName ("");

int parse_int(const char *arg);
void parse_args(int argc, const char **argv);
void draw_image(vector< vector<Color> > image); 

int main(int argc, char **argv) {

   parse_args(argc - 1, (const char **)(argv + 1));

   if(gPixelWidth <= 0 || gPixelHeight <= 0 || gInputFileName.compare(string("")) == 0) {
      printf("Usage: %s +W<int> +H<int> +I<pov input file>\n", argv[0]);
      exit(-1);
   }

   printf("Valid args accepted\n");
   printf("Width: %d\n", gPixelWidth);
   printf("Height: %d\n", gPixelHeight);
   printf("Input file: %s\n", gInputFileName.c_str());

   Vector3 cameraLocation = Vector3(0, 0, 14);
   Vector3 lookAt = Vector3(0, 0, 0);
   Screen screen = Screen(gPixelWidth, gPixelHeight, cameraLocation, -1.3333, 1.3333, 1, -1);

   /* allocate the image */
   vector< vector<Color> > image(gPixelWidth, vector<Color>(gPixelHeight));
   
   /* clear the colors */
   for(int x = 0; x < gPixelWidth; x++) {
      for(int y=0; y < gPixelHeight; y++) {
         image[x][y].r = 0;
         image[x][y].g = 0;
         image[x][y].b = 0;
      }
   }

   Vector3 center = Vector3(0,0,0);
   Sphere sphere = Sphere(center, 2.0);

   Ray zaxis;
   zaxis.origin = Vector3(0, 0, 14);
   zaxis.direction = Vector3(0, 0, -1);

   Ray misser;
   misser.origin = Vector3(-4, 0, 14);
   misser.direction = Vector3(0, 0, -1);

   double intersect = sphere.intersect(zaxis);
   cout << "z-axis intersects sphere at: " << intersect << endl;

   intersect = sphere.intersect(misser);
   cout << "ray that should miss intersects sphere at: " << intersect << endl;

   /* clear the colors */
   for(int x = 0; x < gPixelWidth; x++) {
      for(int y = 0; y < gPixelHeight; y++) {
         double distance = numeric_limits<double>::max();

         Ray ray;
         ray.origin = *screen.pixelToScreen(x, y);

         //cout << "ray.origin: <" << ray.origin.x << "," << ray.origin.y << "," << ray.origin.z << ">" << endl;
         ray.direction = Vector3(0, 0, -1);

         if(sphere.intersect(ray) > 0) {
            image[x][y].r = 255;
         }
      }
   }

   draw_image(image);
   
   return 0;
}

void draw_image(vector< vector<Color> > image) {
   /* determine the output file name */
   string outputFileName (gInputFileName);

   size_t pos = outputFileName.find(".pov");

   if(pos != string::npos) {
      outputFileName.replace(pos, 4, ".ppm");
   } else {
      outputFileName = outputFileName + ".ppm";
   }

   cout << "Output filename: " << outputFileName << endl;

   /* o	pen file */
   FILE * outputFile = fopen(outputFileName.c_str(), "w");
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
   else if(strncmp(argv[0], "+I", 2) == 0) {
      gInputFileName = string(argv[0] + 2);  
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
