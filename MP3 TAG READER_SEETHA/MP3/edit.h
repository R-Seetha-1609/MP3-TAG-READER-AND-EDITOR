#ifndef EDIT_H
#define EDIT_H

#include "types.h" // Contains user defined types


typedef struct _EditInfo
{
  char *mp3_edit_fname;
  
  FILE *fptr_mp3_edit;
  FILE *fptr_original;

  int content_size_e;

    
} TagEdit;

/* Edit function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Edit args from argv */
Status read_and_validate_edit_args(char *argv[], TagEdit *edit);

/* Perform the edit operations */
Status edit_option(TagEdit *edit , char *argv[]);

/* Get File pointers for i/p and o/p files */
Status open_files_edit(TagEdit *edit);

/*Perform the edit operations*/
void edit_operations(char *ptr, TagEdit *edit, char *argv[]);

/*Copy the mp3 header bytes*/
void copy_mp3_header(FILE *fptr_mp3_edit, FILE *fptr_original);

/*Close the Files*/
void file_close_edit(TagEdit *edit);

void write_big_endian(FILE *fp, unsigned int num);

unsigned int read_big_endian(FILE *fp);

#endif