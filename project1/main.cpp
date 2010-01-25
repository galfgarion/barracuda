/* Nate Black
 * Z Wood: CPE 473
 * Project 1
 */
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <vector>
#include <deque>
#include <iostream>
#include <limits>
#include <fstream>

#include "vector3.h"
#include "screen.h"
#include "geom.h"

using namespace std;


typedef struct image_t {
} Image;

// globals
int gPixelWidth = -1;
int gPixelHeight = -1;
string gInputFileName ("");

int parse_int(const char *arg);
void parse_args(int argc, const char **argv);
void draw_image(vector< vector<ByteColor> > image);
void parse_file(vector<GeomObject*> & objects, Camera * camera);

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

   /* allocate the image */
   vector< vector<ByteColor> > image(gPixelWidth, vector<ByteColor>(gPixelHeight));
   
   /* clear the colors */
   for(int x = 0; x < gPixelWidth; x++) {
      for(int y=0; y < gPixelHeight; y++) {
         image[x][y].r = 0;
         image[x][y].g = 0;
         image[x][y].b = 0;
      }
   }


   vector<GeomObject*> objects;
   Camera camera;
   
   parse_file(objects, &camera);

   Screen screen = Screen(gPixelWidth, gPixelHeight, camera.eye, -camera.right.magnitude() / 2.0,
      camera.right.magnitude() / 2.0, camera.up.magnitude() / 2.0, -camera.up.magnitude() / 2.0);

   /* clear the colors */
   for(int x = 0; x < gPixelWidth; x++) {
      for(int y = 0; y < gPixelHeight; y++) {
         double closest = numeric_limits<double>::max();

         Ray ray;
         ray.origin = *screen.pixelToScreen(x, y);

         //cout << "ray.origin: <" << ray.origin.x << "," << ray.origin.y << "," << ray.origin.z << ">" << endl;
         ray.direction = *ray.origin.subtract(&(camera.eye));

         double distance = 0;

         for(unsigned int i=0; i < objects.size(); i++) {
            distance = objects[i]->intersect(ray);
            Point p = ray.origin + (ray.direction.normalize() * distance); // point on object
            Vector3 n = objects[i]->surfaceNormal(p); // surface normal

            if(distance > 0 && distance < closest) {
               closest = distance;
               image[x][y].r = (byte) (objects[i]->color.r * 255);
               image[x][y].g = (byte) (objects[i]->color.g * 255);
               image[x][y].b = (byte) (objects[i]->color.b * 255);
            }
         }
      }
   }

   draw_image(image);
   
   return 0;
}

void parse_file(vector<GeomObject*> & objects, Camera * camera) {
   string delimiters = " \n\t<>,{}";
    deque<string> tokens;
    
   
    ifstream file(gInputFileName.c_str());
    string str;

    while(!file.eof()) {
       getline(file, str);
       // Skip delimiters at beginning.
       string::size_type lastPos = str.find_first_not_of(delimiters, 0);
       // Find first "non-delimiter".
       string::size_type pos     = str.find_first_of(delimiters, lastPos);

       while (string::npos != pos || string::npos != lastPos)
       {
           // handle comments
          if(pos - lastPos > 1 && strncmp(str.substr(lastPos, 2).c_str(), "//", 2) == 0) {
             break;
          }
           // Found a token, add it to the vector.
           tokens.push_back(str.substr(lastPos, pos - lastPos));
           // Skip delimiters.  Note the "not_of"
           lastPos = str.find_first_not_of(delimiters, pos);
           // Find next "non-delimiter"
           pos = str.find_first_of(delimiters, lastPos);
       }
    }
   
   while( tokens.size() > 0) {
      if(!tokens.front().compare("camera")) {
         cout << "parsing camera" << endl;
         *camera = Camera::parse(tokens);
      }
      else if(!tokens.front().compare("sphere")) {
         cout << "parsing sphere" << endl;
         objects.push_back(new Sphere(tokens));
      } else if(!tokens.front().compare("plane")) {
         cout << "parsing plane" << endl;
         objects.push_back(new Plane(tokens));
      }
       cout << tokens.front();
       cout << endl;
       tokens.pop_front(); 
   }

}

void draw_image(vector< vector<ByteColor> > image) {
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
         // irfanview in windows seems to want gbr ordering
#ifdef _WIN32
         fputc(image[x][y].g, outputFile);
         fputc(image[x][y].b, outputFile);
         fputc(image[x][y].r, outputFile);
#else
         fputc(image[x][y].r, outputFile);
         fputc(image[x][y].g, outputFile);
         fputc(image[x][y].b, outputFile);
#endif
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
