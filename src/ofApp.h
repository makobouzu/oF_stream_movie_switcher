#pragma once

#include "ofMain.h"
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
    void setTime(ofVideoPlayer players);
    void setFrame(ofVideoPlayer players, int num);

private:
    ofVideoPlayer videos;
    std::vector<bool> wasPaused = {false, true, true, true};
    
    float offsetWidth, offsetHeight;
    
    const string IPAdress = "localhost";
    const int RECEIVEPORT = 5445;
    const int SENDPORT = 54003;
    
    double currentFrame = 0;
    double totalFrame   = 0;
    float  ratio        = 0;
    double durationTime = 0;
    double currentTime  = 0;
    int    toggleValue  = 0;
};
