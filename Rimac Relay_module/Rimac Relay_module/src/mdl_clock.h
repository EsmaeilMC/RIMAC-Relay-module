/*
 * mdl_clock.h
 *
 * Created: 06/07/2022 07:04:46 م
 *  Author: ahmed
 */ 


#ifndef MDL_CLOCK_H_
#define MDL_CLOCK_H_

typedef unsigned int uint32_t;

typedef uint32_t CLOCK_ticks_T;  
/********************************************************************************************************* 
* @brief Retrieve elapsed miliseconds from power-up. Assume there will be no overflow. 
********************************************************************************************************* 
* @param [in] Nothing. 
* @return Number of ticks elapsed. 
********************************************************************************************************/ 
CLOCK_ticks_T CLOCK_getTicks();


#endif /* MDL_CLOCK_H_ */