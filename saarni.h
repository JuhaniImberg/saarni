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

long crc(unsigned char *octets, size_t len);

#endif