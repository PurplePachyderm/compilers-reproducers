#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include "./lib/src/libWithOpenMP.h"

#define DEVICE_ID 0

int main (void)
{

	double * p = omp_target_alloc(100, DEVICE_ID);	
	void * test = checkAllocDevice(DEVICE_ID);

#pragma omp target is_device_ptr(p, test)
	{
		printf("%p \n", p);
		printf("%p \n", test);
	}

	checkAcces(p);
	checkAcces(test);

	return 0;
}
