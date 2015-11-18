Infinite Self
=============

Identity is fluid.

![infinite-self](infinite-self.mov.gif)

## Deploying to Mac Mini

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

### 0.2.0

 - Added face replacement
 - Included ps3eye cam