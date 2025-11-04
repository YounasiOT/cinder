#include <stdio.h>

#include "cinder/packet.h"

static int failures = 0;

static void expect(bool condition, const char* name) {
  if (condition) {
    printf("ok %s\n", name);
    return;
  }
  printf("FAIL %s\n", name);
  failures += 1;
}

int main() {
  ClimateSample office{2210, 4800, 640, 3920};
  ClimatePacket packet{};
  expect(encode_packet(office, 7, &packet), "encodes an office sample");
  expect(packet.bytes[0] == 1, "version is 1");
  expect(packet.bytes[1] == 0, "a full cell is not low");

  ClimateSample decoded{};
  uint16_t node = 0;
  expect(decode_packet(packet.bytes, 12, &decoded, &node), "decodes the packet");
  expect(node == 7, "keeps the node id");
  expect(decoded.temp_c_x100 == 2210, "keeps the temperature");
  expect(decoded.co2_ppm == 640, "keeps the co2");

  ClimateSample cold{2210, 4800, 640, 3100};
  expect(encode_packet(cold, 7, &packet), "encodes a low cell");
  expect(packet.bytes[1] == 0x01, "marks a cell under 3.3 V");

  ClimateSample bad{2210, 4800, 50, 3920};
  expect(!sample_ok(bad), "rejects a co2 reading below 400");
  expect(!encode_packet(bad, 7, &packet), "refuses to send a bad sample");

  return failures == 0 ? 0 : 1;
}
