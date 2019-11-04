//
//  MyGame.hpp
//  BaseProject
//
//  Created by Daniel Harvey on 03/11/2019.
//  Copyright © 2019 Daniel Harvey. All rights reserved.
//

#ifndef MyGame_hpp
#define MyGame_hpp
#include <iostream>
#include <stdio.h>
#include "Game.hpp"

#include "Track.hpp"
#include "Car.hpp"

class MyGame : public Game
{
public:
    MyGame(ige::FileLogger *LOG,SettingsManager *SM) ;
    void Start() override;
    void Input(sf::Event e) override;
    void Render(Window *window) override;
    void UI() override;
    void EarlyUpdate() override;
    void LateUpdate() override;
    
private:
    Track track;
    sf::Texture cartexture;
    
    std::vector<Car*> cars;
};

#endif /* MyGame_hpp */
