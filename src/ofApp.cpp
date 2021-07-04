#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
//    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofBackground(0);
    
    // Load original movie file
    original.load("movies/yolo.mov");
    original.play();
    original.setLoopState(OF_LOOP_NORMAL);
    
    //Load human movie file
    human.load("movies/semaseg.mov");
    human.play();
    human.setLoopState(OF_LOOP_NORMAL);
    
    ofxSubscribeOsc(RECEIVEPORT, "/param", [=](int value){
        eventListener(value);
    }); //OSC Receiver
    
    ofxPublishOsc("localhost", SENDPORT, "/trigger", 0);
    setTime(&original);
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
    if (original.isLoaded() && human.isLoaded()){
        ofSetColor(255, 255, 255);
        ofRectangle videoRect(0, 0, original.getWidth(), original.getHeight());
        videoRect.scaleTo(ofGetWindowRect());
        if(wasPausedOriginal == false && wasPausedHuman == true){
            original.draw(videoRect.x, videoRect.y, videoRect.width, videoRect.height);
        }else if(wasPausedOriginal == true && wasPausedHuman == false){
            human.draw(videoRect.x, videoRect.y, videoRect.width, videoRect.height);
        }
    }else{
        if (original.getError().length() || human.getError().length()){
            ofDrawBitmapStringHighlight(original.getError(), 20, 20);
        }else{
            ofDrawBitmapStringHighlight("Movie is loading...", 20, 20);
        }
    }
}

//--------------------------------------------------------------
void ofApp::exit(){
    original.stop();
    original.close();
    human.stop();
    human.close();
}

void ofApp::setFrame(ofxHapPlayer *player1, ofxHapPlayer *player2){
    totalFrame = player1->getTotalNumFrames();
    currentFrame = player1->getCurrentFrame();
    ratio = currentFrame / totalFrame;
    
    player2->setPosition(ratio);
    wasPausedHuman    = !wasPausedHuman;
    human.setPaused(wasPausedHuman);
    wasPausedOriginal = !wasPausedOriginal;
    original.setPaused(wasPausedOriginal);
}

void ofApp::setTime(ofxHapPlayer *player1){
    durationTime = player1->getDuration();
    currentTime = player1->getPosition()*player1->getDuration();
    
    cout << currentTime << endl;
    ofxPublishOsc("localhost", SENDPORT, "/duration", durationTime);
    ofxPublishOsc("localhost", SENDPORT, "/time", currentTime);
}

void ofApp::eventListener(int value){
    if(value == 0){
        setTime(&human);
        setFrame(&human, &original);
    }else if(value == 1){
        setTime(&original);
        setFrame(&original, &human);
    }
}
