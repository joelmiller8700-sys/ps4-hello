#include <orbis/libkernel.h>

int main(void) {
    sceKernelDebugOutText(0, "Hello from PS4 9.00\n");
    for (;;) {
        sceKernelSleep(1);
    }
    return 0;
}
