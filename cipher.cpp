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

	if (cipherMode == "ENC")
	{
		if (CipherName == "AES")
		{
			key += '0';
		}

		if (!cipher->setKey(key.c_str()))
			exit(1);
	}
	else if (cipherMode == "DEC")
	{
		if (CipherName == "AES")
		{
			key += '1';
		}

		if(!cipher->setKey(key.c_str()))
			exit(1);
	}

	return 0;
}

void usage()
{
	cout << "USAGE: ./cipher <CIPHER NAME> <KEY> <ENC/DEC> <INPUT FILE> <OUTPUT FILE>" << endl;
	cout <<  "DES: For the DES cipher\n AES: For the AES cipher" << endl;
}
