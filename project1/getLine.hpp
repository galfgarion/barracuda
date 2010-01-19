char *getLine(FILE *infile)
{
    // Default buffer size
    int len = 1024;
    // Individual character read from file
    char input;
    // Line to be output
    char *buf;
    int index = 0;
    
    // Allocate the string
    buf = (char *)malloc(len);
    
    // As long as there are characters to be read in the file
    while ((input = fgetc(infile)) != EOF)
    {
        if (input != '\n')
        {
            // Save the character to the string
            buf[index] = input;
            // Increment string index
            index++;
        }
        
        // If buffer is filled
        if (index == len-1)
        {
            // Double buffer size and reallocate memory for the string
            len = len * 2;
            buf = (char*)realloc(buf, len);
        }
        // If a newline is encountered
        if (input == '\n')
        {
            // Return what has been read so far
            return strdup(buf);
        }
    }
    return NULL;
}
