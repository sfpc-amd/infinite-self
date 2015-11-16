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
    
    bImagesLoaded = false;
    bImagesStartLoading = false;
    
    bDrawGui = true;
    gui.setup();
    gui.add(dMultiply.setup("Displacement", 0.3, 0.0, 10.0));
    
    cout << imagesDirPath << endl;
    
    imagesDir.open(imagesDirPath);
    imagesDir.allowExt("jpg");

}

//--------------------------------------------------------------
void ofApp::update(){
    if(!bImagesLoaded && !bImagesStartLoading) {
        loadImages();
    }
}


//--------------------------------------------------------------
void ofApp::draw(){
    ofClear(0);
    
    if(bImagesLoaded) {
        ofPushMatrix();
         avgFbo.begin();
        

            ofTranslate(ofGetWidth()/2, ofGetHeight()/2);

        
            ofRect(-imageWidth/2, -imageHeight/2, imageWidth, imageHeight);

            avgShader.begin();
        
                avgShader.setUniform1f("dMultiply", dMultiply);// ofMap(mouseY, 0, ofGetHeight(), 0.0, 10.0));
        
                 for(int j=0; j < totalImages; j++) {
                    string name = "tex"+ofToString(j+1);
                    int index = startIndex+j+1;
                    
                    avgShader.setUniformTexture(name, images[index].getTextureReference(), j+1);
                  }

                images[startIndex].draw(-imageWidth/2, -imageHeight/2);
            avgShader.end();
        avgFbo.end();
       
        
        avgFbo.draw(-imageWidth/2, -imageHeight/2);
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
void ofApp::exit()
{
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
