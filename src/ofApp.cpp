#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetFrameRate(30);
	ofSetVerticalSync(true);
    ofEnableSmoothing();
    ofSetLogLevel(OF_LOG_NOTICE);


    // note that this is outside the project,
    // will need to provide pre-formatted images
    // a folder above the project
    // switch to use sampleImages to test
    imagesDirPath = ofToDataPath("../../../sharedData/images/selfie/", true);
//    imagesDirPath = "sampleImages/";
    

    imageHeight = 640;
    imageWidth = 640;
    totalImages = 10;
    startIndex = 0;
    endIndex = totalImages;
    snapshotFrameCount = 0;
    srcImageFound = false;

    avgShader.load("shaders/avg");
    avgFbo.allocate(imageHeight, imageWidth);
    
    
    bImagesLoaded = false;
    bImagesStartLoading = false;
    
    bDrawGui = true;
    gui.setup();
    gui.add(dMultiply.setup("Displacement", 0.3, 0.0, 10.0));
    gui.add(cloneStrength.setup("Clone Strength", 16, 0, 50));
    gui.add(snapshotFrames.setup("Snapshot frames", 150, 0, 1000));

   
    #ifdef USE_MACAM
        cam.setDesiredFrameRate(30);
        cam.setLed(false);
    #endif
    cam.initGrabber(640, 480);

    camShader.load("shaders/cam");
   
    tracker.setup();
	ofFbo::Settings settings;
	settings.width = cam.getWidth();
	settings.height = cam.getHeight();
	maskFbo.allocate(settings);
	srcFbo.allocate(settings);
    camFbo.allocate(settings);
    clone.setup( cam.getWidth(), cam.getHeight());

    ofSetWindowShape(cam.getWidth(), cam.getHeight());
    
    imagesDir.open(imagesDirPath);
    imagesDir.allowExt("jpg");
    
    font.loadFont("fonts/LetterGothicStd.otf", 34);


}

//--------------------------------------------------------------
void ofApp::update(){
    if(!bImagesLoaded && !bImagesStartLoading) {
        loadImages();
        srcPoints = getSrcPoints();
    } else {
    
    
        // manage our timer for the source image
        // this can be added to updateSrcImage eventually
        if(srcImageFound) {
            if(snapshotFrameCount > snapshotFrames) {
                snapshotFrameCount = 0;
                srcImageFound = false;
            } else {
                snapshotFrameCount++;
            }
        }
        
        updateImageAverage();
        
        cam.update();
        if(cam.isFrameNew()) {
        
            // if we don't already have a source image set
//            if(!srcImageFound) {

                tracker.update(ofxCv::toCv(cam));
            
                srcImageFound = tracker.getFound();
            
//                if(tracker.getFound()) {
                    // this will set srcImagFound to true
//                    srcImageFound = updateSrcImage();

//                }
                if(srcImageFound) {
                    updateSrcImage();
                    
                    cout << "SOURCE IMAGE FOUND: " << srcImageFound << endl;
                    
                    updateClone();
                }
                
//            } else {
//                updateClone();
//            }
        }
    }
    
}


//--------------------------------------------------------------
void ofApp::draw(){
    ofClear(0);
	ofSetColor(255);
    
    if(bImagesLoaded) {
        ofPushMatrix();
            ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        
            if(bAlwaysShowCamera) {
                cam.draw(-cam.getWidth()/2, -cam.getHeight()/2, cam.getWidth(), cam.getHeight());
            } else if (srcImageFound) {
                clone.draw(-cam.getWidth()/2, -cam.getHeight()/2, cam.getWidth(), cam.getHeight());
                font.drawStringCentered("@"+names[endIndex], 0, cam.getHeight()/2-50);

            } else {
                avgFbo.draw(-avgFbo.getWidth()/2, -avgFbo.getHeight()/2);
                font.drawStringCentered("@"+names[endIndex], 0, imageHeight/2-50);
            }
        
        ofPopMatrix();
        
    
        if(bDrawGui) {
            gui.draw();
            ofDrawBitmapString(ofToString(startIndex) + " / " + ofToString(ofGetFrameRate()), 5, ofGetHeight()-5);
        }
    }
}

