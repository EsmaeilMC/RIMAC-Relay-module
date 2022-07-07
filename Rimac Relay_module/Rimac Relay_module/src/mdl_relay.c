/*
 * mdl_relay.c
 *
 * Created: 06/07/2022 06:15:08 م
 *  Author: Ahmed
 */ 

#include <mdl_clock.h>
#include <mdl_di.h>
#include <mdl_relay_config.h>
#include <mdl_relay.h>

static uint32_t mdl_relay_active = 0; //a flag to activate API functions and module handler after mdl init
static uint32_t relay_output = 0;
//time out countdown timer for relay de_bounce delay
static uint32_t relay_feedback_check_delay[MDL_RELAY_ABSOLUTE_MAX_REL_COUNT] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static uint32_t relay_feedback_value = 0; //a read_out of relay feedback
static uint32_t relay_type_register = 0; //a construct of relay type NC/NO
static uint32_t relay_feedback_fault_register = 0; //Fault flag for each relay

static uint32_t task_tick_count = 0; //tick count for the task handler


void mdl_relay_set_output(RELAY_index_E relay_index, RELAY_output_state_E output)
{
	uint32_t mask = 0;
	mask = (1u << relay_index); //set the mask to pin index
	
	if (relay_index < MDL_RELAY_ABSOLUTE_MAX_REL_COUNT) //relay index within limit
	{
		
		relay_feedback_check_delay[relay_index] = RELAY_DEBOUNCE_DELAY; //reset debounce count down counter
		
		if (output == RELAY_ON)
		{
			DO_setOutputState(relay_index, DO_state_ON); //set DIO of relay index
			relay_output |= mask; //set corresponding bit of the relay index
		} 
		else if(output == RELAY_OFF )
		{
			DO_setOutputState(relay_index, DO_state_OFF); //clear DIO of relay index
			relay_output &= ~mask; //clear corresponding bit of the relay index
		}
	} 
	else
	{
		// relay index out of range
		//generate run time error if there is a mechanism for runtime errors
	}
}

RELAY_contact_status_E mdl_relay_read_feedback(RELAY_index_E relay_index)
{
 	if (relay_index < MDL_RELAY_ABSOLUTE_MAX_REL_COUNT) //relay index within limit
 	{
 		uint32_t mask; //masking the corresponding bit of the relay feedback register
 		mask = (1u << relay_index); //set the mask to pin index
		 
		if ((mask & relay_feedback_fault_register) && (relay_feedback_check_delay[relay_index] == 0)) //masking the relay feedback fault register for relay index pin
		{
			// correspoding relay fault pin is active
			return RELAY_CONTACT_ERROR;
		}
 		else if (mask & relay_feedback_value) //masking the relay feedback register for relay index pin
 		{
			 // corresponding relay feedback pin is active
			 return RELAY_CONTACT_CLOSED;
 		} 
 		else
 		{
			 // corresponding relay feedback pin is inactive
			 return RELAY_CONTACT_OPEN;
 		}
 	}
 	else
 	{
 		// relay index out of range
 		// generate run time error if there is a mechanism for runtime errors
 		return RELAY_CONTACT_STATES;
 	}
}

/***********************************************************
			Relay feedback error truth table
************************************************************
____________________________________________________________
|			|			|			|				|
|	COIL	|feed back	|	type	|	error type	|address
|			|			|			|				|
____________________________________________________________
|	0		|	0		|	0(NO)	|	No error	| row(0)
____________________________________________________________
|	0		|	0		|	1(NC)	|	Welded		| row(1)
____________________________________________________________
|	0		|	1		|	0(NO)	|	Welded		| row(2)
____________________________________________________________
|	0		|	1		|	1(NC)	|	No error	| row(3)
____________________________________________________________
|	1		|	0		|	0(NO)	|	Open relay	| row(4)
____________________________________________________________
|	1		|	0		|	1(NC)	|	No error	| row(5)
____________________________________________________________
|	1		|	1		|	0(NO)	|	No error	| row(6)
____________________________________________________________
|	1		|	1		|	1(NC)	|	Open relay	| row(7)
____________________________________________________________
*/

