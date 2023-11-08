#ifndef DECODE_H
#define DECODE_H

#include "types.h" 
 
typedef struct _DecodeInfo
{
    /* Output Image info */
    char *stego_image_fname;                     
    FILE *fptr_stego_image;                      

    /* Output File Info */
    char *output_text_fname;                        
    FILE *fptr_output_text;                         

}DecodeInfo;                

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

Status do_decoding(DecodeInfo *decInfo);

Status open_file(DecodeInfo *decInfo);

Status decode_magic_string_length(FILE *fptr_src);

Status decode_magic_string(FILE *fptr_src,int len);

Status decode_secret_file_extn(FILE *fptr_src,DecodeInfo *decInfo);

Status decode_secret_file_size_and_data(FILE *fptr_src,DecodeInfo *decInfo,int len);

Status decode_secret_file_data(FILE *fptr_src,int len,DecodeInfo *decInfo);

#endif