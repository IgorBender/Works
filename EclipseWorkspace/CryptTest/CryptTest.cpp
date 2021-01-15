/*
 * CryptTest.cpp
 *
 *  Created on: Oct 4, 2016
 *      Author: igor
 */

#include <fstream>
#include <gcrypt.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

using namespace std;

int main(void)
{
    // Iniialize GCRYPT library
    if(!gcry_check_version("1.6.0"))
    {
        return 1;
    }

    /* Disable secure memory.  */
    gcry_control(GCRYCTL_DISABLE_SECMEM, 0);

    /* Tell Libgcrypt that initialization has completed. */
    gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);

    cout << "Encoding..." << endl;

    gcry_cipher_hd_t EncodeCfer;
    gcry_error_t Result =
        gcry_cipher_open(&EncodeCfer, GCRY_CIPHER_ARCFOUR, GCRY_CIPHER_MODE_STREAM, 0);
    if(Result)
    {
        cerr << "GCrypt error " << gcry_strsource(Result) << gcry_strerror(Result) << endl;
    }
    Result = gcry_cipher_setkey(EncodeCfer, "Hello World", 12);
    if(Result)
    {
        cerr << "GCrypt error " << gcry_strsource(Result) << gcry_strerror(Result) << endl;
    }

    ifstream In("test.txt", ios::binary);
    if(!In)
    {
        cout << "Errror open file test.txt" << endl;
        exit(1);
    }
    // Discover real file size
    streamoff FileSize = In.seekg(0, In.end).tellg();
    In.seekg(0, In.beg);

    char InBuffer[64];
    char OutBuffer[sizeof(InBuffer)];
    ofstream Out;
    Out.open("test.bin", ios::binary);

    streamsize ReadCount = 0;
    for(streamoff i = 0;
        i < FileSize / streamoff(sizeof(InBuffer)) + (FileSize % sizeof(InBuffer) ? 1 : 0); ++i)
    {
        In.read(InBuffer, sizeof(InBuffer));
        if(!In)
            ReadCount = In.gcount();
        else
            ReadCount = sizeof(InBuffer);
        Result = gcry_cipher_encrypt(EncodeCfer, OutBuffer, sizeof(OutBuffer), InBuffer, ReadCount);
        if(Result)
        {
            cerr << "GCrypt error " << gcry_strsource(Result) << gcry_strerror(Result) << endl;
        }
        else
            Out.write(OutBuffer, ReadCount);
    }

    Out.close();
    In.close();
    gcry_cipher_close(EncodeCfer);

    cout << "Decoding..." << endl;

    gcry_cipher_hd_t DecodeCfer;
    Result = gcry_cipher_open(&DecodeCfer, GCRY_CIPHER_ARCFOUR, GCRY_CIPHER_MODE_STREAM, 0);
    if(Result)
    {
        cerr << "GCrypt error " << gcry_strsource(Result) << gcry_strerror(Result) << endl;
    }
    Result = gcry_cipher_setkey(DecodeCfer, "Hello World", 12);
    if(Result)
    {
        cerr << "GCrypt error " << gcry_strsource(Result) << gcry_strerror(Result) << endl;
    }

    In.open("test.bin", ios::binary);
    if(!In)
    {
        cout << "Errror open file test.bin" << endl;
        exit(1);
    }
    FileSize = In.seekg(0, In.end).tellg();
    In.seekg(0, In.beg);

    Out.open("test2.txt", ios::binary);
//    ReadCount = 0;
    for(streamoff i = 0;
        i < FileSize / streamoff(sizeof(InBuffer)) + (FileSize % sizeof(InBuffer) ? 1 : 0); ++i)
    {
        In.read(InBuffer, sizeof(InBuffer));
        if(!In)
            ReadCount = In.gcount();
        else
            ReadCount = sizeof(InBuffer);
        Result = gcry_cipher_decrypt(DecodeCfer, OutBuffer, sizeof(OutBuffer), InBuffer, ReadCount);
        if(Result)
        {
            cerr << "GCrypt error " << gcry_strsource(Result) << gcry_strerror(Result) << endl;
        }
        else
            Out.write(OutBuffer, ReadCount);
    }
    Out.close();
    In.close();
    gcry_cipher_close(DecodeCfer);

    return 0;
}
