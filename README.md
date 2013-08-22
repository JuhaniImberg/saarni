saarni
======

A visual identification generator. Generates a .png for each hash.

requires
--------

	libpng 1.6.x
	zlib
	gcc

usage
-----

	random:

		./saarni -n number_of_random

		generates random images

	hex string:

		./saarni -s hexstring

		generates a image based on the hexstring
		hexstring should be 32 characters long, aka 128 bits

	example:

		echo "Hello, world!" | md5sum | ./saarni -s -

		returns:

			F0EF7081E1539AC00EF5B761B4FB01B3
			██    ████    ██
			    ██    ██    
			██████    ██████
			██  ████████  ██
			██  ██    ██  ██
			████        ████
			    ████████    
			██  ██    ██  ██
			         #8b3b3a