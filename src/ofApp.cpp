#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
//    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofBackground(0);
    
    videos[0].load("movies/original.mp4");
    videos[1].load("movies/semaseg.mp4");
    videos[2].load("movies/composite.mp4");
    videos[3].load("movies/yolo.mp4");
    
    videos[0].setLoopState(OF_LOOP_NORMAL);
    videos[1].setLoopState(OF_LOOP_NORMAL);
    videos[2].setLoopState(OF_LOOP_NORMAL);
    videos[3].setLoopState(OF_LOOP_NORMAL);
    
    videos[0].play();
    
    ofxSubscribeOsc(RECEIVEPORT, "/param", [=](int value){
        eventListener(value);
    }); //OSC Receiver
    setTime(videos);
}

//--------------------------------------------------------------
void ofApp::update(){
    videos[0].update();
    videos[1].update();
    videos[2].update();
    videos[3].update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofRectangle videoRect(0, 0, videos[0].getWidth(), videos[0].getHeight());
    videoRect.scaleTo(ofGetWindowRect());
    if(wasPaused[0] == false){
        videos[0].draw(videoRect.x, videoRect.y, videoRect.width, videoRect.height);
    }else if(wasPaused[1] == false){
        videos[1].draw(videoRect.x, videoRect.y, videoRect.width, videoRect.height);
    }else if(wasPaused[2] == false){
        videos[2].draw(videoRect.x, videoRect.y, videoRect.width, videoRect.height);
    }else if(wasPaused[3] == false){
        videos[3].draw(videoRect.x, videoRect.y, videoRect.width, videoRect.height);
    }
}

//--------------------------------------------------------------
void ofApp::exit(){
    videos[0].closeMovie();
    videos[1].closeMovie();
    videos[2].closeMovie();
    videos[3].closeMovie();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' '){
        auto itr = std::find(wasPaused.begin(), wasPaused.end(), false);
        const int index = std::distance(wasPaused.begin(), itr);
        
        if(index == 3){
            setTime(videos);
            toggleValue = 0;
            setFrame(videos, 0);
        }else{
            setTime(videos);
            toggleValue = index + 1;
            setFrame(videos, index + 1);
        }
    }
}

void ofApp::setFrame(ofVideoPlayer *players, int num){
    auto itr = std::find(wasPaused.begin(), wasPaused.end(), false);
    const int index = std::distance(wasPaused.begin(), itr);
    
    currentFrame = players[index].getCurrentFrame();
    
    wasPaused[index] = !wasPaused[index];
    videos[index].setPaused(wasPaused[index]);
    wasPaused[num] = !wasPaused[num];
    videos[num].setFrame(currentFrame + 1);
    videos[num].play();
}

void ofApp::setTime(ofVideoPlayer *players){
    auto itr = std::find(wasPaused.begin(), wasPaused.end(), false);
    const int index = std::distance(wasPaused.begin(), itr);
    
    currentFrame = players[index].getCurrentFrame();
    totalFrame   = players[index].getTotalNumFrames();
    ratio        = currentFrame / totalFrame;
    
    durationTime = players[index].getDuration();
    currentTime  = ratio * players[index].getDuration();

    if(std::isinf(currentTime)){
        currentTime = 0;
    }
    cout << currentTime << endl;
    ofxPublishOsc("localhost", SENDPORT, "/duration", durationTime);
    ofxPublishOsc("localhost", SENDPORT, "/time", currentTime);
    ofxPublishOsc("localhost", SENDPORT, "/toggle", toggleValue);
}

void ofApp::eventListener(int value){
    setTime(videos);
    setFrame(videos, value);
}
