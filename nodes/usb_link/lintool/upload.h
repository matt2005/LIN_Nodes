/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */
#pragma once

#include <stdint.h>
#include <stdexcept>

#include "firmware.h"
#include "exceptions.h"

namespace Upload
{
EXCEPTION(Exception, ExProtocol);
EXCEPTION(Exception, ExBadAddress);
EXCEPTION(Exception, ExLink);
EXCEPTION(Exception, ExCRCError);
EXCEPTION(Exception, ExVerifyError);

extern void upload(Firmware *fw, bool readback = false);
} // namespace Upload
