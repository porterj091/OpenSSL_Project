#include "AES.h"

/**
 * Sets the key to use
 * @param key - the first byte of this represents whether
 * to encrypt or to decrypt. 00 means encrypt and any other
 * value to decrypt.  Then come the bytes of the 128-bit key
 * (should be 16 of them).
 * @return - True if the key is valid and False otherwise
 */
bool AES::setKey(const unsigned char* keyArray)
{

	unsigned char *temp_key;
	// Since we are passing in 2 chars as bytes we will need to double the total chars needed
	// Since it takes 16 we need 32.  33 with null terminated string
	if(strlen((char *) keyArray) == 33){
		if(keyArray[32] == '0'){
			temp_key = makeKey(keyArray);

			// The first byte was a 0, use AES_set_encrypt_key(...)
			if (AES_set_encrypt_key(temp_key, 128, &key) != 0)
			{
				fprintf(stderr, "AES_set_encrypt_key failed!\n");
				return false;
			}
		}
		else{
			temp_key = makeKey(keyArray);
			// The first byte was not 0, use AES_set_decrypt_key(...)
			if (AES_set_decrypt_key(temp_key, 128, &key) != 0)
			{
				fprintf(stderr, "AES_set_decrypt_key failed!\n");
				return false;
			}
		}
	}
	else
	{
		fprintf(stderr, "%s\n", "Key is not right length");
		return false;
	}
	return true;

}

/**
 * Encrypts a plaintext string
 * @param plaintext - the plaintext string
 * @return - the encrypted ciphertext string
 */
unsigned char* AES::encrypt(const unsigned char* plainText)
{
	// Size of block we don't care about null terminated since block will be padded and will always be 16 bytes
	unsigned char *encrypted_output = new unsigned char[16];

	AES_ecb_encrypt(plainText, encrypted_output, &key, AES_ENCRYPT);

	return encrypted_output;
}

/**
 * Decrypts a string of ciphertext
 * @param cipherText - the ciphertext
 * @return - the plaintext
 */
unsigned char* AES::decrypt(const unsigned char* cipherText)
{
	unsigned char *decrypted_output = new unsigned char[16];

	AES_ecb_encrypt(cipherText, decrypted_output, &key, AES_DECRYPT);

	return decrypted_output;
}

unsigned char* AES::makeKey(const unsigned char* keyArray)
{
	unsigned char *temp_key = new unsigned char[16];

	int keyIndex = 0;

	int aesKeyIndex = 0;

	while(aesKeyIndex != 16)
	{
		if((temp_key[aesKeyIndex] = twoCharToHexByte(keyArray + keyIndex)) == 'z')
			return NULL;

		keyIndex += 2;

		++aesKeyIndex;
	}

	return temp_key;
}


/**
 * Converts a character into a hexidecimal integer
 * @param character - the character to convert
 * @return - the converted character, or 'z' on error
 */
unsigned char AES::charToHex(const char& character)
{
	/* Is the first digit 0-9 ? */
	if(character >= '0' && character <= '9')
		/* Convert the character to hex */
		return character - '0';
	/* It the first digit a letter 'a' - 'f'? */
	else if(character >= 'a' && character <= 'f')
		/* Conver the cgaracter to hex */
		return (character - 97) + 10;
	/* Invalid character */
	else return 'z';
}

/**
 * Converts two characters into a hex integers
 * and then inserts the integers into the higher
 * and lower bits of the byte
 * @param twoChars - two charcters representing the
 * the hexidecimal nibbles of the byte.
 * @param twoChars - the two characters
 * @return - the byte containing having the
 * valud of two characters e.g. string "ab"
 * becomes hexidecimal integer 0xab.
 */
unsigned char AES::twoCharToHexByte(const unsigned char* twoChars)
{
	/* The byte */
	unsigned char singleByte;

	/* The second character */
	unsigned char secondChar;

	/* Convert the first character */
	if((singleByte = charToHex(twoChars[0])) == 'z')
	{
		/* Invalid digit */
		return 'z';
	}

	/* Move the newly inserted nibble from the
	 * lower to upper nibble.
	 */
	singleByte = (singleByte << 4);

	/* Conver the second character */
	if((secondChar = charToHex(twoChars[1])) == 'z')
		return 'z';

	/* Insert the second value into the lower nibble */
	singleByte |= secondChar;

	return singleByte;
}
