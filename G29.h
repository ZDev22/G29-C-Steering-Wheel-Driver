/* G29.h is an open-source single-header C library for the logitech G29 steering wheel, featuring button detection, forces, auto-centering & rumbling
 * Converted to C with optimizations and tweaks from this library: https://github.com/misarb/G29cpp
 * Some data array commands stolen... i mean borrowed from https://github.com/nightmode/logitech-g29
 *
 *
 * #define G29_IMPLEMENTATION        // define in one C file to include src
 * #define G29_UNCAP                 // uncaps limits set to protect health of wheel
 * #define G29_DEBUG                 // prints debug info
 *
 * #define G29_DISABLE_RUMBLE        // disables rumbling functions and memory
 *
 * #define G29_DISABLE_STEERING      // disables steering button checks
 * #define G29_DISABLE_THROTTLE      // disables throttle button checks
 * #define G29_DISABLE_BRAKE         // disables brake button checks
 * #define G29_DISABLE_CLUTCH        // disables clutch button checks
 * #define G29_DISABLE_X             // disables X button checks
 * #define G29_DISABLE_SQUARE        // disables square button checks
 * #define G29_DISABLE_TRIANGLE      // disables triangle button checks
 * #define G29_DISABLE_CIRCLE        // disables circle button checks
 * #define G29_DISABLE_L2            // disables L2 button checks
 * #define G29_DISABLE_R2            // disables R2 button checks
 * #define G29_DISABLE_L3            // disables L3 button checks
 * #define G29_DISABLE_R3            // disables R3 button checks
 * #define G29_DISABLE_DPADUP        // disables dpadup button checks
 * #define G29_DISABLE_DPADDOWN      // disables dpaddown button checks
 * #define G29_DISABLE_DPADLEFT      // disables dpadleft button checks
 * #define G29_DISABLE_DPADRIGHT     // disables dpadright button checks
 * #define G29_DISABLE_DIAL          // disables dial button checks
 * #define G29_DISABLE_PLUS          // disables plus button checks
 * #define G29_DISABLE_MINUS         // disables minus button checks
 * #define G29_DISABLE_LEFTPADDLE    // disables left paddle button checks
 * #define G29_DISABLE_RIGHTPADDLE   // disables right paddle button checks
 * #define G29_DISABLE_SHARE         // disables share button checks
 * #define G29_DISABLE_OPTION        // disables option button checks
 * #define G29_DISABLE_PS            // disables PS button checks
 */

#ifndef G29_DRIVER_H
#define G29_DRIVER_H

#ifndef _GNU_SOURCE
    #define _GNU_SOURCE
#endif
#ifndef _POSIX_C_SOURCE
    #define _POSIX_C_SOURCE 199309L
#endif

#ifdef G29_DISABLE_CLUTCH
    #ifdef G29_DISABLE_BRAKE
        #ifdef G29_DISABLE_THROTTLE
            #ifdef G29_DISABLE_STEERING
                #if defined(G29_DISABLE_PS) && defined(G29_DISABLE_MINUS) && defined(G29_DISABLE_DIAL)
                    #ifdef G29_DISABLE_PLUS
                        #ifdef defined(G29_DISABLE_LEFTPADDLE) && defined(G29_DISABLE_RIGHTPADDLE) && defined(G29_DISABLE_SHARE) && defined(G29_DISABLE_OPTION) && defined(G29_DISABLE_L2) && defined(G29_DISABLE_R2) && defined(G29_DISABLE_L3) && defined(G29_DISABLE_R3)
                            #ifdef defined(G29_DISABLE_X) && defined(G29_DISABLE_SQUARE) && defined(G29_DISABLE_TRIANGLE) && defined(G29_DISABLE_CIRCLE) && defined(G29_DISABLE_DPADUP) && defined(G29_DISABLE_DPADDOWN) && defined(G29_DISABLE_DPADLEFT) && defined(G29_DISABLE_DPADRIGHT)
                                #define G29_CACHE_SIZE 0
                            #else
                                #define G29_CACHE_SIZE 1
                            #endif
                        #else
                            #define G29_CACHE_SIZE 2
                        #endif
                    #else
                        #define G29_CACHE_SIZE 3
                    #endif
                #else
                    #define G29_CACHE_SIZE 4
                #endif
            #else
                #define G29_CACHE_SIZE 6
            #endif
        #else
            #define G29_CACHE_SIZE 7
        #endif
    #else
        #define G29_CACHE_SIZE 8
    #endif
#else
    #define G29_CACHE_SIZE 9
#endif

#include <hidapi/hidapi.h>
#include <string.h>
#include <time.h>

