#pragma once

#include "ofMain.h"
#include <vector>
#include "json.hpp"

using json = nlohmann::json;

class Player : public ofBaseApp{
    public:
        void setup();
        void update();
        void draw();

        void set_dimension(int,int);
        void load();
        
        void keyPressed(int key);

        void recheck_directory();
        void reset_settings();
   private:
        enum PLAYER_STATE {
            PLAYER_REPOSITION,
            PLAYER_RESIZE,
            PLAYER_PLAY
        };

        PLAYER_STATE state;

        const float FADE_TIME = 1000;
        const unsigned long DIR_INTERVAL = 10000;

        ofVideoPlayer player;

        bool fadeout;
        bool reverse;

        float alpha;
        float volume;

        int screen_w;
        int screen_h;

        int playing;
        int cued;

        unsigned long previous_frame;
        unsigned long last_dir_check;
        unsigned long last_ok;
        int ok_count;

        vector<string> files;
        ofVec2f size;
        ofVec2f pos;

        json settings;
};
