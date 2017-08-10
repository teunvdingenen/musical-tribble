#include "ofMain.h"
#include "ofAppGlutWindow.h"
#include "Player.h"
#include <X11/Xlib.h>

//========================================================================
int main( ){
    Display* d = XOpenDisplay(NULL);
    Screen* s = DefaultScreenOfDisplay(d);

    //ofAppGlutWindow window;

    //ofSetupOpenGL(&window, s->width,s->height,OF_GAME_MODE);
    ofSetupOpenGL(s->width,s->height,OF_GAME_MODE);
    //ofSetupOpenGL(s->width,s->height,OF_FULLSCREEN);
    //ofSetupOpenGL(s->width,s->height,OF_WINDOW);
    Player* p = new Player();
    p->set_dimension(s->width, s->height);
    ofRunApp(p);
}
