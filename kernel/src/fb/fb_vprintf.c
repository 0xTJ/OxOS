#include "fbio.h"
#include <stddef.h>
#include <string.h>

char* convert(unsigned int, int);

int fb_vprintf(const char* format, va_list arg) {	
	const char *traverse; 
    int i; 
    char *s;
	int done = 0;

    for(traverse = format; *traverse != '\0'; traverse++) 
    { 
        while(*traverse != '\0' && *traverse != '%') 
        {
            fb_putchar(*traverse);
			done++;
            traverse++; 
        }
		
		if (*traverse == '\0')
			break;

        traverse++;

        //Module 2: Fetching and executing arguments
        switch(*traverse) 
        { 
            case 'c' : i = va_arg(arg,int);     //Fetch char argument
                        fb_putchar(i);
						done++;
                        break; 

            case 'd' : i = va_arg(arg,int);         //Fetch Decimal/Integer argument
                        if(i<0) 
                        { 
                            i = -i;
                            fb_putchar('-'); 
                        }
						s = convert(i,10);
						while (*s) {
							fb_putchar(*s);
							done ++;
							s++;
						}
                        break; 

            case 'o': i = va_arg(arg,unsigned int); //Fetch Octal representation
                        s = convert(i,8);
						while (*s) {
							fb_putchar(*s);
							done ++;
							s++;
						}
                        break; 

            case 's': s = va_arg(arg,char *);       //Fetch string
						while (*s) {
							fb_putchar(*s);
							done ++;
							s++;
						}
                        break; 

            case 'x': i = va_arg(arg,unsigned int); //Fetch Hexadecimal representation
                        s = convert(i,16);
						while (*s) {
							fb_putchar(*s);
							done ++;
							s++;
						}
                        break; 
        }
    }
	
	return done;
}

char *convert(unsigned int num, int base) 
{ 
    static char Representation[]= "0123456789ABCDEF";
    static char buffer[50]; 
    char *ptr; 

    ptr = &buffer[49]; 
    *ptr = '\0'; 

    do 
    { 
        *--ptr = Representation[num%base]; 
        num /= base; 
    }while(num != 0); 

    return(ptr); 
}
