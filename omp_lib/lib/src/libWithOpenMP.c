
#include "libWithOpenMP.h"
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

/*
 *	Simple MMM, using samesize matrix
 *
 */
void matrixMultiply(double* M1, double* M2, double* Mres, int size, int deviceID)
{
	#pragma omp target is_device_ptr(M1, M2, Mres)
	{
#pragma omp teams distribute parallel for
		for(int i = 0; i < size; i++){
			for(int j = 0; j < size; j++){
				Mres[size * i + j] = M1[size * j + i] * M2[size * i + j];
			}
		}
	}
}


/*
 *	Check acces to a specific pointers allocate on the GPU
 */
void checkAcces(void * pointer)
{
	#pragma omp target is_device_ptr(pointer)
	{
		if(pointer == NULL)
		{
			printf("Can't acces to the device pointer: %p, check if the pointer is correctly allocated \n", pointer);
		}
		printf("acces work and adr is: %p \n", pointer);
	}
}


/*
 *	Try to allocate memory on device
 *	Size is set by default of 100 bytes
 */
void * checkAllocDevice(int deviceID)
{
	void * test = omp_target_alloc(100, deviceID);
	if(test == NULL)
	{
		return NULL;
	}
	return test;
}
