/*Documentation
Name               : Pratheek M Poojary
Date               : 30/09/2023
Description        : Digital Steganography*/


#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "decode.h"

int main(int argc, char *argv[])
{
    printf("------------------------------------------------------------\n");
    printf("------------------------------------------------------------\n");
    if(!strcmp(argv[1],"-e"))
    {
        if(argc == 4|| argc ==5)
        {
            EncodeInfo encInfo;
            if(read_and_validate_encode_args(argv,&encInfo)==e_success)
                printf("--Read and validation of encoding arguments is successful\n");
            else
                printf("**Read and validation of encoding arguments is unsuccessful**\n");

            if(do_encoding(&encInfo) == e_success)
                printf("--Encoding is successful\n");
            else
                printf("**Encoding is unsuccessful**\n");
        }
        else
        {
            printf("**Insufficient Arguments**\n");
        }
    }
    else if(!strcmp(argv[1],"-d"))
    {
        if(argc == 3 || argc ==4)
        {
            DecodeInfo decInfo;
            if(read_and_validate_decode_args(argv,&decInfo)==e_success)
                printf("--Read and validation of decoding arguments is successful\n");
            else
                printf("**Read and validation of decoding arguments is unsuccessful**\n");

            if(do_decoding(&decInfo) == e_success)
                printf("--Decoding is successful\n");
            else
                printf("**Decoding is unsuccessful**\n");
        }
        else
        {
            printf("**Insufficient Arguments**\n");
        }
    }
    else
        printf("**Invalid CLA**");
    
    printf("------------------------------------------------------------\n");
    printf("------------------------------------------------------------\n");

    return 0;
}