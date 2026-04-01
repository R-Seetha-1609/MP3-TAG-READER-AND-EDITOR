/*
NAME : R SEETHA
DESCRIPTION : TO IMPLEMENT MP3 TAG READER AND EDITOR
DATE : 12/11/2025
*/

#include <stdio.h>
#include "view.h"
#include "types.h"
#include <string.h>
#include "edit.h"


int main(int argc,char *argv[])
{   
    //Validating arguments
    if(argc < 2 || ((strcmp(argv[1],"-v") == 0) && argc < 3) || ((strcmp(argv[1],"-e") == 0) && argc < 5))
    {
        //Error message
      printf("ERROR: INVALID ARGUMENTS\n");
      printf("USAGE:\n");
      printf("To View please pass like: ./a.out -v mp3filename\n");
      printf("To Edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
      printf("To get Help pass like: ./a.out --help\n");
     
      return e_failure;

     
    }
    

    
    
    if(check_operation_type(argv) == e_view) //Checks if the operation type is view
    {
        printf("------YOU HAVE SELECTED VIEW OPTION------\n");

        TagView view;  //Creating a structure variable for view option

        if(read_and_validate_view_arg(argv, &view) == e_success)  //Function call for reading and validating view arguments
        {
            printf("Read and validate is successful\n");
            if(view_option(&view) == e_success)  //Function call for performing view operations
            {
                printf("VIEWING SUCCESSFUL!\n");
            }
            else
            {
                printf("VIEWING UNSUCCESSFUL!\n");
                return e_failure;
            }
            file_close(&view);
        }
        else
        {
            printf("Read and validate is unsuccessful\n");
            return e_failure;
        }
    }
   
    else if(check_operation_type(argv) == e_edit)  //Checks if the operation type is Decoding
    {
        printf("------YOU HAVE SELECTED EDIT OPTION------\n");

        TagEdit edit;  //Creating a structure variable for edit option

        if(read_and_validate_edit_args(argv, &edit) == e_success)  //Function call for reading and validating edit arguments
        {
            printf("Read and validate is successful\n");
            if(edit_option(&edit , argv) == e_success)  //Function call for performing edit operations
            {
                printf("EDITING SUCCESSFUL!\n");
            }
            else
            {
                printf("EDITING UNSUCCESSFUL!\n");
                return e_failure;
            }
            
        }
        else
        {
            printf("Read and validate is unsuccessful\n");
            return e_failure;
        }
    }
        

     else if(check_operation_type(argv) == e_help)  //Checks if the operation type is help
    {
        printf("------YOU HAVE SELECTED HELP OPTION------\n");
        
         help_option(); //Function call to perform help operation
            
       
    }

    else
    {
        printf("IT IS UNSUPPORTED\n");
        return e_failure;
    }

    return e_success;
}

//Function definition for checking operation type
OperationType check_operation_type(char *argv[])  
{
    if(strcmp(argv[1],"-v") == 0)  //Checks if the 1st argument is -v or not
    {
        return e_view;
    }
    else if(strcmp(argv[1],"-e") == 0)  //Checks if the 1st argument is -e or not
    {
        return e_edit;
    }
    else if(strcmp(argv[1],"--help") == 0)  //Checks if the 1st argument is --help or not
    {
        return e_help;
    }
    else
    {
        return e_unsupported;
    }

    
}