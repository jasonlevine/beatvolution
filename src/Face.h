//
//  Face.h
//  emptyExample
//
//  Created by Jason Levine on 2013-05-29.
//
//
#include "DNA.h"

class Face {
public:
    Face(DNA _dna, ofVec2f _pos);
    void draw();
    float getFitness();
    DNA getDNA();
    void rollover(int mouseX, int mouseY);
    
private:
    DNA dna;
    float fitness;
    ofVec2f pos;
    int size;
    bool rolloverOn;
    
};
