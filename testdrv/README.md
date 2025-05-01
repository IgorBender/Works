# testdrv loadable kernel module

**testdrv** driver is developed for demonstration of loadable kernel modules.

The module demonstrates possibilities of working with special character device files.

At initialization the module creates 4 special character files:

```
crw-rw-rw- 1 root root 505, 0 Dec  6 06:47 /dev/testdrv0
crw-rw-rw- 1 root root 505, 1 Dec  6 06:47 /dev/testdrv1
crw-rw-rw- 1 root root 505, 2 Dec  6 06:47 /dev/testdrv2
crw-rw-rw- 1 root root 505, 3 Dec  6 06:47 /dev/testdrv3

```


Each file represents one page of kernel allocates memory (4096 bytes).

Regular file operation possible on special character files supported by the module:

- **open**
- **close**
- **read**
- **write**
- **seek**

In addition **ioctl** and **mmap** are supported.

#### *ioctl* operations

|                                |                                            |
| :----------------------------- | ------------------------------------------ |
| **TESTDRV_GET_DRIVER_VERSION** | Obtain module's version                    |
| **TESTDRV_ZERO_MEMORY**        | Fill kernel memory page by zeros           |
| **TESTDRV_FILL_MEMORY**        | Fill kernel memory page by given character |
