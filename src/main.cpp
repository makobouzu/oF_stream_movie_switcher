#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    ofGLFWWindowSettings settings;
    settings.setSize(1920, 1080);
    settings.setPosition({ glm::vec2{ 0, 0 } });
    auto window = ofCreateWindow(settings);
    auto app = std::make_shared<ofApp>();
    
//    settings.setPosition( { glm::vec2{ 1920, 0} });
//    auto right_window = ofCreateWindow(settings);
//    auto right_app = std::make_shared<RightWindowApp>();
    
    ofRunApp(window, app);
//    ofRunApp(right_window, right_app);
    ofRunMainLoop();
}
