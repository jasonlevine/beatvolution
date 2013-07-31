//
//  scaleRef.h
//  beatVolution
//
//  Created by Jason Levine on 2013-07-30.
//
//
#include "ofMain.h"

class scaleRef {
public:
    scaleRef();
    int getNote(int scale, int degree, int octave);
    
private:
    vector<int> ionian, dorian, phrygian, lydian, mixolydian, aeolian, locrian, harmonicMinor;
    vector<vector<int> > scales;
};