RELAY_fault_status_E mdl_relay_read_error(RELAY_index_E relay_index)
{
	if (relay_index < MDL_RELAY_ABSOLUTE_MAX_REL_COUNT) //relay index within limit
	{
		uint32_t mask; //masking the corresponding bit of the relay error register
		mask = (1u << relay_index); //set the mask to pin index
		
		if (relay_feedback_check_delay[relay_index] == 0)//contact debounce delay has passed
		{
			/*
			************************************
				look at truth table above ^^
			************************************
			*/
			uint32_t truth_table_addess = 0;
			// construct Relay feedback error truth table address
			
			if (relay_output & mask) truth_table_addess = 1; // coil active
			
			truth_table_addess = (truth_table_addess << 1); //shift left by 1
			
			if (relay_feedback_value & mask) truth_table_addess |= 1; // feedback active
			
			truth_table_addess = (truth_table_addess << 1); //shift left by 1
			
			if (relay_type_register & mask) truth_table_addess |= 1; // NC/NO
			
//*************************************************************************
			
			if((truth_table_addess == 1) || (truth_table_addess == 2)) return RELAY_WELDED; //welded relay
			if((truth_table_addess == 4) || (truth_table_addess == 7)) return RELAY_CONSTANTLY_OPEN; //constant open
			
			return RELAY_FUNCTIONAL; //normal operation (correct feedback)
		} 
		else //waiting for debounce counter to reach (0)
		{
			return RELAY_FUNCTIONAL; //assume normal operation (correct feedback)
		}
		
	}
	else
	{
		// relay index out of range
		// generate run time error if there is a mechanism for runtime errors
		return RELAY_ERROR_COUNT;
	}
}


/*
***********************************************************************************************
			Scheduler routines
***********************************************************************************************
**assuming operation with a cooperative scheduler
** @mdl_relay_init(); to be called before adding @mdl_relay_handler(); to the scheduler
** @mdl_relay_terminate to be called after removing @mdl_relay_handler(); from the scheduler
**********************************************************************************************
*/
void mdl_relay_init(void)
{
	uint32_t i;
	for (i=0; i<MDL_RELAY_ABSOLUTE_MAX_REL_COUNT; i++)
	{
		relay_feedback_check_delay[i] = 0; //reset delay counter to 0
		DO_setOutputState(i, DO_state_OFF); //clear all relay outputs
	}
	relay_output = 0;
	relay_feedback_value = 0; //a read_out of relay feedback
	relay_feedback_fault_register = 0;
	task_tick_count = 0; 
	
	// construct relay type reg NC/NO
	relay_type_register = (RELAY15 << 15) | (RELAY14 << 14) | (RELAY13 << 13) | (RELAY12 << 12) | 
						(RELAY11 << 11) | (RELAY10 << 10) | (RELAY9 << 9) | (RELAY8 << 8) |
						(RELAY7 << 7) | (RELAY6 << 6) | (RELAY5 << 5) | (RELAY4 << 4) |
						(RELAY3 << 3) | (RELAY2 << 2) | (RELAY1 << 1) | (RELAY0 << 0); 
	
	mdl_relay_active = 1; //activate mdl_relay module
}

static inline relay_fault_register_fill(void)
{
	
	// from the truth table above a relay fault can be detected by ->
	// -> relay_output |XOR| relay_feedback_value |XOR| relay_type_register
	relay_feedback_fault_register = relay_output ^ (relay_feedback_value ^ relay_type_register);
}

/************************************************************************
* mdl_relay_handler()
*************************
****
* 1- scan relay feedback value
*
* 2- detecting relay feedback error by raising a flag
*
* 3- decrement relay debounce counter
************************************************************************/


void mdl_relay_handler(void *param)
{
	uint32_t local_task_tick_count = CLOCK_getTicks();
	
	
	uint32_t i, temp = 0;
	if (mdl_relay_active) // module initialized
	{
		i = MDL_RELAY_ACTIVE_COUNT;
		while (i)
		{
			--i;
			//scan relay feedback inputs
			temp = temp << 1;
			if (DI_getInputState(i) == DI_state_ON) temp |= 1u; //fill relay feedback register
			
			if(relay_feedback_check_delay[i]) relay_feedback_check_delay[i]--; //decrement debounce timer
		}
		relay_feedback_value = temp; 
		relay_fault_register_fill(); // detect faults for each relay feedback
	}
	local_task_tick_count = CLOCK_getTicks() - local_task_tick_count; //assuming no overflow
	
	
	task_tick_count = local_task_tick_count; //single write instruction
}

uint32_t mdl_relay_get_handler_ticks(void)
 {
 	return task_tick_count;
 }
void mdl_relay_terminate(void)
{
	uint32_t i;
	
	mdl_relay_active = 0; //deactivate mdl_relay functions
	task_tick_count = 0; //reset tick count to zero
	
	
	for (i=0; i<MDL_RELAY_ABSOLUTE_MAX_REL_COUNT; i++)
	{
		relay_feedback_check_delay[i] = 0; //reset delay counter to 0
		DO_setOutputState(i, DO_state_OFF); //clear all relay outputs
	}
	
	relay_output = 0;
	relay_feedback_value = 0; //a read_out of relay feedback
	relay_feedback_fault_register = 0;
	
}