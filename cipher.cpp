#include <string>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "CipherInterface.h"
#include "DES.h"
#include "AES.h"

using namespace std;

void usage();

int main(int argc, char** argv)
{

	if (argc != 6)
	{
		cerr << "Incorrect number of arguments!" << endl;
		usage();
		exit(1);
	}

	// Grab the arguments
	string CipherName(argv[1]);
	string key(argv[2]);
	string cipherMode(argv[3]);
	string inFile(argv[4]);
	string outFile(argv[5]);


	/* Create an instance of the DES cipher */
	CipherInterface* cipher;

	// Find out which cipher
	if (CipherName == "DES")
	{
		cipher = new DES();
	}
	else if (CipherName == "AES")
	{
		cipher = new AES();
	}
	else
	{
		cerr << "ERROR: Incorrect Cipher NAME";
		usage();
		exit(1);
	}

	/* Error checks */
	if(!cipher)
	{
		fprintf(stderr, "ERROR [%s %s %d]: could not allocate memory\n",
		__FILE__, __FUNCTION__, __LINE__);
		exit(-1);
	}

	unsigned char* c_key = new unsigned char[key.length() + 1];
	strcpy((char *)c_key, key.c_str());

	FILE *input, *output;

	if ((input=fopen(argv[4], "rb")) == NULL)
	{
		perror("FILE Input");
		exit(-1);
	}

	if((output=fopen(argv[5], "wb")) == NULL)
	{
		perror("FILE Output");
		exit(-1);
	}

	// The blockSize for DES
	int blockSize = 8;

	// Check if we are encyrpting or decrypting
	// Since AES needs two different types of key depending on mode
	// We will place a 0 at beginning of string for encrytping
	// and a 1 for decrypting
	if (cipherMode == "ENC")
	{
		if (CipherName == "AES")
		{
			key = '0' + key;
			blockSize = 16;
		}

		if (!cipher->setKey(c_key))
            exit(1);

        
		unsigned char* data = new unsigned char[blockSize];
		int num_Bytes = 0;
		while ((num_Bytes = fread(data, sizeof(unsigned char), blockSize, input)) > 0)
		{
			// Pad the block with 0 if less than blocksize
			if (num_Bytes < blockSize)
			{
				for(int i = num_Bytes; i < blockSize; ++i) data[i] = 0;
			}
			fwrite(cipher->encrypt(data), sizeof(unsigned char), blockSize, output);
			memset(data, 0, blockSize);
		}
	}
	else if (cipherMode == "DEC")
	{

        cout << "going to decrypt\n";
		if (CipherName == "AES")
		{
			key = '1' + key;
			blockSize = 16;
		}

		if(!cipher->setKey(c_key))
			exit(1);

		unsigned char* data = new unsigned char[blockSize];
		int num_Bytes = 0;
		while ((num_Bytes = fread(data, sizeof(unsigned char), blockSize, input)) > 0)
		{
			// Pad the block with 0 if less than blocksize
			if (num_Bytes < blockSize)
			{
				for(int i = num_Bytes; i < blockSize; ++i) data[i] = 0;
			}
			fwrite(cipher->decrypt(data), sizeof(unsigned char), blockSize, output);
			memset(data, 0, blockSize);
		}
	}

	return 0;
}

void usage()
{
	cout << "USAGE: ./cipher <CIPHER NAME> <KEY> <ENC/DEC> <INPUT FILE> <OUTPUT FILE>" << endl;
	cout <<  "DES: For the DES cipher\n AES: For the AES cipher" << endl;
}
