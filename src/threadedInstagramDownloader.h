//
//  threadedInstagramDownloader.h
//  infiniteSelf
//
//  Created by Andrew Dayton on 11/5/15.
//
//

#include "ofMain.h"
#include "ofxInstagram.h"


class threadedInstagramDownloader : public ofThread {
    
    public:
    
        
    private:
        virtual void threadedFunction();
    
}