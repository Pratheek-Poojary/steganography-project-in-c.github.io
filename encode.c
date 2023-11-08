#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include <string.h>

Status read_and_validate_encode_args(char *argv[],EncodeInfo *encInfo)
{
     // Fill with sample filenames
    encInfo->src_image_fname = argv[2];
    encInfo->secret_fname = argv[3];
    if(argv[4] ==NULL)
        encInfo->stego_image_fname = "output.bmp";  //we can give argv[4] to accept from user as CLA
    else
        encInfo->stego_image_fname=argv[4];

    if(open_files(encInfo)==e_success)
        printf("--Opening file is successful\n");        
    else
        printf("**Opening file is unsuccessful**\n");   

    //check file is BMP or not
    char data[2];
    fread(data,1,2,encInfo->fptr_src_image);
    if(data[0] == 0X42 && data[1] == 0X4d)
        printf("--BMP file validation successful\n");
    else
        printf("**BMP file validation unsuccessful**\n");

    //Check capacity of beautiful.bmp is enough or not
    fseek(encInfo->fptr_src_image,0,SEEK_END);
    int size=ftell(encInfo->fptr_src_image);

    rewind(encInfo->fptr_src_image);

    int s1,s2,s3,total;
    s1=32+strlen(MAGIC_STRING)*8;

    char *cptr=encInfo->secret_fname;
    char *secret_extn=strtok(cptr,".");
    secret_extn=strtok(NULL,".");
    s2=32+strlen(secret_extn)*8;

    total=54+s1+s2+s3;

    if(size>total)
        printf("--Checking capacity is successful\n");
    else
        printf("**Checking capacity is unsuccessful**\n");

    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    printf("--Encoding started :)\n");
    if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image)==e_success)
        printf("--Copying file is successful\n");        
    else
        printf("**Copying file is unsuccessful**\n");

    if(encode_magic_string(MAGIC_STRING,encInfo)==e_success)
        printf("--Encoding length and data of Magic String successful\n");
    else
        printf("**Encoding length and data of Magic String unsuccessful**\n");
        
    if( encode_secret_file_extn("txt",encInfo)==e_success)
        printf("--Encoding secret file extension successful\n");
    else
        printf("**Encoding secret file extension unsuccessful**\n");

    fseek(encInfo ->fptr_secret, 0 ,SEEK_END);
    encInfo->size_secret_file= ftell(encInfo->fptr_secret);

    if(encode_secret_file_size(encInfo->size_secret_file, encInfo)==e_success)
        printf("--Encoding secret file size Successful\n");
    else
        printf("**Encoding secret file size uncuccessful**\n");
       
    if(encode_secret_file_data(encInfo)==e_success)
        printf("--Encoding secret file data successful\n");
    else
        printf("**Encoding secret file data unsuccessful**\n");

    if(copy_remaining_img_data( encInfo->fptr_src_image, encInfo->fptr_stego_image)== e_success)
        printf("--Copying remaining image data successful\n");
    else
        printf("**Copying remaining image data unsuccessful**\n");      

    return e_success;
}

Status open_files(EncodeInfo *encInfo)
{
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    if (encInfo->fptr_src_image == NULL)
    {
        return e_failure;
    }
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    if (encInfo->fptr_secret == NULL)
    {
        return e_failure;
    }
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    if (encInfo->fptr_stego_image == NULL)
    {
        return e_failure;
    }
    return e_success;
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char buffer[54];
    fread(buffer,1,54,fptr_src_image);
    fwrite(buffer,1,54,fptr_dest_image);
    return e_success;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    int len=strlen(magic_string);
    char buffer[32];
    
    fread(buffer,1,32,encInfo->fptr_src_image);
    for(int i=0; i<32; i++)
    {
        buffer[i] = buffer[i] & (~1);
    }

    for(int i=31;i>=0;i--)
    {
        if(len & (1<<i))
        {
            buffer[31-i]= buffer[31-i] | 1;
        }
    }
    fwrite(buffer,1,32,encInfo->fptr_stego_image);

    char s_char;
    for (int i = 0; i < len; i++)
    {
        for (int j = 7; j >= 0; j--)
        {
            fread(&s_char, 1, 1, encInfo->fptr_src_image);
            s_char &= 0xFE;
            if (magic_string[i] & (1 << j))
            {
                s_char = s_char | 1;
            }
            fwrite(&s_char, 1, 1, encInfo->fptr_stego_image);
        }
    }
    return e_success;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    char buffer[32];
    int len=strlen(file_extn);
    fread(buffer,1,32,encInfo->fptr_src_image);
    for(int i=0; i<32; i++)
    {
        buffer[i] = buffer[i] & (~1);
    }
    for(int i=31;i>=0;i--)
    {
        if(len & (1<<i))
        {
            buffer[31-i]= buffer[31-i] | 1;
        }
    }
    fwrite(buffer,1,32,encInfo->fptr_stego_image);

    char s_char;
    for (int i = 0; i < len; i++)
    {
        for (int j = 7; j >= 0; j--)
        {
            fread(&s_char, 1, 1, encInfo->fptr_src_image);
            s_char &= 0xFE;
            if (file_extn[i] & (1 << j))
            {
                s_char = s_char | 1;
            }
            fwrite(&s_char,1, 1, encInfo->fptr_stego_image);
        }
    }
    return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char buffer[32];
    fread(buffer,1,32,encInfo->fptr_src_image);
    for(int i=0; i<32; i++)
    {
        buffer[i] = buffer[i] & (~1);
    }
    
    for(int i=31;i>=0;i--)
    {
        if(file_size & (1<<i))
        {
            buffer[31-i]= buffer[31-i] | 1;
        }
    }
    fwrite(buffer,1,32,encInfo->fptr_stego_image);
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    rewind(encInfo->fptr_secret);
    char buffer[8],ch;
    int len=encInfo->size_secret_file;

    for(int i=0;i<len;i++)
    {
        ch=fgetc(encInfo->fptr_secret);
        fread(buffer, 1, 8, encInfo->fptr_src_image);
        for(int j=7;j>=0;j--)
        {
            buffer[7-j] = buffer[7-j] & (~1);
            if(ch&(1<<j))
            buffer[7-j] = buffer[7-j]  | 1;
        }
        fwrite(buffer, 1, 8, encInfo->fptr_stego_image);
    }
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch[1];
    while (fread(ch,1,1,fptr_src)>0)
    {
        fwrite(ch, 1, 1, fptr_dest);
    }
    return e_success ;
}