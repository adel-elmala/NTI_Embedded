#include <stdio.h>

unsigned int risingEdgeCounter(unsigned char PinReading);
unsigned int risingEdgeCounter(unsigned char PinReading)
{
    static unsigned char last = 1;
    static unsigned int counter = 0;
    if (PinReading == 1 && last == 0)
    {
        counter++;
    }
    last = PinReading;

    return counter;
}
void main()
{
    unsigned int i, counter;
    unsigned char Pin_Readings[] = {1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1};
    for (i = 0; i < sizeof(Pin_Readings) / sizeof(char); i++)
    {
        counter = risingEdgeCounter(Pin_Readings[i]);
        printf("i= %d,counter = %d,state =%d\n", i, counter, Pin_Readings[i]);
    }
    printf("counter = %d\n", counter);
}
