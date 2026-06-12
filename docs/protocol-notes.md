# Protocol Notes

This project only implements the CRSF RC channel reader path needed by typical ESP32 robot / vehicle projects.

## Serial

Default CRSF serial settings:

- Baud: `420000`
- Format: `8N1`
- ESP32 port: usually `Serial2`

## RC Channel Frames

The reader accepts CRSF type `0x16`, which contains packed RC channel data.

The payload is 22 bytes:

- 16 channels.
- 11 bits per channel.
- Little-endian bit packing across the payload.

The library decodes those values into:

- Raw CRSF channel values.
- 1000-2000 us style values.
- Normalized `-1.0` to `+1.0` values.

## CRC

Accepted frames must pass CRSF's DVB-S2 CRC8 check using polynomial `0xD5`.

The CRC covers the frame type plus payload bytes, not the address or length byte.

## Scope

Implemented:

- RC channel frame parsing.
- CRC validation.
- Channel unpacking.
- Link timeout.

Not implemented:

- Telemetry.
- Device discovery.
- Parameter writes.
- Backpack features.
- Full CRSF command support.

