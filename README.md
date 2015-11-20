Infinite Self
=============

Identity is fluid.

![infinite-self](infinite-self.mov.gif)


## Note

 * In its current state **the app will crash if the ps3Eye isn't plugged in**.
 * **It takes a long time to load** since all the umages load up front


## Deploying to Mac Mini

The deploy script will make a backup of the current version on the Mac Mini and then `rsync` over whatever is currently in the "data" directory.

```bash
./scripts/deploy.sh
```
## Setup

You can install the necessary addons by running the install script:

```bash
cd /path/to/app/directory/
./scripts/clone_addons.sh
```

### Addon Dependencies 
 * _ofxGui (comes installed with oF)_
 * _ofxOpenCv (comes installed with oF)_
 * [ofxCv](https://github.com/kylemcdonald/ofxCv)
 * [ofxFaceTracker](https://github.com/kylemcdonald/ofxFaceTracker)
 * [ofxAutoReloadedShader](https://github.com/andreasmuller/ofxAutoReloadedShader)
 
Also, optionally:
 * [ofxMacamPs3Eye](https://github.com/paulobarcelos/ofxMacamPs3Eye)

(uncomment `#define USE_MACAM` in `ofApp.h` if you plan to use the ps3Eye)



## Release Notes

### 1.0.1
- Hide cursor in full screen mode

### 1.0.0
 - This is what was used for the show launch
 - Added username text
 - Set face tracking to show video (no photo booth mode)

### 0.2.0

 - Added face replacement
 - Included ps3eye cam