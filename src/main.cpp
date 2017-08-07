#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    auto window = make_shared<ofAppNoWindow>();
    auto app = make_shared<ofApp>();

    ofRunApp( window, app);
    ofRunMainLoop();
}
