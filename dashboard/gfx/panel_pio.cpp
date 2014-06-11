#include "panel_out.h"
#include "lpc111x.h"

#define GPIO_GPIO0_BITS(_x)     (*((REG32 *) (GPIO_GPIO0_BASE + ((_x) << 2))))
#define GPIO_GPIO1_BITS(_x)     (*((REG32 *) (GPIO_GPIO1_BASE + ((_x) << 2))))

#define GPIO_RGB_BITS       GPIO_GPIO0_BITS(GPIO_IO_P0 | \
                                            GPIO_IO_P1 | \
                                            GPIO_IO_P2 | \
                                            GPIO_IO_P6 | \
                                            GPIO_IO_P7 | \
                                            GPIO_IO_P8)

#define GPIO_ADDR_BITS      GPIO_GPIO0_BITS(GPIO_IO_P3 | GPIO_IO_P9 | GPIO_IO_P11 | GPIO_IO_P8)

#define GPIO_CLK_BITS       GPIO_GPIO1_BITS(GPIO_IO_P4)
#define GPIO_LAT_BITS       GPIO_GPIO1_BITS(GPIO_IO_P5)
#define GPIO_OE_BITS        GPIO_GPIO1_BITS(GPIO_IO_P8)
#define GPIO_CTL_BITS       GPIO_GPIO1_BITS(GPIO_IO_P4 | GPIO_IO_P5 | GPIO_IO_P8)

PanelV2PIO::PanelV2PIO()
{
    SCB_SYSAHBCLKCTRL |= SCB_SYSAHBCLKCTRL_GPIO | SCB_SYSAHBCLKCTRL_IOCON;

    IOCON_nRESET_PIO0_0 = IOCON_nRESET_PIO0_0_FUNC_GPIO;            // R1
    IOCON_PIO0_1 = IOCON_PIO0_1_FUNC_GPIO;                          // G1
    IOCON_PIO0_2 = IOCON_PIO0_2_FUNC_GPIO;                          // B1
    IOCON_PIO0_6 = IOCON_PIO0_6_FUNC_GPIO;                          // R2
    IOCON_PIO0_7 = IOCON_PIO0_7_FUNC_GPIO;                          // G2
    IOCON_PIO0_8 = IOCON_PIO0_8_FUNC_GPIO;                          // B2 / D
    IOCON_PIO0_3 = IOCON_PIO0_3_FUNC_GPIO;                          // A
    IOCON_PIO0_9 = IOCON_PIO0_9_FUNC_GPIO;                          // B
    IOCON_JTAG_TDI_PIO0_11 = IOCON_JTAG_TDI_PIO0_11_FUNC_GPIO |     // C
                             IOCON_JTAG_TDI_PIO0_11_ADMODE_DIGITAL;

    unsigned p0bits = _bR1 |
                      _bG1 |
                      _bB1 |
                      _bR2 |
                      _bG2 |
                      _bB2 |
                      _bA  |
                      _bB  |
                      _bC;
                      
    GPIO_GPIO0DATA &= ~p0bits;
    GPIO_GPIO0DIR |= p0bits;

    IOCON_PIO1_4 = IOCON_PIO1_4_FUNC_GPIO |                         // CLK
                   IOCON_PIO1_4_ADMODE_DIGITAL;
    IOCON_PIO1_5 = IOCON_PIO1_5_FUNC_GPIO;                          // LAT
    IOCON_PIO1_8 = IOCON_PIO1_8_FUNC_GPIO;                          // OE

    unsigned p1bits = _bCLK |
                      _bLAT |
                      _bOE;

    GPIO_GPIO1DATA &= ~p1bits;
    line_off();
    GPIO_GPIO1DIR |= p1bits;
}

void
PanelV2PIO::line_off()
{
        GPIO_OE_BITS = _bOE;
}

void
PanelV2PIO::line_update(unsigned row, unsigned slot, FrameBuffer &buffer)
{
        line_off();

        unsigned row_address = row * buffer.columns();
        const unsigned offset = buffer.rows() / 2 * buffer.columns();

//        for (unsigned col = 0; col < buffer.columns(); col++) {
//                
//                unsigned low_slice = buffer.subCell(row_address + col).getRGB().slice(slot);
//                unsigned high_slice = buffer.subCell(row_address + offset + col).getRGB().slice(slot);
//
//                GPIO_CLK_BITS = 0;                              // CLK low
//                GPIO_RGB_BITS = low_slice | (high_slice << 6);  // present data
//                GPIO_CLK_BITS = _bCLK;                          // latch on rising edge
//
//        }

        // ~33us for 32 columns
        for (unsigned col = 0; col < buffer.columns(); col += Cell::stride()) {
            uint32_t low_cell = buffer.cell(row_address + col).raw();
            uint32_t high_cell = buffer.cell(row_address + offset + col).raw();

            // ~42 cycles per iteration
            for (unsigned subcol = 0; subcol < 8; subcol++) {
                GPIO_CLK_BITS = 0;                              // CLK low
                GPIO_RGB_BITS = palette[low_cell & 0xf].slice(slot) | (palette[high_cell & 0xf].slice(slot) << 6);
                low_cell >>= 4;
                high_cell >>= 4;
                GPIO_CLK_BITS = _bCLK;                          // latch on rising edge
            }
        }

        GPIO_ADDR_BITS = ((row & 1) ? _bA : 0) |                // set row address
                         ((row & 2) ? _bB : 0) | 
                         ((row & 4) ? _bC : 0) |
                         ((row & 8) ? _bD : 0);

        GPIO_LAT_BITS = _bLAT;                                  // lat high to latch row
        GPIO_CTL_BITS = 0;                                      // oe, clk, lat low to enable display
}


