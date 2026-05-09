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

# 5.x Phân tích cảm biến nhiệt độ và độ ẩm DHT11

## Thông số kỹ thuật DHT11

| Thông số              | Giá trị             |
| --------------------- | ------------------- |
| Điện áp hoạt động     | 3V – 5V             |
| Dòng tiêu thụ         | ~0.3mA khi đo       |
| Giao tiếp             | Single-wire digital |
| Dải đo nhiệt độ       | 0 – 50°C            |
| Sai số nhiệt độ       | ±2°C                |
| Độ phân giải nhiệt độ | 1°C                 |
| Dải đo độ ẩm          | 20 – 90%RH          |
| Sai số độ ẩm          | ±5%RH               |
| Độ phân giải độ ẩm    | 1%RH                |
| Chu kỳ lấy mẫu        | ≥ 1 giây            |

---

# Nguyên lý đo nhiệt độ và độ ẩm

## Nguyên lý đo nhiệt độ

DHT11 đo nhiệt độ bằng nhiệt điện trở NTC (Negative Temperature Coefficient).

NTC là điện trở có:

* nhiệt độ tăng → điện trở giảm
* nhiệt độ giảm → điện trở tăng

Bên trong DHT11:

1. NTC tiếp xúc môi trường
2. Giá trị điện trở thay đổi theo nhiệt độ
3. IC nội bộ đo sự thay đổi điện trở
4. Bộ ADC chuyển đổi tín hiệu analog sang digital
5. Giá trị nhiệt độ được xử lý và lưu thành dữ liệu số

Do đó vi điều khiển không cần đọc analog trực tiếp mà chỉ cần nhận dữ liệu digital từ DHT11.

---

## Nguyên lý đo độ ẩm

DHT11 sử dụng cảm biến độ ẩm kiểu điện dung.

Cấu trúc gồm:

* hai điện cực
* lớp vật liệu hút ẩm ở giữa

Khi độ ẩm không khí thay đổi:

* lượng hơi nước hấp thụ thay đổi
* hằng số điện môi thay đổi
* điện dung cảm biến thay đổi

IC nội bộ sẽ:

1. đo điện dung
2. xử lý tín hiệu
3. chuyển đổi sang dữ liệu số

---

# Kiến trúc bên trong DHT11

DHT11 tích hợp:

* cảm biến nhiệt độ NTC
* cảm biến độ ẩm điện dung
* ADC
* bộ xử lý 8-bit
* bộ nhớ calibration

DHT11 đã được hiệu chuẩn sẵn từ nhà máy nên:

* dễ sử dụng
* không cần hiệu chỉnh thêm

Tuy nhiên:

* độ chính xác không cao
* tốc độ phản hồi chậm hơn cảm biến công nghiệp

---

# Nguyên lý giao tiếp giữa MCU và DHT11

DHT11 sử dụng giao thức Single-Wire.

Chỉ cần:

* 1 chân DATA
* 1 điện trở kéo lên (pull-up)

Đường truyền ở trạng thái bình thường luôn ở mức HIGH.

---

# Quá trình truyền nhận dữ liệu

## Bước 1: MCU gửi Start Signal

Để bắt đầu giao tiếp:

1. MCU kéo DATA xuống LOW ít nhất 18ms
2. Sau đó kéo HIGH khoảng 20–40us

Mục đích:

* đánh thức DHT11 khỏi chế độ tiết kiệm năng lượng
* yêu cầu gửi dữ liệu

Trong chương trình:

```text
LOW khoảng 20ms
HIGH khoảng 30us
```

---

# Bước 2: DHT11 phản hồi

Sau khi nhận Start Signal:

DHT11:

1. kéo DATA LOW khoảng 80us
2. kéo HIGH khoảng 80us

Mục đích:

* xác nhận đã nhận yêu cầu
* chuẩn bị gửi dữ liệu

Nếu MCU không nhận được phản hồi này:

* cảm biến lỗi
* hoặc dây kết nối sai

---

# Bước 3: DHT11 gửi dữ liệu 40-bit

DHT11 gửi tổng cộng 40 bit:

| Byte   | Nội dung           |
| ------ | ------------------ |
| Byte 1 | Độ ẩm nguyên       |
| Byte 2 | Độ ẩm thập phân    |
| Byte 3 | Nhiệt độ nguyên    |
| Byte 4 | Nhiệt độ thập phân |
| Byte 5 | Checksum           |

Ví dụ:

```text
00110010 = 50 (%RH)
00011001 = 25 (°C)
```

---

# Nguyên lý truyền bit

Mỗi bit đều bắt đầu bằng:

* mức LOW khoảng 50us

Sau đó:

* thời gian HIGH quyết định bit 0 hay bit 1

