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
    
    currentTrack = -1;
    
    tempo = 120.0;
    
        //Audio
    sampler = ofxAudioUnitSampler('aumu', 'CaC2', 'CamA');
//    sampler.setChannel(10);
    //sampler.setBank(2, 0);
    prog = 0;
    sampler.setProgram(prog);
//    sampler.getParameters(true, true);
    
    //cout << "preset status: " + ofToString(sampler.loadCustomPreset(ofToDataPath("dubstep_drum_kit"))) << endl;
    sampler.connectTo(output);
    output.start();
}

//--------------------------------------------------------------
void testApp::update(){
    float thisFrameTrack = population->rollover(mouseX, mouseY);
    if (thisFrameTrack != currentTrack) {
        currentTrack = thisFrameTrack;
        if (currentTrack != -1) {
            currentMidiFile = population->getMidifile(currentTrack);
            timer = new ofxTimer;
            timer->setAlarm(0);
            lastNoteTime = 0.0;
            eventCounter = 0;
            timeCounter = 0.0;
            tempo = population->getTempo(currentTrack);
            ofVec2f remix = population->getRemix(currentTrack);
      
//            int prog = ofMap(remix.y, 0.0, 1.0, 0, 11);
            sampler.setProgram(population->getProg(currentTrack));
            
            sampler.setParameter(16, kAudioUnitScope_Global, remix.x);
            sampler.setParameter(17, kAudioUnitScope_Global, remix.y);
            
//            int command = 0;
//            // check for tempo
//            for (int event = 0; event<currentMidiFile->getNumEvents(0); event++) {
//
//                command = currentMidiFile->getEvent(0, event).data[0] & 0xf0;
//
//                if (currentMidiFile->getEvent(0, event).data[0] == 0xff &&
//                    currentMidiFile->getEvent(0, event).data[1] == 0x51) {
//                    setTempo(*currentMidiFile, event, tempo);
//                }
//            }
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    population->draw();
            
    ofDrawBitmapString("Generation #" + ofToString(population->getGenerations()), 15, 190);
    
    ofDrawBitmapString("current track #" + ofToString(currentTrack) + " tempo = " + ofToString(tempo), 15, 10);
}

void testApp::audioOut(float *input, int bufferSize, int nChannels) {
    //play midi
    if (currentTrack != -1) {
        if (timer->alarm()) {
            if (currentMidiFile->getEvent(1, eventCounter).isNoteOn()) {
                sampler.midiNoteOn(currentMidiFile->getEvent(1, eventCounter).data[1], currentMidiFile->getEvent(1, eventCounter).data[2]);
                lastNoteTime = currentMidiFile->getEvent(1, eventCounter).time;
            }
            
            if (currentMidiFile->getEvent(1, eventCounter).isNoteOff()) {
                sampler.midiNoteOff(currentMidiFile->getEvent(1, eventCounter).data[1], currentMidiFile->getEvent(1, eventCounter).data[2]);
                lastNoteTime = currentMidiFile->getEvent(1, eventCounter).time;
            }
            
            
            float alarmMS;
            eventCounter++;
            if (eventCounter < currentMidiFile->getNumEvents(1)) {
                
                if (eventCounter == 0) {
                    alarmMS = applyTempo(currentMidiFile->getEvent(1, eventCounter).time, currentMidiFile);
//                    alarmMS-= (loopBeginErr + loopEndErr);
                }
                else {
                    alarmMS = applyTempo(currentMidiFile->getEvent(1, eventCounter).time - lastNoteTime, currentMidiFile);
                    alarmMS-= timer->getDiffA();
                }
                if (alarmMS > 0) {
                    timer->setAlarm(alarmMS);
                    timeCounter+=alarmMS;
                }
                //cout << "track " << track << " event " << eventCounter[track] << " time " << alarmMS << endl;
                
            }
            else {
                eventCounter = 0;
                alarmMS = applyTempo(960, currentMidiFile) - timeCounter;
                timer->setAlarm(alarmMS);
                timeCounter = 0.0;
                //song done start over
            }
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
        sampler.showUI();
    }
    
    if (key == OF_KEY_LEFT) {
        prog--;
        if (prog < 0) prog = 0;

        sampler.setProgram(prog);
    }
    
    if (key == OF_KEY_RIGHT) {
        prog++;
        if (prog > 20) prog = 20;

        sampler.setProgram(prog);
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