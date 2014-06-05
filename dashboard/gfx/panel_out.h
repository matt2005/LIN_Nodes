#pragma once

#include "graphics.h"

class Panel;

/* abstract panel output driver */
class PanelOut
{
public:
	virtual void	line_off() = 0;
	virtual void	line_update(unsigned row, unsigned slot, FrameBuffer &buffer) = 0;
};

class PanelV1PIO : public PanelOut
{
public:
	PanelV1PIO();
	virtual void	line_off() override;
	virtual void	line_update(unsigned row, unsigned slot, FrameBuffer &buffer) override;
};
