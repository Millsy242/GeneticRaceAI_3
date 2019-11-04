//
//  Car.hpp
//  BaseProject
//
//  Created by Daniel Harvey on 17/09/2019.
//  Copyright © 2019 Daniel Harvey. All rights reserved.
//

#ifndef Car_hpp
#define Car_hpp
#include "Entity.hpp"
#include <stdio.h>
#include <fstream>
#include <string>
#include "mySpline.hpp"
#include "SelbaWard.hpp"
#include "Chromosome.hpp"
#include "CarMemory.hpp"

struct Timing
{
    sf::Time SectorTime;
    sf::Time Laptime;
    sf::Time BestLaptime;
    
    std::vector<sf::Time> CurrentSectorTimes;
    std::vector<sf::Time> Laptimes;
    std::vector<sf::Time> PersonalBestSectors;
    
    void EndLap()
    {
        Laptimes.push_back(Laptime);
    }
    void EndSector(int sector)
    {
        switch (sector)
        {
            case 1:
                CurrentSectorTimes[0] = SectorTime;
                if(SectorTime < PersonalBestSectors[0])
                    PersonalBestSectors[0] = SectorTime;
                break;
            case 2:
                CurrentSectorTimes[1] = SectorTime;
                if(SectorTime < PersonalBestSectors[1])
                    PersonalBestSectors[1] = SectorTime;
                break;
            case 3:
                CurrentSectorTimes[2] = SectorTime;
                if(SectorTime < PersonalBestSectors[2])
                    PersonalBestSectors[2] = SectorTime;
                break;
            default:
                break;
        }
        SectorTime.Zero;
    }
    
    sf::Color LapColour()
    {
        if(Laptime <= BestLaptime)
        {
            BestLaptime = Laptime;
            return sf::Color::Green;
        }
        else
        {
            return sf::Color::White;
        }
    }
};

class Car : public Entity
{
public:
    Car(){};
    ~Car(){};
    
    void giveRacingLineSpline(mySpline *CenterLine, int startpoint,float Twidth);
  
    void Start() override ;
    void EntityUpdate()  override ;
    virtual void LoadTexture(std::string filepath) override;
    virtual void LoadTexture(sf::Texture texture);
    void Input(sf::Event event) override ;
    void Exit() override ;
    
    void UI() override;
    
    void Render(Window *window) override;
    
    Timing Laptimes;
    
     bool offTrack = false;
    
    void findTarget(sf::Vector2f next);
    
    void UpdateRacingLine();
    
private:
    void MoveCar(bool Drive,bool Right, bool Left, bool Brake);
    void LoadFromFile(std::string Filepath);
    
    CarMemory CMemory;
    
    bool  RLine{true};
    
    float fitness = 0.f;
    float maxSpeed=15.0;
    float acc=0.08;
    float drag=0.001;
    float brakepower = 0.2;
    float turnSpeed=3.f;
    float offTrackSpeed = 1.f;
    float speed = 1.f;
    
    int radius = 25;
    
    float angle = 0;
    bool D{false},B{false},L{false},R{false};
    bool Debug = false;
   
    Chromosome chromo;
    
    sf::Vector2f forwardVec = sf::Vector2f(0.f, -1.f);
    sf::Vector2f movementVec; //normal vector based on current direction
    
    sf::Color CarColour = sf::Color::White;
    
    mySpline RacingLine;
    mySpline *CSpline;
    
    sw::Line carline;
    sw::Line myline;
    
    float fmarker= 1.f;
    int start = 0;
    
    //This should be simulated, but for now its a standard number
    float grip = 0.8;
    float reactionTime = 0;

    int iteration = 0;

    float fDisplacement[50];
    float TrackWidth;

    bool curve{false}, distance{true};
};

#endif /* Car_hpp */
