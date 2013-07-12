//
//  Track.cpp
//  emptyExample
//
//  Created by Jason Levine on 2013-05-29.
//
//




#include "Track.h"
#include "ofxXmlSettings.h"

Track::Track(DNA _dna, ofVec2f _pos) {
    dna = _dna;
    pos = _pos;
    cout << "pos " << pos << endl;
    fitness = 1;
    width = 960;
    height = 60;
    playing = false;
}

void Track::expressGenes() {
    midiData.assign(16, NULL);
    
    for (int i = 0; i < 16; i++) {
        int beat = int(ofMap(dna.genes[i], 0.0, 1.0, 0, 15));
        trackEvent temp;
        
        temp.prog = int(dna.genes[i+16] * 3 - 0.01);
        temp.midiMessage = noteOn;
        temp.note = int(ofMap(dna.genes[i+32], 0.0, 1.0, 36, 89));
        temp.vel = int(ofMap(dna.genes[i+48], 0.0, 1.0, 0, 127));
        midiData[beat].push_back(temp);
        
        temp.midiMessage = noteOff;
        midiData[beat+1].push_back(temp);
    }
    
    tempo = ofMap(dna.genes[64], 0.0, 1.0, 60, 160);
    
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
    
    tempo = 150; //ofMap(dna.genes[64], 0.0, 1.0, 60, 160);
    remixX = dna.genes[65];
    remixY = dna.genes[66];
    prog = ofMap(dna.genes[67], 0.0, 1.0, 0, 2);
    
    
    
       

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
    cout << "--------------------------------------------" << endl;
    cout << endl << "UNsorted MIDI events" << endl;
    int actiontime = 0;      
    for (int i = 0; i < 16; i++) {
        int beat = int(ofMap(dna.genes[i], 0.0, 1.0, 0, 15));
        actiontime = tpq / 4 * beat;
        midievent[0] = 0x9A;     // store a note on command (MIDI channel 1)
        midievent[1] = ofMap(dna.genes[i+16], 0.0, 1.0, 36, 89); //c1 to f5
        midievent[2] = 127; //ofMap(dna.genes[i+32], 0.0, 1.0, 0, 127);       // store attack/release velocity for note command
        outputFile->addEvent(1, actiontime, midievent);
        cout <<  actiontime << " NoteOn" << int(midievent[1]) << " " << int(midievent[2]) << endl;
        
        actiontime+= tpq / 2;
        midievent[0] = 0x8A;     // store a note off command (MIDI channel 1)
        outputFile->addEvent(1, actiontime, midievent);
        cout <<  actiontime << " NoteOff" << int(midievent[1]) << " " << int(midievent[2]) << endl;
    }
    
    cout << endl << "Sorted Midi tracks" << endl;
    outputFile->sortTracks();         // make sure data is in correct order
    cout << endl;
    for (int i = 0;  i < outputFile->getNumEvents(1); i++) {
        cout << outputFile->getEvent(1, i).time << " ";
        if (outputFile->getEvent(1, i).isNoteOn()) cout << "noteON ";
        if (outputFile->getEvent(1, i).isNoteOff()) cout << "noteOFF ";
        
        cout  << int(outputFile->getEvent(1, i).data[1]) << " " << int(outputFile->getEvent(1, i).data[2]) << endl;
    }
    


    
    
}

void Track::exportMidiFile(string name) {
    string filename = ofToDataPath(name + ".mid");
    outputFile->write(filename.c_str());
}

void Track::draw(int event) {
    
//    ofSetRectMode(OF_RECTMODE_CENTER);
   

    //ofRect(pos.x, pos.y, width, height);
    // Display fitness value
    
    ofEnableAlphaBlending();
    int currEvent = 0;
    for (int i = 0; i < 16; i++) {
        if (midiData[i].size() > 0) {
            int numNoteOns = 0;
            for (int j = 0; j < midiData[i].size(); j++) {
                if (midiData[i][j].midiMessage == noteOn) numNoteOns++;
            }
            if (numNoteOns > 0) {
                int noteIndex = 0;
                for (int j = 0; j < midiData[i].size(); j++) {
                    if (midiData[i][j].midiMessage == noteOn) {
                        int prog = midiData[i][j].prog;
                        int note = midiData[i][j].note;
                        int vel = midiData[i][j].vel;
                        
                        ofColor boxCol(0, 0, 0, vel * 2);
                        switch (prog) {
                            case 0:
                                boxCol.r = note * 2;
                                break;
                                
                            case 1:
                                boxCol.g = note * 2;
                                break;
                                
                            case 2:
                                boxCol.b = note * 2;
                                break;
                                
                        }
                        ofSetColor(boxCol);
                        float w = 50.0;
                        float x = pos.x + i * w;
                        float h = w / numNoteOns;
                        float y = pos.y + noteIndex * h;
                        
                        
                        ofRect(x, y, w, h);
                        ofSetColor(0);
                        ofDrawBitmapString(ofToString(currEvent), x + w / 2, y + h /2);
                        currEvent++;
                        noteIndex++;
                    }
                }
            }
        }
    }
    
    if (rolloverOn) {
        ofSetColor(0);
        ofNoFill();
        ofRect(pos.x, pos.y, 50 * 16, 50);
        ofRect(pos.x + event * 50, pos.y, 50, 50);
        ofFill();
        
        
    }

    
    ofDisableAlphaBlending();
    ofSetColor(0, 0, 0);

    ofDrawBitmapString(ofToString(fitness, 2), pos.x + 50 * 16 + 10, pos.y+height/2);
}

void Track::rollover(int mouseX, int mouseY) {
    if (mouseX > pos.x && mouseX < pos.x + width && mouseY > pos.y && mouseY < pos.y + height) {
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

vector<float> Track::getProgs(){
    vector<float> progs;
    for (int i = 0;  i < 16; i++) {
        progs.push_back(dna.genes[i+48]);
    }
    return progs;
}

void Track::saveTrack(string name) {
    ofxXmlSettings trackXML;
    trackXML.addTag("tempo");
    trackXML.setValue("tempo", tempo);
    
    trackXML.addTag("midiData");
    trackXML.pushTag("midiData");
    
    for (int i = 0; i < midiData.size(); i++) {
        
        string beatTag = "beat" + ofToString(i);
        trackXML.addTag(beatTag);
        trackXML.pushTag(beatTag);
        
        for (int j = 0; j < midiData[i].size(); j++) {
            string eventTag = "event" + ofToString(j);
            trackXML.addTag(eventTag);
            trackXML.pushTag(eventTag);
            trackXML.setValue(eventTag + ":mess", midiData[i][j].midiMessage);
            trackXML.setValue(eventTag + ":prog", midiData[i][j].prog);
            trackXML.setValue(eventTag + ":note", midiData[i][j].note);
            trackXML.setValue(eventTag + ":vel", midiData[i][j].vel);
            trackXML.popTag();
        }
        trackXML.popTag();
    }
    trackXML.popTag();
    
    trackXML.saveFile(name + ".wcb");
}
