#pragma once

#include "ofMain.h"
#include "ofxHapPlayer.h"
#include "ofxOscSubscriber.h"
#include "ofxOscPublisher.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    void exit();
    void keyPressed(int key);
    
    void eventListener(int value);
    void setTime(ofxHapPlayer *player1);
    void setFrame(ofxHapPlayer *player1, ofxHapPlayer *player2);

private:
    ofxHapPlayer original;
    ofxHapPlayer human;
    bool wasPausedOriginal = false;
    bool wasPausedHuman    = true;
    bool keyChange = false;
    
    const int RECEIVEPORT = 5445;
    const int SENDPORT = 54003;
    
    double currentFrame = 0;
    double totalFrame   = 0;
    double durationTime = 0;
    double currentTime  = 0;
    float  ratio        = 0;
};
