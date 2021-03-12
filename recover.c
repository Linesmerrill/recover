#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

//define the blocksize of 512 as each block of memory on the card is 512
#define BLOCKSIZE 512

//create a new typedef called BYTE
typedef uint8_t BYTE;

//my prototypes
bool jpeg(BYTE buffer[]);

int main(int argc, char *argv[])
{
    //check that two arguments are given
    if (argc != 2)
    {
        //if not inform user of correct usage
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    //open the source file given
    FILE *source = fopen(argv[1], "r");

    //if the the source is invalid inform user.
    if (!source)
    {
        printf("Could not open %s.\n", argv[1]);
        return 1;
    }

    //create a filename
    char filename[8];

    //create a destination file
    FILE *destination = NULL;

    //create buffer
    BYTE buffer[BLOCKSIZE];

    //count the number of jpegs
    int counter = 0;

    //loop through all the jpgs until end of file is reached
    while (fread(buffer, sizeof(BYTE), BLOCKSIZE, source) || feof(source) == 0)
    {
        //if it is .jpg
        if (jpeg(buffer))
        {
            //if the jpg was alrady opened, close it.
            if (destination)
            {
                fclose(destination);
            }
            //create a new file named 001.jpg based on count
            sprintf(filename, "%03i.jpg", counter);
            //open the destiation file and set it to write
            destination = fopen(filename, "w");
            //update the counter
            counter++;
        }
        //if the jpg is open
        if (destination)
        {
            // print out the image, one byte at a time
            fwrite(buffer, sizeof(buffer), 1, destination);
        }
    }

    //close both files when complete
    fclose(source);
    fclose(destination);
    return 0;
}

//check if it is a jpg with the given values
bool jpeg(BYTE buffer[])
{
    return  buffer[0] == 0xff
            && buffer[1] == 0xd8
            && buffer[2] == 0xff
            && (buffer[3] & 0xf0) == 0xe0;
}
