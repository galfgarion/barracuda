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
#include <math.h>

#include "vector3.h"
#include "screen.h"
#include "geom.h"
#include "light.h"

#define RECURSION_DEPTH 3

using namespace std;

typedef struct image_t {
} Image;

// globals
int gPixelWidth = 128;
int gPixelHeight = 96;
string gInputFileName ("");

bool shadowsOff = false;
#ifndef EPSILON
#define EPSILON 0.00001;
#endif

int parse_int(const char *arg);
void parse_args(int argc, const char **argv);
void draw_image(vector< vector<Color> > image);
void parse_file(vector<GeomObject*> & objects, vector<Light*> & lights, Camera * camera);
Color raycast(const Ray & ray, const vector<GeomObject*>& objects, const vector<Light*>& lights, int depth);


// incoming ray, vector n on the surface at point p where it hit
// assume d, n are normalized
Vector3 reflect(Vector3 d, Vector3 n) {
   //n = n.normalize();
   if(d * n > 0) {// normal is flipped
      n = -1 * n;
   }
   return (d - (2.0 * (d*n) * n));
}

Vector3 refract(Vector3 d, Vector3 n, double iorIn, double iorOut, bool * tir) {
   double iorRatio = iorIn / iorOut;

   if(d * n > 0) { // ray inside object
     iorRatio = 1.0 / iorRatio;
     n = -1 * n;
   }

   double iorRatioSquared = iorRatio * iorRatio;

   double underRoot = 1 - (iorRatioSquared * (1 - (d * n)*(d * n)));
   // total internal reflection
   if(underRoot < 0) {
      *tir = true;
      return reflect(d, n);
   }


   return (iorRatio * (d - n*(d * n)) - n*sqrt(underRoot)).normalize();
}



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
   vector< vector<Color> > image(gPixelWidth, vector<Color>(gPixelHeight));
   
   /* clear the colors */
   for(int x = 0; x < gPixelWidth; x++) {
      for(int y=0; y < gPixelHeight; y++) {
         image[x][y].r = 0;
         image[x][y].g = 0;
         image[x][y].b = 0;
      }
   }

   vector<GeomObject*> objects;
   vector<Light*> lights;
   Camera camera;
   
   parse_file(objects, lights, &camera);

   Screen screen = Screen(gPixelWidth, gPixelHeight, camera.eye, -camera.right.magnitude() / 2.0,
      camera.right.magnitude() / 2.0, camera.up.magnitude() / 2.0, -camera.up.magnitude() / 2.0);

   /* clear the colors */
   for(int x = 0; x < gPixelWidth; x++) {
      for(int y = 0; y < gPixelHeight; y++) {

         Ray ray;
         ray.origin = Vector3(0, 0, 1);

         //cout << "ray.origin: <" << ray.origin.x << "," << ray.origin.y << "," << ray.origin.z << ">" << endl;
         ray.direction = (*screen.pixelToScreen(x, y) - ray.origin).normalize();
         //ray.direction = (ray.origin - camera.eye);
         //
         ray.origin = camera.transform() * ray.direction;

         image[x][y] = raycast(ray, objects, lights, RECURSION_DEPTH);
      }
   }

   draw_image(image);
   
   return 0;
}

