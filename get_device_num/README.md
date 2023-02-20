# get_device_num 

This reproducer highlihts an issue when trying to call the `omp_get_device_num()` from an OpenMP region. With `nvc` 23.1, running the Makefile produces the following error : 

```
NVC++-S-1000-Call in OpenACC region to procedure 'omp_get_device_num' which has no acc routine information (get_device_num.c: 15)
NVC++/x86-64 Linux 23.1-0: compilation completed with severe errors
```

This would be expected for another function (not declared as `declare target`), but the point of `omp_get_device_num()` is to run on any target device. Currently, it only runs on the host device (see `make CFLAGS=-DNO_TARGET_REGION`).

