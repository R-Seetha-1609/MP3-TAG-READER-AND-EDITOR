#include <stdio.h>
#include "edit.h"
#include "types.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Read and Validate the edit operation arguments
Status read_and_validate_edit_args(char *argv[], TagEdit *edit)
{    if(strstr(argv[3] , ".mp3") != NULL)
    {
      printf("ERROR : The new content is not present!\n");
      return e_failure;
    }

    if (strstr(argv[4] , ".mp3") != NULL) //checks if the original.mp3 argument is present or not
    {
        printf(".mp3 file is present\n");
        edit->mp3_edit_fname = argv[4];  //collecting the .mp3 file name in a structure variable
    }
    else
    {
        printf("ERROR : The .mp3 extension is not proper!\n");
        return e_failure;
    }
    return e_success;
}

// Open both files properly in read and write mode
Status open_files_edit(TagEdit *edit)
{
    edit->fptr_mp3_edit = fopen("duplicate.mp3", "w");  //opens the duplicate file in write mode
    if (edit->fptr_mp3_edit == NULL)  //checks if duplicate file is present or not
    {
        printf("ERROR: Could not create duplicate.mp3\n");
        return e_failure;
    }

    edit->fptr_original = fopen(edit->mp3_edit_fname , "r");  //opens the original file in read mode
    if (edit->fptr_original == NULL)  //checks if original file is present or not
    {
        printf("ERROR: Could not open original.mp3\n");
        fclose(edit->fptr_mp3_edit); //closing the original file
        return e_failure;
    }

    printf("All files are opened successfully!\n");
    return e_success;
}

// Copy first 10-byte header
void copy_mp3_header(FILE *fptr_mp3_edit, FILE *fptr_original)
{
    rewind(fptr_original);
    unsigned char header[10]; //character array to store the header bytes
    fread(header, 10, 1, fptr_original); //reads the 10 header bytes from original file
    fwrite(header, 10, 1, fptr_mp3_edit); //writes the 10 header bytes to duplicate file
}

// To write size in big-endian order
void write_big_endian(FILE *fp, unsigned int num)
{
    unsigned char bytes[4];

    bytes[0] = (num >> 24) & 0xFF;
    bytes[1] = (num >> 16) & 0xFF;
    bytes[2] = (num >> 8) & 0xFF;
    bytes[3] = num & 0xFF;

    fwrite(bytes, 1, 4, fp);
}

//  Read frame size (big-endian)
unsigned int read_big_endian(FILE *fp)
{
    unsigned char bytes[4];
    fread(bytes, 1, 4, fp);
    unsigned int num = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
    return num;
}

// Function definition of the function to perform edit operations
Status edit_option(TagEdit *edit, char *argv[])
{
    if (open_files_edit(edit) != e_success)  //checks if all files are opened or not
    {
        return e_failure;
    }

    copy_mp3_header(edit->fptr_mp3_edit, edit->fptr_original);  //function call to copy the 10 header bytes 

    if (strcmp(argv[2], "-t") == 0)       //checks if the argument passed is '-t'
    {
        edit_operations("TIT2", edit, argv);
    }
    else if (strcmp(argv[2], "-a") == 0)  //checks if the argument passed is '-a'
    {
        edit_operations("TPE1", edit, argv);
    }
    else if (strcmp(argv[2], "-A") == 0)  //checks if the argument passed is '-A'
    {
        edit_operations("TALB", edit, argv);
    }
    else if (strcmp(argv[2], "-m") == 0)  //checks if the argument passed is '-m'
    {
        edit_operations("TCON", edit, argv);
    }
    else if (strcmp(argv[2], "-y") == 0)  //checks if the argument passed is '-y'
    {
        edit_operations("TYER", edit, argv);
    }
    else if (strcmp(argv[2], "-c") == 0)  //checks if the argument passed is '-c'
    {
        edit_operations("COMM", edit, argv);
    }
    else
    {
        printf("Invalid edit option!\n");
        fclose(edit->fptr_original);
        fclose(edit->fptr_mp3_edit);
        return e_failure;
    }

    return e_success;
}

// Edit tag frames safely
void edit_operations(char *ptr, TagEdit *edit, char *argv[])
{
    char arr[5];   //declaring a character array to store tag
    char flags[2]; //declaring a character array to store flag
    unsigned int old_size; 
    char *new_text = argv[3]; //assigning argument-3 to new_text
    unsigned int new_size = strlen(new_text); //finds the length of new_size

    for (int i = 0; i < 6; i++)
    {
        fread(arr, 4, 1, edit->fptr_original);  //reads the tag from original file
        arr[4] = '\0';

        old_size = read_big_endian(edit->fptr_original);  //function call to convert big endien to integer
        fread(flags, 2, 1, edit->fptr_original);  //reads the flag bytes from original file

        if (strcmp(ptr, arr) == 0)
        {
            printf("Editing tag : %s\n", arr);

            fwrite(arr, 4, 1, edit->fptr_mp3_edit);  //writes the tag to duplicate file
            write_big_endian(edit->fptr_mp3_edit, new_size);  //function call to convert the new_size to big endian
            fwrite(flags, 1, 2, edit->fptr_mp3_edit);  //writes the flag bytes to duplicate file
            fwrite(new_text, 1, new_size, edit->fptr_mp3_edit);  //writes the new_text to duplicate file

            // Skip old tag content
            fseek(edit->fptr_original, old_size, SEEK_CUR);
        }
        else
        {
            fwrite(arr, 4, 1, edit->fptr_mp3_edit);   //writes the tag to duplicate file
            write_big_endian(edit->fptr_mp3_edit, old_size); //function call to convert old_size to big endian
            fwrite(flags, 1, 2, edit->fptr_mp3_edit);   //writes the flag bytes to duplicate file

            char buffer[70]; 
            fread(buffer, 1, old_size, edit->fptr_original); //reads the old content from original file
            fwrite(buffer, 1, old_size, edit->fptr_mp3_edit); //writes the old content to duplicate file
           
        }
    }

    // Copy remaining audio data
    char ch;
    
    while (fread(&ch, 1, 1,  edit -> fptr_original) == 1) //reading character by character from the source
    {
        fwrite(&ch, 1, 1, edit -> fptr_mp3_edit); //writing those characters into the destination
    }


    fclose(edit->fptr_mp3_edit); //closing the duplicate file

    fclose(edit->fptr_original); //closing the original file


    remove(edit->mp3_edit_fname);  //to remove original file
    rename("duplicate.mp3", edit->mp3_edit_fname); //to rename the duplicate file as original file

   
}
