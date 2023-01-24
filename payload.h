#pragma once

#include <stdio.h>

typedef enum Action 
{
    OPEN,  // Open vent
    CLOSE, // Close vent
    GET,   // Get temp
    SET,   // Set temp
} Action;

typedef struct Payload
{
    Action action;
} Payload;