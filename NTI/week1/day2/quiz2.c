#include <stdio.h>
#include <stdlib.h>

char *lastLetters(char *string);
unsigned int findLen(char *str)
{
    unsigned int len = 0;
    while (*str)
    {
        len++;
        str++;
    }
    return len;
}
void main()
{
    char s[100] = {0};
    printf("Please Enter string\n");
    if (gets(s) == NULL)
    {
        printf("ERROR\n");
    }

    char *out = lastLetters(s);
    printf("%s", out);
}
char *lastLetters(char *string)
{
    int i = 0;
    char *out = (char *)malloc(sizeof(char) * 5);
    unsigned int len = findLen(string);

    out[i++] = string[len - 1];
    out[i++] = ' ';
    out[i++] = string[len - 2];

    return out;
}