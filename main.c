#define G29_IMPLEMENTATION
#define G29_UNCAP
#include "G29.h"

_Bool G29Input = 1;

#include <pthread.h>

void* input(void* arg) {
    initG29();

    if (G29Initialized) {
        G29disableForces();

        pthread_t G29Rumble;
        pthread_create(&G29Rumble, NULL, G29rumble, NULL);

        while (G29Input) {
            G29update();
        }
    }
    return NULL;
}

int main(void) {
    /* init G29 */
    pthread_t G29Input;
    pthread_create(&G29Input, NULL, input, NULL);

    while (1) {
        // G29State.steering -> -32768 to 32767 based on where the wheel is turned
        // G29State.throttle -> 255 fully raised, 0 fully pressed
        // G29State.brake -> 255 fully raised, 0 fully pressed
        // G29State.clutch -> 255 fully raised, 0 fully pressed
        //
        // G29State.X -> 1 = pressed, 0 = not pressed
        // G29State.square -> 1 = pressed, 0 = not pressed
        // G29State.triangle -> 1 = pressed, 0 = not pressed
        // G29State.circle -> 1 = pressed, 0 = not pressed
        // G29State.L2 -> 1 = pressed, 0 = not pressed
        // G29State.R2 -> 1 = pressed, 0 = not pressed
        // G29State.L3 -> 1 = pressed, 0 = not pressed
        // G29State.R3 -> 1 = pressed, 0 = not pressed
        // G29State.DPadUp -> 1 = pressed, 0 = not pressed
        // G29State.DPadDown -> 1 = pressed, 0 = not pressed
        // G29State.DPadLeft -> 1 = pressed, 0 = not pressed
        // G29State.DPadRight -> 1 = pressed, 0 = not pressed
        // G29State.dial -> 1 = pressed, 0 = not pressed
        // G29State.plus -> 1 = pressed, 0 = not pressed
        // G29State.minus -> 1 = pressed, 0 = not pressed
        // G29State.leftPaddle -> 1 = pressed, 0 = not pressed
        // G29State.rightPaddle -> 1 = pressed, 0 = not pressed
        // G29State.share -> 1 = pressed, 0 = not pressed
        // G29State.options -> 1 = pressed, 0 = not pressed
        // G29State.PS -> 1 = pressed, 0 = not pressed
    }

    G29Input = 0; // tell steering wheel threads to stop
    /* deinit steering wheel */
    if (G29Initialized) {
        /* deinit steering wheel */
        deinitG29();
    }
}
