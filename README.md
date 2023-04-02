# Smart Vents
Solar Suitcase 2022 - 2023 \
IoT system for improved efficiency in multi-zoned heating

## Problem
Heat rises. When I lived in a multi-story house in Seattle, 
it was often below room temperature in the lower floor but 
quite hot upstairs. I sort of accepted the fact that rooms 
heat unevenly. However, large buildings usually have zones 
for heating to ensure a more even coverage. This is a decent fix,
but may be a bit limited in flexibility. For example, if a heating 
zone consists of a kitchen and a living room, the kitchen, with 
stoves, ovens, and fridges, may end up a lot warmer than the other
room (personal experience). And having the option and ability
to adjust to changes is a core part of smart devices.

## Solution
Since each room has heating vents, they would provide a more 
precice form of measuring a room's temperature. Additionally, 
vents on the ceiling can have embedded motion sensors to detect 
if a room is occupied. Heating can then be prioritized for that 
room so it heats up faster. \
**Note:** Of course, automated features should always have a 
setting to manually override. In this case, that would be an 
option to heat every room in a traditional manner and disable 
room prioritiztion.

#### Block Diagram

###### Communitcation
<img src="concept/wifi-comm.png" alt="wifi-comm" height="325px"/> \
The ESP-NOW communication protocol will be used as the method of two-way communication.

###### Hardware
<img src="concept/vent-components.png" alt="vent-components" height="175px"/> \
A vent consists of an ESP32 as its CPU with Wi-Fi capabilities.
This will allow the vent to communicate with the thermostat or 
central hub. Each vent also has a temperature and motion sensor.

###### Start
<img src="concept/vent-idle.png" alt="vent-idle" height="275x"/> \
The vent starts in low-power mode, and checks the temperature 
and motion sensor occasionally with an interrupt.

###### Interrupt
<img src="concept/vent-interrupt.png" alt="vent-interrupt" height="475px"/> \
The interrupt is triggered by a counter overflow and its frequency can be changed 
with a prescaler. In the interrupt, the ESP32 checks whether the temp is 
lower than the set temp and whether something moved.

###### Main (Incomplete)
<img src="concept/vent-main.png" alt="vent-main" height="1100px"/> \
The vent will initiate the heating process by interfacing with the central hub 
once it detects a room with a lower temperature. Of course, the comparison logic 
should consider the hysteresis--it would be bad to turn on and off the heater 
repeatedly when the temp is 24.9 and then turn off at 25 and repeat when it cools down.
#### BOM
| Component    | Value   |     Count  | Price       | Link      |
|--------------|:-------:|-----------:|------------:|----------:|
| U1           |  ESP32-S2    |      4     |    $5.00    |    |
| U2           |  BME280   |      6     |    $6.00    |    |
| U3           |  MCP602   |      3     |    $2.50    |    |
| D1           |  NX4832T035      |      1     |    $39.90   |    |
| R1           |  1M     |      3     |    -    |    |
| R2           |  10K   |      6     |    -    |    |


#### Demo
To demonstrate the concept, a setup of different-sized heat-insulated boxes with thermometers attatched will simulate a house
