#pragma once

#include <stdint.h>
#include "common/protocol.h"

class LINDev
{
public:
	LINDev(unsigned bitrate);

	static void	do_interrupt();

	void		interrupt();

	bool		warnSummary	= false;
	bool		errorSummary	= false;

	bool		ttLeftTurn	= false;
	bool		ttRightTurn	= false;
	bool		ttLowBeam	= false;
	bool		ttHighBeam	= false;

	unsigned	roadSpeed	= 0;
	unsigned	engineRPM	= 0;

private:
	enum State {
		waitBreak,
		waitSynch,
		waitHeader,
		waitResponse
	};

	uint8_t		_buf[9];
	unsigned	_receivedLen	= 0;
	unsigned	_waitLen	= 0;
	uint8_t		_fid		= LIN::kFIDNone;
	State		_state		= waitBreak;

	void		headerReceived();
	void		responseReceived();

	bool		frameBit(uint8_t index) const { return _buf[index / 8] & (1 << (index % 8)); }
};
