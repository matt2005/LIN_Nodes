//******************************************************************************
//! @file $Id: lin_drv.c 581 2008-02-25 10:23:47Z jtellier $
//!
//! Copyright (c) 2008 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the low level functions (drivers) of:
//!             - LIN of LIN/UART Controller
//!             - for ATmega32/64/M1/C1
//!             - for ATtiny167.
//!
//! This file was validated with AVRStudio-413571/WinAVR-20071221.
//!
//******************************************************************************
//_____ I N C L U D E S ________________________________________________________
#include "lin_drv.h"

//_____ D E F I N I T I O N S __________________________________________________

//_____ D E C L A R A T I O N S ________________________________________________

//_____ F U N C T I O N S ______________________________________________________

//------------------------------------------------------------------------------
//  @fn lin_init
//
//  This function initializes the LIN controller and, if needed, the LIN 
//  interrupts.
//
//  Arguments: 2 arguments are passed to the function:
//           - l_type: By construction, ‘l_type’ is either LIN_1X or LIN_2X
//           - b_rate: LIN Baud Rate Register (LINBRR) value
//
//  The function returns:
//           == 0 : Initialization failed, LIN type is not in accordance
//           != 0 : Initialization performed
//
//  Warning: none
//------------------------------------------------------------------------------
unsigned char lin_init (unsigned char l_type, unsigned long b_rate) {

    Lin_full_reset();
    Lin_set_baudrate(b_rate);
			
    if (l_type == LIN_1X) {
    			Lin_1x_enable();
    } else if (l_type == LIN_2X) {
    			Lin_2x_enable();
    } else {
    			return 0;
    }
    // If LIN is interrupt driven, enable the 2 following lines
    //Lin_set_enable_it();
    // asm ("sei");
    
    return 1;
}

//------------------------------------------------------------------------------
//  @fn lin_tx_header
//
//  This function commands the sending of the LIN header, MASTER task of MASTER
//  node.
//
//  Arguments: 3 arguments are passed to the function:
//           - l_type: By construction, ‘l_type’ is either LIN_1X or LIN_2X
//           - l_id: LIN identifier value. In case of ‘LIN_1X’, the coded
//                          length is transported into the LIN identifier.
//           - l_len: True length (not coded), number of data bytes transported
//                          in the response. This information is not used in
//                          ‘LIN_1X’ because it is coded in ‘l_id’.
//
//  The function returns:
//           == 0 : Initialization failed, LIN type is not in accordance
//           != 0 : Header transmission of the header on-going
//
//  Warning: none
//------------------------------------------------------------------------------
unsigned char lin_tx_header (unsigned char l_type, unsigned char l_id, unsigned char l_len) {                                                                                        
                                                                                                                                                                         
    Lin_abort();    // Useful if controller is still in 'lin_tx_response'                                                        
    				// or in 'lin_rx_response' mode. Note that when these                                                                    
    				// modes are running, writing in LIN registers is                                                                        
    				// disabled and the ID cannot be set in the controller.                                                                  
    				// (c.f. “Break-in-Data” behavior”)
    				
    if (l_type == LIN_1X) {
        Lin_1x_set_id(l_id);                                                                                                                             
        Lin_1x_set_len(l_len);
    } else if (l_type == LIN_2X) {
        Lin_2x_set_id(l_id);        // No length transported in LIN 2.X
    } else {
        return 0;
    }
    
    Lin_tx_header();            // Set command
    return 1;
}

//------------------------------------------------------------------------------
//  @fn lin_rx_response
//
//  This function commands the reception of a LIN response, SLAVE task of MASTER
//  or SLAVE node.
//
//  Arguments: 2 arguments are passed to the function:
//           - l_type: By construction, ‘l_type’ is either LIN_1X or LIN_2X
//           - l_len: True length (not coded), number of data bytes transported
//                          in the response. This information is not used in
//                          ‘LIN_1X’ because it is coded in ‘l_id’.
//
//  The function returns:
//           == 0 : Initialization failed, LIN type is not in accordance
//           != 0 : Response reception on-going
//
//  Warning: none
//------------------------------------------------------------------------------
unsigned char lin_rx_response (unsigned char l_type, unsigned char l_len) {                                                                                                         
                                                                                                                                                                      
    if (l_type == LIN_1X) {                                                                                                                                   
        Lin_1x_set_type();              // Change is necessary                                        
    } else if (l_type == LIN_2X) {                                                                                                                            
        Lin_2x_set_type();              // Change is necessary                                        
        Lin_set_rx_len(l_len);                                                                                                                        
    } else {                                                                                                                                                  
        return 0;                                                                                                                                     
    }                                                                                                                                                         
                                                                                                                                                              
    Lin_rx_response();          // Set command                                                
    return 1;                                                                                                                                                 
}

//------------------------------------------------------------------------------
//  @fn lin_tx_response
//
//  This function commands the sending of a LIN response, SLAVE task of MASTER
//  or SLAVE node.
//
//  Arguments: 3 arguments are passed to the function:
//           - l_type: By construction, ‘l_type’ is either LIN_1X or LIN_2X
//           - *l_data: Internal SRAM array pointer. This array contains the
//                          data bytes to transmit.                                                                    
//           - l_len: True length (not coded), number of data bytes transported
//                          in the response. This information is not used in
//                          ‘LIN_1X’ because it is coded in ‘l_id’.
//
//  The function returns:
//           == 0 : Initialization failed, LIN type is not in accordance
//           != 0 : Response transmission on-going
//
//  Warning: none
//------------------------------------------------------------------------------
unsigned char lin_tx_response (unsigned char l_type, unsigned char *l_data, unsigned char l_len) {                                                                                     
                                                                                                                                                                         
unsigned char i;                                                                                                                                                         
                                                                                                                                                                         
    if (l_type == LIN_1X) {                                                                                                                                      
        Lin_1x_set_type();              // Change is necessary                                           
    } else if (l_type == LIN_2X) {                                                                                                                               
        Lin_2x_set_type();				// Change is necessary                                           
        Lin_set_tx_len(l_len);                                                                                                                           
    } else {                                                                                                                                                     
        return 0;                                                                                                                                        
    }                                                                                                                                                            
                                                                                                                                                                 
    Lin_clear_index();                  // Data processing                                               
    for (i = 0; i < l_len; i++) {                                                                                                                                
        Lin_set_data(*l_data++);                                                                                                                         
    }                                                                                                                                                            
                                                                                                                                                                 
    Lin_tx_response();          // Set command                                                   
    return 1;                                                                                                                                                    
}

//------------------------------------------------------------------------------
//  @fn lin_get_response
//
//  This function reads (empties) the reception data buffer when a LIN response
//  had been received. This function is additional of the ‘lin_rx_response()’
//  function.
//
//  Arguments: 1 argument is passed to the function:
//           - *l_data: Internal SRAM array pointer. This array contains the
//                          data bytes to transmit.                                                                    
//
//  This function returns nothing
//
//  Warning: none
//------------------------------------------------------------------------------
void lin_get_response (unsigned char *l_data) {                                                                                                                 
                                                                                                                                                                
unsigned char i, l_len;                                                                                                                                         
                                                                                                                                                                
    l_len = Lin_get_len();                                                                                                                           
    Lin_clear_index();                                                                                                                                  
    for (i = 0; i < l_len; i++) {                                                                                                                       
        (*l_data++) = Lin_get_data();                                                                                                           
    }                                                                                                                                                   
}                                                                                                                                                               
