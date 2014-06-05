#include "panel_out.h"
#include "lpc111x.h"


PanelV1PIO::PanelV1PIO()
{
	SCB_SYSAHBCLKCTRL |= SCB_SYSAHBCLKCTRL_GPIO | SCB_SYSAHBCLKCTRL_IOCON;

	IOCON_nRESET_PIO0_0 = IOCON_nRESET_PIO0_0_FUNC_GPIO;		// R1
	IOCON_PIO0_2 = IOCON_PIO0_2_FUNC_GPIO;				// G1
	IOCON_PIO0_3 = IOCON_PIO0_3_FUNC_GPIO;				// G2
	IOCON_PIO0_6 = IOCON_PIO0_6_FUNC_GPIO;				// B2
	IOCON_PIO0_7 = IOCON_PIO0_7_FUNC_GPIO;				// B1
	IOCON_PIO0_8 = IOCON_PIO0_8_FUNC_GPIO;				// A
	IOCON_PIO0_9 = IOCON_PIO0_9_FUNC_GPIO;				// B
	IOCON_JTAG_TDI_PIO0_11 = IOCON_JTAG_TDI_PIO0_11_FUNC_GPIO |	// C
			IOCON_JTAG_TDI_PIO0_11_ADMODE_DIGITAL;

	unsigned p0bits = GPIO_IO_P0 |
			GPIO_IO_P2 |
			GPIO_IO_P3 |
			GPIO_IO_P6 |
			GPIO_IO_P7 |
			GPIO_IO_P8 |
			GPIO_IO_P9 |
			GPIO_IO_P11;

	GPIO_GPIO0DATA &= ~p0bits;
	GPIO_GPIO0DIR |= p0bits;


	IOCON_PIO1_4 = IOCON_PIO1_4_FUNC_GPIO |	IOCON_PIO1_4_ADMODE_DIGITAL; // R2
	IOCON_PIO1_5 = IOCON_PIO1_5_FUNC_GPIO;				// OE
	IOCON_PIO1_8 = IOCON_PIO1_8_FUNC_GPIO;				// CLK
	IOCON_PIO1_9 = IOCON_PIO1_9_FUNC_GPIO;				// LAT

	unsigned p1bits = GPIO_IO_P4 |
			GPIO_IO_P5 |
			GPIO_IO_P8 |
			GPIO_IO_P9;

	GPIO_GPIO1DATA &= ~p1bits;
	line_off();
	GPIO_GPIO1DIR |= p1bits;
}

void
PanelV1PIO::line_off()
{
	GPIO_GPIO1DATA |= GPIO_IO_P5;
}

// optimised low-slice lookup table
// XXX oddly, doing this for the high slab makes things slower
// XXX -O2 gives best results for gcc 4.7.4
static const unsigned v1lstab[] = {
	0		| 0		| 0,
	0		| 0		| GPIO_IO_P0,
	0		| GPIO_IO_P2	| 0,
	0		| GPIO_IO_P2	| GPIO_IO_P0,
	GPIO_IO_P7	| 0		| 0,
	GPIO_IO_P7	| 0		| GPIO_IO_P0,
	GPIO_IO_P7	| GPIO_IO_P2	| 0,
	GPIO_IO_P7	| GPIO_IO_P2	| GPIO_IO_P0,
};

void
PanelV1PIO::line_update(unsigned row, unsigned slot, FrameBuffer &buffer)
{
	line_off();

	unsigned abits = GPIO_GPIO0DATA & ~(GPIO_IO_P8 | GPIO_IO_P9 | GPIO_IO_P11);
	if (row & 1) {
		abits |= GPIO_IO_P8;
	}
	if (row & 2) {
		abits |= GPIO_IO_P9;
	}
	if (row & 4) {
		abits |= GPIO_IO_P11;
	}
	GPIO_GPIO0DATA = abits;

	unsigned low_address = row * buffer.columns();
	unsigned high_address = (row + buffer.rows() / 2) * buffer.columns();

	for (unsigned col = 0; col < buffer.columns(); col++) {
		
		unsigned low_slice = buffer.subCell(low_address++).getRGB().slice(slot);
		unsigned high_slice = buffer.subCell(high_address++).getRGB().slice(slot);

		unsigned p0bits = GPIO_GPIO0DATA & ~(GPIO_IO_P0 |		// data and
							GPIO_IO_P2 |
							GPIO_IO_P3 |
							GPIO_IO_P6 |
							GPIO_IO_P7);
		unsigned p1bits = GPIO_GPIO1DATA & ~(GPIO_IO_P4 | GPIO_IO_P8);	// clk bit all low

		p0bits |= v1lstab[low_slice];

//		if (low_slice & 1) {
//			p0bits |= GPIO_IO_P4;
//		}
//		if (low_slice & 2) {
//			p0bits |= GPIO_IO_P2;
//		}
//		if (low_slice & 4) {
//			p0bits |= GPIO_IO_P7;
//		}
		if (high_slice & 1) {
			p1bits |= GPIO_IO_P4;
		}
		if (high_slice & 2) {
			p0bits |= GPIO_IO_P3;
		}
		if (high_slice & 4) {
			p0bits |= GPIO_IO_P6;
		}

		GPIO_GPIO0DATA = p0bits;			// present data
		GPIO_GPIO1DATA = p1bits;			// ... to shift register inputs
		GPIO_GPIO1DATA = p1bits | GPIO_IO_P8;		// clk high to shift in
	}
	GPIO_GPIO1DATA |= GPIO_IO_P9;					// lat high to latch row
	GPIO_GPIO1DATA &= ~(GPIO_IO_P8 | GPIO_IO_P9 | GPIO_IO_P5);	// oe, clk, lat low to enable display
}
