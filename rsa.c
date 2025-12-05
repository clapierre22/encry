#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>

#define MSG_MAX 100

/* Global Variables */

uint32_t p, q;
uint64_t n, e, d, z;

/*
 * prime(uint32_t integer);
 * Function to Dertermine if a uint32 is prime
 * P: uint32_t x
 * R: 0 if not prime, 1 if prime
 */
int prime(uint32_t x)
{
	int i, j;

	j = sqrt(x);

	for (i = 2; i <= j; i++) if (x % i == 0) return 0;

	return 1;
}

int euclid(uint64_t a, uint64_t b)
{
	uint64_t large, small;
	int gcd;
	uint64_t rem = 1;
	large = (a > b) ? a : b;
	small = (a < b) ? a : b;

	rem = large % small;

	if (rem == 1) return 1;

	if (rem > 1) gcd = euclid(small, rem);

	if (gcd == 1) return 1;

	return 0;
}

/*
 * gen_d(uint32_t e value);
 * Function that takes the generated e value, and generates a
 * valid d value
 * P: uint32_t gen_e
 * R: uint32_t gen_d
 */
uint32_t gen_d(uint32_t gen_e)
{
	/* Since ed mod z = 1, the generated d value needs to be
	 * at least one greater than z. 
	 * If the temp value is exactly divisible by the generated
	 * e value, then the temp value divided by e will be d.
	 * If not true, then increment the temp value by z until
	 * a valid d value is found
	 * z = ed - 1, ed % z = 1
	 * d = (z + 1) / e, (d + z) % e = 0
	 */

	uint32_t temp = 1;
	
	while (1)
	{
		/* Increment temp by z */
		temp = temp + z;

		/* If temp is divisible by e, return the quotient */
		if (temp % gen_e == 0 && (temp / gen_e) != gen_e) 
			return (temp / gen_e);
	}

	return 0;
}

/*
 * gen_ed(void)
 * Function that generates the e value, initializes the generation
 * of the d value. Both values are global variables
 * P: void
 * R: void
 */
void gen_ed(void)
{
	/*
	 * Since z = ed - 1, ed % z = 1; There needs to be two seperate
	 * values that divide z, so e must be a prime number that is 
	 * less than z, does not divide z exactly
	 * After generating e, generate d
	 */

	int k = 0;

	/* 1 < e < z */
	for (int i = 2; i < z; i++)
	{
		if (i >= n)
		{
			fprintf(stderr, "[Error] e value must be less than n value\n e: %u, n: %u\n", i, n);
			break;
		}
		/* e cannot exactly divide z, need e, d */
		if (z % i == 0) continue;

		uint32_t temp;

		/* e must be prime, not equal to p, q */
		if (prime(i) && i != p && i != q)
		{
			/* Generate d */
			e = i;
			temp = gen_d(e);

			if (temp > 0)
			{
				d = temp;
				break;
			}
		}
	}

	// TODO: Add error handling, specifically if e >= n
}

/*
 * bem(long b, long ex, long m)
 * Base, Exponent, Mod; Calculates the b^ex % m formula
 * P: uint32_t base, uint32_t exponent, uint32_t modulo
 * R: uint32_t result
 */
uint32_t bem(uint32_t b, uint32_t ex, uint32_t m)
{
	uint32_t x = 1, y = b;

	while (ex > 0)
	{
		if (ex % 2 == 1) x = (x * y) % m;
		y = (y * y) % m;
		ex /= 2;
	}
#ifdef DEBUG
	fprintf(stdout, "x: %u, y: %u, m: %u, ex: %u, rslt: %u\n",
			x, y, m, ex, x % m);
#endif
	return x % m;
}

/*
 * encrypt(char* msg)
 * Encrypts the given msg utilizing the global variables and the bem function
 * P: char* message
 * R: char* cyphertext
 */
char* encrypt(char* msg)
{
	uint32_t len = strlen(msg);
	char* c = (char*)malloc(len * sizeof(char));

	for (int i = 0; i < len; i++)
	{
//		if (c[i] == '\0') return c;
		c[i] = bem(msg[i], e, n);
		fprintf(stdout, "%c -> %c\n", msg[i], c[i]);
	}

	c[len] = '\0';

	return c;
}

/*
 * decrypt(char* msg)
 * Decrypts the given cyphertext utilizing the global variables and the bem function
 * P: char* cyphertext
 * R: char* decoded message
 */
char* decrypt(char* msg)
{
	// PROBLEM:
	// Calling this function and printing the rslt shows extra characters that are not present in the encrypted msg
	uint32_t len = strlen(msg);

	char* m = (char*)malloc(len * sizeof(char));

	for (int i = 0; i < len; i++)
	{
//		if (m[i] == '\0') return m;
		m[i] = bem(msg[i], d, n);
		fprintf(stdout, "%c -> %c\n", msg[i], m[i]);
	}

	m[len] = '\0';

	return m;
}

/*
 * Main Loop
 */
int main(int argc, char** argv)
{
	fprintf(stdout, "Good Build\n");

	if (argc < 4)
	{
		fprintf(stderr, "Usage: ./[exe] [p] [q] [msg]\n");
		exit(1);
	}

	char* msg = (char*)malloc(MSG_MAX * sizeof(char));

	p = atoi(argv[1]);
	q = atoi(argv[2]);

	if (!prime(p) || !prime(q))
	{
		fprintf(stderr, "Please enter two prime numbers, you entered: %u, %u\n", p, q);
		exit(1);
	}

	strcpy(msg, argv[3]);

	if (!msg)
	{
		fprintf(stderr, "Please enter a msg\n");
		exit(1);
	}

	n = p * q;
	z = (p - 1) * (q - 1);

	fprintf(stdout, "p: %u, q: %u, n: %u, z: %u\n", p, q, n, z);

	gen_ed();

	fprintf(stdout, "e: %u, d: %u\n", e, d);

	fprintf(stdout, "Public Key: (%u, %u)\nPrivate Key: (%u, %u)\n",
			n, e, n, d);

	char* enc = encrypt(msg);

	fprintf(stdout, "\nEncrypted Msg: %s (%d)\n", enc, enc);

	char* dec = decrypt(enc);

	fprintf(stdout, "\nDecrypted Msg: %s (%d)\n", dec, dec);

	return 0;
}
