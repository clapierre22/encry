#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>

#define MSG_MAX 100
#define DEBUG

typedef struct
{
	uint32_t chunk[32];
} uint1024_t;

/* Global Variables */

uint64_t p, q;
uint64_t n, e, d, z;

/*
 * prime(uint32_t integer);
 * Function to Dertermine if a uint32 is prime
 * P: uint32_t x
 * R: 0 if not prime, 1 if prime
 */
int prime(uint64_t x)
{
	int i, j;

	j = sqrt(x);

	for (i = 2; i <= j; i++) if (x % i == 0) return 0;

	return 1;
}

/*
 * Not used
 */
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
uint64_t gen_d(uint64_t gen_e)
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

	uint64_t temp = 1;
	
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

		uint64_t temp;

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

	if (!e || !d)
	{
		fprintf(stderr, "[Error] Could not generate e, d values\n");
		// Exit?
	}
}

/*
 * bem(long b, long ex, long m)
 * Base, Exponent, Mod; Calculates the b^ex % m formula
 * P: uint32_t base, uint32_t exponent, uint32_t modulo
 * R: uint32_t result
 */
uint32_t bem(uint64_t b, uint64_t ex, uint64_t m)
{
	uint64_t r = 1;
	uint64_t i = 1;
	uint64_t base = b;
	b = b % m;

	while (ex > 0)
	{
		if (ex & 1) r = (r * i * b) % m;
		b = (b * i * b) % m;
		ex /= 2;
	}
#ifdef DEBUG
	fprintf(stdout, "base: %u, r: %u, b: %u, ex: %u, m: %u\n",
			base, r, b, ex, m);
#endif
	return r;
}

/*
 * encrypt(char* msg)
 * Encrypts the given msg utilizing the global variables and the bem function
 * P: char* message
 * R: char* cyphertext
 */
uint64_t* encrypt(char* msg)
{ // TODO: Change from char* to uint64_t*
	uint64_t len = strlen(msg);
	uint64_t* c = (uint64_t*)malloc(len * sizeof(uint64_t));

	for (int i = 0; i < len; i++)
	{
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
char* decrypt(uint64_t* msg)
{
	char* m = (char*)malloc(MSG_MAX * sizeof(char));
	int i = 0;

	while (msg[i] != 0)
	{
		m[i] = bem(msg[i], d, n);
		fprintf(stdout, "%c -> %c\n", msg[i], m[i]);
		i++;
	}

	m[i] = '\0';
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

	fprintf(stdout, "Message to encrypt/decrypt: %s (%d)\n", msg, msg);
	// PROBLEM HERE
	// when multiplying two 1024-bit ints, rslt is 2048 bits
	n = p * q;
	z = (p - 1) * (q - 1);

	fprintf(stdout, "p: %u, q: %u, n: %u, z: %u\n", p, q, n, z);

	gen_ed();

	fprintf(stdout, "e: %u, d: %u\n", e, d);

	fprintf(stdout, "Public Key: (%u, %u)\nPrivate Key: (%u, %u)\n",
			n, e, n, d);

	uint64_t* enc = encrypt(msg);

	fprintf(stdout, "\n\nEncrypted Msg: %s (%d)\n\n", enc, enc);

	fflush(stdout);

	char* dec = decrypt(enc);

	fprintf(stdout, "\n\nDecrypted Msg: %s (%d)\n\n", dec, dec);

	return 0;
}
