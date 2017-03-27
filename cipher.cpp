#include <string>
#include <fstream>
#include <iostream>

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


	// Handle file operations
	ifstream fIn(inFile, ios_base::in);
	ofstream fOut(outFile, ios_base::out);

	if (!fIn)
	{
		cerr << "ERROR: Could not open file: " << inFile << endl;
		exit(1);
	}

	if (!fOut)
	{
		cerr << "ERROR: Could not open file: " << outFile << endl;
		exit(1);
	}

	string cipher_input;

	while(fIn.good())
	{
		string line;
		fIn >> line;

		cipher_input += line;
	}

	// Since the AES and DES need an unsigned char
	unsigned char* cstr = new unsigned char[cipher_input.length() + 1];
	strcpy((char *)cstr, cipher_input.c_str());

	// Do the same with the key
	unsigned char* c_key = new unsigned char[key.length() + 1];
	strcpy((char *)c_key, key.c_str());

	// Check if we are encyrpting or decrypting
	// Since AES needs two different types of key depending on mode
	// We will place a 0 at end of string for encrytping
	// and a 1 for decrypting
	if (cipherMode == "ENC")
	{
		if (CipherName == "AES")
		{
			key += '0';
		}

		if (!cipher->setKey(c_key))
			exit(1);

		cipher->encrypt(cstr);
	}
	else if (cipherMode == "DEC")
	{
		if (CipherName == "AES")
		{
			key += '1';
		}

		if(!cipher->setKey(c_key))
			exit(1);

		cipher->decrypt(cstr);
	}

	return 0;
}

void usage()
{
	cout << "USAGE: ./cipher <CIPHER NAME> <KEY> <ENC/DEC> <INPUT FILE> <OUTPUT FILE>" << endl;
	cout <<  "DES: For the DES cipher\n AES: For the AES cipher" << endl;
}