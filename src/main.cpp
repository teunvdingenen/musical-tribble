#include "ofMain.h"
#include "Player.h"
#include <X11/Xlib.h>

//========================================================================
int main( ){
    Display* d = XOpenDisplay(NULL);
    Screen* s = DefaultScreenOfDisplay(d);

    ofSetupOpenGL(s->width,s->height,OF_FULLSCREEN);
    Player* p = new Player();
    p->set_dimension(s->width, s->height);
    ofRunApp(p);
}