| Bit | Thời gian HIGH |
| --- | -------------- |
| 0   | ~26–28us       |
| 1   | ~70us          |

---

# Cách MCU đọc bit

MCU thực hiện:

1. chờ cạnh lên HIGH
2. delay khoảng 40us
3. đọc trạng thái chân DATA

Nếu sau 40us:

* DATA vẫn HIGH → bit 1
* DATA đã LOW → bit 0

Đây là kỹ thuật:

* đo độ rộng xung (pulse width detection)
* bằng software timing

---

# Kiểm tra dữ liệu

Byte cuối cùng là checksum:

```text
Checksum = Byte1 + Byte2 + Byte3 + Byte4
```

MCU cộng 4 byte đầu:

* nếu bằng byte checksum → dữ liệu hợp lệ
* ngược lại → lỗi truyền

---

# Nguyên lý hoạt động tổng quát

1. DHT11 đo nhiệt độ bằng NTC
2. DHT11 đo độ ẩm bằng cảm biến điện dung
3. IC nội bộ chuyển đổi analog → digital
4. MCU gửi Start Signal
5. DHT11 phản hồi
6. DHT11 truyền dữ liệu 40-bit
7. MCU đo timing xung để giải mã dữ liệu
8. Giá trị nhiệt độ và độ ẩm được hiển thị lên LED 7 đoạ

# 5.x Phân tích module hiển thị TM1637

## Thông số kỹ thuật TM1637

| Thông số           | Giá trị            |
| ------------------ | ------------------ |
| Điện áp hoạt động  | 3.3V – 5V          |
| Giao tiếp          | 2-wire serial      |
| Số chân điều khiển | 2 chân             |
| Hỗ trợ LED         | LED 7 đoạn 4 digit |
| Điều khiển độ sáng | 8 mức              |
| IC điều khiển      | TM1637             |
| Kiểu hiển thị      | Multiplex scanning |

---

# Nguyên lý hiển thị LED 7 đoạn

LED 7 đoạn gồm:

* 7 LED ký hiệu a,b,c,d,e,f,g
* 1 LED dấu chấm (DP)

Mỗi số được tạo bằng cách bật/tắt các segment tương ứng.

Ví dụ:

| Số | Segment bật |
| -- | ----------- |
| 0  | a,b,c,d,e,f |
| 1  | b,c         |
| 2  | a,b,d,e,g   |

Ví dụ số 0:

```text id="5d4w9u"
a b c d e f ON
g OFF
```

---

# Vai trò của TM1637

Nếu điều khiển LED trực tiếp:

* MCU phải tự quét từng LED
* cần nhiều GPIO
* phải tạo timing liên tục

TM1637 giải quyết bằng cách:

* tích hợp IC driver
* tự quét LED tốc độ cao
* tự điều khiển độ sáng

MCU chỉ cần:

* gửi dữ liệu segment.

---

# Nguyên lý Multiplex Scanning

TM1637 không bật toàn bộ LED cùng lúc.

IC sẽ:

1. bật digit thứ nhất
2. xuất segment tương ứng
3. tắt digit thứ nhất
4. bật digit thứ hai
5. lặp lại liên tục

Tốc độ quét rất nhanh:

* vài trăm Hz

Do hiện tượng lưu ảnh của mắt:

* người dùng thấy LED sáng liên tục.

---

# Nguyên lý giao tiếp TM1637

TM1637 sử dụng giao tiếp serial 2 dây:

* CLK
* DIO

Giao thức gần giống I2C nhưng là giao thức riêng.

---

# Trạng thái idle

Bình thường:

```text id="sxph3t"
CLK = HIGH
DIO = HIGH
```

---

# Start Condition

MCU bắt đầu truyền bằng:

1. giữ CLK HIGH
2. kéo DIO từ HIGH xuống LOW

Điều này báo cho TM1637:

* chuẩn bị nhận dữ liệu.

---

# Nguyên lý truyền dữ liệu

Dữ liệu gửi từng bit:

* bit thấp trước (LSB first)

Mỗi bit:

1. MCU kéo CLK LOW
2. đặt giá trị DIO
3. delay khoảng 3us
4. kéo CLK HIGH

TM1637 đọc dữ liệu tại cạnh clock.

Trong chương trình:

```text id="7db6h0"
CLK LOW
Đặt DIO
delay ~3us
CLK HIGH
```

---

# ACK Response

Sau mỗi byte:

1. MCU nhả chân DIO
2. TM1637 kéo LOW để ACK

ACK cho biết:

* dữ liệu nhận thành công.

---

# Dữ liệu hiển thị

MCU gửi:

* mã segment cho từng digit

Ví dụ:

