# FakeCamTCPClient
Another test task from a nameless company with great ambitions, recognized market leaders and blah blah .. .

| Check | Client OS | Server OS |
| ------------- | ------------- | ------------- |
| Ok | Debian 11 | Debian 11 |
| Ok | Windows 10 | Windows 10 |
| Ok | Windows 10 | Debian 11 |
| ??? | Debian 11 | Windows 10 |

Test description:
LED control
It is necessary to implement a camera LED control system. The camera LED (LED) is characterized by the following set of parameters.

1. Status (state): on / off.
2. Color (color): red, green, blue.
3. Measurement frequency (rating): 0..5 (in Hertz).

The system should consist of 2 components.
1. Application in C ++ (server) that simulates interaction with the LED.
2. C++ application (client) that provides an interface to control the LED.

Commands:
| Command | Argument | Result |Description |
| ------------- | ------------- | ------------- |------------- |
| set-led-state | on, off | OK, FAILED |turn on/off LED|
| get-led-state | | OK on|off, FAILED |get LED state|
| set-led-color | red, green, blue | OK, FAILED |set LED color|
| get-led-color | | OK red|green|blue, FAILED |get LED color|
| set-led-rate | 0..5 | OK, FAILED |set LED rate|
| get-led-rate | | OK 0..5, FAILED |get LED rate|

Examples:
Req – request, Res – response.<br>
Req: "get-led-state\n"<br>
Res: "OK on\n"<br><br>
Req: "set-led-color yellow\n"<br>
Res: "FAILED\n"<br>

Server:
1. The server architecture should make it easy to add new commands and change the logic of the existing ones.
2. The server must be able to work with multiple clients, serving requests in real time.
3. The server must correctly handle any abnormal situations and errors.

Client:
1. The client architecture should make it easy to add new commands and change the logic of the existing ones.

Common:
1. Language and Libraries: ISO C++ (up to and including 17) and its standard library, Boost and/or glibc (for Linux).
2. The code must be designed to build and run modern distributions of the Linux OS.
3. Build system: GNU Make or CMake.
