#pragma once
#include "ofMain.h"
#include "ofxMidi.h"

class ofxVirtualMidiKeyboard
{
public:
    ~ofxVirtualMidiKeyboard();

    void setup(string name = "ofxMidiVirtualKeyboard", int channel = 1, int velocity = 127, int octaveShift = 0);
    void setActive(bool value = true);
    void setMappingActive(bool value = true);
    void setMapping(int keys[12]);
    ofParameterGroup & getParameters();
    ofParameterGroup & getMappingParameters();

protected:
private:
    void keyPressed(ofKeyEventArgs & arg);
    void keyReleased(ofKeyEventArgs & arg);
    void activeMappingChanged(bool & value);
    void assignKeyToCurrentNote(int key);

private:
    ofxMidiOut _midiOut;
    map<int, int> _keyMapping;

    ofParameterGroup _parameters;
    ofParameter<bool> _activeParameter;
    ofParameter<int> _channelParameter;
    ofParameter<int> _velocityParameter;
    ofParameter<int> _octaveShiftParameter;

    ofParameterGroup _mappingParameters;
    ofParameter<bool> _activeMappingParameter;
    ofParameter<int> _mappingNoteParameter;
};
