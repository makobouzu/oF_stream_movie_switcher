#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
//    ofSetVerticalSync(true);
    ofSetFullscreen(true);
//    ofHideCursor();
    ofSetFrameRate(60);
    ofBackground(0);
    
    videos.load("movies/sum_movie.mp4");
    videos.setLoopState(OF_LOOP_NORMAL);
    videos.play();
    
    offsetWidth  = (ofGetScreenWidth() - videos.getWidth()/2)/2;
    offsetHeight = (ofGetScreenHeight() - videos.getHeight()/2)/2;
    
    ofxSubscribeOsc(RECEIVEPORT, "/param", [=](int value){
        eventListener(value);
    }); //OSC Receiver
    setTime(videos);
}

//--------------------------------------------------------------
void ofApp::update(){
    videos.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofRectangle videoRect(0, 0, videos.getWidth(), videos.getHeight());
//    videoRect.scaleTo(ofGetWindowRect());
    if(wasPaused[0] == false){
        ofPushMatrix();
            ofTranslate(offsetWidth, offsetHeight);
            videos.draw(videoRect.x, videoRect.y, videoRect.width, videoRect.height);
            ofPushStyle();
                ofSetColor(0);
                ofDrawRectangle(videoRect.x, videoRect.height/2, videoRect.width/2, videoRect.height/2);
                ofDrawRectangle(videoRect.width/2, videoRect.y, videoRect.width/2, videoRect.height/2);
            ofPopStyle();
        ofPopMatrix();
    }else if(wasPaused[1] == false){
        ofPushMatrix();
            ofTranslate(offsetWidth, offsetHeight);
            ofTranslate(-1*videoRect.width/2, 0);
            videos.draw(videoRect.x, videoRect.y, videoRect.width, videoRect.height);
            ofPushStyle();
                ofSetColor(0);
                ofDrawRectangle(videoRect.x, videoRect.y, videoRect.width/2, videoRect.height/2);
                ofDrawRectangle(videoRect.x, videoRect.height/2, videoRect.width/2, videoRect.height/2);
            ofPopStyle();
        ofPopMatrix();
    }else if(wasPaused[2] == false){
        ofPushMatrix();
            ofTranslate(offsetWidth, offsetHeight);
            ofTranslate(0, -1*videoRect.height/2);
            videos.draw(videoRect.x, videoRect.y, videoRect.width, videoRect.height);
            ofPushStyle();
                ofSetColor(0);
                ofDrawRectangle(videoRect.x, videoRect.y, videoRect.width/2, videoRect.height/2);
                ofDrawRectangle(videoRect.width/2, videoRect.y, videoRect.width/2, videoRect.height/2);
            ofPopStyle();
        ofPopMatrix();
    }
}

//--------------------------------------------------------------
void ofApp::exit(){
    videos.closeMovie();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' '){
        auto itr = std::find(wasPaused.begin(), wasPaused.end(), false);
        const int index = std::distance(wasPaused.begin(), itr);
        
        if(index == 2){
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

void ofApp::setFrame(ofVideoPlayer players, int num){
    auto itr = std::find(wasPaused.begin(), wasPaused.end(), false);
    const int index = std::distance(wasPaused.begin(), itr);
    
    currentFrame = players.getCurrentFrame();
    
    wasPaused[index] = !wasPaused[index];
    wasPaused[num] = !wasPaused[num];
    
    toggleValue = num;
    ofxPublishOsc(IPAdress, SENDPORT, "/toggle", toggleValue);
}

void ofApp::setTime(ofVideoPlayer players){
    auto itr = std::find(wasPaused.begin(), wasPaused.end(), false);
    const int index = std::distance(wasPaused.begin(), itr);
    
    currentFrame = players.getCurrentFrame();
    totalFrame   = players.getTotalNumFrames();
    ratio        = currentFrame / totalFrame;
    
    durationTime = players.getDuration();
    currentTime  = ratio * players.getDuration();

    if(std::isinf(currentTime) || currentTime < 0){
        currentTime = 0;
    }
    ofxPublishOsc(IPAdress, SENDPORT, "/duration", durationTime);
    ofxPublishOsc(IPAdress, SENDPORT, "/time", currentTime);
}

void ofApp::eventListener(int value){
    setTime(videos);
    setFrame(videos, value);
}
