#pragma once

#include <stddef.h>
#include <stdint.h>

struct ClimateSample {
  int16_t temp_c_x100;
  uint16_t humidity_x100;
  uint16_t co2_ppm;
  uint16_t battery_mv;
};

struct ClimatePacket {
  uint8_t bytes[12];
};

bool sample_ok(const ClimateSample& sample);
bool encode_packet(const ClimateSample& sample, uint16_t node, ClimatePacket* out);
bool decode_packet(const uint8_t* bytes, size_t length, ClimateSample* sample, uint16_t* node);
