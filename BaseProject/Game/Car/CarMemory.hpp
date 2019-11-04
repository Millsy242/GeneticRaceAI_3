//
//  CarMemory.hpp
//  BaseProject
//
//  Created by Daniel Harvey on 01/10/2019.
//  Copyright © 2019 Daniel Harvey. All rights reserved.
//

#ifndef CarMemory_hpp
#define CarMemory_hpp

#include <stdio.h>
#include <map>
#include <unordered_map>
#include "mySpline.hpp"

struct TrackMemory
{
    std::string name;
    //Full node, inner node, value
    std::map<int, std::pair<float,float>> MemoryMap;
};

class CarMemory
{
public:
    CarMemory();
    ~CarMemory(){};
    
    void Init(mySpline *spline,std::string TrackName);
    std::pair<float,float> getBreakPoint(int point,std::string TrackName);
    void setBreakPoint(int point,std::pair<float,float> BPointSpeed,std::string TrackName);
    
    /*
        If the car has analog brake rather than digital ones, this could be changed to
        store a brake % at each float point perhaps like std::unordered_map<int, std::pair<float,float> MemoryMap;
        then interpolate between points
     */
    
    
private:
    //track name
    std::unordered_map<std::string,TrackMemory> CarMemoryMap;
    
    void SaveToDatabase(std::string TrackName/*Database*/);
       void LoadFromDatabase(std::string TrackName/*Database*/);
    void LoadAllFromDatabase(/*Database*/);
};

#endif /* CarMemory_hpp */
