//
//  Track.h
//  emptyExample
//
//  Created by Jason Levine on 2013-05-29.
//
//
#include "DNA.h"
#include "MidiFile.h"

enum { noteOn, noteOff };

struct trackEvent {
    int prog;
    int midiMessage;
    int note;
    int vel;
};

class Track {
public:
    Track(DNA _dna, ofVec2f _pos);
    void expressGenes();
    void createMidiFile();
    void exportMidiFile(string name);
    void draw(int event);
    float getFitness();
    void setFitness(float fit) { fitness = fit; }
    DNA getDNA();
    void rollover(int mouseX, int mouseY);
    bool isPlaying() { return playing; }
    MidiFile * getMidifile() { return outputFile; }
    float getTempo() { return tempo; }
    vector<ofVec2f> getRemix();
    int getProg() { return prog; }
    vector<float> getProgs();
    vector<vector<trackEvent> > getMidiData() { return midiData; }
    void saveTrack(string name);
    
    DNA dna;
    
    
private:
    vector<vector<trackEvent> > midiData;
    
    float fitness;
    ofVec2f pos;
    int width, height;
    bool rolloverOn;
    MidiFile * outputFile;
    bool playing;
    float tempo;
    float remixX, remixY;
    int prog;
    
    
};
