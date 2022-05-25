// ======================================
// Filename:    main.cpp
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#include <Arduino.h>

#include "base/Machine.h"
#include "utils/Logger.h"

Base::Machine machine;

void setup()
{
    Serial.begin(9600);
    delay(1000);

    INFO("Hello world!");

    machine.Start();
}

void loop()
{
    machine.Run();
}