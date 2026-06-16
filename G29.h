/* G29.h is an open-source single-header C library for the logitech G29 steering wheel, featuring button detection, forces, auto-centering & rumbling
 * Converted to C with optimizations and tweaks from this library: https://github.com/misarb/G29cpp
 * Some data array commands stolen... i mean borrowed from https://github.com/nightmode/logitech-g29
 *
 *
 * #define G29_IMPLEMENTATION // define in one C file to include src
 * #define G29_UNCAP          // uncaps limits set to protect health of wheel
 * #define G29_DEBUG          // prints debug info
 */

#ifndef G29_DRIVER_H
#define G29_DRIVER_H

#include <hidapi/hidapi.h>

typedef struct G29state {
    unsigned short steering;
    unsigned char throttle;
    unsigned char brake;
    unsigned char clutch;

    _Bool X;
    _Bool square;
    _Bool triangle;
    _Bool circle;

    _Bool L2;
    _Bool R2;
    _Bool L3;
    _Bool R3;

    _Bool DPadUp;
    _Bool DPadDown;
    _Bool DPadLeft;
    _Bool DPadRight;

    _Bool dial;

    _Bool plus;
    _Bool minus;

    _Bool leftPaddle;
    _Bool rightPaddle;

    _Bool share;
    _Bool options;
    _Bool PS;
} G29state;

void initG29(void); // initialize the device
void deinitG29(void); // deinitialize the device
void G29reset(void); // reset the device if it's acting up, may break connection
void G29forceConstant(unsigned char val); // push the steering wheel a certain direction, 0 = left <-----, 255 = right ----->
void G29forceFriction(unsigned char val); // apply friction to the wheel to make it harder to turn
void G29setAutocenter(unsigned char strength, unsigned char rate); // auto center the wheel
void G29setRange(unsigned short range); // set the wheel rotation range
void G29setRumble(float timer, float minRumble, float maxRumble, float deterioration, float interpolation); // initialize rumble parameters, call this whenever you like
void G29tickRumble(float deltaTime); // tick rumbling, overwrites G29forceConstant()
void* G29rumble(void* arg); // rumbling is slow, so call this
void G29update(void); // update button and pedal states
void G29disableForces(void); // disable all forces (including rumbling), basically resets the wheel

extern G29state G29State;
extern _Bool G29RumbleEnabled;


#ifdef G29_IMPLEMENTATION

#include <unistd.h>

#ifdef G29_DEBUG
    #include <stdio.h>
    #define G29_PRINT(x, ...) printf(x, ##__VA_ARGS__)
#else
    #define G29_PRINT(x, ...)
#endif

static hid_device* device; // hid device
static unsigned char cache[9]; // raw input data

G29state G29State; // current state of analog inputs.

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

void initG29(void) {
    if (hid_init() != 0) {
        G29_PRINT("Failed to initialize HIDAPI");
    }

    device = hid_open(0x046d, 0xc24f, NULL);
    if (!device) {
        G29_PRINT("Failed to open G29 device");
        hid_exit();
    }

    memset(cache, 0, 9);
    G29State.steering = 255;
    G29State.throttle = 255;
    G29State.clutch = 255;
    G29State.brake = 255;
}

void deinitG29(void) {
    G29Rumbling = 0;
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

void G29update(void) {
    hid_read(device, cache, 9);

    G29State.steering = (unsigned short)cache[4] | ((unsigned short)cache[5] << 8);
    G29State.throttle = cache[6];
    G29State.brake = cache[7];
    G29State.clutch = cache[8];

    G29State.X = (cache[0] & 0x18) == 0x18;
    G29State.square = (cache[0] & 0x28) == 0x28;
    G29State.triangle = (cache[0] & 0x88) == 0x88;
    G29State.circle = (cache[0] & 0x48) == 0x48;

    G29State.L2 = (cache[1] & 0x08) == 0x08;
    G29State.R2 = (cache[1] & 0x04) == 0x04;
    G29State.L3 = (cache[1] & 0x80) == 0x80;
    G29State.R3 = (cache[1] & 0x40) == 0x40;

    G29State.DPadUp = (cache[0] & 0x0F) == 0x00;
    G29State.DPadDown = (cache[0] & 0x0F) == 0x04;
    G29State.DPadLeft = (cache[0] & 0x0F) == 0x06;
    G29State.DPadRight = (cache[0] & 0x0F) == 0x02;

    G29State.dial = (cache[3] & 0x08) == 0x08;

    G29State.plus = (cache[2] & 0x80) == 0x80;
    G29State.minus = (cache[3] & 0x01) == 0x01;

    G29State.leftPaddle = (cache[1] & 0x02) == 0x02;
    G29State.rightPaddle = (cache[1] & 0x01) == 0x01;

    G29State.share = (cache[1] & 0x10) == 0x10;
    G29State.options = (cache[1] & 0x20) == 0x20;
    G29State.PS = (cache[3] & 0x10) == 0x10;

    G29_PRINT("steering: %u, throttle: %hu, clutch: %hu, brake: %hu\n", G29State.steering, G29State.throttle, G29State.clutch, G29State.brake);
    G29_PRINT("X: %d, square: %d, triangle: %d, circle: %d\nL2: %d, R2: %d, L3: %d, R3: %d\nDPadUp: %d, DPadDown: %d, DPadLeft: %d, DPadRight: %d\nrotation thingy: %d\n, plus: %d, minus: %d, leftPaddle: %d, rightPaddle: %d\nshare: %d, options: %d, PS: %d\n",
        G29State.X, G29State.square, G29State.triangle, G29State.circle, G29State.L2, G29State.R2, G29State.L3, G29State.R3, G29State.DPadUp, G29State.DPadDown, G29State.DPadLeft, G29State.DPadRight, G29State.rotaryDialPress, G29State.plus, G29State.minus,
        G29State.leftPaddle, G29State.rightPaddle, G29State.share, G29State.options, G29State.PS);
    G29_PRINT("Rumble timer: %f, current rumble: %f\n", G29RumbleTimer, G29CurrentRumble);
}

void G29disableForces(void) {
    G29RumbleEnabled = 0;

    const unsigned char msg1[7] = {0xf5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    const unsigned char msg2[7] = {0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    const unsigned char msg3[7] = {0xf3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    const unsigned char msg4[7] = {0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    hid_write(device, msg1, 7);
    hid_write(device, msg2, 7);
    hid_write(device, msg3, 7);
    hid_write(device, msg4, 7);
}

#endif // G29_IMPLEMENTATION
#endif // G29_DRIVER_H

