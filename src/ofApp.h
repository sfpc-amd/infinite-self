#pragma once

#include "ofMain.h"
#include "ofxAutoReloadedShader.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include "ofxFaceTracker.h"
#include "ofxFaceTrackerThreaded.h"

#include "Clone.h"

//#define USE_MACAM

#ifdef USE_MACAM
#include "ofxMacamPs3Eye.h"
#endif

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();
    
    
        void loadImages();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        vector<ofVec2f> getSrcPoints();
    
        Clone clone;
        bool cloneReady;
        vector<ofVec2f> srcPoints;
        ofFbo srcFbo, maskFbo, camFbo;
    
        string imagesDirPath;
        ofDirectory imagesDir;
        ofImage srcImage;
        bool srcImageFound;
        ofMesh camMesh;
    
        #ifdef USE_MACAM
            ofxMacamPs3Eye cam;
        #else
            ofVideoGrabber cam;
        #endif
        ofImage squareImg;
    
        ofxFaceTrackerThreaded tracker;
        ofVec2f position;
        float scale;
        ofVec3f orientation;
        ofMatrix4x4 rotationMatrix;
        
        ofxCv::Mat translation, rotation;
        ofMatrix4x4 pose;

        ofxPanel gui;
        ofxFloatSlider dMultiply;
        ofxIntSlider cloneStrength;
    
        float imageHeight;
        float imageWidth;
        int totalImages;

        bool bDrawGui;
        bool bImagesLoaded;
        bool bImagesStartLoading;
    
        deque<ofImage> images;
    
        ofxAutoReloadedShader avgShader;
        ofxAutoReloadedShader camShader;
        ofFbo avgFbo;
    
    
    private:
        int startIndex;
        int endIndex;
};
