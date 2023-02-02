# Next Steps
## ESP-32
1. Get a server (master) to "ping-pong" to 3 vent (slaves)
2. Make some sort of "discover nearby vents" to stop having to pair them by hard-coding the mac addresses
3. Use NV-RAM to be able to save user settings and vent pairings even if power is lost

## Touchscreen Display (the display is a microcontroller too)
1. Figure how to havd the display get data from ESP server (master) (UART protocol)
2. Design a UI that lists the room, whether it is heating or not, temp, etc
3. Design main control panel and settings on there (Talk back and forth with ESP server (master)

## Sensors
1. Make subroutines to check temp and room activity (PIR sensor)

## Hardware
1. How to open / close vent
2. Some plumbing to do for our isolated "room" test demo
3. 3d CAD demo if we have time

###### the more overkill we go, the more likely we get better ranks and prizes lol

# Timeline
02/05: Finish peer-to-peer system
02/12: Temp and Motion Sensor interrupts (vent checks, and server updates, both according to interrupt prescaler)
03/05: Get motorized vents working
03/26: Have house demo box set up with tubes for heat to travel to vent
04/09: Callibration of temp sensor complete
05/01: Wrap up, get some time during SS meeting for a practice demo

