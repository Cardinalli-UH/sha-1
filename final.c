
/*
 *	Cardinalli, Robert
 *	 ICS 212 Final Pj
 *	 Date: 2019-05-08
 */

#include <stdlib.h>
#include <stdio.h>
#include "printbits.h"

// Array size
#define MAX 1048576

//debug boolean
int debug = 0;

//prototypes
unsigned int readFile(unsigned char[]);

unsigned int calculateBlocks(unsigned int);

void charToInt(unsigned char[], unsigned int[], unsigned int, unsigned int);

void addBitCountToLastBlock(unsigned int[],
		unsigned int, unsigned int);

void computeMessageDigest(unsigned int[], unsigned int);

unsigned int shift(int, unsigned int);

unsigned int f(unsigned int, unsigned int, unsigned int, unsigned int);

unsigned int K(unsigned int);

//main function
int main() {

	unsigned char buffer[MAX];
	unsigned int message[MAX];
	unsigned int sizeOfFileInBytes = readFile(buffer);
	unsigned int blockCount;
	unsigned int B, C, D;

	printf("\nblock count:%i\n\n", calculateBlocks(sizeOfFileInBytes));


	blockCount = calculateBlocks(sizeOfFileInBytes);

	charToInt(buffer, message, sizeOfFileInBytes, blockCount);

	addBitCountToLastBlock(message, sizeOfFileInBytes, blockCount);

	computeMessageDigest(message, blockCount);

	return 0;
}

//reads file contents and stores it to buffer[] as chars
//returns the size of the array
unsigned int readFile(unsigned char buffer[]) {

	unsigned int count = 0;
	unsigned int i = 0;
	unsigned int c;

	debug == 1 ? printf("From readFile:\n") : puts("");

	c = getchar();

	//inserts character 'c' into buffer[i] sequentially
	while (c != EOF) {

		if (i <= 1000000) {
			buffer[i] = c;
			c = getchar();
			debug == 1 ? printf("buffer[%i] = %c\n", i, buffer[i]) : puts("");
			i++;
			count++;
		} else {
			printf("too much inputs: %i\n", i);
			break;
		}
	}

	buffer[i] = 128;
	debug == 1 ? printf("buffer[%i] = %c\n", i, buffer[i]) : puts("");
	return count;
}

//returns the number of 512-bit blocks
unsigned int calculateBlocks(unsigned int sizeOfFileInBytes) {

	unsigned int numBlocks = (((8 * sizeOfFileInBytes) + 1) / 512) + 1;

	if ((((8 * sizeOfFileInBytes) + 1) % 512) > (512 - 64)) {
		numBlocks++;
	}

	return numBlocks;

}

//populates message[] with ints converted from chars in buffer[]
void charToInt(unsigned char buffer[], unsigned int message[],
		unsigned int sizeOfFileInBytes, unsigned int blockCount) {

	unsigned int char1, char2, char3, char4;
	unsigned int i = 0;
	unsigned int j = 0;


	//takes chars from buffer[i, i+1, i+2, i+3] and packs them into message[i] as an int
	while (j <= sizeOfFileInBytes) {

		for (i = 0; i < 16; i++) {


			//look for when j == 0x80; if it does, it should push it to the very end, then change it?

			char1 = buffer[j];
			char2 = buffer[j+1];
			char3 = buffer[j+2];
			char4 = buffer[j+3];

			j += 4;

			message[i] = char1 << 24 | char2 << 16 | char3 << 8 | char4;

		}

		//should set the last index of message to the length of the files in bits
		addBitCountToLastBlock(message, sizeOfFileInBytes, blockCount);


	}
}


