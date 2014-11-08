#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <bitset>

#include"Event.hpp"
#include"Track.hpp"

using namespace std;

int main(){

    int nEvents, nTracks, nPoints;
    short int  eventRaw;
    Track thisTrack;
    std::vector<Event> track;

    nPoints = 8;

    //Set up our input stream
    ifstream testBinary("onetrack.raw", ios::binary | ios::in | ios::ate);
    nEvents = testBinary.tellg()/sizeof(eventRaw);
    nTracks = nEvents/8;
    //Look for correct event
    for (int n=0; n < nEvents; n++){
        testBinary.seekg(2*n,ios::beg);
        testBinary.read((char*)&eventRaw,sizeof(eventRaw));
        //Extract Events
        thisTrack.addEvent(eventRaw);
    }
    //Close the file
    testBinary.close();
    
    thisTrack.print();
    thisTrack.fitTrackBad();
    cout << "Line 3 is: " << thisTrack.getIntercept() << " + " << thisTrack.getSlope() << "*x " << endl;
    thisTrack.fitTrack();
    cout << "Line 3 is: " << thisTrack.getIntercept() << " + " << thisTrack.getSlope() << "*x " << endl;

}
//shift y
//cout << "Event " << n << ": " << (bitset<16>)eventRaw << endl;
        /*cout << "Event " << n << ": " << (bitset<16>)eventRaw << endl;
        cout << "x: " << x[n] << ", " << (bitset<16>)x[n] << endl;
        cout << "y: " << y[n] << ", " << (bitset<16>)y[n] << endl;
        cout << "t: " << t[n] << ", " << (bitset<16>)t[n] << endl;*/