typedef struct G29state {
    #ifndef G29_DISABLE_STEERING
        unsigned char steering;
    #endif
    #ifndef G29_DISABLE_THROTTLE
        unsigned char throttle;
    #endif
    #ifndef G29_DISABLE_BRAKE
        unsigned char brake;
    #endif
    #ifndef G29_DISABLE_CLUTCH
        unsigned char clutch;
    #endif

    #ifndef G29_DISABLE_X
        _Bool X;
    #endif
    #ifndef G29_DISABLE_SQUARE
        _Bool square;
    #endif
    #ifndef G29_DISABLE_TRIANGLE
        _Bool triangle;
    #endif
    #ifndef G29_DISABLE_CIRCLE
        _Bool circle;
    #endif

    #ifndef G29_DISABLE_L2
        _Bool L2;
    #endif
    #ifndef G29_DISABLE_R2
        _Bool R2;
    #endif
    #ifndef G29_DISABLE_L3
        _Bool L3;
    #endif
    #ifndef G29_DISABLE_R3
        _Bool R3;
    #endif

    #ifndef G29_DISABLE_DPADUP
        _Bool DPadUp;
    #endif
    #ifndef G29_DISABLE_DPADDOWN
        _Bool DPadDown;
    #endif
    #ifndef G29_DISABLE_DPADLEFT
        _Bool DPadLeft;
    #endif
    #ifndef G29_DISABLE_DPADRIGHT
        _Bool DPadRight;
    #endif

    #ifndef G29_DISABLE_DIAL
        _Bool dial;
    #endif

    #ifndef G29_DISABLE_PLUS
        _Bool plus;
    #endif
    #ifndef G29_DISABLE_MINUS
        _Bool minus;
    #endif

    #ifndef G29_DISABLE_LEFTPADDLE
        _Bool leftPaddle;
    #endif
    #ifndef G29_DISABLE_RIGHTPADDLE
        _Bool rightPaddle;
    #endif

    #ifndef G29_DISABLE_SHARE
        _Bool share;
    #endif
    #ifndef G29_DISABLE_OPTION
        _Bool options;
    #endif
    #ifndef G29_DISABLE_PS
        _Bool PS;
    #endif
} G29state;

void initG29(void); // initialize the device
void deinitG29(void); // deinitialize the device
void G29reset(void); // reset the device if it's acting up, may break connection
void G29forceConstant(unsigned char val); // push the steering wheel a certain direction, 0 = left <-----, 255 = right ----->
void G29forceFriction(unsigned char val); // apply friction to the wheel to make it harder to turn
void G29setAutocenter(unsigned char strength, unsigned char rate); // auto center the wheel
void G29setRange(unsigned short range); // set the wheel rotation range
void G29update(void); // update button and pedal states
void G29disableForces(void); // disable all forces (including rumbling), basically resets the wheel

extern G29state G29State;
extern _Bool G29Initialized;

#ifndef G29_DISABLE_RUMBLE
void G29setRumble(float timer, float minRumble, float maxRumble, float deterioration, float interpolation); // initialize rumble parameters, call this whenever you like
void G29tickRumble(float deltaTime); // tick rumbling, overwrites G29forceConstant()
void* G29rumble(void* arg); // rumbling is slow, so call this

extern _Bool G29RumbleEnabled;
#endif


#ifdef G29_IMPLEMENTATION

#include <unistd.h>

#ifdef G29_DEBUG
    #include <stdio.h>
    #define G29_PRINT(x, ...) printf(x, ##__VA_ARGS__)
#else
    #define G29_PRINT(x, ...)
#endif

static hid_device* device; // hid device
static unsigned char cache[G29_CACHE_SIZE]; // raw input data

G29state G29State; // current state of analog inputs.
_Bool G29Initialized = 0; // boolean to check if G29 successfully initialized

#ifndef G29_DISABLE_RUMBLE
static float G29RumbleTimer = 0.f;
static float G29MaxRumbleTimer = 0.f;
static float G29MinRumble = 127.f;
static float G29MaxRumble = 127.f;
static float G29CurrentRumble = 127.f;
static float G29Deterioration = 0.f;
static float G29Interpolation = 25.f;
static _Bool G29RumbleState = 0;
static _Bool G29Rumbling = 1;
_Bool G29RumbleEnabled = 0;
#endif

void initG29(void) {
    if (hid_init() != 0) {
        G29_PRINT("Failed to initialize HIDAPI");
    }

    device = hid_open(0x046d, 0xc24f, NULL);
    if (!device) {
        G29_PRINT("Failed to open G29 device");
        hid_exit();
        return;
    }

    memset(cache, 0, G29_CACHE_SIZE);
    G29State.steering = 255;
    G29State.throttle = 255;
    G29State.clutch = 255;
    G29State.brake = 255;

    G29Initialized = 1;
}

