#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

	ofSetFrameRate(60);
	ofSetVerticalSync(true);
    ofEnableSmoothing();
    ofSetLogLevel(OF_LOG_NOTICE);

    imagesDir = "img/";

    imageHeight = 640;
    imageWidth = 640;
    totalImages = 10;
    startIndex = 0;
    endIndex = totalImages;
    
    
    avgShader.load("shaders/avg");
    
    bImagesLoaded = false;
    bImagesStartLoading = false;
    
    bDrawGui = true;
    gui.setup();
    gui.add(dMultiply.setup("Displacement", 0.3, 0.0, 10.0));
    
    cam.initGrabber(640, 480);
    tracker.setup();

}

//--------------------------------------------------------------
void ofApp::update(){
    if(!bImagesLoaded && !bImagesStartLoading) {
        loadImages();
    } else {
        cam.update();
        if(cam.isFrameNew()) {
            tracker.update(ofxCv::toCv(cam));
            position = tracker.getPosition();
            scale = tracker.getScale();
            orientation = tracker.getOrientation();
            rotationMatrix = tracker.getRotationMatrix();
        }
    }
    
}


//--------------------------------------------------------------
void ofApp::draw(){
    ofClear(0);
    
    if(bImagesLoaded) {
        ofPushMatrix();
         avgFbo.begin();


            ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        
            if(tracker.getFound()) {
                ofxCv::applyMatrix(rotationMatrix);
            }
        
//            ofRect(-imageWidth/2, -imageHeight/2, imageWidth, imageHeight);

//            if(tracker.getFound()) {
//                tracker.getObjectMesh().draw();
//            }

            avgShader.begin();
        
                avgShader.setUniform1f("dMultiply", dMultiply);// ofMap(mouseY, 0, ofGetHeight(), 0.0, 10.0));
        
                 for(int j=0; j < totalImages+1; j++) {
                    string name = "tex"+ofToString(j);
                    int index = startIndex+j+1;
                    
                    avgShader.setUniformTexture(name, images[index].getTextureReference(), j);
                  }

//                images[startIndex].draw(-imageWidth/2, -imageHeight/2);
//                    if(tracker.getFound()) {
//                        ofScale(7.0, 7.0, 7.0);
//                        tracker.getObjectMesh().draw();
//                    } else {
                        ofRect(-imageWidth/2, -imageHeight/2, imageWidth, imageHeight);
//                    }
            avgShader.end();
        avgFbo.end();
       
        
//        avgFbo.draw(-imageWidth/2, -imageHeight/2);
        
//        avgFbo.getTextureReference().bind();
        
        ofPixels fboPixels;
        ofImage image;
        
        fboPixels.allocate(imageWidth, imageHeight, 3);
        
        avgFbo.readToPixels(fboPixels);
        image.setFromPixels(fboPixels);
        
        image.bind();
        
        if(tracker.getFound()) {
            ofScale(7.0, 7.0, 7.0);
            tracker.getObjectMesh().draw();
        } else {
            ofRect(-imageWidth/2, -imageHeight/2, imageWidth, imageHeight);
        }
        
        image.unbind();
      
//        avgFbo.getTextureReference().unbind();
        
        
        ofPopMatrix();

        
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

    ofDirectory dir(imagesDir);
    dir.allowExt("jpg");
    dir.listDir();
    
    images.resize(dir.numFiles());
    
    for(int i = 0; i < dir.numFiles(); i++){
        images[i].loadImage(dir.getPath(i));
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
