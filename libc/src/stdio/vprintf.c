#include <stdio.h>
#include <stdarg.h>
#include <string.h>

char *_vprintf_convert(unsigned int num, int base) { 
    static char Representation[]= "0123456789ABCDEF";
    static char buffer[50]; 
    char *ptr; 

    ptr = &buffer[49]; 
    *ptr = '\0'; 

    do { 
        *--ptr = Representation[num%base]; 
        num /= base; 
    } while(num != 0); 

    return(ptr); 
}

int _vprintf_write(const char *buf, int len) {
	int i;
	for (i = 0; i < len; i++)
		putchar(buf[i]);
	return len;
}

int vprintf(const char *format, va_list arg) {	
	const char *traverse; 
    int i; 
    char *s;
	int done = 0;

    for(traverse = format; *traverse != '\0'; traverse++) 
    { 
        while(*traverse != '\0' && *traverse != '%') 
        {
            putchar(*traverse);
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
                        putchar(i);
						done++;
                        break; 

            case 'd' : i = va_arg(arg,int);         //Fetch Decimal/Integer argument
                        if(i<0) 
                        { 
                            i = -i;
                            putchar('-'); 
                        }
						s = _vprintf_convert(i,10);
                        done += _vprintf_write(s, strlen(s));
                        break; 

            case 'o': i = va_arg(arg,unsigned int); //Fetch Octal representation
                        s = _vprintf_convert(i,8);
                        done += _vprintf_write(s, strlen(s));
                        break; 

            case 's': s = va_arg(arg,char *);       //Fetch string
                        done += _vprintf_write(s, strlen(s));
                        break; 

            case 'x': i = va_arg(arg,unsigned int); //Fetch Hexadecimal representation
                        s = _vprintf_convert(i,16);
                        done += _vprintf_write(s, strlen(s));
                        break; 
        }
    }
	
	return done;
}
