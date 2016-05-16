#include <Automaton.h>
#include "Atm_pulse.h"
	
Atm_pulse & Atm_pulse::begin( int attached_pin, int minimum_duration )
{
  const static state_t state_table[] PROGMEM = {
  /*              ON_ENTER    ON_LOOP  ON_EXIT  EVT_TIMER   EVT_HIGH  EVT_LOW   ELSE */
  /* IDLE    */         -1,        -1,      -1,        -1,      WAIT,      -1,    -1,
  /* WAIT    */         -1,        -1,      -1,     PULSE,        -1,    IDLE,    -1,
  /* PULSE   */  ACT_PULSE,        -1,      -1,        -1,        -1,    IDLE,    -1,
  };
  Machine::begin( state_table, ELSE );
  pin = attached_pin; 
  timer.begin( this, minimum_duration );
  pinMode( pin, INPUT ); 
  return *this;          
}

Atm_pulse & Atm_pulse::onPulse( Machine * machine, uint8_t msg ) 
{
  client_machine = machine;
  client_msg = msg;
  return *this;  
}

Atm_pulse & Atm_pulse::onPulse( pulsecb_t pulse_callback ) 
{
  callback = pulse_callback;
  return *this;  
}

int Atm_pulse::event( int id ) 
{
  switch ( id ) {
  	case EVT_TIMER :
  	  return timer.expired();
    case EVT_HIGH :
      return digitalRead( pin );
    case EVT_LOW :
      return !digitalRead( pin );
   }
   return 0;
}

void Atm_pulse::action( int id ) 
{
  switch ( id ) {
  	case ACT_PULSE :
      if ( callback ) {
         (*callback)();
         return;
      }
      client_machine->msgWrite( client_msg );
  	  return;
   }
}

Atm_pulse & Atm_pulse::onSwitch( swcb_sym_t switch_callback ) {

  Machine::onSwitch( switch_callback, 
   "IDLE\0WAIT\0PULSE",
    "EVT_TIMER\0EVT_HIGH\0EVT_LOW\0ELSE" );
  return *this;
}


// TinyMachine version
	
Att_pulse & Att_pulse::begin( int attached_pin, int minimum_duration )
{
  const static tiny_state_t state_table[] PROGMEM = {
  /*              ON_ENTER    ON_LOOP  ON_EXIT  EVT_TIMER   EVT_HIGH  EVT_LOW   ELSE */
  /* IDLE    */         -1,        -1,      -1,        -1,      WAIT,      -1,    -1,
  /* WAIT    */         -1,        -1,      -1,     PULSE,        -1,    IDLE,    -1,
  /* PULSE   */  ACT_PULSE,        -1,      -1,        -1,        -1,    IDLE,    -1,
  };
  TinyMachine::begin( state_table, ELSE );
  pin = attached_pin; 
  timer.begin( this, minimum_duration );
  pinMode( pin, INPUT ); 
  return *this;          
}

Att_pulse & Att_pulse::onPulse( Machine * machine, uint8_t msg ) 
{
  client_machine = machine;
  client_msg = msg;
  return *this;  
}

Att_pulse & Att_pulse::onPulse( pulsecb_t pulse_callback ) 
{
  callback = pulse_callback;
  return *this;  
}

int Att_pulse::event( int id ) 
{
  switch ( id ) {
  	case EVT_TIMER :
  	  return timer.expired();
    case EVT_HIGH :
      return digitalRead( pin );
    case EVT_LOW :
      return !digitalRead( pin );
   }
   return 0;
}

void Att_pulse::action( int id ) 
{
  switch ( id ) {
  	case ACT_PULSE :
      if ( callback ) {
         (*callback)();
         return;
      }
      client_machine->msgWrite( client_msg );
  	  return;
   }
}

