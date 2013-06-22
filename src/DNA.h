//
//  DNA.h
//  emptyExample
//
//  Created by Jason Levine on 2013-05-28.
//
//

#include "ofMain.h"

class DNA {
public:
    DNA();
    void expressGenes();
    void testFitness();
    DNA crossover(DNA partner);
    void mutate(float mutationRate);
    
    
    vector<float> genes;
    int num;
    float fitness;
    
};


