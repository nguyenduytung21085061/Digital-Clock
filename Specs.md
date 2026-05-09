# 1. Phân tích lựa chọn hiển thị LED

| Thiết bị | Giá  | Đặc điểm                  | Giao tiếp         | Tốc độ     | Kết luận                  |
| -------- | ---- | ------------------------- | ----------------- | ---------- | ------------------------- |
| TM1637   | ~16k | Có sẵn module LED 4 digit | 2 chân (CLK, DIO) | Trung bình | Dễ dùng, ít chân          |
| MAX7219  | ~30k | Chỉ IC driver             | 3 chân (SPI-like) | Cao        | Điều khiển được nhiều LED |
| 74HC595  | ~6k  | Shift register            | 3 chân            | Cao        | Rẻ nhưng cần tự quét LED  |

👉 Chọn TM1637 vì:

* có sẵn module
* ít dây
* phù hợp hiển thị đồng hồ HH:MM
* giảm tải xử lý quét LED cho MCU

---

# 2. Phân tích lựa chọn cảm biến nhiệt độ

| Thiết bị | Giá  | Dải đo    | Giao tiếp     | Cách đọc      | Kết luận        |
| -------- | ---- | --------- | ------------- | ------------- | --------------- |
| DHT11    | ~18k | 0–50°C    | 1-wire custom | Timing pulse  | Rẻ, đủ dùng     |
| DHT22    | ~95k | -40–80°C  | 1-wire custom | Timing pulse  | Chính xác cao   |
| LM75     | ~35k | -55–125°C | I2C           | Đọc thanh ghi | Công nghiệp hơn |

👉 Chọn DHT11 vì:

* giá thấp
* có thêm độ ẩm
* đủ cho nhiệt độ môi trường
* phù hợp đề tài cơ bản

---

# 3. Phân tích lựa chọn MCU

| MCU            | Giá       | GPIO     | Tần số | Đặc điểm      | Kết luận           |
| -------------- | --------- | -------- | ------ | ------------- | ------------------ |
| ATtiny2313     | ~36k      | ~18 chân | 20MHz  | MCU nhỏ gọn   | Đủ yêu cầu         |
| STM32F103      | ~60k      | Nhiều    | 72MHz  | ARM Cortex-M3 | Mạnh hơn cần thiết |
| ESP32          | ~90k–120k | Nhiều    | 240MHz | Wi-Fi/BLE     | Dư tài nguyên      |
| Arduino Uno R3 | ~100k     | Nhiều    | 16MHz  | Dễ học        | Kích thước lớn     |

👉 Chọn ATtiny2313 vì:

* giá thấp
* đủ GPIO
* phù hợp hệ thống nhỏ
* giúp triển khai giao tiếp bằng GPIO trực tiếp

Câu “GPIO trực tiếp” quan trọng nha.

---

# 4. Phân tích lựa chọn RTC

| Module | Giá  | Giao tiếp | Số chân | Cách đọc    | Kết luận      |
| ------ | ---- | --------- | ------- | ----------- | ------------- |
| DS1302 | ~10k | 3-wire    | 3 chân  | Bit-banging | Rẻ, đơn giản  |
| DS1307 | ~57k | I2C       | 2 chân  | Thanh ghi   | Phổ biến      |
| DS3231 | ~49k | I2C       | 2 chân  | Thanh ghi   | Chính xác cao |

👉 Chọn DS1302 vì:

* chi phí thấp
* dễ triển khai bằng GPIO
* phù hợp yêu cầu clock cơ bản
