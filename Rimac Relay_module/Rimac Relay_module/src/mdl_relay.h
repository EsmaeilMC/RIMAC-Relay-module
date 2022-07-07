/*
 * mdl_relay.h
 *
 * Created: 06/07/2022 06:16:56 م
 *  Author: ahmed
 *
 * Dependencies 
 */ 


#ifndef MDL_RELAY_H_
#define MDL_RELAY_H_

typedef unsigned int uint32_t;

typedef enum RELAY_index_ENUM
{
	RELAY_INDEX_00		= 0u,		//!< Index of Relay 0
	RELAY_INDEX_01		= 1u,		//!< Index of Relay 1
	RELAY_INDEX_02		= 2u,		//!< Index of Relay 2
	RELAY_INDEX_03		= 3u,		//!< Index of Relay 3
	RELAY_INDEX_04		= 4u,		//!< Index of Relay 4
	RELAY_INDEX_05		= 5u,		//!< Index of Relay 5
	RELAY_INDEX_06		= 6u,		//!< Index of Relay 6
	RELAY_INDEX_07		= 7u,		//!< Index of Relay 7
	RELAY_INDEX_08		= 8u,		//!< Index of Relay 8
	RELAY_INDEX_09		= 9u,		//!< Index of Relay 9
	RELAY_INDEX_10		= 10u,		//!< Index of Relay 10
	RELAY_INDEX_11		= 11u,		//!< Index of Relay 11
	RELAY_INDEX_12		= 12u,		//!< Index of Relay 12
	RELAY_INDEX_13		= 13u,		//!< Index of Relay 13
	RELAY_INDEX_14		= 14u,		//!< Index of Relay 14
	RELAY_INDEX_15		= 15u,		//!< Index of Relay 15
	
	RELAY_INDECES_COUNT = 16u		
}RELAY_index_E;


typedef enum RELAY_output_state_ENUM
{
	RELAY_OFF			= 0u,		//!<  RELAY off
	RELAY_ON			= 1u,		//!<  RELAY on	
	
	RELAY_STATES		= 2u,		//!<  RELAY states count	
}RELAY_output_state_E;


typedef enum RELAY_fault_status_ENUM
{
	RELAY_FUNCTIONAL			= 0u,		//!<	normal working relay
	RELAY_WELDED				= 1u,		//!<	welded relay
	RELAY_CONSTANTLY_OPEN		= 2u,		//!<	constantly open relay
	
	RELAY_ERROR_COUNT			= 3u,		//!<	normal working relay
}RELAY_fault_status_E;

typedef enum RELAY_contact_status_ENUM
{
	RELAY_CONTACT_OPEN			= 0u,		//!<  RELAY feedback contact open
	RELAY_CONTACT_CLOSED		= 1u,		//!<  RELAY feedback contact closed
	RELAY_CONTACT_ERROR			= 2u,		//!<  RELAY states error
	
	RELAY_CONTACT_STATES		= 3u,		//!<  RELAY states count
}RELAY_contact_status_E;



/////////////////////////////////mdl_relay API interface funcitons/////////////////////////////////////////

void mdl_relay_set_output(RELAY_index_E relay_index, RELAY_output_state_E output);
RELAY_contact_status_E mdl_relay_read_feedback(RELAY_index_E relay_index);
RELAY_fault_status_E mdl_relay_read_error(RELAY_index_E relay_index);

/////////////////////////////////mdl_relay scheduler handling functions////////////////////////////////////

/*
***********************************************************************************************
			Scheduler routines
***********************************************************************************************
**assuming operation with a cooperative scheduler
** @mdl_relay_init(); to be called before adding @mdl_relay_handler(); to the scheduler
** @mdl_relay_terminate to be called after removing @mdl_relay_handler(); from the scheduler
**********************************************************************************************
*/

void mdl_relay_init(void);
void mdl_relay_handler(void *param);
void mdl_relay_terminate(void);


uint32_t mdl_relay_get_handler_ticks(void);



#endif /* MDL_RELAY_H_ */