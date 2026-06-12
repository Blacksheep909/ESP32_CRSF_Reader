# ESP32_CRSF_Reader

Small Arduino / PlatformIO library for reading CRSF / ExpressLRS RC channel frames on an ESP32.

This code was extracted from my V10 quadruped robot project, where one of the major firmware goals was moving from simpler iBUS-style receiver handling to CRSF. The robot needed a faster, cleaner radio link for mode switching, tilt control, ride-height selection, and failsafe behavior.

## What It Does

- Reads CRSF on ESP32 `Serial2`.
- Uses the standard 420000 baud CRSF serial rate.
- Synchronizes and validates CRSF frames.
- Checks DVB-S2 CRC8.
- Decodes `0x16` packed RC channel frames.
- Unpacks 16 channels from 11-bit CRSF values.
- Converts channel values into 1000-2000 us style values.
- Applies lightweight first- and second-order filtering.
- Tracks link freshness for failsafe logic.

## Why This Exists

CRSF is common in modern ExpressLRS setups, but it is not as plug-and-play as older receiver approaches. The ESP32 needs to parse the packed serial protocol correctly before the rest of the project can trust the channel data.

For a quadruped robot, a bad receiver read is not just a bad steering input. It can toggle state machines and move twelve servos at once. This library exists to keep that receiver layer isolated, testable, and reusable.

## Wiring

Typical ESP32 DevKit wiring:

| Receiver | ESP32 |
| --- | --- |
| TX | GPIO 16 / RX2 |
| GND | GND |
| 5V or 3V3 | Receiver-appropriate power |

The default example uses:

- RX pin: `16`
- TX pin: `17`
- Baud: `420000`

Most receive-only projects only need the receiver TX line into the ESP32 RX pin, plus common ground.

## Quick Start

There are two ways to use this project:

- Copy the library into your own PlatformIO project.
- Open `examples/basic_receiver` and use it as a known-good test sketch first.

Recommended first test:

1. Wire only the ESP32 and receiver.
2. Flash `examples/basic_receiver`.
3. Open the serial monitor.
4. Confirm `link=1`.
5. Move sticks and switches on the transmitter.
6. Confirm channel values change before adding any motors, servos, or robot logic.

```cpp
#include <Arduino.h>
#include <Esp32CrsfReader.h>

Esp32CrsfReader crsf;

void setup() {
  Serial.begin(115200);
  crsf.begin(Serial2, 16, 17);
}

void loop() {
  crsf.update();

  if (crsf.linkAlive()) {
    int throttleUs = crsf.channelUs(2);  // zero-based channel index
    float roll = crsf.channelNormalized(0);
  }
}
```

## Example

Open the included example project:

```text
examples/basic_receiver
```

Build it:

```powershell
pio run
```

Open the serial monitor:

```powershell
pio device monitor
```

The monitor prints link state, accepted frame count, and all 16 channel values.

## More Guides

- [docs/getting-started.md](docs/getting-started.md) - step-by-step first test.
- [docs/wiring.md](docs/wiring.md) - ESP32 and receiver wiring.
- [docs/protocol-notes.md](docs/protocol-notes.md) - what part of CRSF this library implements.
- [docs/troubleshooting.md](docs/troubleshooting.md) - common setup failures.

## API

```cpp
bool begin(HardwareSerial &serial, int rxPin, int txPin, uint32_t baud = 420000);
bool update();
bool update(uint32_t nowMs);
bool linkAlive() const;
bool linkAlive(uint32_t nowMs) const;
int channelUs(uint8_t channelIndex) const;
uint16_t channelRaw(uint8_t channelIndex) const;
float channelNormalized(uint8_t channelIndex) const;
uint32_t frameCount() const;
uint32_t lastFrameMs() const;
```

Channel indexes are zero-based:

- `0` = channel 1
- `1` = channel 2
- ...
- `15` = channel 16

## Tested Context

This was built for an ESP32 DevKit reading an ExpressLRS / CRSF receiver as part of a robot dog controller:

- [CarbonQuadruped-ESP32](https://github.com/Blacksheep909/CarbonQuadruped-ESP32)

## Notes

This is intentionally small and focused. It does not implement the full CRSF ecosystem, telemetry, device discovery, parameter writes, or transmitter backpack features. It is an RC channel reader for projects that need reliable receiver input on ESP32.

No open-source license has been selected yet. Add one before treating this as a reusable public library.
