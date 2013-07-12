//
//  DNA.cpp
//  emptyExample
//
//  Created by Jason Levine on 2013-05-28.
//
//

#include "DNA.h"

DNA::DNA() {
    num = 70;
    for (int i = 0; i < num; i++) {
        genes.push_back(ofRandomuf());
    }
}

void DNA::expressGenes() {
    
}

void DNA::testFitness() {
    float score = 0;
    
    
}

DNA DNA::crossover(DNA partner){
    DNA child;
    int midpoint = ofRandom(num);
    
    for (int i = 0; i < num; i++) {
        if (i > midpoint) child.genes[i] = genes[i];
        else child.genes[i] = partner.genes[i];
    }
    
    return child;
}

void DNA::mutate(float mutationRate){
    for (int i = 0; i < num; i++) {
        if (ofRandomuf() < mutationRate) {
            genes[i] = ofRandomuf();
        }
    }
}