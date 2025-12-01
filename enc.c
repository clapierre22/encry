#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int euclid(int a, int b)
{

}

int main(int argc, char** argv)
{
        fprintf(stdout, "Good Build\n");

        int n, p, q, z, e, d;
        int public[], private[];

        if (argc < 3)
        {
                fprintf(stderr, "Usage: ./[exe] [p] [q]\n");
                return 0;
        }

        n = p * q;
        z = (p - 1) * (q - 1);

        // Make e relatively prime to z

        // Mod e*d-1 by z to generate d


        return 0;
}
