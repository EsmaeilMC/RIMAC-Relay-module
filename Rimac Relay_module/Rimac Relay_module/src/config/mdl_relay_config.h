/*
 * mdl_relay_config.h
 *
 * Created: 06/07/2022 06:10:04 م
 *  Author: ahmed
 * 
 *	configuration file for Rimac mdl_relay.h
 */ 


#ifndef MDL_RELAY_CONFIG_H_
#define MDL_RELAY_CONFIG_H_

#define NO (0u)  //relay type normally open identifier
#define NC (1u)	//relay type normally closed identifier

#define MDL_RELAY_ABSOLUTE_MAX_REL_COUNT		(16) //maximum number of relays this module can handle
#define MDL_RELAY_ACTIVE_COUNT					(3)		//value < MDL_RELAY_ABSOLUTE_MAX_REL_COUNT
/////////////////////////////////Relay types definitions/////////////////////////////////////////
/////Identify relay to either (NO), or (NC) according to the target hardware
#define	RELAY0					NO		//NO relay type
#define	RELAY1					NC		//NC relay type
#define	RELAY2					NO		//NO relay type
#define	RELAY3					NO		//NO relay type
#define	RELAY4					NO		//NO relay type
#define	RELAY5					NO		//NO relay type
#define	RELAY6					NO		//NO relay type
#define	RELAY7					NO		//NO relay type
#define	RELAY8					NO		//NO relay type
#define	RELAY9					NO		//NO relay type
#define	RELAY10					NO		//NO relay type
#define	RELAY11					NO		//NO relay type
#define	RELAY12					NO		//NO relay type
#define	RELAY13					NO		//NO relay type
#define	RELAY14					NO		//NO relay type
#define	RELAY15					NO		//NO relay type
///////////////////////////////////////////////////////////////////////////////////////////////

#define RELAY_DEBOUNCE_DELAY			(100) //RELAY contact debounce delay for feedback error reading

#endif /* MDL_RELAY_CONFIG_H_ */