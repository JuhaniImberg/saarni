#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <png.h>
#include <zlib.h>
#include <sys/stat.h>

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

void set_c(png_byte *ptr, int on, color C) {

	static color def;
	def.r = 238;
	def.g = 238;
	def.b = 238;

	if(!on) {
		ptr[0] = C.r;
		ptr[1] = C.g;
		ptr[2] = C.b;
	}else{
		ptr[0] = def.r;
		ptr[1] = def.g;
		ptr[2] = def.b;
	}
}

void set_s(png_byte *ptr, int on, color C) {
	int x, y;
 	for(y = 0; y < SCALE; y++) {
		for(x = 0; x < SCALE; x++) {
			set_c(&ptr[(x+y*8*SCALE)*3], on, C);
		}
	}
}

void create_png(char* input) {
	data A[16];
	char in[17];
	char hex[33];
	char pngname[64];
	int i, m;
	long color_long;
	color C;

	char hexdict[17];
	hexdict[0] = 0;

	png_image image;
	png_bytep buffer;

	memset(&image, 0, (sizeof image));
	image.version = PNG_IMAGE_VERSION;
	image.format = PNG_FORMAT_RGB;
	image.opaque = NULL;
	image.width = 8*SCALE;
	image.height = 8*SCALE;
	image.flags = 0;
	image.colormap_entries = 0;

	buffer = malloc(PNG_IMAGE_SIZE(image));

	/* end png */

	in[0] = 0;
	hex[0] = 0;
	pngname[0] = 0;
	strcat(in, input);
	strcat(hexdict, "0123456789ABCDEF");
	hexdict[16] = 0;

	for(i = 0; i < 16; i++) {
		sprintf(hex+2*i, "%c%c", hexdict[in[i]&0xf], hexdict[in[i]>>4&0xf]);
	}
	hex[32] = 0;

	color_long = crc((unsigned char*)in, 16);

	C.r = (color_long & 0x000000ffUL);
	C.g = (color_long & 0x0000ff00UL) >> 8;
	C.b = (color_long & 0x00ff0000UL) >> 16;

	for(i = 0; i < 16; i++) {
		A[i].high = (in[i] >> 4) >> 3;
		A[i].low = (in[i] & 0xf) >> 3;

	}

	printf("%s\n", hex);

	for(i = 0; i < 8; i++) {
#if TEXT_OUT == 1
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
#endif

		m = i*8*SCALE*SCALE;

		set_s(&(buffer[(m+ 0*SCALE )*3]), A[i  ].high, C);
		set_s(&(buffer[(m+ 1*SCALE )*3]), A[i  ].low, C);
		set_s(&(buffer[(m+ 2*SCALE )*3]), A[i+8].high, C);
		set_s(&(buffer[(m+ 3*SCALE )*3]), A[i+8].low, C);
		set_s(&(buffer[(m+ 4*SCALE )*3]), A[i+8].low, C);
		set_s(&(buffer[(m+ 5*SCALE )*3]), A[i+8].high, C);
		set_s(&(buffer[(m+ 6*SCALE )*3]), A[i  ].low, C);
		set_s(&(buffer[(m+ 7*SCALE )*3]), A[i  ].high, C);
	}

#if TEXT_OUT == 1
	printf("         #%x%x%x\n",
		(unsigned int)C.r, (unsigned int)C.g, (unsigned int)C.b
		);
#endif

	sprintf(pngname, "png/%s.png", hex);

	png_image_write_to_file(&image, pngname, 0,
               buffer, 0, NULL);

	free(buffer);

}

void create_hash(char* buffer) {
	unsigned int i;

	buffer[0] = 0;
	for(i = 0; i < 16; i++) {
		buffer[i] = rand() % 255;
	}
	buffer[16] = 0;
}

char hex_to_char(unsigned char c)
{
  if (c >= '0' && c <= '9') return c - '0';
  if (c >= 'A' && c <= 'F') return 10 + c - 'A';
  if (c >= 'a' && c <= 'f') return 10 + c - 'a';
  return -1;
}

void create_antihex(char* buffer, const char* hex) {
	int i;
	printf("%s\n", hex);
	for(i = 0; i < 16; i++) {
		buffer[i] = hex_to_char(hex[i*2]) | (hex_to_char(hex[i*2+1])<<4);
	}
}

void help() {
	printf("usage: saarni [-h] [-s hex_string]\n"
		   "              [-n number_of_random]\n");

}

int main(int argc, const char* argv[]) {
	char buffer[17];
	char stdin_buffer[33];
	char c;
	int num_of_rand, i;
	buffer[0] = 0;
	stdin_buffer[0] = 0;
	srand (time(NULL));
	mkdir("png", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	if(argc>1 && argv[1][0] == '-') {

		switch(argv[1][1]) {
			case 's':
				if(argc==3) {
					if(argv[2][0]== '-') {
						while((c=getchar())!= EOF) {
							strcat(stdin_buffer, &c);
						}
						stdin_buffer[32] = 0;
						create_antihex(buffer, stdin_buffer);
						create_png(buffer);
					}else{
						create_antihex(buffer, argv[2]);
						create_png(buffer);
					}
					return 0;
				}
				printf("Missing string or - as argument 2\n");
				return 0;
			case 'n':
				if(argc==3) {
					num_of_rand = atoi(argv[2]);
					for(i = 0; i < num_of_rand; i++) {
						buffer[0] = 0;
						create_hash(buffer);
						create_png(buffer);
					}
					return 0;
				}
				printf("Missing number as argument 2\n");
				return 0;
		}
	}

	help();
	return 0;
}