#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>

#define MSG_MAX 100

// p: 19, q: 7, n: 133, z: 108

/* Global Variables */

uint32_t p, q;
uint64_t n, e, d, z;
char msg[MSG_MAX];

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

uint32_t gen_d(uint32_t i)
{
	uint32_t temp = 1;
	
	while (1)
	{
		temp = temp + z;
		if (temp % i == 0) return (temp / i);
	}

	return 0;
}

void gen_ed(void)
{
	int k = 0;

	for (int i = 2; i < z; i++)
	{
		if (z % i == 0) continue;

		uint32_t temp;

		if (prime(i) && i != p && i != q)
		{
			e = i;
			temp = gen_d(e);

			if (temp > 0)
			{
				d = temp;
				break;
			}
		}
	}
}

void encrypt(char* msg)
{
	int i = 0;
	uint32_t len = strlen(msg);

	//while (i != len)
	//{
		
	//}
}

void decrypt(char* msg)
{
}

int main(int argc, char** argv)
{
	fprintf(stdout, "Good Build\n");

	if (argc < 4)
	{
		fprintf(stderr, "Usage: ./[exe] [p] [q] [msg]\n");
		exit(1);
	}

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

	// gen e, d
	gen_ed();

	fprintf(stdout, "e: %u, d: %u\n", e, d);

	encrypt(msg);

	fprintf(stdout, "Encrypted Msg: %s\n", msg);

	decrypt(msg);

	fprintf(stdout, "Decrypted Msg: %s\n", msg);

	return 0;
}