void deinitG29(void) {
#ifndef G29_DISABLE_RUMBLE
    G29Rumbling = 0;
#endif
    G29disableForces();
    if (device) {
        hid_close(device);
    }
    hid_exit();
}

void G29reset(void) {
    const unsigned char msg1[7] = {0xf8, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00};
    const unsigned char msg2[8] = {0xf8, 0x09, 0x05, 0x01, 0x01, 0x00, 0x00, 0x00};

    hid_write(device, msg1, 7);
    hid_write(device, msg2, 8);
}

void G29forceConstant(unsigned char val) { /* 0 = left <-----, 255 = right -----> */
    const unsigned char msg[7] = {0x11, 0x00, val, 0x00, 0x00, 0x00, 0x00};
    hid_write(device, msg, 7);
}

void G29forceFriction(unsigned char val) {
    const unsigned char msg[7] = {0x21, 0x02, val, 0x00, val, 0x00, 0x00};
    hid_write(device, msg, 7);
}

void G29setAutocenter(unsigned char strength, unsigned char rate) {
    const unsigned char msg1[7] = {0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    const unsigned char msg2[8] = {0xfe, 0x0d, strength, strength, rate, 0x00, 0x00, 0x00};

    hid_write(device, msg1, 7);
    hid_write(device, msg2, 8);
}

void G29setRange(unsigned short range) {
    const unsigned char msg[7] = {0xf8, 0x81, range & 0x00ff, (range & 0xff00) >> 8, 0x00, 0x00, 0x00};
    hid_write(device, msg, 7);
}

void G29update(void) {
    hid_read(device, cache, G29_CACHE_SIZE);

#ifndef G29_DISABLE_STEERING
    G29State.steering = (unsigned short)cache[4] | ((unsigned short)cache[5] << 8);
    G29_PRINT("steering: %hhu ", G29State.steering);
#endif
#ifndef G29_DISABLE_THROTTLE
    G29State.throttle = cache[6];
    G29_PRINT("throttle: %hhu ", G29State.throttle);
#endif
#ifndef G29_DISABLE_BRAKE
    G29State.brake = cache[7];
    G29_PRINT("brake: %hhu ", G29State.brake);
#endif
#ifndef G29_DISABLE_CLUTCH
    G29State.clutch = cache[8];
    G29_PRINT("clutch: %hhu ", G29State.clutch);
#endif

#ifndef G29_DISABLE_X
    G29State.X = (cache[0] & 0x18) == 0x18;
    G29_PRINT("X: %hhu ", G29State.X);
#endif
#ifndef G29_DISABLE_SQUARE
    G29State.square = (cache[0] & 0x28) == 0x28;
    G29_PRINT("square: %hhu ", G29State.square);
#endif
#ifndef G29_DISABLE_TRIANGLE
    G29State.triangle = (cache[0] & 0x88) == 0x88;
    G29_PRINT("triangle: %hhu ", G29State.triangle);
#endif
#ifndef G29_DISABLE_CIRCLE
    G29State.circle = (cache[0] & 0x48) == 0x48;
    G29_PRINT("circle: %hhu ", G29State.circle);
#endif

#ifndef G29_DISABLE_L2
    G29State.L2 = (cache[1] & 0x08) == 0x08;
    G29_PRINT("L2: %hhu ", G29State.L2);
#endif
#ifndef G29_DISABLE_R2
    G29State.R2 = (cache[1] & 0x04) == 0x04;
    G29_PRINT("R2: %hhu ", G29State.R2);
#endif
#ifndef G29_DISABLE_L3
    G29State.L3 = (cache[1] & 0x80) == 0x80;
    G29_PRINT("L3: %hhu ", G29State.L3);
#endif
#ifndef G29_DISABLE_R3
    G29State.R3 = (cache[1] & 0x40) == 0x40;
    G29_PRINT("R3: %hhu ", G29State.R3);
#endif

#ifndef G29_DISABLE_DPADUP
    G29State.DPadUp = (cache[0] & 0x0F) == 0x00;
    G29_PRINT("DPadUp: %hhu ", G29State.DPadUp);
#endif
#ifndef G29_DISABLE_DPADDOWN
    G29State.DPadDown = (cache[0] & 0x0F) == 0x04;
    G29_PRINT("DPadDown: %hhu ", G29State.DPadDown);
#endif
#ifndef G29_DISABLE_DPADLEFT
    G29State.DPadLeft = (cache[0] & 0x0F) == 0x06;
    G29_PRINT("DPadLeft: %hhu ", G29State.DPadLeft);
#endif
#ifndef G29_DISABLE_DPADRIGHT
    G29State.DPadRight = (cache[0] & 0x0F) == 0x02;
    G29_PRINT("DPadRight: %hhu ", G29State.DPadRight);
#endif

#ifndef G29_DISABLE_DIAL
    G29State.dial = (cache[3] & 0x08) == 0x08;
    G29_PRINT("dial: %hhu ", G29State.dial);
#endif

#ifndef G29_DISABLE_PLUS
    G29State.plus = (cache[2] & 0x80) == 0x80;
    G29_PRINT("plus: %hhu ", G29State.plus);
#endif
#ifndef G29_DISABLE_MINUS
    G29State.minus = (cache[3] & 0x01) == 0x01;
    G29_PRINT("minus: %hhu ", G29State.minus);
#endif

#ifndef G29_DISABLE_LEFTPADDLE
    G29State.leftPaddle = (cache[1] & 0x02) == 0x02;
    G29_PRINT("leftPaddle: %hhu ", G29State.leftPaddle);
#endif
#ifndef G29_DISABLE_RIGHTPADDLE
    G29State.rightPaddle = (cache[1] & 0x01) == 0x01;
    G29_PRINT("rightPaddle: %hhu ", G29State.rightPaddle);
#endif

#ifndef G29_DISABLE_SHARE
    G29State.share = (cache[1] & 0x10) == 0x10;
    G29_PRINT("share: %hhu ", G29State.share);
#endif
#ifndef G29_DISABLE_OPTION
    G29State.options = (cache[1] & 0x20) == 0x20;
    G29_PRINT("options: %hhu ", G29State.options);
#endif
#ifndef G29_DISABLE_PS
    G29State.PS = (cache[3] & 0x10) == 0x10;
    G29_PRINT("PS: %hhu ", G29State.PS);
#endif
    G29_PRINT("\n");
}

void G29disableForces(void) {
#ifndef G29_DISABLE_RUMBLE
    G29RumbleEnabled = 0;
#endif

    const unsigned char msg1[7] = {0xf5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    const unsigned char msg2[7] = {0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    const unsigned char msg3[7] = {0xf3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    const unsigned char msg4[7] = {0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    hid_write(device, msg1, 7);
    hid_write(device, msg2, 7);
    hid_write(device, msg3, 7);
    hid_write(device, msg4, 7);
}

#ifndef G29_DISABLE_RUMBLE

void G29setRumble(float timer, float minRumble, float maxRumble, float deterioration, float interpolation) {
#ifndef G29_UNCAP
    /* make sure not to kill the wheel */
    if (timer < .01f) { timer = .01f; }
    if (minRumble < 5.f) { minRumble = 5.f; }
    if (maxRumble > 250.f) { maxRumble = 250.f; }
#endif

    G29MaxRumbleTimer = timer;
    G29MinRumble = minRumble;
    G29MaxRumble = maxRumble;
    G29Deterioration = deterioration;
    G29Interpolation = interpolation;
    G29RumbleEnabled = 1;
}

void G29tickRumble(float deltaTime) {
    if (G29RumbleEnabled) {
        G29CurrentRumble += ((G29RumbleState ? G29MinRumble : G29MaxRumble) - G29CurrentRumble) * G29Interpolation * deltaTime;
        G29forceConstant((unsigned char)G29CurrentRumble);

        G29RumbleTimer -= deltaTime;
        if (G29RumbleTimer < 0.f) {
            G29RumbleTimer += G29MaxRumbleTimer;
            G29RumbleState = !G29RumbleState;
        }

        G29MinRumble += G29Deterioration * deltaTime;
        G29MaxRumble -= G29Deterioration * deltaTime;
        if (G29MinRumble > 127.f) { G29MinRumble = 127.f; }
        if (G29MaxRumble < 127.f) { G29MaxRumble = 127.f; }
        if (G29MinRumble == 127.f && G29MaxRumble == 127.f) { G29RumbleEnabled = 0; }
    }
}

void* G29rumble(void* args) {
    struct timespec G29fpsTime;
    struct timespec G29fpsLastTime;
    float G29deltaTime;

    clock_gettime(CLOCK_MONOTONIC, &G29fpsLastTime);
    while (G29Rumbling) {
        clock_gettime(CLOCK_MONOTONIC, &G29fpsTime);
        G29deltaTime = (float)(G29fpsTime.tv_sec - G29fpsLastTime.tv_sec) + (float)(G29fpsTime.tv_nsec - G29fpsLastTime.tv_nsec) / 1000000000.f;
        G29fpsLastTime.tv_nsec = G29fpsTime.tv_nsec;
        G29fpsLastTime.tv_sec = G29fpsTime.tv_sec;
        G29tickRumble(G29deltaTime);
    }
    return NULL;
}

#endif // G29_DISABLE_RUMBLE

#endif // G29_IMPLEMENTATION
#endif // G29_DRIVER_H
