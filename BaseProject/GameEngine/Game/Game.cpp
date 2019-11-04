//
//  Game.cpp
//  BaseProject
//
//  Created by Daniel Harvey on 13/09/2019.
//  Copyright © 2019 Daniel Harvey. All rights reserved.
//
#include "Game.hpp"
#include <iostream>
Game::Game(ige::FileLogger *LOG,SettingsManager *SM)
{
    log = LOG;
    settings = SM;
    
    DebugTexture.loadFromFile("Game Icons/Debug Icon.png");
    ExitTexture.loadFromFile("Game Icons/Exit Icon.png");
    PauseTexture.loadFromFile("Game Icons/Pause Icon.png");
    SettingsTexture.loadFromFile("Game Icons/Settings Icon.png");
    
    Active = true;
}

void Game::Start()
{
    *log << "Game Start";
    
}
void Game::Input(sf::Event e)
{
   *log << "Game Input";
}
void Game::Render(Window *window)
{
    *log << "Game Render";
    window->BeginDraw(sf::Color(20,55,34));
    
    window->EndDraw();
}
void Game::UI()
{
    *log << "UI";
    GameUI();
}
void Game::EarlyUpdate()
{
    *log << "Game Early Update";

}
void Game::LateUpdate()
{
    *log << "Game Late Update";
}

void Game::GameUI()
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoNav;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoNavFocus;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
    
    ImGui::Begin("GameUI",NULL,window_flags);
    if(Pause)
    {
        *log << "Game Paused";
        //centre window on screen
        ImGui::SetWindowPos(ImVec2((window->GetCentre().x - ImGui::GetWindowSize().x/2),(window->GetCentre().y - ImGui::GetWindowSize().y/2)));
        
        ImGui::Text("PAUSED");
        if(ImGui::Button("Return"))
        {
            Pause = false;
        }
        if(ImGui::Button("Menu"))
        {
             Active = false;
        }
    }
    else
    {
        ImGui::SetWindowPos(ImVec2((window->GetCentre().x - ImGui::GetWindowSize().x/2),(window->GetSize().y - ImGui::GetWindowSize().y-50)));
        ImGui::SetCursorPos(ImVec2(5,5));
        if(ImGui::ImageButton(PauseTexture, sf::Vector2f(20,20)))
        {
            Pause = true;
        }
        ImGui::SetCursorPos(ImVec2(35,5));
        if(ImGui::ImageButton(SettingsTexture, sf::Vector2f(20,20)))
        {
            Pause = true;
        }
        ImGui::SetCursorPos(ImVec2(65,5));
        if(ImGui::ImageButton(ExitTexture, sf::Vector2f(20,20)))
        {
            Continue = false;
            Active = false;
        }
        ImGui::SetCursorPos(ImVec2(95,5));
        if(ImGui::ImageButton(DebugTexture, sf::Vector2f(20,20)))
        {
            Debug = !Debug;
        }
    }
    ImGui::End();

    if(Debug)
    {
        ImGui::Begin("Debug Menu");
            ImGui::Checkbox("Metrics", &DebugMetrics);
            ImGui::Checkbox("User Guide", &DebugUserGuide);
            ImGui::Checkbox("Style Selector", &DebugStyleSel);
            ImGui::Checkbox("Style Editor", &DebugStyleEditer);
        ImGui::End();
    }
}
