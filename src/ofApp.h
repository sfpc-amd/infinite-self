#pragma once

#include "ofMain.h"
#include "ofxAutoReloadedShader.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include "ofxFaceTracker.h"
#include "ofxFaceTrackerThreaded.h"
#include "ofxCenteredTrueTypeFont.h"


#include "Clone.h"

#define USE_MACAM

#ifdef USE_MACAM
#include "ofxMacamPs3Eye.h"
#endif

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();
    
        vector<ofVec2f> getSrcPoints();
        void loadImages();
        void updateImageAverage();
        bool updateSrcImage();
        void updateClone();
    
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        // flags
        bool bDrawGui;
        bool bImagesLoaded;
        bool bImagesStartLoading;
        bool bAlwaysShowCamera;
        bool srcImageFound;
        bool isFullscreen;

        // basic configuration
        float imageHeight;
        float imageWidth;
        int totalImages;
        int snapshotFrameCount;

        // camera input
        #ifdef USE_MACAM
        ofxMacamPs3Eye cam;
        #else
        ofVideoGrabber cam;
        #endif
    
        // fbos and shaders
        ofFbo srcFbo;
        ofFbo maskFbo;
        ofFbo camFbo;
        ofFbo avgFbo;
        ofxAutoReloadedShader avgShader;
        ofxAutoReloadedShader camShader;
    
        // face replacement & tracking stuff
        ofxFaceTrackerThreaded tracker;
        Clone clone;
        vector<ofVec2f> srcPoints;
        ofImage srcImage;
        ofMesh camMesh;
    
        // image loading
        // @todo: separate class?
        string imagesDirPath;
        ofDirectory imagesDir;
        deque<ofImage> images;
        vector<string> names;
        int startIndex;
        int endIndex;

        // gui panel
        ofxPanel gui;
        ofxFloatSlider dMultiply;
        ofxIntSlider cloneStrength;
        ofxIntSlider snapshotFrames;
   
        // display
        ofxCenteredTrueTypeFont font;

};
