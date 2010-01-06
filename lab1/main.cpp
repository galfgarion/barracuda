#include <boost/regex.hpp>
#include <stdio.h>

int gWidth = -1;
int gHeight = -1;

int parse_int(const char *arg);
void parse_args(int argc, const char **argv);

int main(int argc, char **argv) {
   parse_args(argc - 1, (const char **)(argv + 1));

   if(gWidth <= 0 || gHeight <= 0) {
      printf("Usage: %s +W<int> +H<int>\n", argv[0]);
      exit(-1);
   }

   printf("Valid args accepted\n");
   printf("Width: %d\n", gWidth);
   printf("Height: %d\n", gHeight);
   
   return 0;
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
