#ifndef TESTDRVIOCTL_H
#define TESTDRVIOCTL_H

#define TESTDRV_MAGIC 'T'

struct Version
{
	unsigned int Major;
	unsigned int Minor;
};


#define GET_DRIVER_VERSION		0x01
#define ZERO_MEMORY				0x02
#define FILL_MEMORY				0x03


#define TESTDRV_GET_DRIVER_VERSION		_IOR(TESTDRV_MAGIC, GET_DRIVER_VERSION, char*)
#define TESTDRV_ZERO_MEMORY				_IO(TESTDRV_MAGIC, ZERO_MEMORY)
#define TESTDRV_FILL_MEMORY				_IOW(TESTDRV_MAGIC, FILL_MEMORY, unsigned char)

#endif /* TESTDRVIOCTL_H */
