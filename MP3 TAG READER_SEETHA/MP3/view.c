#include <stdio.h>
#include "view.h"
#include "types.h"
#include <string.h>

// Function definition of the function to perform help operation
void help_option()
{   printf("---------------HELP--------------\n");
    printf("Usage:\n");

    printf("To View please pass like: ./a.out -v mp3filename\n");
    printf("To Edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
    printf("To get Help pass like: ./a.out --help\n");
    printf("\n");
    printf("-t ---> Modifies a Title tag\n");
    printf("-a ---> Modifies an Artist tag\n");
    printf("-A ---> Modifies an Album tag\n");
    printf("-m ---> Modifies the Genre tag\n");
    printf("-y ---> Modifies a Year tag\n");
    printf("-c ---> Modifies a Comment tag\n");
}

// Read and Validate the view operation arguments
Status read_and_validate_view_arg(char *argv[], TagView *view) 
{
    if(strstr(argv[2],".mp3") != NULL) //checks if original.mp3 argument is present or not
    {
        printf(".mp3 file is present\n");
        view -> mp3_view_fname= argv[2];  //collecting the .mp3 file name in a structure variable
    }
    else
    {
       printf("ERROR : The .mp3 extension is not proper!\n");
       return e_failure;
    }


    return e_success;
}

// Function definition of the function to perform view operations
Status view_option(TagView *view)
{    
    if(open_files(view) == e_success)  //checks if all files are opened or not
    {
        printf("All files are opened successfully!\n");
    }
    else
    {
        printf("Files are not opened successfully!\n");
        return e_failure;
    }

    if(view_operations(view) == e_success) //checks if view operations is successful or not
    {
        printf("Audio details displayed successfully!\n");
    }
    else
    {
        printf("Displaying of Audio details unsuccessful!\n");
        return e_failure;
    }
    
    return e_success;
}

// Function definition of the function to open files
Status open_files(TagView *view)
{
   
    view -> fptr_mp3_view = fopen(view -> mp3_view_fname,"r");  //opens the mp3 file in read mode
    if(view -> fptr_mp3_view == NULL) //checks if mp3 file is present or not
    {
       
        printf("mp3 file is not present\n");
        return e_failure;
    }
    else
    {  
        printf("mp3 file is present\n");

    }

    return e_success;
}

// Function definition of the function to perform view operations
Status view_operations(TagView *view)
{

    fseek(view -> fptr_mp3_view ,10 ,SEEK_SET);  //makes the file pointer to point after the header bytes
    int i;
    for(i = 0;i < 6;i++)
    {
        fetch_tag(view);  //function call to fetch and display tags

        content_size(view); //function call to calculate content size

        content(view);  //function call to fetch and display content


    }

    return e_success;
}

// Function definition to fetch and display tags
void fetch_tag(TagView *view)
{
    char arr[10]; //declaring an array to store the tags
    fread(arr, 4, 1, view -> fptr_mp3_view);  //reads 4 bytes of tag from original file
    arr[4] = '\0';
   
    printf("%s ------> ",arr); //prints the tag
}

// Function definition to calculate content size 
void content_size(TagView *view)
{   int size = 4;
    int i;
    unsigned char arr[size];
    
    fread(arr, 4, 1, view -> fptr_mp3_view);  //reads 4 bytes of content size from original file

   /*converting big endian to little endian*/
    char *ptr = (char *)arr;
    int start= 0;
    int end = size - 1;
    
    while(start < end)
    { int temp = ptr[start];
      ptr[start] = ptr[end];
      ptr[end] = temp;
      start++;
      end--;

    }
    
   unsigned int num = (arr[0]) | (arr[1] << 8) | (arr[2] << 16) | (arr[3] << 24);  //converting the little endian data to integer

   view -> content_size = num; //storing the content size in a structure variable
   fseek(view -> fptr_mp3_view ,2 ,SEEK_CUR);  //skips the 2 bytes of flag

}

// Function call to fetch and display content
void content(TagView *view)
{   
    
    char arr[ view -> content_size + 1];  //declaring an array to store the content
    fread(arr, view -> content_size  , 1, view -> fptr_mp3_view);  //reads the content from the original file
    arr[view -> content_size] = '\0';
    
    /*prints the content*/
    for(int i = 0;i < view -> content_size;i++)
    {
        printf("%c",arr[i]);
    }
    printf("\n");
    

}

// Function definition to close original file
void file_close(TagView *view)
{
    fclose(view -> fptr_mp3_view);

}