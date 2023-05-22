#include "../X_routines.h"
#include <stdlib.h>

Display *dis;
int screen;
Window win;
GC gc;
XEvent event;
KeySym key;
char text[255];

void init_x() {
    unsigned long background = 0x3D3D3D;

    dis = XOpenDisplay((char *)0);
    screen = DefaultScreen(dis);

    win = XCreateSimpleWindow(dis, DefaultRootWindow(dis), 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 1, 0x000000, background);

    XSetStandardProperties(dis, win, ProgramName, ProgramName, None, NULL, 0, NULL);
    XSelectInput(dis, win, ExposureMask | KeyPressMask);

    gc = XCreateGC(dis, win, 0, 0);
    XMapRaised(dis, win);

    char** fontNames;
    int count;
    fontNames = XListFonts(dis, "*Bold*", 1, &count);
    if (count == 0) {
        return;
    }

    XFontStruct* boldFontStruct;
    Font boldFont;
    boldFontStruct = XLoadQueryFont(dis, fontNames[0]);
    if (boldFontStruct == NULL) {
        return;
    }
    boldFont = boldFontStruct->fid;

    XSetFont(dis, gc, boldFont);

    XFreeFontNames(fontNames);
    XFreeFont(dis, boldFontStruct);
}

void close_x() {
    XFreeGC(dis, gc);
    XDestroyWindow(dis, win);
    XCloseDisplay(dis);
    exit(0);
}

void redraw_x() {
    XClearWindow(dis, win);

}