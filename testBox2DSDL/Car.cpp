#pragma once
#include "Car.h"
#include "InitialSetup.h"
#include "Constants.h"

void Car::addCar(int x, int y, int w, int h)
{
    // Add the car body
    carBody = sq1->addRect(x, y, w, h);

    wheel1 = c1->addCircle(x - w / 2, y + h / 2, h / 2.5);
    wheel2 = c1->addCircle(x + w / 2, y + h / 2, h / 2.5);
    //wheel3 = addCircle(x - w / 2, y - h / 2, h / 2.5); 
    //wheel4 = addCircle(x + w / 2, y - h / 2, h / 2.5);

    // Add the revolute joints
    b2RevoluteJointDef jointdef1, jointdef2, jointdef3, jointdef4;
    jointdef1.Initialize(carBody, wheel1, wheel1->GetWorldCenter());
    jointdef2.Initialize(carBody, wheel2, wheel2->GetWorldCenter());
    //jointdef3.Initialize(carBody, wheel3, wheel3->GetWorldCenter()); 
    //jointdef4.Initialize(carBody, wheel4, wheel4->GetWorldCenter());

    jointdef1.enableMotor = true;
    jointdef2.enableMotor = true;
    //jointdef3.enableMotor = true;
    //jointdef4.enableMotor = true;

    joint1 = (b2RevoluteJoint*)world->CreateJoint(&jointdef1);
    joint2 = (b2RevoluteJoint*)world->CreateJoint(&jointdef2);
    //joint3 = (b2RevoluteJoint*)world->CreateJoint(&jointdef3); 
    //joint4 = (b2RevoluteJoint*)world->CreateJoint(&jointdef4);
}