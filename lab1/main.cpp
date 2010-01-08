#include <boost/regex.hpp>
#include <stdio.h>
#include <vector>

using namespace std;

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

int main(int argc, char **argv) {
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

   //for(int y = 0; y < gHeight; y++

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
      fputs("\n", outputFile);
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
