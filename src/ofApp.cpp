#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetFrameRate(60);
	ofSetVerticalSync(true);
    ofEnableSmoothing();
    ofSetLogLevel(OF_LOG_VERBOSE);

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
    
    fetchImages();
    
    bDrawGui = true;
    gui.setup();
    gui.add(dMultiply.setup("Displacement", 0.3, 0.0, 10.0));

//	cam.initGrabber(640, 480);
//	tracker.setup();

    
//    sample1.loadImage("sample1.png");
//    sample2.loadImage("sample2.png");
//    sample3.loadImage("sample3.png");
//    sample4.loadImage("sample4.png");
//    sample5.loadImage("sample5.png");
//    sample6.loadImage("sample6.png");
}

//--------------------------------------------------------------
void ofApp::update(){

//    cam.update();
//    
//	if(cam.isFrameNew()) {
//		tracker.update(ofxCv::toCv(cam));
//        ofVec3f orientation = tracker.getOrientation();
//        ofVec3f euler = ofVec3f(
//            orientation.x
//            , orientation.y
//            , orientation.z
//        );
//        orientationMatrix.makeRotationMatrix(
//            ofRadToDeg(euler.x)
//            , ofVec3f(1,0,0)
//            , ofRadToDeg(euler.y)
//            , ofVec3f(0,1,0)
//            , ofRadToDeg(euler.z)
//            , ofVec3f(0,0,1)
//        );
//	}


    
//    updateImageAverage();

//    if(!bImagesAlloc) {
        // do the check
        bImagesAlloc = imagesAllocated(images, startIndex, endIndex);
//    }


//    if(ofGetFrameNum() % 1000 == 0) {
//        bImagesAlloc = false;
//        fetchImages();
//    }
    
}


//--------------------------------------------------------------
void ofApp::draw(){
    ofClear(0);
//    avgImage.draw(0, 0);

//    glEnable(GL_EXT_texture_array);
    
    if (bImagesAlloc) {

        ofPushMatrix();
         avgFbo.begin();
        
//            float centerLeft = ofGetWidth()/2-(imageWidth/2);
//            float centerTop = ofGetHeight()/2-(imageHeight/2), imageWidth, imageHeight;
        
        
            ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
//            ofxCv::applyMatrix(orientationMatrix);

        
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
//    tracker.waitForThread();
}

void ofApp::updateImageAverage() {
    if(!images.empty()) {
        // ignore if images are already allocated
        if(!bImagesAlloc) {
            // do the check
            bImagesAlloc = imagesAllocated(images, startIndex, endIndex);
        }

        // if allocated, go ahead
        if (bImagesAlloc) {
            
            // get the pixels from our source image
            unsigned char * pixels = avgImage.getPixels();
            // get the number of pixels in our source image
            // store our averaged pixel values
            int avg;

            for(int i = 0; i < imageSize; i++) {
                avg = 0;
                
                for(int j=startIndex; j < endIndex; j++) {
                    avg += images[j].getPixels()[i];
                }
                
                avg = floor(avg / images.size());
                pixels[i] = avg;
            }

            avgImage.update();


            // http://opencv-users.1802565.n2.nabble.com/How-to-calculate-the-average-pixel-by-pixel-of-n-images-td2613763.html
            
//            cv::Mat img;
//            cv::Mat sum;
//            cv::Mat avg = ofxCv::toCv(avgImage);
//            ofxCv::imitate(avg, img);
//            for(int j=startIndex; j < endIndex; j++) {
//                img = ofxCv::toCv(images[j]);
//                cv::accumulate(img, sum);
//            }
//            ofxCv::copy(sum, avg);
            
         }
        
         startIndex++;
         endIndex++;
        
         if(endIndex >= images.size()-1) {
            startIndex = 0;
            endIndex = totalImages;
         }

    }
}

void ofApp::fetchImages() {

    int maxCount = 100;
    

    images.clear();
    instagram.getListOfTaggedObjectsNormal("selfie", 50);
    paginationIds.push_back(instagram.getMaxIdForPagination());
    updateImages();


//    while(instagram.getImageURL().size() < maxCount && paginationIds.size()) {
//        instagram.getListOfTaggedObjectsPagination("selfie", 30,paginationIds.back());
//        updateImages();
//        paginationIds.push_back(instagram.getMaxIdForPagination());
    
//    }

    cout << "images fetched! " << instagram.getImageURL().size() << endl;
}

void ofApp::updateImages() {
    if (!instagram.getImageURL().empty())
    {
        images.resize(instagram.getImageURL().size());
        for ( int i = 0; i < instagram.getImageURL().size(); i++)
        {
            getImages.loadFromURL(images[i], instagram.getImageURL()[i]);
        }
    }
}

bool ofApp::imagesAllocated(deque<ofImage>& images, int start, int end){
    bool alloc = false;
    
    if(!images.empty()) {
        alloc = true;
        // counting down -- just a guess but maybe
        // tha later ones in the array will load last?
        for(int i=end-1; i>=start; i--) {
            // if we find an image that isn't allocated,
            // set to false and break out of the loop
            if(!images[i].isAllocated()) {
                alloc = false;
                break;
            }
            
            if(images[i].getWidth() < imageWidth || images[i].getHeight() < imageHeight) {
                cout << "resize img" << endl;
                images[i].resize(imageWidth, imageHeight);
                cout << images[i].getWidth() << "," << images[i].getHeight() << endl;
               
            }
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
