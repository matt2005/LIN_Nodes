/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#pragma once

#pragma pack(push, 1)

enum USBRequest {
    kUSBRequestStatus,              // wValue = kind index or ignored, wIndex = status kind, IN = uint8_t
    kUSBRequestClearStatus,         // wValue = ignored, wIndex = status kind
    kUSBRequestGetHistory,          // wValue = ignored, wIndex = ignored, IN = RQHistory
    kUSBRequestSelectNode,          // wValue = node address, wIndex = ignored
    kUSBRequestReadData,            // wValue = ignored, wIndex = parameter address
    kUSBRequestReadResult,          // wValue = ignored, wIndex = parameter address, IN = uint16_t
    kUSBRequestWriteData,           // wValue = value, wIndex = parameter address
    kUSBRequestSendBulk,            // wValue = data0,1, wIndex = data2,3
    kUSBRequestEnableMaster,        // wValue = 1 to enable, 0 to disable
};

#define RQ_STATUS_FLAGS         0
#define RQ_STATUS_DATA_READY        (1<<0)
#define RQ_STATUS_DATA_ERROR        (1<<1)
#define RQ_STATUS_AWAKE             (1<<2)
#define RQ_STATUS_WAITING           (1<<3)
#define RQ_STATUS_MASTER            (1<<4)

#define RQ_STATUS_FREEMEM       1
#define RQ_STATUS_LINERR        2   // wValue = error counter index

// kUSBRequestGetHistory -> IN
struct RQHistory {
#define RQ_HISTORY_FRAME_VALID      (1<<7)  //< set in frame[0] when the response is valid
#define RQ_HISTORY_RESPONSE_VALID   (1<<6)  //< set in frame[0] when the response is valid
#define RQ_HISTORY_FID_MASK         (0x3f)  //< mask for frame[0] to extract FID
    uint8_t frame[9];
};

struct RQBeginUpdate {
    uint8_t nad;                        //< node that we are going to update
};

struct RQUpdateData {
    uint8_t     sequence;               //< low 8 bits of program address (high bits are implicit)
    uint8_t     data[8];                //< next 8 bytes of program data
};

struct RQFinishUpdate {
    uint16_t    length;                 //< total length of image (may be up to 7 less than transferred)
    uint16_t    crc;                    //< crc-16 of firmware over total length
};

#pragma pack(pop)
