# Cinder

Cinder is a battery ESP32-C3 that reads temperature, humidity, and CO2, then sleeps for three minutes. Each wake sends a 12-byte packet. A cell under 3.3 V is flagged, and a reading outside the sensor range is not sent.

## Check the packet logic

```bash
mkdir -p build
make test
```

## Flash

```bash
pio run -t upload
pio device monitor
```

The board target is `esp32-c3-devkitm-1`.
