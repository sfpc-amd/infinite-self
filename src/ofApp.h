#pragma once

#include "ofMain.h"
#include "ofxInstagram.h"
#include "ofxThreadedImageLoader.h"
//#include "ofxCv.h"
#include "ofxAutoReloadedShader.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();
    
        void updateImageAverage();
        void fetchImages();
        void updateImages();
        bool imagesAllocated(deque<ofImage>& images, int start, int end);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        ofxInstagram instagram;
        vector<string> paginationIds;
        
        float imageHeight;
        float imageWidth;
        int imageSize;
    
        int totalImages;

    
        bool bImagesAlloc;
    
        ofxThreadedImageLoader getImages;
        deque<ofImage> images;
        ofImage avgImage;
    
        ofxAutoReloadedShader avgShader;
        ofFbo avgFbo;
    
    
    private:
        int startIndex;
        int endIndex;
};
