//
//  population.h
//  emptyExample
//
//  Created by Jason Levine on 2013-05-28.
//
//

#include "ofMain.h"
#include "Track.h"

class Population {
public:
    
    Population(float m, int num);
    void draw();
    int rollover(int mouseX, int mouseY);
    void calcFitness();
    void selection();
    void reproduction();
    int getGenerations();
    float getMaxFitness();
    MidiFile * getMidifile(int track) { return population[track].getMidifile(); }
    float getTempo(int track) { return population[track].getTempo(); }
    ofVec2f getRemix(int track) { return population[track].getRemix(); }
    int getProg(int track) { return population[track].getProg(); }
    vector<float> getProgs( int track) { return population[track].getProgs(); }
    vector<float> target;
    
    
    
private:
    
    float mutationRate;           // Mutation rate
    vector<Track> population;            // array to hold the current population
    vector<Track> matingPool;   // ArrayList which we will use for our "mating pool"
    int generations;              // Number of generations
    
};
