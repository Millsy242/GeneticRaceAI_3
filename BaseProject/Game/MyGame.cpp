//
//  MyGame.cpp
//  BaseProject
//
//  Created by Daniel Harvey on 03/11/2019.
//  Copyright © 2019 Daniel Harvey. All rights reserved.
//

#include "MyGame.hpp"

MyGame::MyGame(ige::FileLogger *LOG,SettingsManager *SM) : Game(LOG, SM)
{
    
}

void MyGame::Start()
{
    *log << "Game Start";
    track.LoadTrackFromFile("NewTrack.yml",window->GetSize());
    for(int i{0};i<2;i++)
    {
        Car *car = new Car();
        car->LoadTexture("car.png");
        car->SetPosition(track.GetStart());
        car->SetScale(track.GetScale());
        car->giveRacingLineSpline(&track.CSpline,track.getStartNode(),track.TrackWidth);
        cars.push_back(car);
    }
}
void MyGame::Input(sf::Event e)
{
   *log << "Game Input";
    // car.Input(e);
     track.Input(e);
}
void MyGame::Render(Window *window)
{
    *log << "Game Render";
    sf::RectangleShape shape;
    shape.setSize({10,10});
    shape.setScale(track.GetScale());
    shape.setFillColor(sf::Color::Blue);
    shape.setPosition(track.GetStart());

    window->BeginDraw(sf::Color(20,55,34));
    
    track.Render(window);
    window->draw(shape);
    for(int i{0};i<cars.size();i++)
        cars[i]->Render(window);
    
    window->EndDraw();
}
void MyGame::UI()
{
    *log << "UI";
    GameUI();
    
        cars[0]->UI();
    
    track.UI();
}
void MyGame::EarlyUpdate()
{
    *log << "Game Early Update";
    track.EntityUpdate();
    
    for(int i{0};i<cars.size();i++)
    {
        cars[i]->offTrack = !cars[i]->isCollision(&track.temp);
        cars[i]->EntityUpdate();
    }
}
void MyGame::LateUpdate()
{
    *log << "Game Late Update";
}
