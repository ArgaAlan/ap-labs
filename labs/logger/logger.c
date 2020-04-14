#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>

#define RESET 0
#define BRIGHT 1
#define DIM 2
#define UNDERLINE 3
#define BLINK 4
#define REVERSE 7
#define HIDDEN 8

#define BLACK 0
#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define MAGENTA 5
#define CYAN 6
#define WHITE 7

void myPrint(char c)
{
    extern long write(int, const char *, unsigned long);
    (void)write(1, &c, 1);
}
/*
void textcolor(int attr, int fg, int bg)
{
    char command[13];

    sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
    printf("%s", command);
}

int infof(const char *format, ...)
{
    textcolor(BRIGHT, BLUE, BLACK);
    va_list arg;

    va_start(arg, format);

    char *string = format;

    for (int i = 0; i < strlen(string); i++)
    {
        if (string[i] != '%')
        {
            myPrint(string[i]);
        }
        else
        {
            i += 1;
            if (string[i] == 'c')
            {
                //imprimir el char
                format = va_arg(arg, const char *);
                if (format != NULL)
                {
                    myPrint(format[0]);
                }
                else
                {
                    return 1;
                }
            }
            else if (string[i] == 's')
            {
                format = va_arg(arg, const char *);
                if (format != NULL)
                {
                    for (int j = 0; j < strlen(format); j++)
                    {
                        myPrint(format[j]);
                    }
                }
                else
                {
                    return 1;
                }
            }
            else if (string[i] == 'd')
            {
                //revisar si todos son numeros
                format = va_arg(arg, int);
                if (format != NULL)
                {
                    for (int j = 0; j < strlen(format); j++)
                    {
                        if (isdigit(format[j]))
                        {
                            myPrint(format[j]);
                        }
                        else
                        {
                            return 1; //si se topa con algo que no es un numero algo fallo en atoi asi que lo tomo como error
                        }
                    }
                }
                else
                {
                    return 1;
                }
            }
            else if (string[i] == 'f')
            {
                format = va_arg(arg, int);
                if (format != NULL)
                {
                    for (int j = 0; j < strlen(format); j++)
                    {
                        if (isdigit(format[j]) || format[j] == '.')
                        {
                            myPrint(format[j]);
                        }
                        else
                        {
                            return 1; //si se topa con algo que no es un numero algo fallo en gcvt asi que lo tomo como error
                        }
                    }
                }
                else
                {
                    return 1;
                }
            }
        }
    }
    va_end(arg);
    myPrint('\n');
    textcolor(RESET, WHITE, BLACK);
    return 0;
}

int warnf(const char *format, ...)
{
    textcolor(BRIGHT, YELLOW, BLACK);
    va_list arg;

    va_start(arg, format);

    char *string = format;

    for (int i = 0; i < strlen(string); i++)
    {
        if (string[i] != '%')
        {
            myPrint(string[i]);
        }
        else
        {
            i += 1;
            if (string[i] == 'c')
            {
                //imprimir el char
                format = va_arg(arg, const char *);
                if (format != NULL)
                {
                    myPrint(format[0]);
                }
                else
                {
                    return 1;
                }
            }
            else if (string[i] == 's')
            {
                format = va_arg(arg, const char *);
                if (format != NULL)
                {
                    for (int j = 0; j < strlen(format); j++)
                    {
                        myPrint(format[j]);
                    }
                }
                else
                {
                    return 1;
                }
            }
            else if (string[i] == 'd')
            {
                //revisar si todos son numeros
                format = va_arg(arg, int);
                if (format != NULL)
                {
                    for (int j = 0; j < strlen(format); j++)
                    {
                        if (isdigit(format[j]))
                        {
                            myPrint(format[j]);
                        }
                        else
                        {
                            return 1; //si se topa con algo que no es un numero algo fallo en atoi asi que lo tomo como error
                        }
                    }
                }
                else
                {
                    return 1;
                }
            }
            else if (string[i] == 'f')
            {
                format = va_arg(arg, int);
                if (format != NULL)
                {
                    for (int j = 0; j < strlen(format); j++)
                    {
                        if (isdigit(format[j]) || format[j] == '.')
                        {
                            myPrint(format[j]);
                        }
                        else
                        {
                            return 1; //si se topa con algo que no es un numero algo fallo en gcvt asi que lo tomo como error
                        }
                    }
                }
                else
                {
                    return 1;
                }
            }
        }
    }
    va_end(arg);
    myPrint('\n');
    textcolor(RESET, WHITE, BLACK);
    return 0;
}

int errorf(const char *format, ...)
{
    textcolor(BRIGHT, RED, BLACK);
    va_list arg;

    va_start(arg, format);

    char *string = format;

    for (int i = 0; i < strlen(string); i++)
    {
        if (string[i] != '%')
        {
            myPrint(string[i]);
        }
        else
        {
            i += 1;
            if (string[i] == 'c')
            {
                //imprimir el char
                format = va_arg(arg, const char *);
                if (format != NULL)
                {
                    myPrint(format[0]);
                }
                else
                {
                    return 1;
                }
            }
            else if (string[i] == 's')
            {
                format = va_arg(arg, const char *);
                if (format != NULL)
                {
                    for (int j = 0; j < strlen(format); j++)
                    {
                        myPrint(format[j]);
                    }
                }
                else
                {
                    return 1;
                }
            }
            else if (string[i] == 'd')
            {
                //revisar si todos son numeros
                format = va_arg(arg, int);
                if (format != NULL)
                {
                    for (int j = 0; j < strlen(format); j++)
                    {
                        if (isdigit(format[j]))
                        {
                            myPrint(format[j]);
                        }
                        else
                        {
                            return 1; //si se topa con algo que no es un numero algo fallo en atoi asi que lo tomo como error
                        }
                    }
                }
                else
                {
                    return 1;
                }
            }
            else if (string[i] == 'f')
            {
                format = va_arg(arg, int);
                if (format != NULL)
                {
                    for (int j = 0; j < strlen(format); j++)
                    {
                        if (isdigit(format[j]) || format[j] == '.')
                        {
                            myPrint(format[j]);
                        }
                        else
                        {
                            return 1; //si se topa con algo que no es un numero algo fallo en gcvt asi que lo tomo como error
                        }
                    }
                }
                else
                {
                    return 1;
                }
            }
        }
    }
    va_end(arg);
    myPrint('\n');
    textcolor(RESET, WHITE, BLACK);
    return 0;
}

int panicf(const char *format, ...)
{
    textcolor(BRIGHT, WHITE, RED);
    va_list arg;

    va_start(arg, format);

    char *string = format;

    for (int i = 0; i < strlen(string); i++)
    {
        if (string[i] != '%')
        {
            myPrint(string[i]);
        }
        else
        {
            i += 1;
            if (string[i] == 'c')
            {
                //imprimir el char
                format = va_arg(arg, const char *);
                if (format != NULL)
                {
                    myPrint(format[0]);
                }
                else
                {
                    return 1;
                }
            }
            else if (string[i] == 's')
            {
                format = va_arg(arg, const char *);
                if (format != NULL)
                {
                    for (int j = 0; j < strlen(format); j++)
                    {
                        myPrint(format[j]);
                    }
                }
                else
                {
                    return 1;
                }
            }
            else if (string[i] == 'd')
            {
                //revisar si todos son numeros
                format = va_arg(arg, int);
                if (format != NULL)
                {
                    for (int j = 0; j < strlen(format); j++)
                    {
                        if (isdigit(format[j]))
                        {
                            myPrint(format[j]);
                        }
                        else
                        {
                            return 1; //si se topa con algo que no es un numero algo fallo en atoi asi que lo tomo como error
                        }
                    }
                }
                else
                {
                    return 1;
                }
            }
            else if (string[i] == 'f')
            {
                format = va_arg(arg, int);
                if (format != NULL)
                {
                    for (int j = 0; j < strlen(format); j++)
                    {
                        if (isdigit(format[j]) || format[j] == '.')
                        {
                            myPrint(format[j]);
                        }
                        else
                        {
                            return 1; //si se topa con algo que no es un numero algo fallo en gcvt asi que lo tomo como error
                        }
                    }
                }
                else
                {
                    return 1;
                }
            }
        }
    }
    va_end(arg);
    myPrint('\n');
    textcolor(RESET, WHITE, BLACK);
    return 0;
}

char *numtoStr(int num)
{
    char *buffer = malloc(sizeof(char[20]));
    sprintf(buffer, "%d", num);
    return buffer;
}

char *floattoStr(float num, int digits)
{
    char *buffer = malloc(sizeof(char[20]));
    gcvt(num, 6, buffer);
    return buffer;
}

char *chartoStr(char c)
{
    char *buffer = malloc(sizeof(char[1]));
    buffer[0] = c;
    return buffer;
}

char *asciitoStr(int c)
{
    char *buffer = malloc(sizeof(char[1]));
    char a = c;
    buffer[0] = a;
    return buffer;
}
*/

void textcolor(int attr, int fg, int bg)
{
    char command[13];

    sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
    printf("%s", command);
}

int infof(const char *format, ...)
{
    textcolor(BRIGHT, BLUE, BLACK);
    va_list arg;

    va_start(arg, format);

    vprintf(format, arg);
    printf("\n");

    va_end(arg);
    textcolor(RESET, WHITE, BLACK);
    return 0;
}

int warnf(const char *format, ...)
{
    textcolor(BRIGHT, YELLOW, BLACK);
    va_list arg;

    va_start(arg, format);

    vprintf(format, arg);
    printf("\n");

    va_end(arg);
    textcolor(RESET, WHITE, BLACK);
    return 0;
}

int errorf(const char *format, ...)
{
    textcolor(BRIGHT, RED, BLACK);
    va_list arg;

    va_start(arg, format);

    vprintf(format, arg);
    printf("\n");

    va_end(arg);
    textcolor(RESET, WHITE, BLACK);
    return 0;
}

int panicf(const char *format, ...)
{
    textcolor(BRIGHT, WHITE, RED);
    va_list arg;

    va_start(arg, format);

    vprintf(format, arg);
    printf("\n");

    va_end(arg);
    textcolor(RESET, WHITE, BLACK);
    return 0;
}
