/***********************************************************************
 *  FILE NAME: ts100mhz.h
 *
 *  DESCRIPTION: I provide memory access to a timestamp register in
 *  the Cavalier ASIC. This is a 32 bit register that increments at
 *  the rate of 100Mz. You can use it to do timing analysis. I provide
 *  a function to map the register into your memory space and another
 *  function to read the register.
 *
 *  This free-running running timestamp register is not sync'd to any
 *  external event. The only practical thing you can do with it is to
 *  measure the time between two events. Having only 32 bits, the
 *  register will wrap around in a little bit less than 43 seconds, so
 *  there is ambiguity between time t and time t+43s, and time t +
 *  2*43s, etc. This is normally not a problem, as we are pretty sure
 *  the events we are timing are on the order of milliseconds apart.
 *
 *  Suppose at event e1 you read the timestamp register and save it as
 *  t1. Then at event e2 you read and save the timestamp register as
 *  t2. As long as the events are no more than 42 seconds apart, the
 *  unsigned difference (t2 - t1) will be the number of hundredths of
 *  microseconds between the two events. This simple calculation always
 *  works because of the modular nature of 2's complement arithmetic.
 *  This relation is always true, NO MATTER whether the register
 *  OVERFLOWED between the two samples, as long as it didn't overflow
 *  more than once.
 *
 *  NOTE: This method of accessing the register is quick and
 *  efficient, but it has a drawback. In exposing the register of
 *  interest, you also have to expose all the other registers on the
 *  same 4K virtual page -- at least from a hardware point of view. I
 *  can limit the damage from a rogue user by granting only read
 *  access to the page. But some registers have a "destructive" read,
 *  that changes the state of the hardware (e.g., resets an interrupt
 *  or clears an error). We limit rogue access as best we can by
 *  following the convention that no-one accesses this register except
 *  through the functions defined here. And we make those functions
 *  inline (or macros) so they'll run as fast as possible.
 *
 *  DATE    NAME    REFERENCE   REASON
 *  ----    ----    ---------   ------
 *  08Feb11 O'Neil              Create original
 *  05Mar12 O'Neil              Make idempotent
 *
 *
 *    DATE         NAME           REFERENCE        REASON
 *  ----------  -------------   --------------    ----------------------
 *  05-29-2012  zfan                              Adapt for Sahara Callisto
 *  07-02-2012  V. Ganesan                        Callisto related changes merged to main line
 *  11-12-2012  Z. Fan                            Improved handling in Ganymede timestamp read
 *  01-11-2012  zfan                              Add MACROs for convenience
 *
 *  Copyright 2012-13, Hughes Network Systems
 **********************************************************************/

#ifndef TS100MHZ_H
#define TS100MHZ_H

/* free-running 100 MHz clock */
#define TIMESTAMP_CLOCK_MHZ   (100)

/* I convert 100 MHZ clock ticks into ms */
#define CLK_TICK_2_MS(timeInTicks) \
                ((((timeInTicks)/TIMESTAMP_CLOCK_MHZ) + 500)/1000) /*ms with 0.5 to carry up*/

/* I convert 100 MHZ clock ticks into us */
#define CLK_TICK_2_US(timeInTicks) \
                ((timeInTicks)/TIMESTAMP_CLOCK_MHZ) /*us*/

/* For the Acadia platform the SWP general purpose timer is used by the kernel and configured to run at 1MHz and needs to be scaled up to 100MHZ .       
   For other platforms the timer is already at 100 MHz. */
#ifdef THEBE_PLATFORM
#define KERNEL_100MHZ_SCALE_FACTOR  100
#else
#define KERNEL_100MHZ_SCALE_FACTOR  1
#endif

/* stub to make it compatible with old code that uses this as a function call */
#define ts100MhzMap()

#ifdef  CALLISTO_PLATFORM
#include "memmapping.h"

#include "hw-regs.h"


/***********************************************************************
 *  FUNCTION NAME: ts100MhzRead
 *
 *  DESCRIPTION: I open /dev/mem and map the page containing the
 *  timestamp register into your virtual space. I save the virtual
 *  address of the pointer at pTimeStamp.
 *
 *  NOTE: This utility ONLY handles events are NO more than 42 seconds apart
 **********************************************************************/
static inline int ts100MhzRead(void)
{
    static volatile int *regPtr = NULL;

    if(!regPtr)
    {
        regPtr = (int *)MAP_A_REG(SWP_GP_TMR2_COUNT_RW);
        if(!regPtr)
            return 0;
    }

    return ((*regPtr)*KERNEL_100MHZ_SCALE_FACTOR);
}

#else // GANYMEDE implementation.
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

/***********************************************************************
 *  DATA DESCRIPTION: static pointer to memory mapped register
 **********************************************************************/

#define _TS_REGISTER 0x10016048
#define _PAGE_SZ     4096

/***********************************************************************
 *  FUNCTION NAME: tsRead
 *
 *  DESCRIPTION: I open /dev/mem and map the page containing the
 *  timestamp register into your virtual space. I save the virtual
 *  address of the pointer at pTimeStamp.
 *
 *  NOTE: This utility ONLY handles events are NO more than 42 seconds apart
 **********************************************************************/

static inline int ts100MhzRead(void) {
    static volatile int *pTimeStamp = 0;

    if(!pTimeStamp)
    {
        int  fd;
        int  pageStart = _TS_REGISTER & ~(_PAGE_SZ - 1);
        void *vpage;

        fd = open("/dev/mem", O_RDONLY);
        if (fd == -1)
            perror("ts100MhzRead failed to open /dev/mem");
        else
        {
            vpage = mmap(0, _PAGE_SZ, PROT_READ, MAP_SHARED, fd, pageStart);
            if (vpage == (void *)-1)
                perror("ts100MhzRead failed to map /dev/mem");
            else
                pTimeStamp = vpage + (_TS_REGISTER % _PAGE_SZ);
            close(fd);
        }

        if(!pTimeStamp)
            return 0;
    }

    return *pTimeStamp;
}
#endif
#endif
