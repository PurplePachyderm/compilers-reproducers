#include <stdio.h>
#include <omp.h>


int main(void)
{

	int nDevices = omp_get_num_devices();
	printf("Number of devices: %d\n", nDevices);
	printf("Current device (host): %d\n", omp_get_device_num());
	if(nDevices <= 0) {
		fprintf(stderr, "No devices detected, aborting execution\n");
		return 1;
	}

	#ifndef NO_TARGET_REGION
	{
		#pragma omp target
		{
			printf("Current device (target): %d\n", omp_get_device_num());
		}
	}
	#endif

	return 0;
}
