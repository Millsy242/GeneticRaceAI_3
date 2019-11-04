//
//  CarMemory.cpp
//  BaseProject
//
//  Created by Daniel Harvey on 01/10/2019.
//  Copyright © 2019 Daniel Harvey. All rights reserved.
//

#include "CarMemory.hpp"

CarMemory::CarMemory()
{
    
}

void CarMemory::Init(mySpline *spline, std::string TrackName)
{
    LoadAllFromDatabase();
    auto search = CarMemoryMap.find(TrackName);
    if(search == CarMemoryMap.end())
    {
        TrackMemory newTrackMemory;
        newTrackMemory.name = TrackName;
        //Add track
        for(int i{0}; i<spline->getNumberofPoints(); i++)
        {
            if(i == 19 || i > 38 || i < 4.4 )
                 newTrackMemory.MemoryMap.insert({i,{(float)i,15}});
            else
                newTrackMemory.MemoryMap.insert({i,{(float)i,3}});
        }
        CarMemoryMap.insert(std::pair<std::string,TrackMemory>(TrackName,newTrackMemory));
    }
}
void CarMemory::SaveToDatabase(std::string TrackName/*Database*/)
{
    
}
void CarMemory::LoadFromDatabase(std::string TrackName/*Database*/)
{
    
}
void CarMemory::LoadAllFromDatabase(/*Database*/)
{
    
}
std::pair<float,float> CarMemory::getBreakPoint(int point, std::string TrackName)
{
    auto search = CarMemoryMap[TrackName].MemoryMap.find(point);
    if(search != CarMemoryMap[TrackName].MemoryMap.end())
    {
        return CarMemoryMap[TrackName].MemoryMap[point];
    }
    return {0,0};
}
void CarMemory::setBreakPoint(int point ,std::pair<float,float> BPointSpeed, std::string TrackName)
{
    auto search = CarMemoryMap[TrackName].MemoryMap.find(point);
    if(search != CarMemoryMap[TrackName].MemoryMap.end())
    {
        CarMemoryMap[TrackName].MemoryMap[point] = BPointSpeed;
    }
}
