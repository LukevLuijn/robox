// ======================================
// Filename:    main.cpp
// Project:     ROBOX BKE
// Subsystem:   BKE control
// Author:      Luke van Luijn
// Data:        09-06-2022
// ======================================

#include <Arduino.h>

#include "Game.h"

Base::Game game;

void setup()
{
    Serial.begin(9600);
    delay(1000);

#ifdef DEBUG_MODE
    Serial.println("Hello world");
#endif 

    game.Start();
}

void loop()
{
    game.Run();
}