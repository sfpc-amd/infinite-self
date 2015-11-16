#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

	ofSetFrameRate(60);
	ofSetVerticalSync(true);
    ofEnableSmoothing();
    ofSetLogLevel(OF_LOG_NOTICE);

    imagesDirPath = ofToDataPath("../../../sharedData/images/selfie/", true);

    imageHeight = 640;
    imageWidth = 640;
    totalImages = 10;
    startIndex = 0;
    endIndex = totalImages;

    avgShader.load("shaders/avg");
    avgFbo.allocate(imageHeight, imageWidth);
    
    bImagesLoaded = false;
    bImagesStartLoading = false;
    
    bDrawGui = true;
    gui.setup();
    gui.add(dMultiply.setup("Displacement", 0.3, 0.0, 10.0));
    gui.add(cloneStrength.setup("Clone Strength", 16, 0, 50));
    
    cam.initGrabber(640, 480);
    tracker.setup();
	ofFbo::Settings settings;
	settings.width = cam.getWidth();
	settings.height = cam.getHeight();
	maskFbo.allocate(settings);
	srcFbo.allocate(settings);
    clone.setup(cam.getWidth(), cam.getHeight());

    ofSetWindowShape(cam.getWidth(), cam.getHeight());
    
    imagesDir.open(imagesDirPath);
    imagesDir.allowExt("jpg");
    
    // get our dummy data
    srcPoints = getSrcPoints();

}

//--------------------------------------------------------------
void ofApp::update(){
    if(!bImagesLoaded && !bImagesStartLoading) {
        loadImages();
    } else {
    
        avgFbo.begin();
            avgShader.begin();
                avgShader.setUniform1f("dMultiply", dMultiply);
        
                 for(int j=0; j < totalImages; j++) {
                    string name = "tex"+ofToString(j+1);
                    int index = startIndex+j+1;
                    avgShader.setUniformTexture(name, images[index].getTextureReference(), j+1);
                  }

                images[startIndex].draw(0, 0);
            avgShader.end();
        avgFbo.end();
       
        
    
        cam.update();
        if(cam.isFrameNew()) {
            tracker.update(ofxCv::toCv(cam));
            
            cloneReady = tracker.getFound();
            
            if(cloneReady) {
                ofMesh camMesh = tracker.getImageMesh();
                camMesh.clearTexCoords();
                camMesh.addTexCoords(srcPoints);
                
                maskFbo.begin();
                    ofClear(0, 255);
                    camMesh.draw();
                maskFbo.end();
                
                srcFbo.begin();
                    ofClear(0, 255);
                    avgFbo.getTextureReference().bind();
                    camMesh.draw();
                    avgFbo.getTextureReference().unbind();
                srcFbo.end();
                
                clone.setStrength(cloneStrength);
                clone.update(srcFbo.getTextureReference(), cam.getTextureReference(), maskFbo.getTextureReference());
                
            }
        }
    }
    
}


//--------------------------------------------------------------
void ofApp::draw(){
    ofClear(0);
    
    if(bImagesLoaded) {
//        avgFbo.draw(0, 0);


	ofSetColor(255);
	
	if(cloneReady) {
		clone.draw(0, 0);
	} else {
		cam.draw(0, 0);
	}
	
	if(!tracker.getFound()) {
        ofDrawBitmapStringHighlight("camera face not found", 10, 10);
	}
        
    ///////////////////////////////
        startIndex++;
        endIndex++;

        if(endIndex >= images.size()-1) {
            startIndex = 0;
            endIndex = totalImages;
        }
    
        if(bDrawGui) {
            gui.draw();
            ofDrawBitmapString(ofToString(startIndex) + " / " + ofToString(ofGetFrameRate()), 5, ofGetHeight()-5);
        }
    } else {
        ofDrawBitmapString("Loading", ofGetWidth()/2, ofGetHeight()/2);
    }
}

//--------------------------------------------------------------
void ofApp::exit() {
    tracker.waitForThread();
}

