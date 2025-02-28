//
//  Game.cpp
//  BaseProject
//
//  Created by Daniel Harvey on 15/06/2019.
//  Copyright © 2019 Daniel Harvey. All rights reserved.
//

#include "WindowHolder.hpp"
#include <math.h>
#include <iostream>

void WindowHolder::Start()
{
	Active = true;
	c.restart();
}
void WindowHolder::GiveWindow(Window *w)
{
	window = w;
}
void WindowHolder::Stats()
{
	*log << "Display Stats";
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoBackground;
	window_flags |= ImGuiWindowFlags_NoNav;
	window_flags |= ImGuiWindowFlags_NoInputs;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoNavFocus;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	
    ImGui::Begin("Stats",NULL,window_flags);
	ImGui::SetWindowSize("Stats", ImVec2(110,70));
    ImGui::Text("FPS: %i \nDT: %fms",window->GetFPS(),window->getDT()*1000);
    ImGui::End();
	
	
	
	if(Debug)
	{
		if(DebugMetrics)
			ImGui::ShowMetricsWindow();
		if(DebugUserGuide)
			ImGui::ShowUserGuide();
		if(DebugStyleSel)
			ImGui::ShowStyleSelector("Style");
		if(DebugStyleEditer)
			ImGui::ShowStyleEditor();
	}
	
	
}

void WindowHolder::Update()
{
	const float frameDuration = 17;
	const int maxTimePerFrame = 200;
	float accumulatedTime = 0;
	Active = true;
	while (Active)
	{
		accumulatedTime += c.restart().asMilliseconds();
		if (accumulatedTime > maxTimePerFrame)
		{
			accumulatedTime = maxTimePerFrame;
		}
		window->Update();
		Stats();
		UI();
		if(!Pause)
		{
			//Ensure Fps doesnt change User experiance
			while (accumulatedTime >= frameDuration)
			{
				Input(window->GetEvent());
				accumulatedTime -= frameDuration;
				EarlyUpdate();
				LateUpdate();
			}
		}
		Render(window);
	}
	return;
}
void WindowHolder::Exit()
{
	
}
