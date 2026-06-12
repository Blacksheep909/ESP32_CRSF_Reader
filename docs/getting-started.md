# Getting Started

This guide is for checking CRSF / ExpressLRS receiver input on an ESP32 before integrating it into a larger project.

## 1. Hardware

You need:

- ESP32 DevKit or similar ESP32 board.
- CRSF / ExpressLRS receiver.
- USB cable for the ESP32.
- Common ground between receiver and ESP32.
- Receiver power appropriate for your receiver.

Default wiring:

| Receiver | ESP32 |
| --- | --- |
| TX | GPIO 16 / RX2 |
| GND | GND |
| Power | Receiver-appropriate power |

## 2. Bind The Radio Link First

Before testing the ESP32 code, bind the transmitter/module and receiver exactly like an ExpressLRS FPV drone:

1. Flash or configure both devices with the same major ExpressLRS version.
2. Use the same binding phrase on transmitter and receiver, or use the traditional bind process.
3. Confirm the receiver LED/status indicates a live link when the transmitter is on.
4. Set the receiver serial output/protocol to `CRSF`.

Useful ExpressLRS references:

- [Getting Started](https://www.expresslrs.org/quick-start/getting-started/)
- [Binding ExpressLRS](https://www.expresslrs.org/quick-start/binding/)
- [Receiver Wiring](https://www.expresslrs.org/quick-start/receivers/wiring-up/)
- [Receiver Serial Protocols](https://www.expresslrs.org/software/serial-protocols/)

## 3. Software

Install:

- VS Code.
- PlatformIO IDE extension.

Open:

```text
examples/basic_receiver
```

Build:

```powershell
pio run
```

Upload:

```powershell
pio run -t upload
```

Monitor:

```powershell
pio device monitor
```

## 4. Expected Output

The example prints one line about every 100 ms:

```text
link=1 frames=123 ch1=1500 ch2=1500 ...
```

What to look for:

- `link=1` means fresh CRSF frames are being received.
- `frames=` should keep increasing.
- Stick movement should change channel values.
- Switch movement should change some channel values between low/mid/high positions.

## 5. Integrate Into Your Own Project

Include the library:

```cpp
#include <Esp32CrsfReader.h>
```

Create a reader:

```cpp
Esp32CrsfReader crsf;
```

Start it:

```cpp
crsf.begin(Serial2, 16, 17);
```

Update it often in `loop()`:

```cpp
crsf.update();
```

Read channels:

```cpp
int ch1Us = crsf.channelUs(0);
float ch1Norm = crsf.channelNormalized(0);
```

Check link health before trusting controls:

```cpp
if (!crsf.linkAlive()) {
  // Enter failsafe or ignore controls.
}
```

## 6. Channel Indexing

The API uses zero-based indexes:

| API index | RC channel |
| --- | --- |
| 0 | CH1 |
| 1 | CH2 |
| 2 | CH3 |
| 3 | CH4 |
| 15 | CH16 |
