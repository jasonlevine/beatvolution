//
//  population.cpp
//  emptyExample
//
//  Created by Jason Levine on 2013-05-28.
//
//

#include "Population.h"

   
// Create the population
Population::Population(float m, int num) {
    ofSeedRandom();
    mutationRate = m;
    generations = 0;
    for (int i = 0; i < num; i++) {
        population.push_back(Track(DNA(), ofVec2f(50, 50+i*55)));
    }
}

// create all Tracks
void Population::draw(int event) {
    for (int i = 0; i < population.size(); i++) {
        population[i].draw(event);
    }
}

// Are we rolling over any of the Tracks?
int Population::rollover(int mouseX, int mouseY) {
    for (int i = 0; i < population.size(); i++) {
        population[i].rollover(mouseX, mouseY);
        if (population[i].isPlaying()) {
            return i;
        }
    }
    return -1;
}

void Population::exportTrack(int track, string name) {
    population[track].exportMidiFile(name);
}


// Generate a mating pool
void Population::selection() {
    // Clear the ArrayList
    matingPool.clear();
    
    // Calculate total fitness of whole population
    float maxFitness = getMaxFitness();
    
    // Calculate fitness for each member of the population (scaled to value between 0 and 1)
    // Based on fitness, each member will get added to the mating pool a certain number of times
    // A higher fitness = more entries to mating pool = more likely to be picked as a parent
    // A lower fitness = fewer entries to mating pool = less likely to be picked as a parent
    for (int i = 0; i < population.size(); i++) {
        float fitnessNormal = ofMap(population[i].getFitness(), 0, maxFitness, 0, 1);
        int n = (int) (fitnessNormal * 100);  // Arbitrary multiplier
        for (int j = 0; j < n; j++) {
            matingPool.push_back(population[i]);
        }
    }
}

// Making the next generation
void Population::reproduction() {
    // Refill the population with children from the mating pool
    for (int i = 0; i < population.size(); i++) {
        // Sping the wheel of fortune to pick two parents
        int m = int(ofRandom(matingPool.size()));
        int d = int(ofRandom(matingPool.size()));
        // Pick two parents
        Track mom = matingPool[m];
        Track dad = matingPool[d];
        // Get their genes
        DNA momgenes = mom.getDNA();
        DNA dadgenes = dad.getDNA();
        // Mate their genes
        DNA child = momgenes.crossover(dadgenes);
        // Mutate their genes
        child.mutate(mutationRate);
        // Fill the new population with the new child
        population[i] = Track(child, ofVec2f(50, 50+i*65));
        population[i].createMidiFile();
    }
    generations++;
}

int Population::getGenerations() {
    return generations;
}

// Find highest fintess for the population
float Population::getMaxFitness() {
    float record = 0;
    for (int i = 0; i < population.size(); i++) {
        if (population[i].getFitness() > record) {
            record = population[i].getFitness();
        }
    }
    return record;
}



//saving
//--------------------------------------------------------------
void Population::saveSession() {
    session.clear();
    
    session.addTag("mutationRate");
    session.setValue("mutationRate", mutationRate);
    
    session.addTag("generations");
    session.setValue("generations", generations);
    
    int numGenes = population[0].dna.num;
    session.addTag("numGenes");
    session.setValue("numGenes", numGenes);
    
    session.addTag("Tracks");
    session.pushTag("Tracks");
    for (int i = 0; i < population.size(); i++){
        
        session.addTag("Track" + ofToString(i));
        session.pushTag("Track" + ofToString(i));
        session.addTag("fitness");
        session.setValue("fitness", population[i].getFitness());
        for (int j = 0; j < numGenes; j++) {
            session.addTag("gene" + ofToString(j));
            session.setValue("gene" + ofToString(j), population[i].dna.genes[j]);
        }
        session.popTag();
    }
    session.popTag();
    
    session.saveFile("session.xml");
    cout << "session saved" << endl;
}

//--------------------------------------------------------------
void Population::loadSession() {
    session.loadFile("session.xml");
    mutationRate = session.getValue("mutationRate", 0.05);
    generations = session.getValue("generations", 1);
    int numGenes = session.getValue("numGenes", 68);
    
    session.pushTag("Tracks");
    for (int i = 0; i < population.size(); i++) {
        session.pushTag("Track" + ofToString(i));
        population[i].setFitness(session.getValue("fitness", 1.0));
        for (int j = 0; j < numGenes; j++) {
            population[i].dna.genes[j] = session.getValue("gene" + ofToString(j), 0.0);
        }
        population[i].createMidiFile();
        session.popTag();
    }
    session.popTag();
    
    cout << "session loaded" << endl;
    
}