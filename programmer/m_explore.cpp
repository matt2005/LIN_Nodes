
#include "lin_protocol.h"

#include "m_explore.h"

namespace Menu
{

ExploreMode modeExplore;

////////////////////////////////////////////////////////////////////////////////
// Explore mode
//

void
ExploreMode::enter(Mode *from)
{
    gDisplay.clear();
    _node = LIN::kNADMaster;
    check();
}

Mode *
ExploreMode::action(Encoder::Event bp)
{
    switch (bp) {
//    case kButtonCancel:
//        return &_modeTop;

    case Encoder::kEventDown:
        if (_node > LIN::kNADMaster) {
            _node--;
            check();
        }
        break;

    case Encoder::kEventUp:
        if (_node < (LIN::kNADMaster + 15)) {
            _node++;
            check();
        }
        break;

    default:
        break;
    }

    return this;
}

void
ExploreMode::check()
{
    gDisplay.clear();
    gDisplay.move(0, 0);

//    LIN::Frame f = LIN::Frame::makeReadByIDRequest(_node, LIN::kRBIProductID);
//    if (!gMaster.doRequestResponse(f)) {
//        _present = false;
//        gDisplay.printf(PSTR("%u missing"), _node);
//    } else {
//        _present = true;
//        gDisplay.printf(PSTR("%u found"), _node);
//    }
}

} // namespace Menu
