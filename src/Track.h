//
//  Track.h
//  emptyExample
//
//  Created by Jason Levine on 2013-05-29.
//
//
#include "DNA.h"
#include "MidiFile.h"


class Track {
public:
    Track(DNA _dna, ofVec2f _pos);
    void createMidiFile();
    void draw();
    float getFitness();
    DNA getDNA();
    void rollover(int mouseX, int mouseY);
    bool isPlaying() { return playing; }
    MidiFile * getMidifile() { return outputFile; }
    float getTempo() { return tempo; }
    ofVec2f getRemix() { return ofVec2f(remixX, remixY); }
    int getProg() { return prog; }
    vector<float> getProgs();
    
private:
    DNA dna;
    float fitness;
    ofVec2f pos;
    int size;
    bool rolloverOn;
    MidiFile * outputFile;
    bool playing;
    float tempo;
    float remixX, remixY;
    int prog;
    
};
