# Troubleshooting

## `link=0`

The ESP32 is not receiving valid fresh CRSF frames.

Check:

- Receiver TX is wired to ESP32 GPIO 16.
- Receiver ground and ESP32 ground are connected.
- Receiver is powered.
- Receiver is bound to the transmitter.
- Receiver output protocol is CRSF.
- Baud is 420000.
- You are using `Serial2`, not the USB serial port.

## `frames=` Does Not Increase

Likely causes:

- Wrong RX pin.
- Receiver is outputting another protocol.
- Receiver is not bound.
- Signal and ground are not referenced together.

Try swapping to another ESP32 hardware serial RX pin and update:

```cpp
crsf.begin(Serial2, NEW_RX_PIN, 17);
```

## Values Stay Near 1500

Check:

- Move sticks while watching serial output.
- Try switches too; some radios map switches to higher channels.
- Confirm your transmitter model is actually sending channels.
- Confirm receiver output is live and not in failsafe.

## Values Move But Channels Are Not Where Expected

Channel order depends on the transmitter model and mapping.

Use the example to discover your mapping:

1. Move one stick axis at a time.
2. Write down which printed channel changes.
3. Flip one switch at a time.
4. Update your project constants to match your radio.

## Upload Fails

Check:

- Correct USB cable.
- Correct board/port in PlatformIO.
- No serial monitor is already holding the port.
- ESP32 boot button if your board requires manual boot mode.

## Serial Monitor Is Blank

Check:

- Monitor speed is 115200.
- ESP32 reset after upload.
- USB cable supports data.
- You opened the example project folder, not the repository root.

