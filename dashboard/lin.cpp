/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include "lin.h"

#include "lpc111x.h"
#include "common/lin_protocol.h"
//#include "common/protocol.h"

extern "C" void	UART_Handler(void) __attribute__((used));

void
UART_Handler(void)
{
    gLIN.interrupt();
}

LINDev::LINDev(unsigned bitrate) :
    warnSummary(false),
    errorSummary(false),
    linkUp(false),
    _perfFrames("LIN FRAMES")
{
    NVIC_DisableIRQ(UART_IRQn);

    // pin configuration
    SCB_SYSAHBCLKCTRL |= SCB_SYSAHBCLKCTRL_IOCON;
    IOCON_PIO1_6 &= ~IOCON_PIO1_6_FUNC_MASK;
    IOCON_PIO1_6 |= IOCON_PIO1_6_FUNC_UART_RXD;
    IOCON_PIO1_7 &= ~IOCON_PIO1_7_FUNC_MASK;
    IOCON_PIO1_7 |= IOCON_PIO1_7_FUNC_UART_TXD;

    // now we can enable the UART clock
    SCB_SYSAHBCLKCTRL |= SCB_SYSAHBCLKCTRL_UART;
    SCB_UARTCLKDIV = SCB_UARTCLKDIV_DIV1;     /* divided by 1 */

    // 8n1 with DLAB enabled
    UART_U0LCR = (UART_U0LCR_Word_Length_Select_8Chars |
                  UART_U0LCR_Stop_Bit_Select_1Bits |
                  UART_U0LCR_Parity_Disabled |
                  UART_U0LCR_Parity_Select_OddParity |
                  UART_U0LCR_Break_Control_Disabled |
                  UART_U0LCR_Divisor_Latch_Access_Enabled);

    // compute bitrate (rounds towards fast)
    unsigned div = (((CFG_CPU_CCLK * SCB_SYSAHBCLKDIV) / SCB_UARTCLKDIV) / 16) / bitrate;

    UART_U0DLM = div / 256;
    UART_U0DLL = div % 256;
    UART_U0FDR = 0x10;

    // 8n1 with DLAB disabled
    UART_U0LCR = (UART_U0LCR_Word_Length_Select_8Chars |
                  UART_U0LCR_Stop_Bit_Select_1Bits |
                  UART_U0LCR_Parity_Disabled |
                  UART_U0LCR_Parity_Select_OddParity |
                  UART_U0LCR_Break_Control_Disabled |
                  UART_U0LCR_Divisor_Latch_Access_Disabled);

    // FIFO on and reset
    UART_U0FCR = (UART_U0FCR_FIFO_Enabled |
                  UART_U0FCR_Rx_FIFO_Reset |
                  UART_U0FCR_Tx_FIFO_Reset);

    // clear any pending line status
    (void)UART_U0LSR;

    // enable interrupts
    UART_U0IER = UART_U0IER_RBR_Interrupt_Enabled | UART_U0IER_RLS_Interrupt_Enabled;
    NVIC_EnableIRQ(UART_IRQn);
}

void
LINDev::interrupt()
{
    unsigned iir = UART_U0IIR;
    unsigned reason = iir & UART_U0IIR_IntId_MASK;

    // do we have a line status update?
    if (reason == UART_U0IIR_IntId_RLS) {

        // any line status update will cause us to drop an in-progress frame
        _fid = kFrameIDNone;
        _receivedLen = 0;
        _waitLen = 0;

        // check what sort of update we got
        unsigned lsr = UART_U0LSR;

        if ((lsr & UART_U0LSR_BI_MASK) == UART_U0LSR_BI) {
            // it was a break, so we are now waiting for the synch character
            _state = waitSynch;

        } else {
            // some other sort of error, we are waiting for a break
            _state = waitBreak;
        }

        // discard the corresponding data
        (void)UART_U0RBR;

        // we're done here
        return;
    }

    // did we receive a character?
    if ((reason == UART_U0IIR_IntId_RDA) || (reason == UART_U0IIR_IntId_CTI)) {

        // grab the character
        uint8_t c = UART_U0RBR;

        if (_state == waitSynch) {
            if (c == 0x55) {
                // got synch, now wait for header
                _state = waitHeader;

            } else {
                // not expected, wait for the next frame
                _state = waitBreak;
            }

        } else if (_state == waitHeader) {

            // save the frame ID
            // XXX check parity
            _fid = c & 0x3f;

            // check what we want to do here
            header_received();

            // decide whether we want to collect the response or just wait for a break
            if (_waitLen > 0) {
                // XXX _waitLen++;			// collect CRC byte as well
                _receivedLen = 0;
                _state = waitResponse;

            } else {
                _state = waitBreak;
            }

        } else if (_state == waitResponse) {
            _buf[_receivedLen++] = c;

            if (_receivedLen == _waitLen) {
                // XXX check CRC

                // pass off to response handler
                response_received();

                // wait for the next frame
                _state = waitBreak;
            }
        }
    }

    if (reason == UART_U0IIR_IntId_THRE) {
        // XXX transmit handler
    }
}

void
LINDev::header_received()
{
    switch (_fid) {
    case kFrameIDRelays:
    case kFrameIDECUData:
        _waitLen = 8;
        break;

    default:
        break;
    }
}

void
LINDev::response_received()
{
    _perfFrames.count();

    switch (_fid) {
    case kFrameIDRelays:
        ttLeftTurn = frame_bit(kRelayIDLeftTurn);
        ttRightTurn = frame_bit(kRelayIDRightTurn);
        ttLowBeam = frame_bit(kRelayIDHeadLights) || frame_bit(kRelayIDLowBeam);
        ttHighBeam = frame_bit(kRelayIDHighBeam);
        linkUp = true;
        break;

    case kFrameIDECUData: {
        //LIN::ECUDataFrame f(_buf);

        //roadSpeed = f.roadSpeed();
        //engineRPM = f.RPM() * 30;
        //batteryVoltage = f.voltage();
        //waterTemperature = f.coolantTemperature();
        //oilPressure = f.oilPressure();
        //airFuelRatio = f.AFR();

        break;
    }

    default:
        break;
    }
}
