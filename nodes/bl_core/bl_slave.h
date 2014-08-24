/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <lin_dev.h>

/* polled-mode LIN slave */
class BLSlave : public LINDev
{
public:
    BLSlave() : LINDev(true) {}

    static bool         is_program_valid();
    static void         run_program();

protected:
    virtual void        st_header_received() override;
    virtual void        st_response_received(Response &frame) override;

private:
    static const uint16_t kInfoPage = FLASHEND - (SPM_PAGESIZE - 1);
    static const uint16_t kNoSendResponse = 0xffff;

    static uint8_t      _nad;
    static uint16_t     _sendIndex;
    static uint16_t     _pageAddress;
    static uint8_t      _pageOffset;
    static uint16_t     _runningCrc;
    static uint16_t     _pageStatus;
    static uint16_t     _programEnd;
    static uint16_t     _resetVector;
    static uint16_t     _pageBuffer[];
    static uint16_t     _memoryPointer;

    static uint8_t      nad();
    static uint8_t      function();
    static void         set_page_address(uint16_t address);
    static bool         add_page_byte(uint8_t byte);
    void                send_response();
    static void         program_page();
    static void         update_program_info();
    static uint16_t     get_program_crc(uint16_t length);
};
