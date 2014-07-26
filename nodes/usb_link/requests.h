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

enum USBRequest
{
    kUSBRequestStatus,
    kUSBRequestGetHistory,
    kUSBRequestReadData,
    kUSBRequestWriteData,
    kUSBRequestBeginUpdate,
    kUSBRequestUpdateData,
    kUSBRequestFinishUpdate
};

struct RQStatus
{
    uint8_t flags;
#define RQ_STATUS_CONNECTED         (1<<0)  //< set when connected to a working network
};

struct RQHistory
{
#define RQ_HISTORY_FRAME_VALID      (1<<7)  //< set in frame[0] when the response is valid
#define RQ_HISTORY_RESPONSE_VALID   (1<<6)  //< set in frame[0] when the response is valid
#define RQ_HISTORY_FID_MASK         (0x3f)  //< mask for frame[0] to extract FID
    uint8_t frame[9];
};

struct RQData
{
    uint8_t nad;
    uint8_t page;
    uint8_t index;
    uint16_t value;
};

struct RQBeginUpdate
{
    uint8_t nad;                        //< node that we are going to update
};

struct RQUpdateData
{
    uint8_t     sequence;               //< low 8 bits of program address (high bits are implicit)
    uint8_t     data[8];                //< next 8 bytes of program data
};

struct RQFinishUpdate
{
    uint16_t    length;                 //< total length of image (may be up to 7 less than transferred)
    uint16_t    crc;                    //< crc-16 of firmware over total length
};

union USBDataIn
{
    struct RQStatus         status;
    struct RQHistory        history;
    struct RQData           data;
};

union USBDataOut
{
    struct RQData           data;
    struct RQBeginUpdate    begin;
    struct RQUpdateData     update;
    struct RQFinishUpdate   finish;
};

#pragma pack(pop)
