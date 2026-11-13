#include <orbis/libkernel.h>
#include <orbis/Pad.h>
#include <orbis/UserService.h>

int holdTimer = 0;
bool menuVisible = false;
int selectedOption = 0;
int currentFPS = 30;

int main(void) {
    sceUserServiceInitialize(NULL);
    scePadInit();

    OrbisUserServiceUserId userId;
    sceUserServiceGetInitialUser(&userId);
    int padHandle = scePadOpen(userId, 0, 0, NULL);

    OrbisPadData padData;
    uint32_t prevButtons = 0;

    while (1) {
        if (scePadReadState(padHandle, &padData) == 0) {
            bool l1 = (padData.buttons & ORBIS_PAD_BUTTON_L1) != 0;
            bool r1 = (padData.buttons & ORBIS_PAD_BUTTON_R1) != 0;

            if (l1 && r1) {
                holdTimer++;
                if (holdTimer >= 120) {
                    menuVisible = !menuVisible;
                    holdTimer = 0;
                }
            } else {
                holdTimer = 0;
            }

            if (menuVisible) {
                if ((padData.buttons & ORBIS_PAD_BUTTON_DOWN) && !(prevButtons & ORBIS_PAD_BUTTON_DOWN)) {
                    selectedOption = (selectedOption + 1) % 3;
                }
                if ((padData.buttons & ORBIS_PAD_BUTTON_UP) && !(prevButtons & ORBIS_PAD_BUTTON_UP)) {
                    selectedOption = (selectedOption + 2) % 3;
                }
                if ((padData.buttons & ORBIS_PAD_BUTTON_CROSS) && !(prevButtons & ORBIS_PAD_BUTTON_CROSS)) {
                    if(selectedOption == 0) currentFPS = 20;
                    if(selectedOption == 1) currentFPS = 25;
                    if(selectedOption == 2) currentFPS = 40;
                }
            }
            prevButtons = padData.buttons;
        }
        sceKernelUsleep(1000000 / currentFPS);
    }
    return 0;
}
