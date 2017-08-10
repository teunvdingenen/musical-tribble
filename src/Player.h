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
        const unsigned long DIR_INTERVAL = 10000;

        ofVideoPlayer player_a;
        ofVideoPlayer player_b;

        ofVideoPlayer* current_player;
        ofVideoPlayer* next_player;

        bool skip;
        bool reverse;

        float current_alpha;
        float remain;

        int screen_w;
        int screen_h;

        int playing;
        int cued;

        unsigned long previous_frame;
        unsigned long last_dir_check;

        vector<string> files;
};
