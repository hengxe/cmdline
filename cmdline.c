#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "cmdline_api.h"

#define CMD_NUM 8
#define CMD_LEN 128

static char argcmd[CMD_NUM][CMD_LEN] = {{0}};
char * trim(char *ptr);
unsigned int str2num(const char *str);

int cmd_read(int argc, char *argv[])
{
    int offset;
    uint32_t valret = 0;

    if (argc != 2)
        return -1;

    trim(argv[1]);
    offset = str2num(argv[1]);

    print("offset is %d(0x%x)\r\n", offset, offset);

    if (0 != cmdline_register_read(offset, &valret))
    {
        printf("Address 0x%08X: %s\n", offset, "FAILED");
        return -1;
    }

    printf("Address 0x%08X: 0x%08X / %u\n", offset, valret, valret);
    return 0;
}

int cmd_write(int argc, char *argv[])
{
    int offset;
    uint32_t val;

    if (argc != 3)
        return -1;
  
    trim(argv[1]);
    offset = str2num(argv[1]);

    trim(argv[2]);
    val = str2num(argv[2]);
            
    print("offset %d(0x%x), value %d(0x%x)\r\n", offset, offset, val, val);
    printf("Address 0x%08X: set (0x%X / %u) %s\n", offset, val, val,
               (0 == cmdline_register_write(offset, val) ? "OK" : "NOK"));
    return 0;
}

int main(int argc, char *argv[])
{
    int i = 0;
    char tmp[CMD_LEN] = "";
    char tmpcopy[CMD_LEN] = "";
    char *pargs[CMD_NUM] = {NULL};

    memset(argcmd, 0, sizeof(argcmd));
    for (i = 0; i < CMD_NUM; i++)
        pargs[i] = argcmd[i];
    
    while (1) {
        memset(tmp, 0, sizeof(tmp));
        printf("command>:");
        fgets(tmp, CMD_LEN, stdin);
        trim(tmp);
        
        
        if (strlen(tmp) != 0) 
            memcpy(tmpcopy, tmp, sizeof(tmpcopy));
        else
            printf("command>:%s\n", tmpcopy);
        
        sscanf(tmpcopy, "%s", argcmd[0]);

        if (0 == strcmp(argcmd[0], "exit"))
        {
            break;
        }
        else if (0 == strcmp(argcmd[0], "set"))
        {
            sscanf(tmpcopy, "%s%s%s", argcmd[0], argcmd[1], argcmd[2]);
            cmd_write(3, pargs);
        }
        else if (0 == strcmp(argcmd[0], "get"))
        {
            sscanf(tmpcopy, "%*s%s", argcmd[1]);
            cmd_read(2, pargs);
        }
        else
        {
            if (strlen(tmp) == 0) 
                continue;

            strcpy(tmpcopy, " ");
            printf("Usage:\n");
            printf("\tset <addr> <val>\n");
            printf("\tget <addr>\n");
            printf("\texit\n");
        }
    }

    return 0;
}


char * trim_left(char * pstr)
{
    char *p = pstr;
    char *q = pstr;

    if (pstr == NULL)
        return pstr;

    while((*p != '\0') && isspace(*p)) p++;
    while(*p != '\0') *q++ = *p++;

    *q = '\0';

    return pstr;
}

char * trim_right(char * pstr)
{
    char *p = pstr;

    if (pstr == NULL)
        return pstr;

    while(*p != '\0') p++;

    while(p-- != pstr)
    {
        if (!isspace(*p))
        {
            *++p = '\0';
            break;
        }
    }

    return pstr;
}

char * trim(char *ptr) 
{ 
    trim_left(ptr);
    trim_right(ptr);
    return ptr; 
} 

/** 
* 字符串16进制转10进制整数 
* @param str “不包括0x” 
* @return 10进制整数 
*/ 
static unsigned int htoi(const char *str) 
{ 
    unsigned int n = 0; 
    const char* hstr = str; 

    while(*hstr)
    {
        if (!isxdigit(*hstr))
            return 0;
        hstr++;
    }

    hstr = str;
    while (*hstr) 
    { 
        n <<= 4; 
        if (*hstr <= '9') 
            n |= ( *hstr & 0xf ); 
        else 
            n |= ( (*hstr & 0xf) + 9 ); 
        hstr++; 
    } 
    return n; 
} 

unsigned int str2num(const char *str) 
{ 
    int len = strlen(str);
    if (str == NULL)
        return 0;

    if (len == 0)
        return 0;

    if (len > 2)
    {
        if (str[0]=='0' && (str[1]== 'x' || str[1]== 'X')) 
            return htoi(&str[2]); 
        else 
            return atoi(str); 
    }
    else if (len == 2)
    {
        if (str[0]=='0' && (str[1]== 'x' || str[1]== 'X')) 
            return 0; 
        else 
            return atoi(str);
    }
    else
    {
        return atoi(str);
    }
} 

