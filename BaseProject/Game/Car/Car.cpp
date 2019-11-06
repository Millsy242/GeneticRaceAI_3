//
//  Car.cpp
//  BaseProject
//
//  Created by Daniel Harvey on 17/09/2019.
//  Copyright © 2019 Daniel Harvey. All rights reserved.
//

#include "Car.hpp"
#include <iostream>
void Car::Start()
{

}
void Car::LoadTexture(std::string filepath)
{
    Collision::CreateTextureAndBitmask(EntityTexture, filepath);

    EntitySprite.setTexture(EntityTexture);
    EntitySprite.setColor(CarColour);
    EntitySprite.setOrigin(EntitySprite.getLocalBounds().width/2, EntitySprite.getLocalBounds().height-10);
}
void Car::LoadTexture(sf::Texture texture)
{
    EntityTexture = texture;
    Collision::CreateTextureAndBitmask(&EntityTexture);
    EntitySprite.setTexture(EntityTexture);
    EntitySprite.setTexture(EntityTexture);
    EntitySprite.setColor(CarColour);
    EntitySprite.setOrigin(EntitySprite.getLocalBounds().width/2, EntitySprite.getLocalBounds().height-10);
}

void Car::giveRacingLineSpline(mySpline *CenterLine, int startpoint,float Twidth)
{
    CSpline = CenterLine;
    for(int i{0}; i<CenterLine->points.size();i++)
    {
        RacingLine.AddPoint(CenterLine->GetSplinePoint(i));
    }
    RacingLine.UpdateSplineProperties();
    
    TrackWidth = Twidth;
    radius = TrackWidth*1.8;
   // FlipTexture(true);
    
    CMemory.Init(&RacingLine,"TestTrack_1");
    
    
    
    start = startpoint;
    fmarker = start;
}

