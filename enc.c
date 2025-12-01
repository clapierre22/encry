/*
 * Encryption Program for Communications
 * A: Calvin LaPierre
 * LM: 12/1/25
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define RP_MAX 100

/*
 * Euclidean Algorithim for Finding GCD
 * P: int a, int b; order does not matter for input
 * R: Boolean, 1 == True 0 == False
 */
int euclid(int a, int b)
{
	int large, small, gcd;
	int rem = 1;
	large = (a > b) ? a : b;
	small = (a < b) ? a : b;

//	fprintf(stdout, "large: %d, small: %d\n", large, small);

	rem = large % small;

//	fprintf(stdout, "rem: %d\n", rem);

	if (rem == 1) return 1;

	if (rem > 1) gcd = euclid(small, rem);
	
//	fprintf(stdout, "gcd: %d\n", gcd);

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

//	fprintf(stdout, "i = %d\n", i);
        int n, p, q, z, e, d;
//	int public[], private[];

	if (argc < 3)
        {
        	fprintf(stderr, "Usage: ./[exe] [p] [q]\n");
        	return 0;
        }

	// TODO: Check if numbers are prime

        n = p * q;
        z = (p - 1) * (q - 1);

        // Make e relatively prime to z, e < n
	for (int i = 0; i < n; i++)
	{
		if (euclid(i, z))
		{
			e = i;
			break;
		}
	}

	if (!e)
	{
		fprintf(stderr, "Could not find RP num for z: %d\n", z);
		return 0;
	}

        // Mod e*d-1 by z to generate d
	for (int j = 0; j < n; j++)
	{
		if ((e*j) % z == 1)
		{
			d = j;
			break;
		}
	}

	if (!d)
	{
		fprintf(stderr, "Could not find d for z: %d\n", z);
		return 0;
	}

	fprintf(stdout, "Public Key: (%d, %d); Private Key: (%d, %d)\n",
			n, e, n, d);

        return 0;
}
