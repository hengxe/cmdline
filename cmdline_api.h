#ifndef __CMDLINE_H__
#define __CMDLINE_H__

#include <stdint.h>

#define CMDLINE_DEBUG  0

#ifdef CMDLINE_DEBUG
 #define print(format, args...) do{\
        fprintf(stderr, format, ##args);\
 }while(0)
#else
 #define print(format, args...)
#endif

int cmdline_register_read(uint32_t addr, uint32_t *val);
int cmdline_register_write(uint32_t addr, uint32_t val);

#endif
