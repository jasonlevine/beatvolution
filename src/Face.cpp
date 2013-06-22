//
//  Face.cpp
//  emptyExample
//
//  Created by Jason Levine on 2013-05-29.
//
//

#include "Face.h"

Face::Face(DNA _dna, ofVec2f _pos) {
    dna = _dna;
    pos = _pos;
    fitness = 1;
    size = 70;
}

void Face::draw() {
    // We are using the face's DNA to pick properties for this face
    // such as: head size, color, eye position, etc.
    // Now, since every gene is a floating point between 0 and 1, we map the values
    float r          = ofMap(dna.genes[0],0,1,0,35);
    ofFloatColor c   = ofFloatColor(dna.genes[1],dna.genes[2],dna.genes[3]);
    float eye_y      = ofMap(dna.genes[4],0,1,0,5);
    float eye_x      = ofMap(dna.genes[5],0,1,0,10);
    float eye_size   = ofMap(dna.genes[5],0,1,0,10);
    ofFloatColor eyecolor   = ofFloatColor(dna.genes[4],dna.genes[5],dna.genes[6]);
    ofFloatColor mouthColor = ofFloatColor(dna.genes[7],dna.genes[8],dna.genes[9]);
    float mouth_y    = ofMap(dna.genes[5],0,1,0,25);
    float mouth_x    = ofMap(dna.genes[5],0,1,-25,25);
    float mouthw     = ofMap(dna.genes[5],0,1,0,50);
    float mouthh     = ofMap(dna.genes[5],0,1,0,10);
    
    // Once we calculate all the above properties, we use those variables to draw rects, ellipses, etc.
    ofPushMatrix();
    ofTranslate(pos);
    ofFill();
    // Draw the head
    ofSetColor(c);
    ofCircle(0, 0, r);
    ofSetRectMode(OF_RECTMODE_CENTER);
    
    // Draw the eyes
    ofSetColor(eyecolor);
    
    ofRect(-eye_x, -eye_y, eye_size, eye_size);
    ofRect( eye_x, -eye_y, eye_size, eye_size);
    
    // Draw the mouth
    ofSetColor(mouthColor);
    ofRect(mouth_x, mouth_y, mouthw, mouthh);
    
    // Draw the bounding box
    ofSetLineWidth(0.25);
    ofNoFill();
    if (rolloverOn) ofSetColor(0);
    else {
        ofSetColor(64);
        ofRect(0, 0, size, size);
    }
    ofPopMatrix();
    
    // Display fitness value
    if (rolloverOn) ofSetColor(0);
    else ofSetColor(64);
    ofDrawBitmapString(ofToString(fitness, 2), pos.x, pos.y + 55);
}

void Face::rollover(int mouseX, int mouseY) {
    if (mouseX > pos.x - size / 2 && mouseX < pos.x + size / 2 && mouseY > pos.y - size / 2 && mouseY < pos.y + size / 2) {
        rolloverOn = true;
        fitness += 0.25;
    } else {
        rolloverOn = false;
    }
}

DNA Face::getDNA() {
    return dna;
}

float Face::getFitness() {
    return fitness;
}
