#!/bin/bash
install_name_tool -change /usr/local/lib/libSDL2-2.0.0.dylib @executable_path/../Plugins/libSDL2-2.0.0.dylib ${PWD}/Hypnos.app/Contents/MacOs/Hypnos
