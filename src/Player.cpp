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
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    ofHideCursor();
    playing = 0; 
    current_alpha = 255;
    skip = false;
    reverse = false;

    recheck_directory();
    if( files.size() == 0 ) {
        std::cout << "No video files found!" << std::endl;
        ofExit();
    }

    clock_t start = clock();
    previous_frame = start / 1000;
    last_dir_check = previous_frame;
    load();
    cue();
}

void Player::load() {
    current_player = &player_a;
    next_player = &player_b;

    current_player->load(files[playing]);
    current_player->setLoopState(OF_LOOP_NONE);
    current_player->setVolume(1);
    current_player->play();
}

void Player::cue() {
    int next = playing+1;
    if( reverse ) {
        next = playing-1;
    }
    if( next >= files.size() ) {
        next = 0;
    } else if( next < 0 ) {
        next = files.size() - 1;
    }
    if( next_player->isLoaded() ) {
        next_player->close();
    }
    next_player->load(files[next]);
    next_player->setLoopState(OF_LOOP_NONE);
    next_player->stop();
    next_player->setPosition(0);
    skip = false;
    cued = next;
}

void Player::update(){
    clock_t time = clock();
    unsigned long dt = time / 1000 - previous_frame;
    previous_frame = time / 1000;

    if( (previous_frame - last_dir_check) > DIR_INTERVAL ) {
        std::cout << "Checking dir" << std::endl;
        last_dir_check = previous_frame;
        recheck_directory();
    }

    current_player->update();
    if( next_player->isPlaying() ) {
        next_player->update();
    }
    if( skip ) {
        remain -= dt;
    } else {
        remain = (current_player->getDuration() - 
            current_player->getPosition() * 
            current_player->getDuration()) * 1000; 
    }
    if( remain < FADE_TIME ) {
        current_alpha = 255.0f * remain / FADE_TIME;
        float v = 1 * remain / FADE_TIME;
        current_player->setVolume(v);
        next_player->setVolume(1 - v);
        if( !next_player->isPlaying() ) {
            next_player->play();
        }
        if( current_alpha < 10 ) {
            current_alpha = 255;
            ofVideoPlayer* foo = next_player;
            next_player = current_player;
            current_player = foo;
            current_player->setVolume(1);
            playing = cued;
            cue();
        }
    }
}

void Player::draw(){
    if( next_player->isPlaying() ) {
        ofPushMatrix();
        ofPushStyle();

        ofSetColor(255);
        ofTranslate(screen_w/2,screen_h/2);
        next_player->draw(-next_player->getWidth()/2,
                -next_player->getHeight()/2);

        ofPopStyle();
        ofPopMatrix();

    }
    ofPushMatrix();
    ofPushStyle();

    ofSetColor(255,255,255,current_alpha);
    ofTranslate(screen_w/2,screen_h/2);
    current_player->draw(-current_player->getWidth()/2,
            -current_player->getHeight()/2);

    ofPopStyle();
    ofPopMatrix();
}

void Player::keyPressed(int key){
    switch( key ) {
        case 'd': //LEFT
            if( !skip ) {
                skip = true;
                reverse = true;
                remain = FADE_TIME;
            }
            break;
        case 'f': //RIGHT
            if( !skip ) {
                skip = true;
                reverse = false;
                remain = FADE_TIME;
            }
            break;
        case 'e': // UP
            break;
        case 'g': //DOWN
            break;
        case 13: //OK
            break;
        case 'h': //UPUP
            break;
        case 'i': //DOWNDOWN
            break;
    }
}

void Player::recheck_directory() {
    ofDirectory dir(".");
    dir.listDir();
    for(int i = 0; i < dir.size(); i++) {
        files.push_back(dir.getPath(i)); 
    }
}
