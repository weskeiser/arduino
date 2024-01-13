# LED Transport times tracker

Made with the purpose of teaching myself C and embedded programming. Use at your own risk!

Pulls data from [Entur's public API](https://developer.entur.org/pages-real-time-api) through `cURL`. The data is lightly massaged with `jq` and `lua` before it is sent to an RF24 supported arduino. The arduino passes the data along to another arduino, using RF24 communication, which then displays it with a connected `WS2812` LED display. 

Makes use of the [Mirf nRF24L01](https://github.com/aaronds/arduino-nrf24l01/tree/master/Mirf) library.

Edit `place` and `coordinates` in request/req.json to change the stops. Edit `numTripPatterns` to change the number of departures to request.

## Default config
```lua
{
    interval_seconds = 15, -- how often to pull data
    expected_minutes_max = 17, -- fare departure time maximum
    port = "/dev/ttyUSB1", -- port where emitting arduino is connected
    stty =
    "115200 -parenb -parodd -cmspar cs8 -hupcl -cstopb cread clocal -crtscts -ignbrk brkint ignpar -parmrk -inpck -istrip -inlcr -igncr -icrnl ixon -ixoff -iuclc -ixany -imaxbel -iutf8 -opost -olcuc -ocrnl -onlcr -onocr -onlret -ofill -ofdel nl0 cr0 tab0 bs0 vt0 ff0 -isig -icanon iexten -echo echoe -echonl -noflsh -xcase -tostop -echoprt echoctl echoke -flusho -extproc" -- I haven't taken the time to optimize the tty, but I read this stty config from GNU Screen and it seems to work fine. It only needs to be ran once, when the device connects to the port.
}
```

## Requirements
- Lua 5.4
- 2x Arduino Nano

## Run
`cd request && chmod u+x ./init.lua`
`./init.lua` -- should be ran with `-stty` every time the arduino is plugged into the computer


