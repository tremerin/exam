#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>

int ft_string(char *str)
{
    int leng;

    leng = 0;
    if (!str)
        str = "(null)";
    while (str[leng])
        leng++;
    write(1, str, leng);
    return (leng);
}

int ft_decimal(long int decimal, int len, char *sign)
{
    int leng;

    leng = 0;
    if (decimal < 0)
    {
        write (1, "-", 1);
        leng++;
        decimal = decimal * -1;
    }
    if (decimal >= len)
        leng += ft_decimal(decimal / len, len, sign);
    write(1, &sign[decimal % len], 1);
    leng++;
    return (leng);
}

int ft_hexadecimal(unsigned int hexadecimal, unsigned int len, char *sign)
{
    int leng;

    leng = 0;
    if (hexadecimal >= len)
        leng += ft_hexadecimal(hexadecimal / len, len, sign);
    write(1, &sign[hexadecimal % len], 1);
    leng++;
    return (leng);
}

int ft_printf(const char *format, ...)
{
    int leng;
    va_list(arg);

    va_start(arg, format);
    leng = 0;
    while (*format)
    {
        if (*format == '%')
        {
            format++;
            if (*format == 's')
                leng += ft_string(va_arg(arg, char *));
            else if (*format == 'd')
                leng += ft_decimal(va_arg(arg, int), 10, "0123456789");
            else if (*format == 'x')
                leng += ft_hexadecimal(va_arg(arg, unsigned int), 16, "0123456789abcdef");
            format++;
        }
        else
        {
            write(1, format, 1);
            leng++;
            format++;
        }
    }
    va_end(arg);
    return (leng);
}

int main(void)
{
    //unsigned int num = -444745;
    int num2 = -234111;
    int len = 0;
    len = ft_printf("Hola holita %x % s %d adios!", num2, "Hola caracola", num2);
    printf("\nlen: %d\n", len);
    //en =    printf("Hola holita %x % s %d adios!", num2, "Hola caracola", num2);
    printf("\nlen: %d\n", len);
    return (0);
}