| Giá trị HEX | Số |
| ----------- | -- |
| 0x3F        | 0  |
| 0x06        | 1  |
| 0x5B        | 2  |

TM1637:

* lưu dữ liệu vào RAM nội bộ
* tự điều khiển LED hiển thị.

---

# Quá trình hiển thị thời gian

1. MCU đọc giờ từ DS1302
2. Tách thành:

   * hàng chục
   * hàng đơn vị
3. Chuyển sang mã segment
4. Gửi dữ liệu sang TM1637
5. TM1637 quét LED liên tục
6. LED hiển thị:

```text id="ll6pmo"
HH:MM
```

---

# Ưu điểm của TM1637

* Chỉ cần 2 chân GPIO
* Không cần tự quét LED bằng software
* Giảm tải xử lý cho MCU
* Hiển thị ổn định
* Dễ lập trình

---

---

# 5.x Phân tích module RTC DS1302

## Thông số kỹ thuật DS1302

| Thông số          | Giá trị               |
| ----------------- | --------------------- |
| Điện áp hoạt động | 2V – 5.5V             |
| Giao tiếp         | 3-wire serial         |
| Chân giao tiếp    | CE, SCLK, IO          |
| Dao động ngoài    | 32.768kHz             |
| Backup battery    | Có                    |
| Dữ liệu thời gian | Giây, phút, giờ, ngày |
| Dạng dữ liệu      | BCD                   |

---

# Nguyên lý hoạt động RTC

DS1302 là mạch Real-Time Clock (RTC).

Chức năng:

* lưu thời gian thực
* đếm giây/phút/giờ liên tục
* hoạt động ngay cả khi MCU tắt

---

# Nguyên lý tạo thời gian

DS1302 sử dụng:

* thạch anh 32.768kHz

Tần số này được chọn vì:

```text id="n7egtm"
32768 = 2^15
```

Mạch chia tần nội bộ:

* chia 32768Hz xuống 1Hz

Kết quả:

* mỗi xung tương ứng 1 giây.

DS1302 tự động:

* tăng giây
* tăng phút
* tăng giờ

giống đồng hồ điện tử thực tế.

---

# Vai trò pin backup

DS1302 có chân pin backup.

Khi mất nguồn chính:

* RTC vẫn được cấp nguồn
* thời gian vẫn tiếp tục chạy

Do đó:

* đồng hồ không bị reset thời gian.

---

# Nguyên lý giao tiếp DS1302

DS1302 dùng giao tiếp 3 dây:

| Chân | Chức năng            |
| ---- | -------------------- |
| CE   | Enable communication |
| SCLK | Clock                |
| IO   | Data                 |

---

# Vai trò từng chân

## CE (Chip Enable)

* kích hoạt giao tiếp
* HIGH → bắt đầu truyền
* LOW → kết thúc truyền

---

## SCLK

MCU tạo xung clock:

* đồng bộ dữ liệu

Mỗi xung:

* truyền hoặc nhận 1 bit.

---

## IO

* truyền dữ liệu 2 chiều
* vừa đọc vừa ghi.

---

# Nguyên lý ghi dữ liệu

MCU:

1. kéo CE HIGH
2. gửi byte địa chỉ
3. gửi byte dữ liệu
4. tạo xung SCLK

DS1302 đọc dữ liệu tại cạnh clock.

Trong chương trình:

```text id="2m5k6e"
SCLK HIGH ~2us
SCLK LOW ~2us
```

Dữ liệu truyền:

* bit thấp trước (LSB first).

---

# Nguyên lý đọc dữ liệu

MCU:

1. gửi lệnh đọc
2. chuyển IO sang input
3. tạo xung clock
4. đọc từng bit từ DS1302

DS1302 xuất:

* dữ liệu giờ
* phút
* giây

ra chân IO.

---

# Dữ liệu dạng BCD

DS1302 lưu dữ liệu dạng BCD.

Ví dụ:

```text id="nd9q9t"
0x25 = 25
```

Trong đó:

* 0010 = 2
* 0101 = 5

MCU cần chuyển:

```text id="kdzl0t"
BCD → Decimal
```

để hiển thị.

---

# Quá trình đọc thời gian

1. MCU gửi lệnh đọc giờ/phút
2. DS1302 trả dữ liệu BCD
3. MCU chuyển sang decimal
4. MCU gửi dữ liệu sang TM1637
5. TM1637 hiển thị lên LED

---

# Vì sao DS1302 giữ thời gian chính xác?

Vì:

* dùng dao động thạch anh 32.768kHz
* có bộ chia tần phần cứng
* hoạt động độc lập với MCU
* có pin backup

MCU chỉ đóng vai trò:

* đọc dữ liệu
* hiển thị thời gian.
