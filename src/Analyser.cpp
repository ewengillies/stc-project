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
    std::vector<int> x,y,t;
    double sumX, sumY, sumT, sumX2, sumT2, sumXT, sumYT, sumXY;
    double meanX, meanY, meanT;
    double weightY;
    double slope1, slope2, slope3;
    double velocity;
    double interY1, interY2, interY3;
    double sumYminusT, sumXYminusT;
    double varyYT, varyXT, varyXX, varyXY, varyTT;

    nPoints = 8;

    //Set up our input stream
    ifstream testBinary("onetrack.raw", ios::binary | ios::in | ios::ate);
    nEvents = testBinary.tellg()/sizeof(eventRaw);
    nTracks = nEvents/8;
    cout << nEvents << endl;
    cout << nTracks << endl;

    //Look for correct event
    for (int n=0; n < nEvents; n++){
        testBinary.seekg(2*n,ios::beg);
        testBinary.read((char*)&eventRaw,sizeof(eventRaw));

        //Extract time and stuff
        thisTrack.addEvent(eventRaw);
        x.push_back(eventRaw & ~(~0 << (2-0+1)));
        y.push_back((eventRaw >> 3) & ~(~0 << (5-3+1)));
        t.push_back((eventRaw >> 6) & ~(~0 << (15-6+1)));
        std::cout << "Event here: (" << x[n] <<", " << y[n] <<", " << t[n] <<")" << std::endl;
        //Read out our bits and values
        //Line of best fit
        sumY += y[n];
        sumT += t[n];
        sumT2 += t[n]*t[n];
        sumXT += x[n]*t[n];
        sumYT += y[n]*t[n]; 
        sumXY += x[n]*y[n];
        sumX2 += x[n]*x[n];
        sumX += x[n];
    }
    
    //sumX = 28;
    //meanX = 3.5;
    //sumX2 = 140;
    meanY = sumY/nPoints;
    meanX = sumX/nPoints;
    meanT = sumT/nPoints;
    slope1 = (sumXY - sumX*meanY) / (sumX2 - sumX*meanX);
    
    std::cout << "MeanT here: " << meanT << std::endl;
    std::cout << "MeanY here: " << meanY << std::endl;
    std::cout << "MeanX here: " << meanX << std::endl;
    
    interY2 = meanY;

    varyYT = sumYT - meanY*sumT;  
    std::cout << "VaryYT here: " << varyYT << std::endl;
    varyXT = sumXT - meanX*sumT;  
    std::cout << "VaryXT here: " << varyXT << std::endl;
    varyXX = sumX2 - meanX*sumX;  
    std::cout << "VaryXX here: " << varyXX << std::endl;
    varyXY = sumXY - meanX*sumY;  
    std::cout << "VarXY here: " << varyXY << std::endl;
    varyTT = sumT2 - meanT*sumT;  
    std::cout << "VaryTT here: " << varyTT << std::endl;
/*
    varyYT = sumYT - meanY*sumT;  
    varyXT = sumXT - meanX*sumT;  
    varyXX = sumX2 - meanX*sumX;  
    varyXY = sumXY - meanX*sumY;  
    varyTT = sumT2 - meanT*sumT;  
*/
    velocity = ( varyYT + varyXY*varyXT/varyXX ) / (varyTT + varyXT*varyXT/varyXX); 
    std::cout << "Velocity : " << velocity << std::endl;
    interY2 -= meanT*velocity;
    
    for ( int n = 0; n < nEvents; n++){
        sumYminusT += (double)y[n] - velocity*(double)t[n];
        sumXYminusT += x[n]*(y[n] - velocity*t[n]);
    }
    std::cout << "sumXYminusT here: " << sumXYminusT << std::endl;
    std::cout << "sumYminusT here: " << sumYminusT << std::endl;

    slope2 = (sumXYminusT - meanX*sumYminusT) / varyXX;

    velocity = velocity * sqrt(1 + slope2*slope2 )  / ( 1 - slope2*slope2);


    //Close the file
    testBinary.close();
    
    thisTrack.print();
    thisTrack.fitTrack();

    /*
    cout << "sum x : " << sumX << endl; 
    cout << "mean x  : " << meanX << endl; 
    cout << "sum x2 : " << sumX2 << endl; 
    */

    //Find line of best fit
    interY1 = meanY - slope1*meanX;
    interY2 -= slope2*meanX;
    interY3 = meanY - slope3*meanX;

    cout << "Line 1 is: " << interY1 << " + " << slope1 << "*x " << endl;
    cout << "Line 2 is: " << interY2 << " + " << slope2 << "*x " << endl;
    cout << "Line 3 is: " << thisTrack.getIntercept() << " + " << thisTrack.getSlope() << "*x " << endl;

}
//shift y
//cout << "Event " << n << ": " << (bitset<16>)eventRaw << endl;
        /*cout << "Event " << n << ": " << (bitset<16>)eventRaw << endl;
        cout << "x: " << x[n] << ", " << (bitset<16>)x[n] << endl;
        cout << "y: " << y[n] << ", " << (bitset<16>)y[n] << endl;
        cout << "t: " << t[n] << ", " << (bitset<16>)t[n] << endl;*/