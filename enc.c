/*
 * Encryption Program for Communications
 * A: Calvin LaPierre
 * LM: 12/1/25
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>

#define RP_MAX 100
#define DEBUG 1

// TODO: Create u1024 version of functions

typedef struct
{
	uint32_t chunk[32];
} uint1024_t;

//uint1024_t from_uint(unsigned int i)
//{
//}

/*
 * Euclidean Algorithim for Finding GCD
 * P: int a, int b; order does not matter for input
 * R: Boolean, 1 == True 0 == False
 */
int euclid(uint64_t a, uint64_t b)
{
	uint64_t large, small;
	int gcd;
	uint64_t rem = 1;
	large = (a > b) ? a : b;
	small = (a < b) ? a : b;

	// PROBLEM: Wrong numbers printed
#ifdef DEBUG
	fprintf(stdout, "large: %u, small: %u\n", large, small);
#endif
	
//	larger_smaller(a , b, *large, *small);

	rem = large % small;
#ifdef DEBUG
	fprintf(stdout, "rem: %u\n", rem);
#endif
	if (rem == 1) return 1;

	if (rem > 1) gcd = euclid(small, rem);
	
//	fprintf(stdout, "gcd: %u\n", gcd);

	if (gcd == 1) return 1;

	return 0;
}

/*
 * Main Loop
 */
int main(int argc, char** argv)
{
	fprintf(stdout, "Good Build\n");

//	int i = euclid(12, 18);

//	fprintf(stdout, "i = %u\n", i);
        uint32_t n, p;
	uint64_t q, z, e, d;
	uint64_t min, max;
//	int public[], private[];

	if (argc < 3)
        {
        	fprintf(stderr, "Usage: ./[exe] [p] [q]\n");
        	return 0;
        }

	// TODO: Check if numbers are prime

	p = atoi(argv[1]);
	q = atoi(argv[2]);

        n = p * q;
        z = (p - 1) * (q - 1);
#ifdef DEBUG
	fprintf(stdout, "p: %u, q: %u, n: %u, z: %u\n", p, q, n, z);
#endif
        // Make e relatively prime to z, e < n
	// What should be lowest i value

	max = (p > q) ? p : q;
	min = (p < q) ? p : q;

	for (uint64_t i = min; i < n; i++)
	{
		if (euclid(i, z))
		{
			e = i;
			break;
		}
	}

	if (!e)
	{
		fprintf(stderr, "Could not find RP num for z: %u\n", z);
		return 0;
	}

        // Mod e*d-1 by z to generate d
	for (uint64_t j = min; j < n; j++)
	{
		if ((e*j) % z == 1)
		{
			d = j;
			break;
		}
	}

	if (!d)
	{
		fprintf(stderr, "Could not find d for z: %u\n", z);
		return 0;
	}

	fprintf(stdout, "Public Key: (%u, %u); Private Key: (%u, %u)\n",
			n, e, n, d);

        return 0;
}