void Car::EntityUpdate()
{
    /*
    fmarker -=speed * 0.0016; //+ or - depending on direction
        
    if (fmarker >= (float)RacingLine->points.size())
        fmarker -= (float)RacingLine->points.size();
    
    if (fmarker < 0.0f)
        fmarker += (float)RacingLine->points.size();
    
    float fOffset =  fmarker;//RacingLine->GetNormalisedOffset(fmarker);
    sf::Vector2f p1 = RacingLine->GetSplinePoint(fOffset);
    sf::Vector2f g1 = RacingLine->GetSplineGradient(fOffset);
    float r = atan2(-g1.y, g1.x);
    float len = 50.f;

    sf::Vector2f start = {len * sin(r) + p1.x, len * cos(r) + p1.y};
    sf::Vector2f end = {-len * sin(r) + p1.x, -len * cos(r) + p1.y};
    myline = sw::Line(start,end,5.f);
    */
    
    
    angle = sf::degToRad(EntitySprite.getRotation());
    
    float x{GetPosition().x},y{GetPosition().y},tx{RacingLine.GetSplinePoint(fmarker).x},ty{RacingLine.GetSplinePoint(fmarker).y};
    
    if ((x-tx)*(x-tx)+(y-ty)*(y-ty)<radius*radius)
    {
        fmarker=std::fmod(fmarker-(1/speed), RacingLine.points.size());
        if(fmarker < 0)
            fmarker = RacingLine.points.size() -1;
    }
    
    bool D_{1},B_{0},R_{0},L_{0};
    
    sf::Vector2f next = RacingLine.GetSplinePoint(fmarker);
    
    tx=next.x;
    ty=next.y;

    float beta = angle-atan2(tx-GetPosition().x,-ty+GetPosition().y);

    if (sin(beta) < 0)
    {
        R_ = true;
    }
    else
    {
        L_ = true;
    }
    
    auto BpointANDSpeed = CMemory.getBreakPoint(std::roundf(fmarker),"TestTrack_1");
    float DistanceToNode = 0; //CHNAGEME
    float speedDif = speed - BpointANDSpeed.second;
    float BrakingDistance = (speedDif * speedDif)/(250.f * grip);
    float ReactionDistance = speed * reactionTime;
    //std::cout<< "Braking Distance = " << BrakingDistance << "\n Fmarker = " << fmarker<<"\n";
    if(speed > BpointANDSpeed.second)
    {
        if(ReactionDistance + BrakingDistance >= DistanceToNode)
        {
            B_ = true;
            EntitySprite.setColor(sf::Color::Red);
        }
    }
    else
    {
         EntitySprite.setColor(sf::Color::Blue);
    }
    
    /*
    if(BpointANDSpeed.first < fmarker-0.01 && speed > BpointANDSpeed.second)
    {
        
        B_ = true;
        D_ = false;
    }
    else
    {
        D_ = true;
       
    }
     */
    MoveCar(D_,R_,L_,B_);
}
void Car::UpdateRacingLine()
{
    // Reset racing line
    for (int i = 0; i < RacingLine.points.size(); i++)
    {
        RacingLine.points[i] = CSpline->points[i];
        fDisplacement[i] = 0;
    }
    RacingLine.UpdateSplineProperties();

    for (int n = 0; n < iteration; n++)
    {
        for (int i = 0; i < RacingLine.points.size(); i++)
        {
            // Get locations of neighbour nodes
            sf::Vector2f pointRight = RacingLine.GetSplinePoint((i + 1) % RacingLine.points.size());
            sf::Vector2f pointLeft = RacingLine.GetSplinePoint((i + RacingLine.points.size() - 1) % RacingLine.points.size());
            sf::Vector2f pointMiddle = RacingLine.GetSplinePoint(i);

            // Create vectors to neighbours
            sf::Vector2f vectorLeft = { pointLeft.x - pointMiddle.x, pointLeft.y - pointMiddle.y };
            sf::Vector2f vectorRight = { pointRight.x - pointMiddle.x, pointRight.y - pointMiddle.y };

            // Normalise neighbours
            float lengthLeft = sqrtf(vectorLeft.x*vectorLeft.x + vectorLeft.y*vectorLeft.y);
            sf::Vector2f leftn = { vectorLeft.x / lengthLeft, vectorLeft.y / lengthLeft };
            float lengthRight = sqrtf(vectorRight.x*vectorRight.x + vectorRight.y*vectorRight.y);
            sf::Vector2f rightn = { vectorRight.x / lengthRight, vectorRight.y / lengthRight };

            // Add together to create bisector vector
            sf::Vector2f vectorSum = { rightn.x + leftn.x, rightn.y + leftn.y };
            float len = sqrtf(vectorSum.x*vectorSum.x + vectorSum.y*vectorSum.y);
            vectorSum.x /= len;
            vectorSum.y /= len;

            // Get point gradient and normalise
            sf::Vector2f g = CSpline->GetSplineGradient(i);
            float glen = sqrtf(g.x*g.x + g.y*g.y);
            g.x /= glen; g.y /= glen;

            // Project required correction onto point tangent to give displacment
            float dp = -g.y*vectorSum.x + g.x * vectorSum.y;

            // Shortest path
            if(distance)
                fDisplacement[i] += (dp * 0.3f);

            // Curvature
            if(curve)
            {
                fDisplacement[(i + 1) % RacingLine.points.size()] += dp * -0.2f;
                fDisplacement[(i - 1 + RacingLine.points.size()) % RacingLine.points.size()] += dp * -0.2f;
            }
        }

        // Clamp displaced points to track width
        for (int i = 0; i < RacingLine.points.size(); i++)
        {
            if (fDisplacement[i] >= TrackWidth)
                fDisplacement[i] = TrackWidth;
            if (fDisplacement[i] <= -TrackWidth)
                fDisplacement[i] = -TrackWidth;

            sf::Vector2f g = CSpline->GetSplineGradient(i);
            float glen = sqrtf(g.x*g.x + g.y*g.y);
            g.x /= glen; g.y /= glen;

            RacingLine.points[i].x = CSpline->points[i].x + -g.y * fDisplacement[i];
            RacingLine.points[i].y = CSpline->points[i].y + g.x * fDisplacement[i];
        }
    }
    RacingLine.UpdateSplineProperties();
}
void Car::Render(Window *window)
{
    if(Debug)
    {
        sf::CircleShape circle;
        circle.setRadius(radius);
        circle.setOutlineThickness(5);
        
        circle.setFillColor(sf::Color::Transparent);
        circle.setOrigin(circle.getLocalBounds().width/2, circle.getLocalBounds().height/2);
        
        for(float t{0};t<RacingLine.points.size();t+=(1/speed))
        {
            if(t >= fmarker)
            {
                circle.setOutlineColor(sf::Color::Magenta);
            }
            else
            {
                circle.setOutlineColor(sf::Color::White);
            }
            circle.setPosition(RacingLine.GetSplinePoint(t));
            window->draw(circle);
        }
        
        if(RLine)
        RacingLine.DrawSelf(window,sf::Color::Magenta,{5,5});
        
        window->draw(myline);
    }
    
    
    if(Active)
        window->draw(EntitySprite);
   
    
    
}
void Car::Input(sf::Event event)
{
    
     D = false;B = false;L = false;R = false;
    /*
    if(event.type == sf::Event::KeyPressed )
    {
        if(event.key.code == sf::Keyboard::Left)
        {
            L = 1;
        }
        if(event.key.code == sf::Keyboard::Right)
        {
            R = 1;
        }
        if(event.key.code == sf::Keyboard::Space)
        {
            D = 1;
        }
        if( event.key.code == sf::Keyboard::Down)
        {
            B = 1;
        }
    }
  */
   
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        R=1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        B=1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        L=1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        D=1;
    }
     
     //MoveCar(D,R,L,B);
}
void Car::UI()
{
    ImGui::Begin("car");
    ImGui::SliderInt("radius", &radius, 1, 200);
    ImGui::Text("%fmph",speed*12);
    ImGui::Checkbox("DEBUG", &Debug);
    ImGui::End();
    
    static int maxit{100},minit{0};
    ImGui::Begin("track");
    ImGui::Checkbox("Show Racing Line", &RLine);
    ImGui::InputInt("max iteration", &maxit);
    ImGui::InputInt("min iteration", &minit);
    if(ImGui::SliderInt("iterations", &iteration, minit, maxit) || ImGui::Checkbox("Curve", &curve) || ImGui::Checkbox("Distance", &distance))
    {
        UpdateRacingLine();
    }
    ImGui::End();
    
}
void Car::Exit()
{
    return;
}
void Car::MoveCar(bool Drive,bool Right, bool Left, bool Brake)
{
    float turnSpeed_temp = turnSpeed * (1-(speed/maxSpeed));
    
    if(speed > 0)
    {
        if(Right || Left)
        {
           // if(speed > 3)
             //   speed-=(acc*0.75);
          //  speed-=0.0001; // some speed is scrubbed off when turning.
            
            if(Left)
            {
                EntitySprite.rotate(-turnSpeed_temp);
            }
            else if(Right)
            {
                EntitySprite.rotate(turnSpeed_temp);
            }
        }
        if(Brake)
        {
            speed -= drag;
            speed -= brakepower;
        }
    }
    if(Drive)
    {
        if (speed<maxSpeed)
        {
            speed += acc;
        }
    }
    else
    {
        if (speed - drag > 0)
        {
            speed -= drag;
        }
        else if (speed + drag < 0)
        {
            speed += drag ;
        }
        else
        {
            speed = 0;
        }
    }
    if(Brake)
    {
        speed -= drag;
        speed -= brakepower;
    }
    if(speed < 0.5)
    {
        speed = 0.5; //make sure car doesn't go backwards.
    }
    if(offTrack && speed>offTrackSpeed)
    {
        speed = offTrackSpeed;
    }

    sf::Vector2f oldVec = movementVec;
    
    sf::Transform t;
    t.rotate(EntitySprite.getRotation());
    movementVec = t.transformPoint(forwardVec);
    //calc the dot product so any rotation reduces the
    //current speed - gives the impression of wheel spin
    //when accelerating after a tight turn
    speed *= oldVec * movementVec;
   
    SetPosition(sf::Vector2f(GetPosition().x + (movementVec.x * speed),GetPosition().y + (movementVec.y * speed)));


}
void Car::LoadFromFile(std::string Filepath)
{
    std::string line;

    std::ifstream myfile(Filepath);
    bool exit = false;
    int i = 0;
    if (myfile.is_open())
    {
        while (getline (myfile,line) && !exit)
        {
            switch(i)
            {
                case(0):
                    break;
                case(1):
                    maxSpeed = std::stof(line);
                    break;
                case(2):
                    acc = std::stof(line) ;
                    break;
                case(3):
                    drag = std::stof(line);
                    break;
                case(4):
                    brakepower = std::stof(line) ;
                    break;
                case(5):
                    turnSpeed = std::stof(line) ;
                    break;
                case(6):
                    offTrackSpeed = std::stof(line);
                    break;
                case(7):
                    break;
                case(8):
                    exit = true;
                    break;
                default:
                    break;
            }
            i++;
        }
        myfile.close();
    }

}





