#include "SPI_REG.h"
void SPI_MasterInit(void)
{
    /* Set MOSI and SCK output, all others input */
    SPI_DDR = (1 << SPI_DDR_MOSI) | (1 << SPI_DDR_SCK);
    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}
void SPI_SlaveInit(void)
{
    /* Set MISO output, all others input */
    SPI_DDR = (1 << SPI_DDR_MISO);
    /* Enable SPI */
    SPCR = (1 << SPE);
}

void SPI_MasterTransmit(char cData)
{
    /* Start transmission */
    SPDR = cData;
    /* Wait for transmission complete */
    while (!(SPSR & (1 << SPIF)))
        ;
}

char SPI_SlaveReceive(void)
{
    /* Wait for reception complete */
    while (!(SPSR & (1 << SPIF)))
        ;
    /* Return data register */
    return SPDR;
}