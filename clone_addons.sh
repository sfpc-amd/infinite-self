#!/bin/bash

cd ../../../addons

if [ -z $1 ]; then
    PREFIX="git clone https://github.com/"
else
    PREFIX="git clone git@github.com:"
fi

${PREFIX}kylemcdonald/ofxCv.git

${PREFIX}kylemcdonald/ofxFaceTracker.git

${PREFIX}andreasmuller/ofxAutoReloadedShader.git

${PREFIX}paulobarcelos/ofxMacamPs3Eye.git