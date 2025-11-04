#include "cinder/packet.h"

namespace {

void put_u16(uint8_t* dest, uint16_t value) {
  dest[0] = static_cast<uint8_t>(value >> 8);
  dest[1] = static_cast<uint8_t>(value);
}

uint16_t get_u16(const uint8_t* src) {
  return static_cast<uint16_t>((src[0] << 8) | src[1]);
}

}  // namespace

bool sample_ok(const ClimateSample& sample) {
  if (sample.temp_c_x100 < -4000 || sample.temp_c_x100 > 8500) return false;
  if (sample.humidity_x100 > 10000) return false;
  if (sample.co2_ppm < 400 || sample.co2_ppm > 5000) return false;
  if (sample.battery_mv < 2500 || sample.battery_mv > 4500) return false;
  return true;
}

bool encode_packet(const ClimateSample& sample, uint16_t node, ClimatePacket* out) {
  if (out == nullptr || !sample_ok(sample)) return false;
  out->bytes[0] = 1;
  out->bytes[1] = sample.battery_mv < 3300 ? 0x01 : 0x00;
  put_u16(out->bytes + 2, node);
  put_u16(out->bytes + 4, static_cast<uint16_t>(sample.temp_c_x100));
  put_u16(out->bytes + 6, sample.humidity_x100);
  put_u16(out->bytes + 8, sample.co2_ppm);
  put_u16(out->bytes + 10, sample.battery_mv);
  return true;
}

bool decode_packet(const uint8_t* bytes, size_t length, ClimateSample* sample, uint16_t* node) {
  if (bytes == nullptr || sample == nullptr || node == nullptr || length != 12) return false;
  if (bytes[0] != 1) return false;
  *node = get_u16(bytes + 2);
  sample->temp_c_x100 = static_cast<int16_t>(get_u16(bytes + 4));
  sample->humidity_x100 = get_u16(bytes + 6);
  sample->co2_ppm = get_u16(bytes + 8);
  sample->battery_mv = get_u16(bytes + 10);
  return sample_ok(*sample);
}
