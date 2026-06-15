/* licensed under GPL v3.0 see https://github.com/ZDev22/ZEngine/blob/main/LICENSE for current license

An example implementation on how to init and use zengine, as well as a few zdeps!

#define FPS_CAP 60.f // set the framerate, dont define for no FPS cap
*/

#define G29_IMPLEMENTATION
#define G29_UNCAP
#include "G29.h"

_Bool G29Input = 1;

#include <pthread.h>

void* input(void* arg) {
    initG29();
    G29disableForces();

    pthread_t G29Rumble;
    pthread_create(&G29Rumble, NULL, G29rumble, NULL);

    while (G29Input) {
        G29update();
    }
    return NULL;
}

int main(void) {
    /* init G29 */
    pthread_t G29Input;
    pthread_create(&G29Input, NULL, input, NULL);

    while (1) {
        // your code here
    }

    G29Input = 0; // tell steering wheel threads to stop
    /* deinit steering wheel */
    deinitG29();
}
