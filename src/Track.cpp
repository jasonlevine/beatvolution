//
//  Track.cpp
//  emptyExample
//
//  Created by Jason Levine on 2013-05-29.
//
//

#include "Track.h"


Track::Track(DNA _dna, ofVec2f _pos) {
    dna = _dna;
    pos = _pos;
    fitness = 1;
    size = 70;
    playing = false;
}

void Track::createMidiFile() {
    outputFile = new MidiFile();
    outputFile->absoluteTime();  // time information stored as absolute time
    // (will be coverted to delta time when written)
    outputFile->addTrack(1);     // Add  track to the MIDI file
    Array<uchar> midievent;     // temporary storage for MIDI events
    midievent.setSize(3);       // set the size of the array to 3 bytes
    int tpq = 120;              // default value in MIDI file is 48
    outputFile->setTicksPerQuarterNote(tpq);
    
    tempo = ofMap(dna.genes[48], 0.0, 1.0, 90, 150);
    remixX = dna.genes[49];
    remixY = dna.genes[50];
    prog = ofMap(dna.genes[51], 0.0, 1.0, 0, 11);
    
    
    
       

//    int microTempo = 1.0 / 120.0 * 6000000;
//    
//    Array<uchar> tempoEvent;     
//    tempoEvent.setSize(6);
//    tempoEvent[0] = 0xFF; //FF 51 03
//    tempoEvent[1] = 0x51;
//    tempoEvent[2] = 0x03;
//    tempoEvent[3] = microTempo << 16;
//    tempoEvent[4] = microTempo << 8;
//    tempoEvent[5] = microTempo << 0;
//    outputFile->addEvent(0, 0, tempoEvent);
    
    int actiontime = 0;      
    for (int i = 0; i < 16; i++) {
        actiontime = tpq * ofMap(dna.genes[i], 0.0, 1.0, 0, 8);
        midievent[0] = 0x8A;     // store a note on command (MIDI channel 1)
        midievent[1] = ofMap(dna.genes[i+16], 0.0, 1.0, 36, 89); //c1 to f5
        midievent[2] = ofMap(dna.genes[i+32], 0.0, 1.0, 0, 127);       // store attack/release velocity for note command
//        midievent[3] = ofMap(dna.genes[i+48], 0.0, 1.0, 0, 2);
        outputFile->addEvent(1, actiontime, midievent);
        
        midievent[0] = 0x9A;     // store a note off command (MIDI channel 1)
        
        outputFile->addEvent(1, actiontime+tpq, midievent);
    }
    
    outputFile->sortTracks();         // make sure data is in correct order
    
//    string filename = ofToDataPath(ofGetTimestampString() + ".mid");
//    outputFile->write(filename.c_str());
    
    
}

void Track::draw() {
    ofSetRectMode(OF_RECTMODE_CENTER);
    if (rolloverOn) ofSetColor(0);
    else ofSetColor(64);

    ofRect(pos.x, pos.y, size, size);
    // Display fitness value
    ofSetColor(0, 0, 0);
    ofDrawBitmapString(ofToString(fitness, 2), pos.x-size/2, pos.y+size/2 + 10);
}

void Track::rollover(int mouseX, int mouseY) {
    if (mouseX > pos.x - size / 2 && mouseX < pos.x + size / 2 && mouseY > pos.y - size / 2 && mouseY < pos.y + size / 2) {
        rolloverOn = true;
        fitness += 0.25;
        playing = true;
    } else {
        rolloverOn = false;
        playing = false;
    }
}

DNA Track::getDNA() {
    return dna;
}

float Track::getFitness() {
    return fitness;
}
