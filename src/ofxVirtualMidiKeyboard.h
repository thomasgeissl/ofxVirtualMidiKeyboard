#pragma once
#include "ofMain.h"
#include "ofxMidi.h"

class ofxVirtualMidiKeyboard
{
public:
    ~ofxVirtualMidiKeyboard()
    {
        ofRemoveListener(ofEvents().keyPressed, this, &ofxVirtualMidiKeyboard::keyPressed);
        ofRemoveListener(ofEvents().keyReleased, this, &ofxVirtualMidiKeyboard::keyReleased);
        _activeMappingParameter.removeListener(this, &ofxVirtualMidiKeyboard::activeMappingChanged);
    }

    void setup(string name = "ofxMidiVirtualKeyboard", int channel = 1, int velocity = 127, int octaveShift = 0)
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
    }
    void setActive(bool value)
    {
        _activeParameter.set(value);
    }
    void setMappingActive(bool value)
    {
        _activeMappingParameter.set(value);
    }

    void setMapping(int keys[12])
    {
        _keyMapping.clear();
        for(int i = 0; i < 12; i++)
        {
            _keyMapping[keys[i], 60+i];
        }

    }
    ofParameterGroup & getParameters()
    {
        return _parameters;
    }
    ofParameterGroup & getMappingParameters()
    {
        return _mappingParameters;
    }

protected:
private:
    void keyPressed(ofKeyEventArgs & arg)
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
            _midiOut.sendNoteOn(_channelParameter, note);
        }
    }
    void keyReleased(ofKeyEventArgs & arg)
    {
        if(!_activeParameter)
        {
            return;
        }
        if(_keyMapping.find(arg.key) != _keyMapping.end())
        {
            int note = _keyMapping[arg.key] + 12*_octaveShiftParameter;
            _midiOut.sendNoteOff(_channelParameter, note);
        }
    }
    void activeMappingChanged(bool & value)
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
    void assignKeyToCurrentNote(int key)
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
