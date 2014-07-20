//******************************************************************************
//! @file $Id: lin_drv.h 581 2008-02-25 10:22:47Z jtellier $
//!
//! Copyright (c) 2008 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the prototypes and the macros of the
//!        low level functions (drivers) of:
//!             - LIN of LIN/UART Controller
//!             - for ATmega32/64/M1/C1
//!             - for ATtiny167.
//!
//! This file was validated with AVRStudio-413571/WinAVR-20071221.
//!
//******************************************************************************

#ifndef _LIN_DRV_H_
#define _LIN_DRV_H_

//----- I N C L U D E S                                                                                                    

#include <avr/io.h>

//_____ D E F I N I T I O N S __________________________________________________

#define LIN_BAUDRATE    19200       // in bit/s - max speed
//#define LIN_BAUDRATE    10417       // in bit/s - SAE J2602

// Specific to ATtiny167
#define LIN_PORT_IN PINA
#define LIN_PORT_DIR DDRA
#define LIN_PORT_OUT PORTA
#define LIN_INPUT_PIN 0
#define LIN_OUTPUT_PIN 1

// ---- Bit Time

    // LINBRR should be set to deliver a clock suitable for 32x (the default) 
    // oversampling of the desired bit clock. Per the datasheet:
#define CONF_LINBRR     ((F_CPU / (32UL * LIN_BAUDRATE)) - 1)

// ---- Configuration

    // LIN protocols
#define LIN_1X          (1<<LIN13)
#define LIN_2X          (0<<LIN13)

    // LIN commands
#define LIN_CMD_MASK    ((1<<LCMD1)|(1<<LCMD0))
#define LIN_RX_HEADER   ((0<<LCMD1)|(0<<LCMD0))
#define LIN_ABORT       ((0<<LCMD1)|(0<<LCMD0))
#define LIN_TX_HEADER   ((0<<LCMD1)|(1<<LCMD0))
#define LIN_RX_RESPONSE ((1<<LCMD1)|(0<<LCMD0))
#define LIN_TX_RESPONSE ((1<<LCMD1)|(1<<LCMD0))

    // LIN interrupt flags
#define LIN_ERROR       (1<<LERR )
#define LIN_IDOK        (1<<LIDOK)
#define LIN_RXOK        (1<<LRXOK)
#define LIN_TXOK        (1<<LTXOK)

    // LIN ID masks
#define LIN_1X_ID_MASK  ((1<<LID3)|(1<<LID2)|(1<<LID1)|(1<<LID0))
#define LIN_1X_LEN_MASK ((1<<LID5)|(1<<LID4))
#define LIN_2X_ID_MASK  ((1<<LID5)|(1<<LID4)| LIN_1X_ID_MASK )

//_____ M A C R O S ____________________________________________________________

// ---- Use LIN 1.x protocol

//#define Lin_1x_set_type()   ( LINCR|= LIN_1X )
#define	Lin_1x_enable()     ( LINCR = LIN_1X|(1<<LENA)|(0<<LCMD2) )
#define	Lin_1x_set_id(id)   { LINIDR &= ~LIN_1X_ID_MASK; LINIDR |= id & LIN_1X_ID_MASK; }
#define Lin_1x_set_len(len) { LINIDR &= ~LIN_1X_LEN_MASK; LINIDR |= (((len+4)<<2) & LIN_1X_LEN_MASK); }
#define Lin_get_len()       ( LINDLR & (0x0F << LRXDL0) )
// Lin_set_rx_len(len) ... Automatic setting in LIN 1.x for response
// Lin_set_tx_len(len) ... Automatic setting in LIN 1.x for response

// ---- Use LIN 2.x protocol

//#define Lin_2x_set_type()   ( LINCR|= LIN_2X )
#define Lin_2x_enable()     ( LINCR = LIN_2X|(1<<LENA)|(0<<LCMD2) )
#define Lin_2x_set_id(id)   { LINIDR &= ~LIN_2X_ID_MASK; LINIDR |= id & LIN_2X_ID_MASK; }
// Lin_2x_set_len(len) . Not available in LIN 2.1
// Lin_get_len() ....... Not available in LIN 2.1
#define Lin_set_rx_len(len) ( LINDLR = len & (0x0F << LRXDL0) )
#define Lin_set_tx_len(len) ( LINDLR = len << LTXDL0 )

