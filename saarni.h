#ifndef SAARNI_H
#define SAARNI_H

typedef struct {
	unsigned int low : 4;
	unsigned int high : 4;
} data;

typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} color;

#define P ?" ":"\u2588"
#define SCALE 16
#define TEXT_OUT 1

long crc(unsigned char *octets, size_t len);
void set_c(png_byte *ptr, int on, color C);
void set_s(png_byte *ptr, int on, color C);
void create_png(char* input);
void create_hash(char* buffer);
void create_antihex(char* buffer, const char* hex);

#endif