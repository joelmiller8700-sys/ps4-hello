#include <orbis/types.h>
#include <orbis/libkernel.h>
#include <orbis/pad.h>

int buttonHoldTimer = 0;
bool isMenuOpen = false;

void CheckControllerInput() {
    OrbisPadData padData;
    if (scePadReadState(0, &padData) == 0) {
        bool l1Pressed = (padData.buttons & ORBIS_PAD_BUTTON_L1) != 0;
        bool r1Pressed = (padData.buttons & ORBIS_PAD_BUTTON_R1) != 0;

        if (l1Pressed && r1Pressed) {
            buttonHoldTimer++;
            if (buttonHoldTimer >= 120 && !isMenuOpen) {
                isMenuOpen = true;
                buttonHoldTimer = 0;
            }
        } else {
            buttonHoldTimer = 0;
        }
    }
}
