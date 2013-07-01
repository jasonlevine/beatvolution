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
    ofxAudioUnitSampler AlchemyPlayer('aumu', 'CaC2', 'CamA');
    
    sampler.assign(3, AlchemyPlayer);
    sampler[0].loadCustomPreset("distressed_kit");
    sampler[1].loadCustomPreset("dubstep_bio_kit");
    sampler[2].loadCustomPreset("dubstep_drum_kit");
    cout << "sampler0 " << sampler[0].midiChannelInUse << endl
         << "sampler1 " << sampler[1].midiChannelInUse << endl
        << "sampler2 " << sampler[2].midiChannelInUse << endl;
    
    mixer.setInputBusCount(3);
    sampler[0].connectTo(mixer, 0);
    sampler[1].connectTo(mixer, 1);
    sampler[2].connectTo(mixer, 2);
    mixer.connectTo(output);
    output.start();
}

//--------------------------------------------------------------
void testApp::update(){
    float thisFrameTrack = population->rollover(mouseX, mouseY);
    if (thisFrameTrack != currentTrack) {
        currentTrack = thisFrameTrack;
        if (currentTrack != -1) {
            currentMidiFile = population->getMidifile(currentTrack);
            int currentTime = currentMidiFile->getEvent(1, 0).time;
            float alarmMS = applyTempo(currentTime, currentMidiFile);
            timer = new ofxTimer;
            timer->setAlarm(alarmMS);
            lastNoteTime = 0;
            eventCounter = 0;
            noteOnCounter = 0;
            timeCounter = 0.0;
            tempo = population->getTempo(currentTrack);
            ofVec2f remix = population->getRemix(currentTrack);
            progs.clear();
            progs = population->getProgs(currentTrack);
      
//            int prog = ofMap(remix.y, 0.0, 1.0, 0, 11);
//            sampler.setProgram(population->getProg(currentTrack));
            
//            sampler.setParameter(16, kAudioUnitScope_Global, remix.x);
//            sampler.setParameter(17, kAudioUnitScope_Global, remix.y);
            
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

            
            int note = currentMidiFile->getEvent(1, eventCounter).data[1];
            int vel = currentMidiFile->getEvent(1, eventCounter).data[2];
            
            cout << "event " << eventCounter;
            
            if (currentMidiFile->getEvent(1, eventCounter).isNoteOn()) {
                //int prog = ofMap(progs[noteOnCounter], 0.0, 1.0, 0, 2);
                int prog = int(progs[noteOnCounter] * 3 - 0.01);
                sampler[prog].midiNoteOn(note, vel);
                notePlayed temp;
                temp.note = note;
                temp.prog = prog;
                notesPlayed.push_back(temp);
                noteOnCounter++;
                cout << " noteON " << "prog " << prog;
            }
            
            if (currentMidiFile->getEvent(1, eventCounter).isNoteOff()) {
                int prog;
                for (int i = 0; i < notesPlayed.size(); i++) {
                    if (notesPlayed[i].note == note) {
                        prog = notesPlayed[i].prog;
                        notesPlayed.erase(notesPlayed.begin() + i);
                        break;
                    }
                }
                sampler[prog].midiNoteOff(note, vel);
                cout << " noteOff " << "prog " << prog;
            }
            cout << " note " << note << "vel " << vel << endl;
            

            float alarmMS;
            eventCounter++;
            if (eventCounter < currentMidiFile->getNumEvents(1)) {
                int currentTime = currentMidiFile->getEvent(1, eventCounter).time;
                cout << "currentime " << currentTime << endl;
                cout << "lastTime " << lastNoteTime << endl;
                if (eventCounter == 0) {
                    alarmMS = applyTempo(currentTime, currentMidiFile);
                }
                else {
                    alarmMS = applyTempo(currentTime - lastNoteTime, currentMidiFile);
                    cout << "alarmMS pre " << alarmMS << endl;
                    alarmMS-= timer->getDiffA();
                    cout << "alarmMS post " << alarmMS << endl << endl;
                }
                if (alarmMS > 0) {
                    timer->setAlarm(alarmMS);
                    timeCounter+=alarmMS;
                }
                
                //cout << "track " << track << " event " << eventCounter[track] << " time " << alarmMS << endl;
                lastNoteTime = currentTime;
            }
            else {
                eventCounter = 0;
                noteOnCounter = 0;
                
                int endOfLoop = 960 - lastNoteTime;
                alarmMS = applyTempo(endOfLoop + currentMidiFile->getEvent(1, 0).time, currentMidiFile);
                cout << "elapsed time " << timeCounter << " last alarm : " << alarmMS << endl;
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
        sampler[0].showUI();
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