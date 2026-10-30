# Weather_Station


An embedded environmental monitoring and alarm system built in C++ for the Grove Kit / Arduino (`SSD1306` OLED, `DHT20`, and `BMP280` sensors). Developed as the lead module for a five-person engineering project at Aston University.

## Key Engineering Features
* **Live Hardware Threshold Recalibration:** Uses 10-bit ADC reads (`analogRead` on `A0`) with linear interpolation (`15.0 + (potValue / 1023.0) * 25.0`) to let users tune the temperature alarm trigger dynamically between `15.0°C` and `40.0°C` on the OLED screen during testing without reflashing firmware.
  
* **Multi-Sensor I2C Integration:** Polls real-time temperature and humidity from the `DHT20` sensor alongside atmospheric pressure from the `Seeed_BMP280` barometer over `Wire` (I2C).
  
* **OLED UI & Unit Scaling:** Formats real-time telemetry onto a `128x64` `SSD1306` I2C display (`U8x8lib`), converting raw Pascal barometer readings into hectopascals (`hPa`) to prevent line-wrapping artifacts on the 8x8 character grid.
* **Cross-Architecture Serial Debugging:** Uses preprocessor macros (`ARDUINO_ARCH_AVR` / `ARDUINO_ARCH_SAMD`) to map serial diagnostics cleanly across AVR and SAMD boards.
