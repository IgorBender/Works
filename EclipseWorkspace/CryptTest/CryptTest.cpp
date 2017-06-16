/*
 *  CryptTest.cpp
 *
 *  Created on: Oct 4, 2016
 *      Author: igor
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <rpc/des_crypt.h>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;


int main(void)
{
    // Define 8 bytes key.
    char key[8];
    memcpy(key, "Hello World", 8);
    des_setparity(key);

    cout << "Encoding..." << endl;

    /*
     * To encode data the containing buffer should be aligned to 8 bytes.
     * The file needs to padded to required amount of bites.
     */
    ifstream In("test.txt", ios::binary);
    if(!In)
    {
        cout << "Errror open file test.txt" << endl;
        exit(1);
    }
    // Discover real file size
    In.seekg(0, In.end);
    size_t FileSize = In.tellg();
    In.seekg(0, In.beg);

    char Buffer[8];
    ofstream Out("test.bin", ios::binary);
//    stringstream Store;
    // Write real file size to result encoded file to preserve for decoding.
    Out.write(reinterpret_cast<char*>(&FileSize), sizeof(FileSize));
//    Store.write(reinterpret_cast<char*>(&FileSize), sizeof(FileSize));
    // Encode and write encoded data into file with portions of 8 bytes (may be any amount equal to multiplies of 8).
    for(size_t i = 0; i < FileSize / 8 + (FileSize % 8 ? 1 : 0); ++i)
    {
        In.read(Buffer, 8);
        if(!In)
        {
            // If last portion of data is less than 8 bytes padd with zeroes.
            bzero(&Buffer[In.gcount()], 8 - In.gcount());
        }
        int Result = ecb_crypt(key, Buffer, 8, DES_ENCRYPT | DES_SW);
        if(DES_FAILED(Result))
        {
            cout << "Encryption error" << endl;
            exit(1);
        }
        Out.write(Buffer, 8);
//        Store.write(Buffer, 8);
    }
    Out.close();
    In.close();

    cout << "Decoding..." << endl;

    In.open("test.bin", ios::binary);
    if(!In)
    {
        cout << "Errror open file test.bin" << endl;
        exit(1);
    }
    // Read original file size from file.
    In.read(reinterpret_cast<char*>(&FileSize), sizeof(FileSize));

    Out.open("test2.txt", ios::binary);
    for(size_t i = 0; i < FileSize / 8 + (FileSize % 8 ? 1 : 0); ++i)
    {
        In.read(Buffer, 8);
//        Store.read(Buffer, 8);
        if(!In)
        {
            cout << "Corrupted encripted file" << endl;
            exit(1);
        }
        int Result = ecb_crypt(key, Buffer, 8, DES_DECRYPT | DES_SW);
        if(DES_FAILED(Result))
        {
            cout << "Decryption error" << endl;
            exit(1);
        }
        Out.write(Buffer, ((FileSize - Out.tellp()) < 8 ? (FileSize - Out.tellp()) : 8));
    }
    Out.close();
    In.close();

    return 0;
}




