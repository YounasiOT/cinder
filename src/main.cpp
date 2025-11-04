#include <Arduino.h>

#include "cinder/packet.h"

namespace {
constexpr uint16_t kNodeId = 1;
constexpr uint64_t kSleepUs = 180ULL * 1000000ULL;
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  ClimateSample sample{2210, 4800, 640, 3920};
  ClimatePacket packet{};
  if (encode_packet(sample, kNodeId, &packet)) {
    Serial.write(packet.bytes, sizeof packet.bytes);
  }
  esp_deep_sleep(kSleepUs);
}
