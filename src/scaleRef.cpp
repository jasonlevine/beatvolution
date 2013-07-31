//
//  scaleRef.cpp
//  beatVolution
//
//  Created by Jason Levine on 2013-07-30.
//
//

#include "scaleRef.h"

scaleRef::scaleRef() {
    int ionianDegrees[] = { 0,2,4,5,7,9,11 };
    ionian.assign(ionianDegrees, ionianDegrees + 6);
    scales.push_back(ionian);
    
    int dorianDegrees[] = { 0,2,3,5,7,9,11 };
    dorian.assign(dorianDegrees, dorianDegrees + 6);
    scales.push_back(dorian);
    
    int phrygianDegrees[] = { 0,1,3,5,7,8,10 };
    phrygian.assign(phrygianDegrees, phrygianDegrees + 6);
    scales.push_back(phrygian);
    
    int lydianDegrees[] = { 0,2,4,6,7,9,11 };
    lydian.assign(lydianDegrees, lydianDegrees + 6);
    scales.push_back(lydian);
    
    int mixolydianDegrees[] = { 0,2,4,5,7,9,10 };
    mixolydian.assign(mixolydianDegrees, mixolydianDegrees + 6);
    scales.push_back(mixolydian);
    
    int aeolianDegrees[] = { 0,2,3,5,7,9,10 };
    aeolian.assign(aeolianDegrees, aeolianDegrees + 6);
    scales.push_back(aeolian);
    
    int locrianDegrees[] = { 0,1,3,5,6,8,10 };
    locrian.assign(locrianDegrees, locrianDegrees + 6);
    scales.push_back(locrian);
    
    int harmonicMinorDegrees[] = { 0,2,3,5,7,8,11 };
    harmonicMinor.assign(harmonicMinorDegrees, harmonicMinorDegrees + 6);
    scales.push_back(harmonicMinor);
}

int scaleRef::getNote(int scale, int degree, int octave) {
    octave+=degree/7;
    return scales[scale][degree] + octave * 12;
}