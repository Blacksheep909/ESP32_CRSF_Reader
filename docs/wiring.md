# Wiring

Typical receive-only wiring for an ESP32 DevKit and CRSF / ExpressLRS receiver:

| Receiver | ESP32 |
| --- | --- |
| TX | GPIO 16 / RX2 |
| GND | GND |
| Power | Receiver-appropriate supply |

The example uses GPIO 16 for receive and GPIO 17 for transmit:

```cpp
crsf.begin(Serial2, 16, 17);
```

For most RC channel reading, the ESP32 only needs to receive data from the receiver. TX is still configured because the ESP32 `HardwareSerial::begin()` API takes both pins, and future telemetry work may need the return line.

Make sure the receiver and ESP32 share ground. If the receiver is powered from a different regulator or BEC, common ground is still required for the serial signal to be meaningful.

