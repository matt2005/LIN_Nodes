#pragma once

//
// v1 relay node
//

#define BOARD_FUNCTION_ID   1

#define pinOUT1        portB[0]
#define pinSTATUS1     portB[1]
#define pinOUT2        portB[2]
#define pinSTATUS2     portB[3]
#define pinOUT3        portB[4]
#define pinSTATUS3     portB[5]
#define pinOUT4        portB[6]
#define pinSTATUS4     portA[7]

#define pinMode1       portA[4]
#define pinMode2       portA[2]
#define pinMode4       portA[5]
#define pinMode8       portA[3]

#define pinLINRX       portA[0]
#define pinLINTX       portA[1]
#define pinLINCS       portA[6]

#define pinDebugTX     portA[7]

#define paramRelay1Assign  (LIN::RelayID)Parameter(1)
#define paramRelay2Assign  (LIN::RelayID)Parameter(2)
#define paramRelay3Assign  (LIN::RelayID)Parameter(3)
#define paramRelay4Assign  (LIN::RelayID)Parameter(4)
