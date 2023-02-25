#include "timer.h"
#include "timer_regs.h"
#include "../../LIB/Calcbit.h"

static Timer_Config Config = {TIMER0_NORMAL_MODE, TIMER0_INTR_DISABLE, PRESCALER_1};
// static Timer_Config Config = {TIMER0_CTC_MODE, TIMER0_INTR_DISABLE, PRESCALER_1};
void TIMER0_SetConfig()
{
    // setup TCCR0
    // setup TIMSK
    switch (Config.Mode)
    {
    case TIMER0_NORMAL_MODE:
    {
        // set control reg to select
        clearbit(TCCR0, TCCR0_WGM00);
        clearbit(TCCR0, TCCR0_WGM01);
        if (Config.Enable_Interrupt == TIMER0_INTR_ENABLE)
            // enable interrupt
            setbit(TIMSK, TIMSK_TOIE0);

        break;
    }
    case TIMER0_CTC_MODE:
    {
        // set control reg to select

        clearbit(TCCR0, TCCR0_WGM00);
        setbit(TCCR0, TCCR0_WGM01);
        if (Config.Enable_Interrupt == TIMER0_INTR_ENABLE)
            // enable interrupt
            setbit(TIMSK, TIMSK_OCIE0);

        break;
    }
    default:
        break;
    }
}

void stopTimer0()
{
    set_Timer0_Prescaler(PRESCALER_CLEAR);
}
void set_Timer0_Prescaler(uint16 prescaler)
{
    switch (prescaler)
    {

    case PRESCALER_CLEAR:
    {
        clearbit(TCCR0, TCCR0_CS00);
        clearbit(TCCR0, TCCR0_CS01);
        clearbit(TCCR0, TCCR0_CS02);
        break;
    }

    case PRESCALER_1:
    {
        setbit(TCCR0, TCCR0_CS00);
        clearbit(TCCR0, TCCR0_CS01);
        clearbit(TCCR0, TCCR0_CS02);
        break;
    }

    case PRESCALER_8:
    {
        clearbit(TCCR0, TCCR0_CS00);
        setbit(TCCR0, TCCR0_CS01);
        clearbit(TCCR0, TCCR0_CS02);
        break;
    }

    case PRESCALER_64:
    {
        setbit(TCCR0, TCCR0_CS00);
        setbit(TCCR0, TCCR0_CS01);
        clearbit(TCCR0, TCCR0_CS02);
        break;
    }

    case PRESCALER_256:
    {
        clearbit(TCCR0, TCCR0_CS00);
        clearbit(TCCR0, TCCR0_CS01);
        setbit(TCCR0, TCCR0_CS02);
        break;
    }

    case PRESCALER_1024:
    {
        setbit(TCCR0, TCCR0_CS00);
        clearbit(TCCR0, TCCR0_CS01);
        setbit(TCCR0, TCCR0_CS02);
        break;
    }

    case EXT_CLK_FAILING_EGDE:
    {
        clearbit(TCCR0, TCCR0_CS00);
        setbit(TCCR0, TCCR0_CS01);
        setbit(TCCR0, TCCR0_CS02);
        break;
    }

    case EXT_CLK_RISING_EGDE:
    {
        setbit(TCCR0, TCCR0_CS00);
        setbit(TCCR0, TCCR0_CS01);
        setbit(TCCR0, TCCR0_CS02);
        break;
    }

    default:
        break;
    }
}

void TIMER0_Delay_Milli_Seconds_with_Blocking(uint16 Milli_Seconds)
{
    double tick_time = (double)Config.Prescaler / F_CPU;
    double number_of_counts = (double)Milli_Seconds / (1000 * tick_time);
    double n_overflows = (double)number_of_counts / (TIMER0_MAX_COUNT + 1);
    // double frac = n_overflows - (long)n_overflows;
    // int remaining = frac * number_of_counts;
    // int start_from_at_first = (TIMER0_MAX_COUNT + 1) - remaining;
    if (n_overflows > (double)((int)n_overflows)) // ceil
        n_overflows = ((int)n_overflows) + 1;
    int counts_per_overflow = number_of_counts / n_overflows;
    int initial_value = (TIMER0_MAX_COUNT + 1) - counts_per_overflow;

    switch (Config.Mode)
    {
    case TIMER0_NORMAL_MODE:
    {
        double overFlow_counter = 0;
        // load with initial value
        TCNT0 = (uint8)initial_value;
        while (overFlow_counter < n_overflows)
        {

            set_Timer0_Prescaler(Config.Prescaler); // select clock -> start timer
            while (getbit(TIFR, TIRF_TOV0) == 0)    // poll-on overflow flag
                ;
            stopTimer0();
            setbit(TIFR, TIRF_TOV0);      // clear overflow flag
            TCNT0 = (uint8)initial_value; // init counter reg
            overFlow_counter++;
        }
        break;
    }

    case TIMER0_CTC_MODE:
    {

        double overFlow_counter = 0;
        OCR0 = (uint8)counts_per_overflow; // load the compared on value
        while (overFlow_counter < n_overflows)
        {
            set_Timer0_Prescaler(Config.Prescaler); // select clock -> start timer
            while (getbit(TIFR, TIRF_OCF0) == 0)    // poll-on overflow flag
                ;
            stopTimer0();
            overFlow_counter++;
            setbit(TIFR, TIRF_OCF0);
        }

        break;
    }

    default:
        break;
    }
}

void TIMER0_Delay_MicroSeconds_with_Blocking(uint16 Micro_Seconds)
{
    double tick_time = (double)Config.Prescaler / F_CPU;
    double number_of_counts = (double)Micro_Seconds / (1000000 * tick_time);
    double n_overflows = (double)number_of_counts / TIMER0_MAX_COUNT;
    if (n_overflows > (double)((unsigned int)n_overflows)) // ceil
        n_overflows = ((unsigned int)n_overflows) + 1;
    int counts_per_overflow = number_of_counts / n_overflows;
    int initial_value = (TIMER0_MAX_COUNT + 1) - counts_per_overflow;

    switch (Config.Mode)
    {
    case TIMER0_NORMAL_MODE:
    {
        double overFlow_counter = 0;
        // load with initial value
        TCNT0 = (uint8)initial_value;
        while (overFlow_counter < n_overflows)
        {

            set_Timer0_Prescaler(Config.Prescaler); // select clock -> start timer
            while (getbit(TIFR, TIRF_TOV0) == 0)    // poll-on overflow flag
                ;
            stopTimer0();
            setbit(TIFR, TIRF_TOV0); // clear overflow flag
            // TCNT0 = (uint8)initial_value; // init counter reg
            overFlow_counter++;
        }
        break;
    }

    case TIMER0_CTC_MODE:
    {

        double overFlow_counter = 0;
        OCR0 = (uint8)counts_per_overflow; // load the compared on value
        while (overFlow_counter < n_overflows)
        {
            set_Timer0_Prescaler(Config.Prescaler); // select clock -> start timer
            while (getbit(TIFR, TIRF_OCF0) == 0)    // poll-on overflow flag
                ;
            stopTimer0();
            overFlow_counter++;
            setbit(TIFR, TIRF_OCF0);
        }

        break;
    }

    default:
        break;
    }
}

void TIMER0_Delay_Without_Blocking(uint16 Milli_Seconds)
{
}