void ofApp::loadImages() {

    bImagesStartLoading = true;

    imagesDir.listDir();
    images.resize(imagesDir.numFiles());
    
    for(int i = 0; i < imagesDir.numFiles(); i++){
        images[i].loadImage(imagesDir.getPath(i));
        if(images[i].getWidth() < imageWidth || images[i].getHeight() < imageHeight) {
            images[i].resize(imageWidth, imageHeight);
           
        }
        images[i].update();
    }

    bImagesLoaded = true;
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'f') {
        ofToggleFullscreen();
    } else if (key == 'g') {
        bDrawGui = !bDrawGui;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


vector<ofVec2f> ofApp::getSrcPoints() {
    vector<ofVec2f> pts;
    pts.push_back(ofVec2f(91.8534, 238.834));
    pts.push_back(ofVec2f(97.1524, 290.602));
    pts.push_back(ofVec2f(107.306, 341.773));
    pts.push_back(ofVec2f(122.897, 393.892));
    pts.push_back(ofVec2f(146.323, 445.755));
    pts.push_back(ofVec2f(180.62, 490.796));
    pts.push_back(ofVec2f(223.354, 525.781));
    pts.push_back(ofVec2f(271.096, 552.042));
    pts.push_back(ofVec2f(323.457, 562.238));
    pts.push_back(ofVec2f(374.665, 551.654));
    pts.push_back(ofVec2f(421.105, 525.986));
    pts.push_back(ofVec2f(462.14, 491.178));
    pts.push_back(ofVec2f(493.659, 445.756));
    pts.push_back(ofVec2f(513.472, 392.945));
    pts.push_back(ofVec2f(526.376, 339.748));
    pts.push_back(ofVec2f(534.937, 287.633));
    pts.push_back(ofVec2f(539.003, 235.092));
    pts.push_back(ofVec2f(126.616, 139.449));
    pts.push_back(ofVec2f(151.894, 114.404));
    pts.push_back(ofVec2f(187.203, 103.828));
    pts.push_back(ofVec2f(224.661, 107.528));
    pts.push_back(ofVec2f(260.171, 120.867));
    pts.push_back(ofVec2f(372.876, 118.832));
    pts.push_back(ofVec2f(408.098, 104.632));
    pts.push_back(ofVec2f(445.36, 100.079));
    pts.push_back(ofVec2f(480.717, 109.999));
    pts.push_back(ofVec2f(506.249, 134.622));
    pts.push_back(ofVec2f(317.28, 206.494));
    pts.push_back(ofVec2f(318.278, 243.588));
    pts.push_back(ofVec2f(319.369, 280.611));
    pts.push_back(ofVec2f(320.509, 317.331));
    pts.push_back(ofVec2f(287.744, 346.44));
    pts.push_back(ofVec2f(303.767, 348.923));
    pts.push_back(ofVec2f(319.731, 349.796));
    pts.push_back(ofVec2f(335.797, 348.442));
    pts.push_back(ofVec2f(351.754, 345.458));
    pts.push_back(ofVec2f(186.775, 231.289));
    pts.push_back(ofVec2f(207.933, 218.147));
    pts.push_back(ofVec2f(234.185, 217.954));
    pts.push_back(ofVec2f(256.423, 228.737));
    pts.push_back(ofVec2f(233.835, 238.717));
    pts.push_back(ofVec2f(209.504, 240.23));
    pts.push_back(ofVec2f(377.546, 227.173));
    pts.push_back(ofVec2f(399.721, 215.533));
    pts.push_back(ofVec2f(426.108, 215.062));
    pts.push_back(ofVec2f(447.762, 227.55));
    pts.push_back(ofVec2f(425.191, 237.146));
    pts.push_back(ofVec2f(400.692, 236.443));
    pts.push_back(ofVec2f(242.214, 430.129));
    pts.push_back(ofVec2f(264.752, 413.45));
    pts.push_back(ofVec2f(290.409, 398.214));
    pts.push_back(ofVec2f(320.199, 401.44));
    pts.push_back(ofVec2f(349.815, 397.488));
    pts.push_back(ofVec2f(375.704, 412.131));
    pts.push_back(ofVec2f(398.551, 428.123));
    pts.push_back(ofVec2f(375.817, 444.62));
    pts.push_back(ofVec2f(349.178, 454.946));
    pts.push_back(ofVec2f(320.68, 459.55));
    pts.push_back(ofVec2f(292.163, 455.608));
    pts.push_back(ofVec2f(265.341, 445.854));
    pts.push_back(ofVec2f(291.398, 422.948));
    pts.push_back(ofVec2f(320.365, 423.589));
    pts.push_back(ofVec2f(349.002, 422.21));
    pts.push_back(ofVec2f(348.848, 426.6));
    pts.push_back(ofVec2f(320.27, 429.495));
    pts.push_back(ofVec2f(291.539, 427.203));

    return pts;
}

