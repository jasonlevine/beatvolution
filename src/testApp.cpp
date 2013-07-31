#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    int popmax = 10;
    float mutationRate = 0.05;
    population = new Population(mutationRate,popmax);
    population->selection();
    population->reproduction();
    
    int bufferSize		= 512;
	int sampleRate 			= 44100;
    soundStream.setup(this, 2, 0, sampleRate, bufferSize, 4);
    

    numPresets = aupresets.listDir("aupresets");
    
    
    currentTrack = -1;
    barCounter = 0;
    
    tempo = 120.0;
    samplerSetupComplete = false;
    //Audio
    ofxAudioUnitSampler AlchemyPlayer('aumu', 'CaC2', 'CamA');
    
    sampler.assign(3, AlchemyPlayer);
    sampler[0].loadCustomPreset("distressed_kit.aupreset");
    sampler[1].loadCustomPreset("dubstep_bio_kit.aupreset");
    sampler[2].loadCustomPreset("dubstep_drum_kit.aupreset");
    cout << "sampler0 " << sampler[0].midiChannelInUse << endl
         << "sampler1 " << sampler[1].midiChannelInUse << endl
        << "sampler2 " << sampler[2].midiChannelInUse << endl;
    
    chords = AlchemyPlayer;
    chords.setBank(0, 0);
    chords.setProgram(0);
//    chords.printParameterList(true, true);
    
    mixer.setInputBusCount(4);
    sampler[0].connectTo(mixer, 0);
    sampler[1].connectTo(mixer, 1);
    sampler[2].connectTo(mixer, 2);
    chords.connectTo(mixer, 3);
    mixer.setInputVolume(0.7, 3);
    mixer.connectTo(output);
    output.start();
    
}

//--------------------------------------------------------------
void testApp::update(){
    float thisFrameTrack = population->rollover(mouseX, mouseY);
    if (thisFrameTrack != currentTrack) {
        currentTrack = thisFrameTrack;
        if (currentTrack != -1) {
            //samplerSetupComplete = false;
            eventCounter = 0;
//            noteOnCounter = 0;
            tempo = population->getTempo(currentTrack);
            
            sixteenthNoteMs = 125; //ofMap(tempo, 60, 160, 250, 94);
            currentTrackMidiData = population->getMidiData(currentTrack);
            vector<ofVec2f> remix = population->getRemix(currentTrack);
            for (int i = 0; i < sampler.size(); i++) {
                sampler[i].setParameter(10, kAudioUnitScope_Global, remix[i].x);
                sampler[i].setParameter(11, kAudioUnitScope_Global, remix[i].y);
            }
            //chords.setParameter(kAudioUnitParameterUnit_BPM, kAudioUnitScope_Global, tempo);
            int presetIndex = int(ofMap(population->getProg(currentTrack), 0.0, 1.0, 0, numPresets));
            chords.loadCustomPreset("aupresets/" + aupresets.getName(presetIndex));
            timer = new ofxTimer;
            timer->setAlarm(0);
            samplerSetupComplete = true;
        }
        else {
            samplerSetupComplete = false;
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 3; j++) {
                    int note = currentTrackMidiData[16+i][j].note;
                    chords.midiNoteOff(note, 0);
                }
            }
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    population->draw(eventCounter);
    
    ofDrawBitmapString("Generation #" + ofToString(population->getGenerations()) + " Current Track #" + ofToString(currentTrack) + " Tempo = " + ofToString(tempo) + " eventCounter " + ofToString(eventCounter), 15, 10);
}

//--------------------------------------------------------------
void testApp::audioOut(float *input, int bufferSize, int nChannels) {
    //play midi
    
    if (currentTrack != -1 && samplerSetupComplete) {

        if (timer->alarm()) {
            if (eventCounter == 0) {
                for (int i = 0; i < 3; i++) {
                    int note = currentTrackMidiData[16 + barCounter][i].note;
                    int lastBar = barCounter - 1;
                    if (lastBar == -1) lastBar = 3;
                    int lastNote = currentTrackMidiData[16 + lastBar][i].note;
                    chords.midiNoteOff(note, 0);
                    chords.midiNoteOn(note, 50);
                }
                barCounter++;
                barCounter%=4;
            }
            
            if (currentTrackMidiData[eventCounter].size() > 0) {
                for (int i = 0; i < currentTrackMidiData[eventCounter].size(); i++) {
                    int message = currentTrackMidiData[eventCounter][i].midiMessage;
                    int prog = currentTrackMidiData[eventCounter][i].prog;
                    int note = currentTrackMidiData[eventCounter][i].note;
                    int vel = currentTrackMidiData[eventCounter][i].vel;
                    
                    if (message == noteOn) {
                        sampler[prog].midiNoteOn(note, vel);
//                        noteOnCounter++;
                    }
                    else if (message == noteOff) sampler[prog].midiNoteOff(note, vel);
                }
            }
            eventCounter++;
            eventCounter%=16;
            timer->setAlarm(sixteenthNoteMs);
        }
    }
}




//--------------------------------------------------------------
void testApp::setTempo(MidiFile& midifile, int index, double& tempo) {
    double newtempo = 0.0;
    static int count = 0;
    count++;
    
    MFEvent& mididata = midifile.getEvent(0, index);
    
    int microseconds = 0;
    microseconds = microseconds | (mididata.data[3] << 16);
    microseconds = microseconds | (mididata.data[4] << 8);
    microseconds = microseconds | (mididata.data[5] << 0);
    
    newtempo = 60.0 / microseconds * 1000000.0;
    if (count <= 1) {
        tempo = newtempo;
    } else if (tempo != newtempo) {
        cout << "; WARNING: change of tempo from " << tempo
        << " to " << newtempo << " ignored" << endl;
    }
}



//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 32) {
        population->selection();
        population->reproduction();
    }
    
    if (key == 's') {
        chords.showUI();
    }
    
    
    if (key == 'S') {
        population->saveSession();
    }
    
    if (key == 'L') {
        population->loadSession();
    }

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    if (currentTrack != -1) {
        if (currentTrack != -1) {
            population->saveTrack(currentTrack, "session1");
        }
    }
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}