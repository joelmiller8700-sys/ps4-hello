#include <orbis/libkernel.h>
#include <orbis/Pad.h>
#include <orbis/UserService.h>

// إعدادات القائمة
int holdTimer = 0;
bool menuVisible = false;
int selectedOption = 0; // 0 = 20 FPS, 1 = 25 FPS, 2 = 40 FPS
int fpsValues[3] = {20, 25, 40};
int currentFPS = 30;

int main(void) {
    // تهيئة خدمة المستخدم ويد التحكم
    sceUserServiceInitialize(NULL);
    scePadInit();

    OrbisUserServiceUserId userId;
    sceUserServiceGetInitialUser(&userId);
    int padHandle = scePadOpen(userId, 0, 0, NULL);

    OrbisPadData padData;
    uint32_t prevButtons = 0;

    // الحلقة الرئيسية للتطبيق
    while (1) {
        if (scePadReadState(padHandle, &padData) == 0) {
            bool l1 = (padData.buttons & ORBIS_PAD_BUTTON_L1) != 0;
            bool r1 = (padData.buttons & ORBIS_PAD_BUTTON_R1) != 0;

            // فحص الضغط المستمر على L1 + R1 لمدة ثانيتين (حوالي 120 دورة)
            if (l1 && r1) {
                holdTimer++;
                if (holdTimer >= 120) {
                    menuVisible = !menuVisible; // فتح أو إغلاق القائمة
                    holdTimer = 0;
                }
            } else {
                holdTimer = 0;
            }

            // التحكم داخل القائمة إذا كانت مفتوحة
            if (menuVisible) {
                if ((padData.buttons & ORBIS_PAD_BUTTON_DOWN) && !(prevButtons & ORBIS_PAD_BUTTON_DOWN)) {
                    selectedOption = (selectedOption + 1) % 3;
                }
                if ((padData.buttons & ORBIS_PAD_BUTTON_UP) && !(prevButtons & ORBIS_PAD_BUTTON_UP)) {
                    selectedOption = (selectedOption + 2) % 3;
                }
                if ((padData.buttons & ORBIS_PAD_BUTTON_CROSS) && !(prevButtons & ORBIS_PAD_BUTTON_CROSS)) {
                    currentFPS = fpsValues[selectedOption];
                }
            }
            prevButtons = padData.buttons;
        }

        // تأخير بسيط لمحاكاة معدل الإطارات المختار
        sceKernelUsleep(1000000 / currentFPS);
    }

    return 0;
}
