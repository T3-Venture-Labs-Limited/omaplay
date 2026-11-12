/* vptr: drive a pointer inside a wlroots compositor (cage) through wlr-virtual-pointer.
 *   vptr move X Y | click X Y [button] | scroll X Y DELTA | wait MS ...
 * Coordinates are pixels of a WxH output given by VPTR_SIZE (default 1280x720).
 * It talks only to the compositor named by WAYLAND_DISPLAY, so it cannot touch the real desktop. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <wayland-client.h>
#include "wlr-virtual-pointer-unstable-v1-client-protocol.h"

static struct wl_seat *seat;
static struct zwlr_virtual_pointer_manager_v1 *mgr;

static void global(void *d, struct wl_registry *r, uint32_t name, const char *iface, uint32_t v)
{
    if (!strcmp(iface, "wl_seat") && !seat)
        seat = wl_registry_bind(r, name, &wl_seat_interface, 1);
    else if (!strcmp(iface, "zwlr_virtual_pointer_manager_v1"))
        mgr = wl_registry_bind(r, name, &zwlr_virtual_pointer_manager_v1_interface, v < 2 ? v : 2);
}
static void global_remove(void *d, struct wl_registry *r, uint32_t n) {}
static const struct wl_registry_listener rl = { global, global_remove };

static uint32_t now(void)
{
    struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1000 + t.tv_nsec / 1000000;
}

int main(int argc, char **argv)
{
    unsigned W = 1280, H = 720;
    if (getenv("VPTR_SIZE")) sscanf(getenv("VPTR_SIZE"), "%ux%u", &W, &H);
    struct wl_display *dpy = wl_display_connect(NULL);
    if (!dpy) { fprintf(stderr, "vptr: cannot connect to the compositor\n"); return 1; }
    struct wl_registry *reg = wl_display_get_registry(dpy);
    wl_registry_add_listener(reg, &rl, NULL);
    wl_display_roundtrip(dpy);
    if (!mgr) { fprintf(stderr, "vptr: compositor has no virtual pointer support\n"); return 1; }
    struct zwlr_virtual_pointer_v1 *p = zwlr_virtual_pointer_manager_v1_create_virtual_pointer(mgr, seat);

    for (int i = 1; i < argc;) {
        const char *c = argv[i];
        if (!strcmp(c, "move") && i + 2 < argc) {
            zwlr_virtual_pointer_v1_motion_absolute(p, now(), atoi(argv[i+1]), atoi(argv[i+2]), W, H);
            zwlr_virtual_pointer_v1_frame(p); i += 3;
        } else if (!strcmp(c, "click") && i + 2 < argc) {
            int btn = 272; /* BTN_LEFT */
            int used = 3;
            if (i + 3 < argc && argv[i+3][0] >= '0' && argv[i+3][0] <= '9') { btn = atoi(argv[i+3]); used = 4; }
            zwlr_virtual_pointer_v1_motion_absolute(p, now(), atoi(argv[i+1]), atoi(argv[i+2]), W, H);
            zwlr_virtual_pointer_v1_frame(p); wl_display_roundtrip(dpy); usleep(120000);
            zwlr_virtual_pointer_v1_button(p, now(), btn, WL_POINTER_BUTTON_STATE_PRESSED);
            zwlr_virtual_pointer_v1_frame(p); wl_display_roundtrip(dpy); usleep(80000);
            zwlr_virtual_pointer_v1_button(p, now(), btn, WL_POINTER_BUTTON_STATE_RELEASED);
            zwlr_virtual_pointer_v1_frame(p); i += used;
        } else if (!strcmp(c, "scroll") && i + 3 < argc) {
            zwlr_virtual_pointer_v1_motion_absolute(p, now(), atoi(argv[i+1]), atoi(argv[i+2]), W, H);
            zwlr_virtual_pointer_v1_frame(p);
            zwlr_virtual_pointer_v1_axis(p, now(), WL_POINTER_AXIS_VERTICAL_SCROLL, wl_fixed_from_int(atoi(argv[i+3])));
            zwlr_virtual_pointer_v1_frame(p); i += 4;
        } else if (!strcmp(c, "wait") && i + 1 < argc) {
            wl_display_roundtrip(dpy); usleep(atoi(argv[i+1]) * 1000); i += 2;
        } else { fprintf(stderr, "vptr: bad command near '%s'\n", c); return 2; }
        wl_display_roundtrip(dpy);
    }
    wl_display_roundtrip(dpy);
    zwlr_virtual_pointer_v1_destroy(p);
    wl_display_disconnect(dpy);
    return 0;
}
