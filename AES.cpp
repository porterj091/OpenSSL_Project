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
	// Check if the array passed is 17 bytes long
	if(strlen((char *) keyArray) == 18){
		if(keyArray[0] == '0'){
			// The first byte was a 0, use AES_set_encrypt_key(...)
			if (AES_set_encrypt_key(keyArray+1, 128, &key) != 0)
			{
				fprintf(stderr, "AES_set_encrypt_key failed!\n");
				return false;
			}
		}
		else{
			// The first byte was not 0, use AES_set_decrypt_key(...)
			if (AES_set_decrypt_key(keyArray+1, 128, &key) != 0)
			{
				fprintf(stderr, "AES_set_decrypt_key failed!\n");
				return false;
			}
		}
	}
	else
	{
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
	unsigned char encrypted_output[16];

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
	unsigned char decrypted_output[16];

	AES_ecb_encrypt(cipherText, decrypted_output, &key, AES_DECRYPT);

	return decrypted_output;
}
