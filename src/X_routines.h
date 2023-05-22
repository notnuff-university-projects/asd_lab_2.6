//
// Created by notnuff on 25.04.23.
//

#ifndef INC_2_3_X_ROUTINES_H
#define INC_2_3_X_ROUTINES_H
#define ProgramName "Laboratory work 6"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include "consts_n_structures.h"

extern Display *dis;
extern int screen;
extern Window win;
extern GC gc; //graphic context
extern XEvent event;
extern KeySym key;
extern char text[255];

void init_x ();
void close_x ();
void redraw_x ();

#endif //INC_2_3_X_ROUTINES_H
