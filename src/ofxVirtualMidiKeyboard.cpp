#include "ofxVirtualMidiKeyboard.h"

ofxVirtualMidiKeyboard::~ofxVirtualMidiKeyboard()
{
    ofRemoveListener(ofEvents().keyPressed, this, &ofxVirtualMidiKeyboard::keyPressed);
    ofRemoveListener(ofEvents().keyReleased, this, &ofxVirtualMidiKeyboard::keyReleased);
    _activeMappingParameter.removeListener(this, &ofxVirtualMidiKeyboard::activeMappingChanged);
}

void ofxVirtualMidiKeyboard::setup(string name, int channel, int velocity, int octaveShift)
{
    _midiOut.openVirtualPort(name);
    ofAddListener(ofEvents().keyPressed, this, &ofxVirtualMidiKeyboard::keyPressed);
    ofAddListener(ofEvents().keyReleased, this, &ofxVirtualMidiKeyboard::keyReleased);

    _parameters.setName(name);
    _activeParameter.set("On/off", true);
    _channelParameter.set("Channel", channel, 1, 16);
    _velocityParameter.set("Velocity", velocity, 0, 127);
    _octaveShiftParameter.set("Octave shift", octaveShift, -6,6);

    _parameters.add(_activeParameter);
    _parameters.add(_channelParameter);
    _parameters.add(_velocityParameter);
    _parameters.add(_octaveShiftParameter);

    _mappingParameters.setName("Mapping");
    _activeMappingParameter.set("On/off", false);
    _activeMappingParameter.addListener(this, &ofxVirtualMidiKeyboard::activeMappingChanged);
    _mappingNoteParameter.set("Note", 0,0,11);
    _mappingParameters.add(_activeMappingParameter);
    _mappingParameters.add(_mappingNoteParameter);

    int keys_qwerty[12];
    keys_qwerty[0] = 97;
    keys_qwerty[1] = 119;
    keys_qwerty[2] = 115;
    keys_qwerty[3] = 101;
    keys_qwerty[4] = 100;
    keys_qwerty[5] = 102;
    keys_qwerty[6] = 116;
    keys_qwerty[7] = 103;
    keys_qwerty[8] = 121; //z 122
    keys_qwerty[9] = 104;
    keys_qwerty[10] = 117;
    keys_qwerty[11] = 106;

    setMapping(keys_qwerty);
}

void ofxVirtualMidiKeyboard::setActive(bool value)
{
    _activeParameter.set(value);
}

void ofxVirtualMidiKeyboard::setMappingActive(bool value)
{
    _activeMappingParameter.set(value);
}

void ofxVirtualMidiKeyboard::setMapping(int keys[])
{
    _keyMapping.clear();
    for(int i = 0; i < 12; i++)
    {
        _keyMapping[keys[i]] = 60+i;
    }
}

ofParameterGroup &ofxVirtualMidiKeyboard::getParameters()
{
    return _parameters;
}

ofParameterGroup &ofxVirtualMidiKeyboard::getMappingParameters()
{
    return _mappingParameters;
}

void ofxVirtualMidiKeyboard::keyPressed(ofKeyEventArgs &arg)
{
    if(!_activeParameter)
    {
        if(_activeMappingParameter)
        {
            assignKeyToCurrentNote(arg.key);
        }
        return;
    }
    if(_keyMapping.find(arg.key) != _keyMapping.end())
    {
        int note = _keyMapping[arg.key] + 12*_octaveShiftParameter;
        _midiOut.sendNoteOn(_channelParameter, note, _velocityParameter);
    }
}

void ofxVirtualMidiKeyboard::keyReleased(ofKeyEventArgs &arg)
{
    if(!_activeParameter)
    {
        return;
    }
    if(_keyMapping.find(arg.key) != _keyMapping.end())
    {
        int note = _keyMapping[arg.key] + 12*_octaveShiftParameter;
        _midiOut.sendNoteOff(_channelParameter, note, _velocityParameter);
    }
}

void ofxVirtualMidiKeyboard::activeMappingChanged(bool &value)
{
    if(value)
    {
        _activeParameter.set(!value);
        _mappingNoteParameter = 0;
    }
    else
    {
        _mappingNoteParameter = 0;
    }
}

void ofxVirtualMidiKeyboard::assignKeyToCurrentNote(int key)
{
    _keyMapping[key] = _mappingNoteParameter + 60;
    if(_mappingNoteParameter < _mappingNoteParameter.getMax())
    {
        _mappingNoteParameter++;
    }
    else
    {
        _mappingNoteParameter = 0;
        _activeMappingParameter = false;
    }
    if(_keyMapping.size() > 12)
    {
        _keyMapping.clear();
        _mappingNoteParameter = 0;
    }
}
