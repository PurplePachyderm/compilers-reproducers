#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define N 1000
#define METAREPETS 1000


void saxpy(double alpha, double* x, double* y, double* z) {
	// This function is perfectly valid.

	#pragma omp target data map(to: x[0:N], y[0:N]) map(from: z[0:N])
	{
		#pragma omp target teams distribute parallel for simd collapse(2)
		for (int meta=0; meta<METAREPETS; meta ++)
		{
		for(int i=0; i<N; i++) {
			z[i] = alpha * x[i] + y[i];
		}
		}
	}

}

void simplifiedSaxpy(double alpha, double* x, double* y, double* z) {
	// This function declares a VLA in a target region,
	// which is apparently not supported. However, it
	// seems the compiler is able to simplify this into
	// a static array as "n" is declared just before the
	// target region and can be traced back to a static
	// expression. Thus, no VLA is needed.

	int n = N;

	#pragma omp target data map(to: x[0:N], y[0:N]) map(from: z[0:N])
	{
		for (int meta=0; meta<METAREPETS; meta ++)
		{
		double intermediate[n];
		#pragma omp target teams distribute parallel for simd
		for(int i=0; i<N; i++) {
			intermediate[i] = alpha * x[i];
			z[i] = intermediate[i] + y[i];
		}
		}
	}

}



void vlaSaxpy(double alpha, double* x, double* y, double* z, int n) {
	// This function declares a VLA in a target region,
	// which is apparently not supported. We would expect
	// compilation to fail because simplifiedSaxpy works,
	// which means the compiler does look for a simplification
	// of the declaration of intermediate. Thus, an error message
	// pointing us to the declaration of intermediate could be
	// thrown. However, the compilation terminates, and this
	// function fails at runtime without any explanation.
	//
	// In previous nvc versions (<23) this seemed to prevent the
	// use of any device kernel in the executable, which was much
	// more problematic.

	#pragma omp target data map(to: x[0:N], y[0:N]) map(from: z[0:N])
	{
		for (int meta=0; meta<METAREPETS; meta ++)
		{
		double intermediate[n];
		#pragma omp target teams distribute parallel for simd
		for(int i=0; i<N; i++) {
			intermediate[i] = alpha * x[i];
			z[i] = intermediate[i] + y[i];
		}
		}
	}

}


int main(int argc, char *argv[])
{
	srand(0);

	if (argc != 2) {
		fprintf(stderr, "Program accepts exactly one additional parameter: \"saxpy\", \"simplifiedSaxpy\" or \"vlaSaxpy\"\n");
		return 1;
	}

	int nDevices = omp_get_num_devices();
	printf("Number of devices: %d\n", nDevices);
	if(nDevices <= 0) {
		fprintf(stderr, "No devices detected, aborting execution\n");
		return 1;
	}

	errno = 0;
	char* funcall = argv[1];


	// Prepare arrays for saxpy
	double * x = (double*) malloc(N * sizeof(double));
	double * y = (double*) malloc(N * sizeof(double));
	double * z = (double*) malloc(N * sizeof(double));

	for(int i=0; i<N; i++) {
		x[i] = rand()%10;
		y[i] = rand()%10;
		z[i] = 0.;
	}
	double alpha = rand()%10;

	printf("Current device = %d\n", omp_get_device_num());
	if(!strcmp(funcall, "saxpy")) {
		printf("Calling valid saxpy\a");
		saxpy(alpha, x, y, z);
	}
	else if(!strcmp(funcall, "simplifiedSaxpy")) {
		printf("Calling valid simplifiedSaxpy\n");
		simplifiedSaxpy(alpha, x, y, z);
	}
	else if(!strcmp(funcall, "vlaSaxpy")) {
		printf("Calling invalid vlaSaxpy\n");
		vlaSaxpy(alpha, x, y, z, N);
	}
	else {
		fprintf(stderr, "Pass either \"saxpy\", \"siplifiedSaxpy\" or \"vlaSaxpy\" as argument\n");
	}

	for(int i=0; i<10; i++) {
		printf("z[%d] = %lf\n", i, z[i]);
	}

	return 0;
}
