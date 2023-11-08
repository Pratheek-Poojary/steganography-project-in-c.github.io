#include <stdio.h>
#include "decode.h"
#include "types.h"
#include <string.h>

Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo)
{
     // Fill with sample filenames
    decInfo->stego_image_fname=argv[2];
    if(argv[3] == NULL)
        decInfo->output_text_fname="output.txt";
    else
        decInfo->stego_image_fname=argv[3];
    
    if(open_file(decInfo)==e_success)
        printf("--Opening file is successful\n");        
    else
        printf("**Opening file is unsuccessful**\n");  

    char cptr[20];
    strcpy(cptr,decInfo->output_text_fname);
    char *secret_extn=strtok(cptr,".");
    secret_extn=strtok(NULL,".");

    if(!strcmp(secret_extn,"txt"))
        printf("--Output file validation is successful\n");
    else
        printf("**Output file validation is unsuccessful**\n");

    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    printf("--Decoding started:)\n");
    if(decode_magic_string_length(decInfo->fptr_stego_image)==e_success)
        printf("--Decoding magic string length & magic string is successful\n");        
    else
        printf("**Decoding magic string length & magic string is unsuccessful**\n");
    if(decode_secret_file_extn(decInfo->fptr_stego_image,decInfo)==e_success)
        printf("--Decoding secret file extension length,secret data size and secret data is successful\n");        
    else
        printf("**Decoding secret file extension length,secret data size and secret data is unsuccessful**\n");       

    return e_success;
}

Status open_file(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    if (decInfo->fptr_stego_image == NULL)
    {
        return e_failure;
    }
    decInfo->fptr_output_text = fopen(decInfo->output_text_fname, "w");
    if (decInfo->fptr_output_text== NULL)
    {
        return e_failure;
    }
    return e_success;
}

Status decode_magic_string_length(FILE * fptr_src)
{
    fseek(fptr_src,54,SEEK_SET);
    char buffer[32];
    fread(buffer,1,32,fptr_src);
    int len=0;
    for(int i=0;i<32;i++)
    {
        len=len|((buffer[i]&1)<<(31-i));
    }
    
    if(decode_magic_string(fptr_src,len)==e_success)
    return e_success;   
    
}

Status decode_magic_string(FILE *fptr_src,int len)
{
    char buffer[8];
    char mag_str[len];
    char s_char;
    for (int i = 0; i <len; i++)
    {
        s_char=0;
        fread(buffer,1,8,fptr_src);
        for(int j=0;j<8;j++)
        {
            s_char=s_char|((buffer[j]&1)<<(7-j));
        }
       mag_str[i]=s_char;
    }
    
    return e_success;
}

Status decode_secret_file_extn(FILE *fptr_src,DecodeInfo *decInfo)
{
    char buffer[32];
    fread(buffer,1,32,fptr_src);
    int len=0;
    for(int i=0;i<32;i++)
    {
        len=len|((buffer[i]&1)<<(31-i));
    }
    
    if(decode_secret_file_size_and_data(fptr_src,decInfo,len)==e_success)
    return e_success;
}

Status decode_secret_file_size_and_data(FILE * fptr_src,DecodeInfo *decInfo,int len)
{
    fseek(fptr_src,len*8,SEEK_CUR);

    char buffer[32];
    fread(buffer,1,32,fptr_src);
    int len1=0;
    for(int i=0;i<32;i++)
    {
        len1=len1|((buffer[i]&1)<<(31-i));
    }
    
    if(decode_secret_file_data(fptr_src,len1,decInfo)==e_success)
    return e_success;
}

Status decode_secret_file_data(FILE *fptr_src,int len,DecodeInfo *decInfo)
{
    char buffer[8];
    char mag_str[len];
    char s_char;
    for (int i = 0; i <len; i++)
    {
        s_char=0;
        fread(buffer,1,8,fptr_src);
        for(int j=0;j<8;j++)
        {
            s_char=s_char|((buffer[j]&1)<<(7-j));
        }
        fputc(s_char,decInfo->fptr_output_text);
    }
    return e_success;
    
}