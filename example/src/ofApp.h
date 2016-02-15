#pragma once

#include "ofMain.h"
#include "ofxVirtualMidiKeyboard.h"
#include "ofxGui.h"


class ofApp : public ofBaseApp
{
	
public:
	
	void setup();
	void update();
	void draw();
	void exit();
	
	void keyPressed(int key);
	void keyReleased(int key);
	
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();
private:
    ofxVirtualMidiKeyboard _virtualMidiKeyboard;
    ofxPanel _gui;
};
