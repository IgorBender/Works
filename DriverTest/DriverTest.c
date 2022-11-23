/*
 *  DriverTest.c
 *
 *  Created on: 2022-11-19
 *  Author: Igor Bender
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include "testdrv-ioctl.h"

int main(int32_t argc, char** argv)
{
    int32_t TestFd = 0;
    int32_t OperationResult = 0;
    char ReadBuffer[8192];
    char WriteBuffer[8192];
    off_t FileOffset;
    char DeviceName[128];
    char ErrStr[128];
    char* BuffPtr;
    struct Version ModuleVersion;

    /*
     * Usage message in case of missing parameter
     */
    if(2 != argc)
    {
        fprintf(stderr, "Usage %s device_number (0 - 3)\n", argv[0]);
        return 1;
    }

    sprintf(DeviceName, "%s%s", "/dev/testdrv", argv[1]);

    TestFd = open(DeviceName, O_RDWR);
    if(0 > TestFd)
    {
        sprintf(ErrStr, "Open %s ", DeviceName);
        perror(ErrStr);
        return 1;
    }

    /*
     * Obtain module version via ioctl.
     */
    OperationResult = ioctl(TestFd, TESTDRV_GET_DRIVER_VERSION, &ModuleVersion);
    if(0 != OperationResult)
    {
        sprintf(ErrStr, "ioctl TESTDRV_GET_DRIVER_VERSION %s ", DeviceName);
        perror(ErrStr);
        close(TestFd);
        return 1;
    }
    printf("Kernel module version : %d.%d\n", ModuleVersion.Major, ModuleVersion.Minor);

    /*
     * Test regular read - write - seek operations.
     */
    OperationResult = read(TestFd, ReadBuffer, sizeof(ReadBuffer));
    if(0 > OperationResult)
    {
        sprintf(ErrStr, "Read %s ", DeviceName);
        perror(ErrStr);
        close(TestFd);
        return 1;
    }
    /*
     * Only 4096 bytes could be read, check it.
     */
    if(4096 != OperationResult)
    {
        fprintf(stderr, "Wrong read amount of data %d.\n", OperationResult);
    }

    FileOffset = lseek(TestFd, SEEK_SET, 0);
    if(0 > FileOffset)
    {
        sprintf(ErrStr, "Seek %s ", DeviceName);
        perror(ErrStr);
        close(TestFd);
        return 1;
    }

    memset(WriteBuffer, 'a', sizeof(WriteBuffer));
    OperationResult = write(TestFd, WriteBuffer, sizeof(WriteBuffer));
    if(0 > OperationResult)
    {
        sprintf(ErrStr, "Write %s ", DeviceName);
        perror(ErrStr);
        close(TestFd);
        return 1;
    }
    /*
     * Only 4096 bytes could be written, check it.
     */
    if(4096 != OperationResult)
    {
        fprintf(stderr, "Wrong write amount of data %d.\n", OperationResult);
    }

    FileOffset = lseek(TestFd, SEEK_SET, 0);
    if(0 > FileOffset)
    {
        sprintf(ErrStr, "Seek %s ", DeviceName);
        perror(ErrStr);
        close(TestFd);
        return 1;
    }
    OperationResult = read(TestFd, ReadBuffer, sizeof(ReadBuffer));
    if(0 > OperationResult)
    {
        sprintf(ErrStr, "Read %s ", DeviceName);
        perror(ErrStr);
        close(TestFd);
        return 1;
    }
    if(0 != memcmp(ReadBuffer, WriteBuffer, OperationResult))
    {
       fprintf(stderr, "Data do not match.\n");
    }

    /*
     * Test ioctl operations.
     */
    OperationResult = ioctl(TestFd, TESTDRV_ZERO_MEMORY);
    if(0 != OperationResult)
    {
        sprintf(ErrStr, "ioctl TESTDRV_ZERO_MEMORY %s ", DeviceName);
        perror(ErrStr);
        close(TestFd);
        return 1;
    }
    FileOffset = lseek(TestFd, SEEK_SET, 0);
    if(0 > FileOffset)
    {
        sprintf(ErrStr, "Seek %s ", DeviceName);
        perror(ErrStr);
        close(TestFd);
        return 1;
    }
    OperationResult = read(TestFd, ReadBuffer, sizeof(ReadBuffer));
    if(0 > OperationResult)
    {
        sprintf(ErrStr, "Read %s ", DeviceName);
        perror(ErrStr);
        close(TestFd);
        return 1;
    }
    memset(WriteBuffer, 0, sizeof(WriteBuffer));
    if(0 != memcmp(ReadBuffer, WriteBuffer, OperationResult))
    {
       fprintf(stderr, "Data do not match.\n");
    }

    OperationResult = ioctl(TestFd, TESTDRV_FILL_MEMORY, 0x55);
    if(0 != OperationResult)
    {
        sprintf(ErrStr, "ioctl TESTDRV_FILL_MEMORY %s ", DeviceName);
        perror(ErrStr);
        close(TestFd);
        return 1;
    }
    FileOffset = lseek(TestFd, SEEK_SET, 0);
    if(0 > FileOffset)
    {
        sprintf(ErrStr, "Seek %s ", DeviceName);
        perror(ErrStr);
        close(TestFd);
        return 1;
    }
    OperationResult = read(TestFd, ReadBuffer, sizeof(ReadBuffer));
    if(0 > OperationResult)
    {
        sprintf(ErrStr, "Read %s ", DeviceName);
        perror(ErrStr);
        close(TestFd);
        return 1;
    }
    memset(WriteBuffer, 0x55, sizeof(WriteBuffer));
    if(0 != memcmp(ReadBuffer, WriteBuffer, OperationResult))
    {
       fprintf(stderr, "Data do not match.\n");
    }

    /*
     * Test mmap operations
     */
    BuffPtr = (char*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_SYNC | MAP_LOCKED, TestFd, 0);
    if(NULL == BuffPtr)
    {
        sprintf(ErrStr, "mmap of %s ", DeviceName);
        perror(ErrStr);
        close(TestFd);
        return 1;
    }
    OperationResult = ioctl(TestFd, TESTDRV_ZERO_MEMORY);
    if(0 != OperationResult)
    {
        sprintf(ErrStr, "ioctl TESTDRV_ZERO_MEMORY %s ", DeviceName);
        perror(ErrStr);
        close(TestFd);
        return 1;
    }
    if(0x00 != *(BuffPtr +4))
    {
        fprintf(stderr, "Wrong memory pattern.");
    }

    memset(BuffPtr, 0xde, 4096);


    FileOffset = lseek(TestFd, SEEK_SET, 0);
    if(0 > FileOffset)
    {
        sprintf(ErrStr, "Seek %s ", DeviceName);
        perror(ErrStr);
        close(TestFd);
        return 1;
    }
    OperationResult = read(TestFd, ReadBuffer, sizeof(ReadBuffer));
    if(0 > OperationResult)
    {
        sprintf(ErrStr, "Read %s ", DeviceName);
        perror(ErrStr);
        close(TestFd);
        return 1;
    }

    munmap(BuffPtr, 4096);

    memset(WriteBuffer, 0xde, 4096);
    if(0 != memcmp(ReadBuffer, WriteBuffer, OperationResult))
    {
       fprintf(stderr, "Data do not match.\n");
    }

    close(TestFd);
    return 0;
}
