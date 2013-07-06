//
//  population.h
//  emptyExample
//
//  Created by Jason Levine on 2013-05-28.
//
//

#include "ofMain.h"
#include "Track.h"
#include "ofxXmlSettings.h"


class Population {
public:
    
    Population(float m, int num);
    //GUI
    void draw();
    int rollover(int mouseX, int mouseY);
    //GA
    void calcFitness();
    void selection();
    void reproduction();
    //Getters
    int getGenerations();
    float getMaxFitness();
    MidiFile * getMidifile(int track) { return population[track].getMidifile(); }
    float getTempo(int track) { return population[track].getTempo(); }
    ofVec2f getRemix(int track) { return population[track].getRemix(); }
    int getProg(int track) { return population[track].getProg(); }
    vector<float> getProgs( int track) { return population[track].getProgs(); }
    // Savers
    void exportTrack(int track, string name);
    void saveSession();
    void loadSession();
        
private:
    ofxXmlSettings session;
    
    float mutationRate;           // Mutation rate
    vector<Track> population;            // array to hold the current population
    vector<Track> matingPool;   // ArrayList which we will use for our "mating pool"
    int generations;              // Number of generations
    
};
