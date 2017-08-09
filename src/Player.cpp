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
    playing = 1; // TODO change to 0
    current_alpha = 255;

    recheck_directory();
    if( files.size() == 0 ) {
        std::cout << "No video files found!" << std::endl;
        ofExit();
    }

    clock_t start = clock();
    previous_frame = start / 1000;
    load();
    cue();
}

void Player::load() {
    current_player = &player_a;
    next_player = &player_b;

    current_player->load(files[playing]);
    current_player->setLoopState(OF_LOOP_NORMAL);
    current_player->play();
}

void Player::cue() {
    int next = playing+1;
    if( next >= files.size() ) {
        next = 0;
    }
    next_player->load(files[next]);
    next_player->setLoopState(OF_LOOP_NORMAL);
    next_player->stop();
}

void Player::update(){
    clock_t time = clock();
    unsigned long dt = time / 1000 - previous_frame;
    previous_frame = time / 1000;

    current_player->update();
    float remain = current_player->getDuration() - 
        current_player->getPosition() * 
        current_player->getDuration(); 
    remain *= 1000;
    if( remain < FADE_TIME ) {
        current_alpha -= 255.0f * remain / FADE_TIME;
        float v = 1 * remain / FADE_TIME;
        current_player->setVolume(v);
        next_player->setVolume(1 - v);
        if( !next_player->isPlaying() ) {
            next_player->play();
        }
        if( current_alpha < 0 ) {
            current_alpha = 255;
            ofVideoPlayer* foo = next_player;
            next_player = current_player;
            current_player = foo;
            current_player->setVolume(1);
            cue();
        }
    }
}

void Player::draw(){
    if( next_player->isPlaying() ) {
        ofSetColor(255);
        next_player->draw(0,0);
    }
    ofSetColor(255,255,255,current_alpha);
    current_player->draw(0,0);

}

void Player::keyPressed(int key){
    std::cout << key << std::endl;
    ofExit();
}

void Player::recheck_directory() {
    ofDirectory dir(".");
    dir.listDir();
    if( files.size() != dir.size() ) {
        files.clear();
        for(int i = 0; i < dir.size(); i++) {
            files.push_back(dir.getPath(i)); 
        }
    }
}
