#include "Player.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

void Player::set_dimension(int w,int h) {
    screen_w = w;
    screen_h = h;
}

void Player::setup(){
    ofBackground(0);
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofHideCursor();
    playing = 0; 
    alpha = 255;
    reverse = false;
    state = PLAYER_PLAY;

    std::ifstream i("settings.json");
    if( i.good() ) {
        i >> settings;
        i.close();
    } else {
        settings["width"] = screen_w;
        settings["height"] = screen_h;
        settings["position"]["x"] = 0;
        settings["position"]["y"] = 0;
    }

    size = ofVec2f(settings["width"], settings["height"]);
    pos = ofVec2f(settings["position"]["x"],settings["position"]["y"]);

    recheck_directory();
    if( files.size() == 0 ) {
        std::cout << "No video files found!" << std::endl;
        ofExit();
    }

    /**
    int fd;
    if( (fd = open("/dev/input/mouse1", O_RDONLY)) == -1 )
        printf("/dev/input/mouse1 is not a valid device\n");

    ioctl(fd, EVIOCGRAB, 1);
    **/

    clock_t start = clock();
    previous_frame = start / 1000;
    last_dir_check = previous_frame;
    load();
}

void Player::load() {
    if( reverse ) {
        playing -= 1;
    } else {
        playing += 1;
    }
    if( playing < 0 ) {
        playing += files.size(); 
    } else if( playing >= (int)files.size() ) {
        playing -= files.size();
    } 
    if( player.isLoaded() ) {
        player.closeMovie();
    }
    fadeout = false;
    player.load(files[playing]);
    player.setLoopState(OF_LOOP_NONE);
    player.setPosition(0);
    player.play();
}

void Player::update(){
    if( previous_frame - last_ok > 100 ) {
        ok_count = 0;
    }

    clock_t time = clock();
    unsigned long dt = time / 1000 - previous_frame;
    previous_frame = time / 1000;

    if( (previous_frame - last_dir_check) > DIR_INTERVAL ) {
        std::cout << "Checking dir" << std::endl;
        last_dir_check = previous_frame;
        recheck_directory();
    }

    player.update();
    
    float remain = (player.getDuration() - 
            player.getPosition() * 
            player.getDuration()) * 1000; 
    fadeout = fadeout || remain < FADE_TIME;

    if( fadeout ) {
        alpha -= 255.0f / FADE_TIME * dt;
        volume -= 1.0f / FADE_TIME * dt;
        player.setVolume(volume);
        if( volume < 0 ) {
            volume = 0;
        }
        if( alpha < 0 ) {
            player.setVolume(0);
            load();
        }
    } else if( alpha < 255  || volume < 1.0f) {
        alpha += 255.0f / FADE_TIME * dt;
        if ( alpha > 255 ) {
            alpha = 255;
        }
        volume += 1 / FADE_TIME * dt;
        if( volume > 1 ) {
            volume = 1;
        }
        player.setVolume(volume);
    }
}

void Player::draw(){
    ofPushMatrix();
    ofPushStyle();

    ofSetColor(255,255,255,alpha);
    ofTranslate(screen_w/2+pos.x,screen_h/2+pos.y);
    player.draw(-size.x/2,-size.y/2,
            size.x, size.y);

    ofSetColor(255);
    switch( state ) {
        case PLAYER_RESIZE:
            ofNoFill();
            ofSetLineWidth(5);
            ofDrawRectangle(-size.x/4,-size.y/4,
                    size.x/2,size.y/2);
            break;
        case PLAYER_REPOSITION:
            ofFill();
            ofDrawEllipse(-10,-10, 20, 20);
            break;
        case PLAYER_PLAY:
            if( ok_count > 5 ) {
                ofSetLineWidth(10);
                ofPath p;
                p.moveTo(0,0);
                p.lineTo(200,0);
                p.arc(0,0,200,200,0,360*(ok_count-5)/60);
                p.lineTo(0,0);
                p.draw();
            }
            break;
    }
    ofPopStyle();
    ofPopMatrix();
}

void Player::keyPressed(int key){
    bool update_settings = false;
    switch( key ) {
        case 356: //LEFT
            {
                switch(state) {
                    case PLAYER_PLAY:
                        reverse = true;
                        fadeout = true;
                        break;
                    case PLAYER_RESIZE:
                        size.x-=1;
                        break;
                    case PLAYER_REPOSITION:
                        pos.x-=1;
                        break;
                }
            }
            break;
        case 358: //RIGHT
            {
                switch(state) {
                    case PLAYER_PLAY:
                        reverse = false;
                        fadeout = true;
                        break;
                    case PLAYER_RESIZE:
                        size.x+=1;
                        break;
                    case PLAYER_REPOSITION:
                        pos.x+=1;
                        break;
                }
            }
            break;
        case 357: // UP
            {
                switch(state) {
                    case PLAYER_PLAY:
                        reverse = true;
                        fadeout = true;
                        break;
                    case PLAYER_RESIZE:
                        size.y-=1;
                        break;
                    case PLAYER_REPOSITION:
                        pos.y-=1;
                        break;
                }
            }
            break;
        case 359: //DOWN
            {
                switch(state) {
                    case PLAYER_PLAY:
                        reverse = false;
                        fadeout = true;
                        break;
                    case PLAYER_RESIZE:
                        size.y+=1;
                        break;
                    case PLAYER_REPOSITION:
                        pos.y+=1;
                        break;
                }
            }
            break;
        case 13: //OK
            if( state != PLAYER_PLAY ) {
                state = PLAYER_PLAY;
                update_settings = true;
            } else {
                if( previous_frame - last_ok < 100 ) {
                    ok_count += 1;
                    if( ok_count >= 65 ) {
                        reset_settings();
                        ok_count = -20;
                    }
                }
                last_ok = previous_frame;
            }
            break;
        case 360: //UPUP
            if( state == PLAYER_REPOSITION ) {
                state = PLAYER_PLAY;
                update_settings = true;
            } else {
                state = PLAYER_REPOSITION;
            }
            break;
        case 361: //DOWNDOWN
            if( state == PLAYER_RESIZE ) {
                state = PLAYER_PLAY;
                update_settings = true;
            } else {
                state = PLAYER_RESIZE;
            }
            break;
    }
    if( update_settings) { 
        settings["width"] = size.x;
        settings["height"] = size.y;
        settings["position"]["x"] = pos.x;
        settings["position"]["y"] = pos.y;

        std::ofstream o("settings.json");
        o << std::setw(4) << settings << std::endl;
        o.close();
    }
}

void Player::recheck_directory() {
    ofDirectory dir(".");
    dir.listDir();
    for(size_t i = 0; i < dir.size(); i++) {
        files.push_back(dir.getPath(i)); 
    }
}

void Player::reset_settings() {
    settings["width"] = screen_w;
    settings["height"] = screen_h;
    settings["position"]["x"] = 0;
    settings["position"]["y"] = 0;

    std::ofstream o("settings.json");
    o << std::setw(4) << settings << std::endl;
    o.close();

    size = ofVec2f(settings["width"], settings["height"]);
    pos = ofVec2f(settings["position"]["x"],settings["position"]["y"]);
}