Color raycast(const Ray & ray, const vector<GeomObject*>& objects, const vector<Light*>& lights, int recursionDepth) {

   double closest = numeric_limits<double>::max();
   double distance = 0;
   GeomObject * closestObj = NULL;

   for(unsigned int i=0; i < objects.size(); i++) {
      distance = objects[i]->intersect(ray);

      if(distance > EPSILON && distance < closest) {
         closest = distance;
         closestObj = objects[i];
      }
   }

   if(closestObj != NULL) {
      distance = closest;
      Point p = ray.origin + (ray.direction * distance); // point on object
      Vector3 n = closestObj->surfaceNormal(p); // surface normal

      //if(ray.direction * n > 0) { // normal is flipped
      //   n = -1.0 * n; // reverse normal
      //}

      Color ambient = closestObj->color * closestObj->finish.ambient;
      Color localShading = ambient;
      Color reflectionShading = Color(0, 0, 0);
      Color refractionShading = Color(0, 0, 0);
      bool tir = false;

      for(unsigned int l=0; l < lights.size(); l++) {
         bool inShadow = false;
         // check shadows
         if(!shadowsOff) {
            for(unsigned int j=0; j < objects.size(); j++) {
               Ray shadowFeeler;
               shadowFeeler.origin = p;
               shadowFeeler.direction = (lights[l]->location - p).normalize();
               double feelerDistance = objects[j]->intersect(shadowFeeler);
               double lightDistance = (lights[l]->location
                 - shadowFeeler.origin).magnitude();
               if(objects[j] != closestObj && feelerDistance > EPSILON && feelerDistance < lightDistance) {
                  inShadow = true;
               }
            }
         }

         if(shadowsOff || !inShadow) {
            // if hitting the back of something, reverse the normal
            Vector3 N; 
            if(ray.direction * n > EPSILON) {
               N = -1 * n;
            } else {
               N = Vector3(n);
            }

            // diffuse
            Vector3 L = (lights[l]->location - p).normalize();
            double nDotL = max(0.0, N * L);
            Color diffuse = closestObj->color * nDotL * closestObj->finish.diffuse; 
            localShading = localShading + diffuse;

            //specular
            Vector3 V = (-1 * ray.direction).normalize();
            Vector3 h = (L + V).normalize();

            double specFactor = closestObj->finish.specular;
            assert(specFactor >= 0);
            double roughness = closestObj->finish.roughness;
            specFactor = max(0.0, specFactor * pow((N * h), 1 / roughness));
            Color specular = lights[l]->color * specFactor;
            localShading = localShading + specular;
         }

         if(closestObj->finish.refraction == 1 && recursionDepth > 0) {
            Ray refractray;
            refractray.origin = p;
            refractray.direction = refract(ray.direction, n, 1, closestObj->finish.ior, &tir);

            // TODO testing HACK
            //refractray.direction = ray.direction;

            if(tir) { //TIR
               // HACK PURPLE
               //refractionShading = Color(138.0/255, 43.0/255, 226.0/255);
               refractionShading = Color(0, 0, 0);
            } else {
               refractionShading = raycast(refractray, objects, lights, recursionDepth - 1);
            }
         }

         if((closestObj->finish.reflection != 0 || 
            (closestObj->finish.refraction == 1 && ray.direction * n <= EPSILON))
            && recursionDepth > 0) {

            Ray reflectray;
            reflectray.origin = p;
            reflectray.direction = reflect(ray.direction, n);

            reflectionShading = raycast(reflectray, objects, lights, 
               recursionDepth - 1);
         }

      }

      double filter = min(1.0, max(0.0, closestObj->finish.filter));
      double reflect = min(1.0, max(0.0, closestObj->finish.reflection));

      Color totalShading;


      if(closestObj->finish.refraction == 1 && !tir) {
         double local = max(0.0, 1 - filter - reflect);
         assert(local >= 0);
         totalShading = localShading * local + reflectionShading * reflect
            + refractionShading * filter;
      } else {
         double local = 1 - reflect;
         totalShading = localShading * local + reflectionShading * reflect;
      }

      return totalShading;

   }

   // no objects hit, return black
   //return Color(0, 0, 0);
   // TODO HACK debug ORANGE
   //return Color(255.0/255, 69.0/255, 0);
   return Color(0, 0, 0);
}

void parse_file(vector<GeomObject*> & objects, vector<Light*> & lights, Camera * camera) {
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
      } else if(!tokens.front().compare("light_source")) {
         cout << "parsing light source" << endl;
         lights.push_back(new Light(tokens));
      } else if(!tokens.front().compare("triangle")) {
         cout << "parsing triangle" << endl;
         objects.push_back(new Triangle(tokens));
      } else {
         cout << tokens.front();
         cout << endl;
         tokens.pop_front(); 
      }
   }

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

   double maxValue = 0.0;
   // TODO calc the max value
   for(int y=0; y < gPixelHeight; y++) {
      for(int x=0; x < gPixelWidth; x++) {
         Color pixel = image[x][y];
         if(pixel.r > maxValue) maxValue = pixel.r;
         if(pixel.g > maxValue) maxValue = pixel.g;
         if(pixel.b > maxValue) maxValue = pixel.b;
      }
   }

   // TODO scale by the max value if it is greater than 1
   if(maxValue > 1.0) {
      double scaleFactor = 1.0 / maxValue;
      for(int y=0; y < gPixelHeight; y++) {
         for(int x=0; x < gPixelWidth; x++) {
            image[x][y] = image[x][y] * scaleFactor;
         }
      }
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

         Color pixel = image[x][y];
         //scale to fit byte
         pixel.r = pixel.r * 255;
         pixel.g = pixel.g * 255;
         pixel.b = pixel.b * 255;

// irfanview in windows seems to want gbr ordering
#ifdef _WIN32
         fputc((byte) pixel.g, outputFile);
         fputc((byte) pixel.b, outputFile);
         fputc((byte) pixel.r, outputFile);
#else
         fputc((byte) pixel.r, outputFile);
         fputc((byte) pixel.g, outputFile);
         fputc((byte) pixel.b, outputFile);
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
