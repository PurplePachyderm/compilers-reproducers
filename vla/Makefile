CC=nvc
CFLAGS=-mp=gpu -L/omp/nvidia/hpc_sdk/Linux_x86_64/23.1/compilers/lib -lpgc -lnvf

vla: vla.c
	$(CC) $(CFLAGS) -o $@ $?
