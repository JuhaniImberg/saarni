#include <stdio.h>
#include <string.h>
#include "saarni.h"

#define CRC24_INIT 0xb704ceL
#define CRC24_POLY 0x1864cfbL

/* crc24 as defined in http://www.ietf.org/rfc/rfc2440.txt 6.1 */
long crc(unsigned char *octets, size_t len) {
	long crc = CRC24_INIT;
	int i;

	while (len--) {
		crc ^= (*octets++) << 16;
		for (i = 0; i < 8; i++) {
			crc <<= 1;
			if (crc & 0x1000000)
				crc ^= CRC24_POLY;
		}
	}
	return crc & 0xffffffL;
}

int main(int argc, const char* argv[]) {

	data A[16];
	char in[17];
	int i;
	long color_long;
	color C;

	in[0] = 0;
	strcat(in, "pr81N4i5huxmt8qd");

	/*
		good ones:
		- S5Xy7hlaKekcmizy
		- 2zmNluLcoHMeYxNU
		- pr81N4i5huxmt8qd pink
	*/

	color_long = crc((unsigned char*)in, 16);

	C.r = (color_long & 0x000000ffUL);
	C.g = (color_long & 0x0000ff00UL) >> 8;
	C.b = (color_long & 0x00ff0000UL) >> 16;

	for(i = 0; i < 16; i++) {
		A[i].high = (in[i] >> 4) >> 3;
		A[i].low = (in[i] & 0xf) >> 3;

	}

	/* this works in konsole (rgb color) */
	printf("%s\033[38;2;%i;%i;%im\n", in, 
		(unsigned int)C.r, (unsigned int)C.g, (unsigned int)C.b
		);

	for(i = 0; i < 8; i++) {
		printf("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n", 
			A[i  ].high P, A[i  ].high P,
			A[i  ].low  P, A[i  ].low  P,
			A[i+8].high P, A[i+8].high P, 
			A[i+8].low  P, A[i+8].low  P,
			A[i+8].low  P, A[i+8].low  P, 
			A[i+8].high P, A[i+8].high P,
			A[i  ].low  P, A[i  ].low  P, 
			A[i  ].high P, A[i  ].high P
		);
	}

	
	printf("         %s#%x%x%x\n", "\033[0m",
		(unsigned int)C.r, (unsigned int)C.g, (unsigned int)C.b
		);

	return 0;

}