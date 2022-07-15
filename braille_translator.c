/**
 * Name: Sophie Pallanck
 * Date: 4/26/2021
 * Description: This program reads an input file and translates
 * all alphabetic characters and spaces into Braille letters which
 * are written to an output file.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_CHAR (30)

FILE* in_file;
FILE* out_file;

/*This method will write the Braille letters either horizontally or vertically
 to the output_file.
 
 @param braille_line: an array of int arrays(Braille letters).
 @param length: length of braille_line, number of Braille letters in line.
 
 @return 1 on success, -1 if an error occurs*/
int write_braille_to_file(int **braille_line, int length) {
    int **arr = braille_line;
    int *temp = *arr;
    
    int error;
    //write Braille character to output file
    error = fprintf(out_file, "%d ", temp[0]);
    if (error < 0) {
        printf("Error in writing to output file!");
        return -1;
    }
    fprintf(out_file, "%d  ", temp[3]);
    fprintf(out_file, "\n");
    fprintf(out_file, "%d ", temp[1]);
    fprintf(out_file, "%d ", temp[4]);
    fprintf(out_file, "\n");
    fprintf(out_file, "%d ", temp[2]);
    fprintf(out_file, "%d ", temp[5]);

    //deletes trailing whitespaces if last character is a-z or space
    if (length != 1) {
        fprintf(out_file, "\n");
        fprintf(out_file, "\n");
    }

    return 1;
}

/*This method translates an ASCII char to an array of 6 ints(a Brailler letter):
 0 3
 1 4
 2 5
 
 @param letter: the ASCII char to be translated.
 
 @return an int* that represents a Braille char, or NULL if that char is invalid.*/
int *ASCII_to_braille(char letter) {
    //make the letter lowecaser
    letter = tolower(letter);
     
    //make a dictionary in this method for first a-j letters
    int *dictionary[10]; 
    int static a[6] = {1, 0, 0, 0, 0, 0};
    int static b[6] = {1, 1, 0, 0, 0, 0};
    int static c[6] = {1, 0, 0, 1, 0, 0};
    int static d[6] = {1, 0, 0, 1, 1, 0};
    int static e[6] = {1, 0, 0, 0, 1, 0};
    int static f[6] = {1, 1, 0, 1, 0, 0};
    int static g[6] = {1, 1, 0, 1, 1, 0};
    int static h[6] = {1, 1, 0, 0, 1, 0};
    int static i[6] = {0, 1, 0, 1, 0, 0};
    int static j[6] = {0, 1, 0, 1, 1, 0};
    dictionary[0] = a;
    dictionary[1] = b;
    dictionary[2] = c;
    dictionary[3] = d;
    dictionary[4] = e;
    dictionary[5] = f;
    dictionary[6] = g;
    dictionary[7] = h;
    dictionary[8] = i;
    dictionary[9] = j;
    int static *result;

    // if ascii 97-122 or 32 (space) then it is a valid braille letter
    if ((int) letter == 32) {
        int static space[6] = {0, 0, 0, 0, 0, 0};
        result = space;
        return result;
    } else if ((int) letter >= 97 && (int) letter <= 122) {
        int position = letter - 'a';
        if (position < 10) {
            int *arr = dictionary[position];
            result = arr;
            return result;
        } else if (position < 20) {
            position = position - 10;
            int *arr = dictionary[position];
            int static output[6];
            for (int i = 0; i < 6; i++) {
                if (i == 2) {
                    output[i] = 1;
                } else {
                   output[i] = arr[i];
                }
                
            }
            return output;
        } else if (position < 22) { 
            position = position - 20;
            int *arr = dictionary[position];
            int static output[6];
            for (int i = 0; i < 6; i++) {
                if (i == 2) {
                    output[i] = 1;
                } else if (i == 5) {
                    output[i] = 1;
                } else {
                   output[i] = arr[i];
                }
                
            }
            return output;
        } else if (position == 22) { //this is a w
            int static w[6] = {0, 1, 0, 1, 1, 1};
            return w;
        } else {
            position = position - 21;
            int *arr = dictionary[position];
            int static output[6];
            for (int i = 0; i < 6; i++) {
                if (i == 2) {
                    output[i] = 1;
                } else if (i == 5) {
                    output[i] = 1;
                } else {
                   output[i] = arr[i];
                }
                
            }
            return output;
        }
    } else { //not a valid ascii character
      return NULL;  
    }
    
    return NULL;   
}

/*This method reads from in_file up to MAX_CHAR per call to fread(), then builds an 
 array of Braille letters by calling ASCII_to_braille(). It will write the Braille
 to out_file by calling write_braille_to_file().
 
 @return 1 on success, and -1 if an error occurs.*/
int translate_to_braille() {
    int c;
    int charCount = 0;

    //read file to get num of characters
    while ((c = fgetc(in_file)) != EOF) {
        if (feof(in_file)) {
            printf("Error in reading file!");
            return -1;
        }
        charCount++;
    }
    rewind(in_file);

    //read file char by char and translate to Braille
    while ((c = fgetc(in_file)) != EOF) {
        int *braille = ASCII_to_braille(c);
        if (braille) {
           write_braille_to_file(&braille, charCount); 
        }
        charCount--;
    }
    return 1;
}

/*The program should validate that only one argument was supplied, and that it opens
 a valid file. The in_file will be translated from ASCII to Braille. It will create 
 the output file, and call translate_to_braille().

 The program should output the translation to a file named in_file.txt.output.
 e.g.: in_file: some_text.txt 
       out_file: some_text.txt.output
 
 @return 0 on success, and a 1 on error, exiting.*/
int main(int argc, char **argv) {
    
    // Check if correct number of parameters were passed
    if (argc != 2) {
        printf("Usage: ");
        printf("./braille <src_file_name>\n");
        return 0;
    }
    
    char *file = argv[1];
    FILE *fp;
    fp = fopen(argv[1], "r");
    
    // Check if input file can be found
    if (fp == NULL) {
        printf("Error in opening file!");
        return 0;
    }

    in_file = fp;

    //create output file
    strcat(file, ".output");
    out_file = fopen(file, "a");

    translate_to_braille();

    fclose(in_file);
    fclose(out_file);
    return 0;
}
