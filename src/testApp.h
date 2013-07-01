#pragma once

#include "ofMain.h"
#include "Population.h"
#include "ofxTimer.h"
#include "ofxAudioUnit.h"

struct notePlayed {
    int note;
    int prog;
};

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void audioOut(float * input, int bufferSize, int nChannels);
    void setTempo(MidiFile& midifile, int index, double& tempo);
    float applyTempo(float ms, MidiFile * file) { return ms * 60.0f / tempo / file->getTicksPerQuarterNote() * 1000.0f; } ;
    
    
    ofSoundStream soundStream;
    
    Population  * population;
    ofFbo expressionFbo;
    vector<float> target;
    
    int currentTrack;
    
    double      tempo;
    ofxTimer  * timer;
    int         eventCounter;
    int         noteOnCounter;
    float       timeCounter;
    float       lastNoteTime;
    MidiFile * currentMidiFile;
    vector<float> progs;
    vector<notePlayed> notesPlayed;
    
    
    vector<ofxAudioUnitSampler> sampler;
    ofxAudioUnitMixer mixer;
    ofxAudioUnitOutput  output;
};
