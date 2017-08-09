#pragma once

#include "ofMain.h"
#include <vector>

class Player : public ofBaseApp{
    public:
        void setup();
        void update();
        void draw();

        void set_dimension(int,int);
        void load();
        void cue();
        
        void keyPressed(int key);

        void recheck_directory();
   private:
        const float FADE_TIME = 7000;

        ofVideoPlayer player_a;
        ofVideoPlayer player_b;

        ofVideoPlayer* current_player;
        ofVideoPlayer* next_player;

        float current_alpha;

        int screen_w;
        int screen_h;

        int playing;

        unsigned long previous_frame;

        vector<string> files;
};
