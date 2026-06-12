#include <Arduino.h>
#include <Esp32CrsfReader.h>

constexpr int kCrsfRxPin = 16;
constexpr int kCrsfTxPin = 17;
constexpr uint32_t kPrintIntervalMs = 100;

Esp32CrsfReader crsf;
uint32_t lastPrintMs = 0;

void setup() {
  Serial.begin(115200);
  crsf.begin(Serial2, kCrsfRxPin, kCrsfTxPin);

  Serial.println("ESP32 CRSF reader example ready.");
  Serial.println("Receiver TX -> ESP32 GPIO16/RX2, receiver GND -> ESP32 GND.");
}

void loop() {
  crsf.update();

  const uint32_t now = millis();
  if ((now - lastPrintMs) < kPrintIntervalMs) {
    return;
  }
  lastPrintMs = now;

  Serial.printf("link=%d frames=%lu", crsf.linkAlive(now) ? 1 : 0,
                static_cast<unsigned long>(crsf.frameCount()));
  for (uint8_t channel = 0; channel < Esp32CrsfReader::kChannelCount; ++channel) {
    Serial.printf(" ch%u=%d", channel + 1, crsf.channelUs(channel));
  }
  Serial.println();
}