//--------------------------------------------------------------
void ofApp::exit() {
    // correctly cload out face tracker thread
//    tracker.waitForThread();
}

void ofApp::updateImageAverage() {
    avgFbo.begin();
        avgShader.begin();
            avgShader.setUniform1f("dMultiply", dMultiply);
            avgShader.setUniform2f("direction",
                ofMap(mouseX, 0, ofGetWidth(), -1, 1, true),
                ofMap(mouseY, 0, ofGetHeight(), -1, 1, true)
            );

             for(int j=0; j < totalImages; j++) {
                string name = "tex"+ofToString(j+1);
                int index = startIndex+j+1;
                avgShader.setUniformTexture(name, images[index].getTextureReference(), j+1);
              }

            images[startIndex].draw(0, 0);
        avgShader.end();
    avgFbo.end();
    
    // update index iterator
    if(endIndex >= images.size()-1) {
        startIndex = 0;
        endIndex = totalImages;
    } else {
        startIndex++;
        endIndex++;
    }
}

bool ofApp::updateSrcImage() {


    // update the camera fbo
    camFbo.begin();
        ofClear(0, 255);
            camShader.begin();
                    cam.draw(0, 0);
            camShader.end();
        ofPopMatrix();
    camFbo.end();

//    srcImage.setFromPixels(cam.getPixels(), cam.getWidth(), cam.getHeight(), OF_IMAGE_COLOR);

    // update the camera mesh from the source image
    camMesh = tracker.getImageMesh();
    camMesh.clearTexCoords();
    camMesh.addTexCoords(srcPoints);

    
    cout << "UPDATE CAM MESH" << endl;
    

    
    // set our image found property to true
    return true;
}

void ofApp::updateClone() {

    cout << "UPDATE CLONE" << endl;

    // update masking fbo from camera mesh
    maskFbo.begin();
        ofClear(0, 255);
        camMesh.draw();
    maskFbo.end();
    
    
    // update source image fbo from averaged image
    srcFbo.begin();
        ofClear(0, 255);
        avgFbo.getTextureReference().bind();
        camMesh.draw();
        avgFbo.getTextureReference().unbind();
    srcFbo.end();

    // update clone
    clone.setStrength(cloneStrength);
    clone.update(srcFbo.getTextureReference(), camFbo.getTextureReference(), maskFbo.getTextureReference());
}


// @todo store images and names in a struct rather than separate arrays
void ofApp::loadImages() {

    // indicate that we've started loading
    bImagesStartLoading = true;

    // get file in directory and adjust the size of
    // the images deque accordingly
    imagesDir.listDir();
    images.resize(imagesDir.numFiles());
    
    // iterate through files in the directory
    for(int i = 0; i < imagesDir.numFiles(); i++){
    
        // add names to vector of names
        string name = imagesDir.getName(i);
        name.erase(name.end()-8, name.end());
        names.push_back(name);
        
        // load images into deque using file path
        images[i].loadImage(imagesDir.getPath(i));
        
        // resize image if not at desire size
        if(images[i].getWidth() != imageWidth || images[i].getHeight() != imageHeight) {
            images[i].resize(imageWidth, imageHeight);
           
        }
        // update the image to bind texture, etc
        images[i].update();
    }
    
    // indicate that we're done loading images
    bImagesLoaded = true;
}


vector<ofVec2f> ofApp::getSrcPoints() {
    vector<ofVec2f> pts;
 
    // create new tracker to hold texture-mapping points
    ofxFaceTracker imgTracker;
    imgTracker.setup();
	imgTracker.setIterations(25);
	imgTracker.setAttempts(4);
    
    // loop through images until a face is found and
    // use that one as the source for image points
    for(int i = 0; i < images.size(); i++) {
        imgTracker.update(ofxCv::toCv(images[i]));
        if(imgTracker.getFound()) {
            ofLogVerbose("image points found!");
            pts = imgTracker.getImagePoints();
            break;
        }
        imgTracker.reset();
    }
    

    return pts;
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'f') {
        ofToggleFullscreen();
    } else if (key == 'g') {
        bDrawGui = !bDrawGui;
    } else if (key == 'c') {
        bAlwaysShowCamera = !bAlwaysShowCamera;
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