//calculates the total size of the file in bits
void addBitCountToLastBlock(unsigned int message[],	unsigned int sizeOfFileInBytes,
		unsigned int blockCount) {


	unsigned int sizeOfFileInBits = sizeOfFileInBytes * 8;
	unsigned int lastBlockLastIndex = (blockCount * 16) - 1;
	message[lastBlockLastIndex] = sizeOfFileInBits;

	debug == 1 ? printf("\nsizeInBytes: %i \nsizeInBits: %i \nlastIndex: %i\n", sizeOfFileInBytes, sizeOfFileInBits, lastBlockLastIndex) : puts("");

}




void computeMessageDigest(unsigned int message[], unsigned int blockCount){

	unsigned int A, B, C, D, E, TEMP;
	unsigned int H[5]= {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};
	unsigned int W[blockCount][80];
	unsigned int t = 0;
	//block iterator
	unsigned int itr = 0;
	unsigned int k = 0;



	printf("\nThe initial hex values of H are:  0:%08X  1:%08X  2:%08X  3:%08X  4:%08X \n", H[0], H[1], H[2], H[3], H[4]);


	for (itr = 0; itr < blockCount; itr++) {

		for (t = 0; t < 80; t++) {

			if (t < 16) {
				W[itr][t] = message[t];
				debug == 1 ? printf("compiling W[0-15] @ t equals: %i; block #%i\n", t, itr+1) : printf(" ");
			} else {
				W[itr][t] = shift(1, W[itr][t-3] ^ W[itr][t-8] ^ W[itr][t-14] ^ W[itr][t-16]);
				debug == 1 ? printf("compiling W[16-79] @ t equals: %i; block #%i\n", t, itr+1) : printf(" ");
			}
		}
		puts("");

		if (blockCount <= 2) {
			for(k = 0+(itr*16); k < 16+(itr*16); k++) {
				printf("k=%i:", k);
				printbits(message[k]);
			}
		}


		A = H[0]; B = H[1]; C = H[2]; D = H[3]; E = H[4];

			for (t = 0; t < 80; t++) {

			TEMP = shift(5,A) + f(t,B,C,D) + E + W[itr][t] + K(t);

			E = D; D = C; C = shift(30, B); B = A; A = TEMP;

			if (blockCount <= 2) {

				printf("t = %02d: %08X %08X %08X %08X %08X\n", t, A, B, C, D, E);
			}

		}

		H[0] = H[0] + A;
		H[1] = H[1] + B;
		H[2] = H[2] + C;
		H[3] = H[3] + D;
		H[4] = H[4] + E;

		if (blockCount <= 2) {
			printf("block#%i message digest: %08X %08X %08X %08X %08X\n", itr+1, H[0], H[1], H[2], H[3], H[4]);
		}
	}

    printf("final message digest: %08X %08X %08X %08X %08X\n", H[0], H[1], H[2], H[3], H[4]);

}

unsigned int shift(int n, unsigned int X) {
   unsigned int shift;
   shift = (X << n) | (X >> 32-n);
   return shift;
}


unsigned int f(unsigned int t, unsigned int B, unsigned int C, unsigned int D){


	unsigned int f = 0;

	if (0 <= t && t <= 79) {
		if (0 <= t && t <= 19) {
			f = (B & C) | (~B & D);
		} else if (20 <= t && t <= 39) {
			f = B ^ C ^ D;
		} else if (40 <= t && t <= 59) {
			f = (B & C) | (B & D) | (C & D);
		} else {
			f = B ^ C ^ D;
		}
	}

	//debug == 1 ? printf("f is: %i\n", f) : puts("");

	return f;

}

unsigned int K(unsigned int t){

	unsigned int K = 0;

	if (0 <= t && t <= 79) {
		if (0 <= t && t <= 19) {
			K = 0x5A827999;
		} else if (20 <= t && t <= 39) {
			K = 0x6ED9EBA1;
		} else if (40 <= t && t <= 59) {
			K = 0x8F1BBCDC;
		} else {
			K = 0xCA62C1D6;
		}
	}

	//debug == 1 ? printf("K is: %i\n", K) : puts("");

	return K;

}
