/*
 * Title   : Source File for Server
 * File    : Server.hpp
 * Author  : Ani
 * Date    : 01/30/23
*/

#include "../inc/Server.hpp"
#include "driver/gpio.h"

void Server::init()
{
    gpio_set_direction(GPIO_NUM_21, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_NUM_19, GPIO_MODE_INPUT);
}

void Server::espNowRecvCb(const Message& message)
{
    printf("task: %u, action: %u\n", message.task, message.action);
}