// ---- Shared C-macros

    // Use of any protocol
//#define Lin_set_type(ltype) (( ltype == LIN_2X ) ? Lin_2x_set_type(): Lin_1x_set_type() )
#define Lin_get_type()      ( LINCR & (1<<LIN1X) )																							
#define Lin_set_len(len)    ( LINDLR = (len<<LTXDL0)|(len &(0x0F<<LRXDL0) )

    // Miscellaneous C-macros
#define Lin_get_error_status()  ( LINERR )
#define Lin_set_baudrate(br)    { LINBRRH = (unsigned char)(((unsigned short)br)>>8); \
								  LINBRRL = (unsigned char) ((unsigned short)br);	  }
#define Lin_sw_reset()          ( LINCR = 1<<LSWRES )
#define Lin_full_reset()        { Lin_sw_reset(); Lin_clear_enable_it(); LINBRRL = 0x00; LINBRRH = 0x00; }

// Interrupt handling
#define Lin_get_it()            ( LINSIR & ((1<<LERR)|(1<<LIDOK)|(1<<LTXOK)|(1<<LRXOK)) )
#define Lin_set_enable_it()     ( LINENIR = (1<<LENERR)|(1<<LENIDOK)|(1<<LENTXOK)|(1<<LENRXOK) )
#define Lin_clear_enable_it()   ( LINENIR = (0<<LENERR)|(0<<LENIDOK)|(0<<LENTXOK)|(0<<LENRXOK) )
#define Lin_clear_err_it()		( LINSIR = 1<<LERR  )
#define Lin_clear_idok_it()		( LINSIR = 1<<LIDOK )
#define Lin_clear_txok_it()		( LINSIR = 1<<LTXOK )
#define Lin_clear_rxok_it()		( LINSIR = 1<<LRXOK )

// LIN commands
#define Lin_abort()         ( LINCR &= ~LIN_CMD_MASK )
#define Lin_rx_header()     ( LINCR &= ~LIN_CMD_MASK )   
#define Lin_tx_header()     { LINCR &= ~LIN_CMD_MASK; LINCR|= LIN_TX_HEADER ;  }
#define Lin_rx_response()   { LINCR &= ~LIN_CMD_MASK; LINCR|= LIN_RX_RESPONSE; }
#define Lin_tx_response()   { LINCR &= ~LIN_CMD_MASK; LINCR|= LIN_TX_RESPONSE; }

// LIN checking
#define Is_lin_header_ready()       ( (LINSIR & (1<<LIDOK)) ? 1 : 0 )
#define Is_lin_rx_response_ready()	( (LINSIR & (1<<LRXOK)) ? 1 : 0 )
#define Is_lin_tx_response_ready()  ( (LINSIR & (1<<LTXOK)) ? 1 : 0 )

// ID & data handling
#define Lin_get_id()        ( LINIDR & LIN_2X_ID_MASK)      
#define Lin_clear_index()   ( LINSEL = (0<<LAINC)|(0x00<<LINDX0) )
#define Lin_get_data()      ( LINDAT )      
#define Lin_set_data(data)  ( LINDAT = data )

//_____ P R O T O T Y P E S ____________________________________________________

#ifdef __cplusplus
extern "C" {
#endif

extern unsigned char lin_init (unsigned char l_type, unsigned long b_rate);
extern unsigned char lin_tx_header (unsigned char l_type, unsigned char l_id, unsigned char l_len);
extern unsigned char lin_rx_response (unsigned char l_type, unsigned char l_len);
extern unsigned char lin_tx_response (unsigned char l_type, unsigned char *l_data, unsigned char l_len);
extern void lin_get_response (unsigned char *l_data);

#ifdef __cplusplus
}
#endif

//______________________________________________________________________________

#endif // _LIN_DRV_H_
