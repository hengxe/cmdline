#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>

#define DEV_NAME "/dev/cmdline"

struct io_cmd{
    uint32_t offset;
    uint32_t value;
} __attribute__ ((packed));

#define CMDLINE_MAGIC        0xDB
#define CMDLINE_REG_RD       _IOR(CMDLINE_MAGIC, 0x10, struct io_cmd)
#define CMDLINE_REG_WR       _IOW(CMDLINE_MAGIC, 0x11, struct io_cmd)
#define CMDLINE_CMD_NR       0x22
/*ioctl end*/

#define READ_REGISTER   CMDLINE_REG_RD
#define WRITE_REGISTER  CMDLINE_REG_WR
 
static inline int cmdline_ioctl(uint32_t request, struct io_cmd *cmd)
{
    int res = 0;
    int fd   = 0;   
    
    fd = open(DEV_NAME, O_RDWR);
    if (fd == -1) {
        printf("Open %s failed!\n", DEV_NAME);
        return fd;
    }
 
    res = ioctl(fd, request, cmd);
    close(fd);

    return res;
}

int cmdline_register_read(uint32_t addr, uint32_t *val)
{
    int res = 0;
    struct io_cmd cmd = {
    	.offset = addr,
     	.value = *val
    };
    	
    res = cmdline_ioctl(READ_REGISTER, &cmd);
    *val = cmd.value;
	
    return res;
}

int cmdline_register_write(uint32_t addr, uint32_t val)
{
    struct io_cmd cmd = {
    	.offset = addr,
       	.value = val
    };
    	
    return cmdline_ioctl(WRITE_REGISTER, &cmd);
}


