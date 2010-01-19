#define  BOOST_SPIRIT_NO_REGEX_LIB

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <boost/spirit.hpp>
#include <boost/regex.h>
//#include <boost/spirit/actor/assign_actor.hpp>
#include <boost/spirit/core.hpp>
//#include <boost/spirit/iterator/file_iterator.hpp>
using namespace std;
using namespace boost::spirit;

#include "objects.hpp"
#include "getLine.hpp"
#include "parser.hpp"

typedef struct color_t
{
    int r;
    int g;
    int b;
} color_t;

int main( int argc, char** argv )
{
    if (argc != 4)
    {
        printf("Incorrect usage\n");
        return EXIT_FAILURE;
    }

    int width;
    int height;
    char *povFile;
    char *line;
    FILE *fp, *input;
    int x, y;
    int status;
    povFile = argv[3]+2;
    width = atoi(argv[1]+2);
    height = atoi(argv[2]+2);
    fp = fopen("out.tga", "w+");
    input = fopen(povFile, "r");
    if (input == NULL)
    {
        cerr << "No input file\n";
        return EXIT_FAILURE;
    }

    vector<vector<color_t> > image(width, vector<color_t>(height));
    vector<geomObj*> objects;
    for (x = 0; x < width; x++)
    {
        if ((x%20) > 10)
        {
            for (y = 0; y < height; y++)
            {
                if ((y%20) > 10)
                {
                    image[x][y].r = 255;
                    image[x][y].g = 255;
                    image[x][y].b = 255;
                }
                else
                {
                    image[x][y].r = 60;
                    image[x][y].g = 30;
                    image[x][y].b = 120;
                }
            }
        }
        else
        {
            for (y = 0; y < height; y++)
            {
                if ((y%20) <= 10)
                {
                    image[x][y].r = 255;
                    image[x][y].g = 255;
                    image[x][y].b = 255;
                }
                else
                {
                    image[x][y].r = 60;
                    image[x][y].g = 30;
                    image[x][y].b = 120;
                }
            }
        }
    }

    putc(0,fp);
    putc(0,fp);
    putc(2,fp);                         /* uncompressed RGB */
    putc(0,fp); putc(0,fp);
    putc(0,fp); putc(0,fp);
    putc(0,fp);
    putc(0,fp); putc(0,fp);           /* X origin */
    putc(0,fp); putc(0,fp);           /* y origin */
    putc((width & 0x00FF),fp);
    putc((width & 0xFF00) / 256,fp);
    putc((height & 0x00FF),fp);
    putc((height & 0xFF00) / 256,fp);
    putc(24,fp);                        /* 24 bit bitmap */
    putc(0,fp);

    for (x = 0; x < width; x++)
    {
        for (y = 0; y < height; y++)
        {
            putc((int)image[x][y].b, fp);
            putc((int)image[x][y].r, fp);
            putc((int)image[x][y].g, fp);
        }
    }

    //parseObjs(input, povFile);    
    //cout<< objects.size() << endl;
    objects = parseObjs(input, povFile);
    //cout<< objects.size() << endl;
    return EXIT_SUCCESS;
}
