#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

	ofSetFrameRate(60);
	ofSetVerticalSync(true);
    ofEnableSmoothing();
    ofSetLogLevel(OF_LOG_NOTICE);

    imagesDir = "img/";

instagram.setup("1700247.32b0b80.919c867a6b794dde8400a32d87339ba5","self");
    instagram.setCertFileLocation(ofToDataPath("ca-bundle.crt",false));
   
    imageHeight = 640;
    imageWidth = 640;
    imageSize = imageWidth*imageHeight*3;
    
    totalImages = 10;
    startIndex = 0;
    endIndex = totalImages;
    
    bImagesAlloc = false;
    
    avgShader.load("shaders/avg");
    
    avgImage.allocate(imageWidth, imageHeight, OF_IMAGE_COLOR);
    avgImage.setColor(ofColor(0));
    avgImage.update();
    
//    fetchImages();
    
    loadImages();
    
    bDrawGui = true;
    gui.setup();
    gui.add(dMultiply.setup("Displacement", 0.3, 0.0, 10.0));



//    ofDirectory dir(imagesDir);
//    dir.allowExt("jpg");
//    dir.listDir();
//    
//    for(int i = 0; i < dir.numFiles(); i++){
//        ofLogNotice(dir.getPath(i));
//    }
}

//--------------------------------------------------------------
void ofApp::update(){

        bImagesAlloc = imagesAllocated(images, startIndex, endIndex);
}


//--------------------------------------------------------------
void ofApp::draw(){
    ofClear(0);
    
    if (bImagesAlloc) {

        ofPushMatrix();
         avgFbo.begin();
        

            ofTranslate(ofGetWidth()/2, ofGetHeight()/2);

        
            ofRect(-imageWidth/2, -imageHeight/2, imageWidth, imageHeight);

            avgShader.begin();
        
                avgShader.setUniform1f("dMultiply", dMultiply);// ofMap(mouseY, 0, ofGetHeight(), 0.0, 10.0));
        
                 for(int j=0; j < totalImages; j++) {
                    string name = "tex"+ofToString(j+1);
                    int index = startIndex+j+1;
                    
                    cout << images[index].getWidth() << "," << images[index].getHeight() << endl;
                    
//                    cout << name << ", " << index << endl;
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
    }
    
    if(bDrawGui) {
        gui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::exit()
{
    getImages.stopThread();
}

void ofApp::loadImages() {

    ofDirectory dir(imagesDir);
    dir.allowExt("jpg");
    dir.listDir();
    
    images.resize(dir.numFiles());
    
    for(int i = 0; i < dir.numFiles(); i++){
        getImages.loadFromDisk(images[i], dir.getPath(i));
    }

}

void ofApp::fetchImages() {

    int maxCount = 100;
    

    images.clear();
    instagram.getListOfTaggedObjectsNormal("selfie", 50);
    paginationIds.push_back(instagram.getMaxIdForPagination());
    updateImages();

    cout << "images fetched! " << instagram.getImageURL().size() << endl;
}

void ofApp::updateImages() {

    deque<ofImage> imagesTmp;

    if (!instagram.getImageURL().empty())
    {
        imagesTmp.resize(instagram.getImageURL().size());
        for ( int i = 0; i < instagram.getImageURL().size(); i++)
        {
            getImages.loadFromURL(imagesTmp[i], instagram.getImageURL()[i]);
            
        }
    }
    
    while(!imagesAllocated(imagesTmp, 0, imagesTmp.size())) {
        ofLogNotice("Waiting for images to load");
    }
    
    for(int i = 0; i < imagesTmp.size(); i++) {
        imagesTmp[i].saveImage(imagesDir + instagram.getImageID()[i] + ".jpg");
    }
    
}

bool ofApp::imagesAllocated(deque<ofImage>& img, int start, int end){
    bool alloc = false;
    
    if(!img.empty()) {
        alloc = true;
        // counting down -- just a guess but maybe
        // tha later ones in the array will load last?
        for(int i=end-1; i>=start; i--) {
            // if we find an image that isn't allocated,
            // set to false and break out of the loop
            if(!img[i].isAllocated()) {
                alloc = false;
                break;
            }
            
            if(img[i].getWidth() < imageWidth || img[i].getHeight() < imageHeight) {
                img[i].resize(imageWidth, imageHeight);
               
            }
            
//            images[i].saveImage(imagesDir + instagram.getImageID()[i] + ".jpg");
        }
    }
    
    if(alloc) {
        cout << "images allocated!";
    }
    
    return alloc;
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'f') {
        ofToggleFullscreen();
    } else if (key == 'g') {
        bDrawGui = !bDrawGui;
    } else if (key == 'r') {
        fetchImages();
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
