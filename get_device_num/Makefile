CC=nvc
CFLAGS=-mp=gpu -L/omp/nvidia/hpc_sdk/Linux_x86_64/23.1/compilers/lib -lpgc -lnvf

get_device_num: get_device_num.c
	$(CC) $(CFLAGS) -o $@ $?
