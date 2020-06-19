/*
 * HRTtimer.h
 *
 *  Created on: May 7, 2015
 *      Author: cgalperti
 *
 *      assembly code for reading TSC counter on Intel processors,
 * 		some type definitions for using it
 *
 */

#ifndef HIC_COMMON_HRTTIMER_H_
#define HIC_COMMON_HRTTIMER_H_

//#define _LINUX_IA64
#define _LINUX

/** 64 Bit unsigned integer. */
typedef unsigned long long uint64;
/** 64 Bit signed integer. */
typedef long long          int64;
/** 32 Bit unsigned integer. */
typedef unsigned int       uint32;
/** 32 Bit signed integer. */
typedef signed int         int32;


/** Reads the High Resolution Timer as 64 bit int. Fast inline */
/** Thanks very much to MARTe framework and to LucaB */
static inline int64 HRTRead64 () {
	unsigned cycles_low, cycles_high;
	uint64 timestamp;

#if defined(_MSC_VER)
    __asm  {
        _emit 0x0F
        _emit 0x31
    }
#elif (defined(_CY32) || defined(__EMX__) || defined(_RSXNT))
    volatile int64 perf;
    uint32 *pperf = (uint32 *)&perf;
    asm(
"\n"
"        rdtsc        \n"
       : "=a"(pperf[0]) , "=d"(pperf[1])
       :
       : "eax","edx"
    );
    return perf;
#elif (defined(_LINUX) || defined(_MACOSX))
    volatile int64 perf;
    //register volatile int64 perf;
    uint32 *pperf = (uint32 *)&perf;
    asm volatile(
"\n"
"        rdtsc        \n"
       : "=a"(pperf[0]) , "=d"(pperf[1])
    );
    return perf;
#elif (defined(_LINUX_IA64))

    asm volatile ("RDTSC\n\t"
    "mov %%edx, %0\n\t"
    "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::“%eax”, “%edx”);

    timestamp= ( ((uint64)cycles_high << 32) | cycles_low );
    return (int64)timestamp;


#elif (defined(_RTAI) )
    int64 perf;
    uint32 *pperf = (uint32 *)&perf;
    asm(
        "rdtsc"
       : "=a"(pperf[0]) , "=d"(pperf[1])
       :
//       : "eax","edx"
    );
    return perf;
#elif defined(_VX5100) || defined(_VX5500)|| defined(_V6X5100)|| defined(_V6X5500)

    int a ,b ,c ,d;
    asm(
        "mftbu %0\n"
        "mftb %1\n"
        "mftbu %2\n"
        "mftb %3\n"
        : "=r" (a), "=r" (b), "=r" (c), "=r" (d)
    );

    int64 time;
    int *pout = (int *) &time;
    if (a != c){
        pout[0] = c;
        pout[1] = d;
    } else {
        pout[0] = a;
        pout[1] = b;
    }
    return time;
#else
    return 0;
#endif
}


#endif /* HIC_COMMON_HRTTIMER_H_ */
