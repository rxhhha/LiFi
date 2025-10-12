# LiFi Prototype
## Description
Experimental Li-Fi system using Arduino for optical wireless data transmission.

---

## Materials
- 2 Arduino boards — one for transmitter, one for receiver  
- 2 Breadboards  
- Transmitter: [Super bright white LEDs](https://www.adafruit.com/product/754)
- Receiver: [Solar panel](https://www.adafruit.com/product/700)  
- Wires 

---

## Protocol
- **Default state:** HIGH  
- **Threshold:** Set dynamically at startup based on ambient lighting  
- **Setup:** Delay at the beginning to ignore setup noise  

**Encoding schemes:**
- **Non-Return-to-Zero (NRZ):** HIGH = 1, LOW = 0  
- **Manchester Encoding:** HIGH → LOW = 1, LOW → HIGH = 0  

---

## Experiments

### 1. Arduino-LED-Solar NRZ
- **Transmitter:** 3 white LEDs  
- **Receiver:** Solar panel  
- **Protocol:** NRZ on-off  
  - Sample in the middle (`sampling time * 1.5`)  
- **Setup:** Threshold 150; box 25cm × 17cm; transmitter and receiver diagonally placed, 20cm apart  
- **Performance:** Smallest reliable sampling time: 4ms; 3ms works with some errors  

### 2. Arduino-LED-Solar Manchester
- **Setup:** Same as above  
- **Protocol:** Manchester  
  - 1: HIGH → LOW (first half of bit)  
  - 0: LOW → HIGH (first half of bit)  
  - Receiver samples after `< sampling time * 1.5`  
- **Threshold:** ~0.6 × LDR value when LED is ON  
  - More LEDs → larger optimal threshold range  
- **Performance:** Smallest reliable sampling time: 2ms

### 3. Arduino-LED-Solar 4B/5B
- **Setup:** Same as above
- **Protocol:** 4B/5B encoding + NRZ  
  - 4-bit nibbles mapped to 5-bit codes (see table below)
  
| 4B Data | 5B Code |
|---------|---------|
| 0000    | 11110   |
| 0001    | 01001   |
| 0010    | 10100   |
| 0011    | 10101   |
| 0100    | 01010   |
| 0101    | 01011   |
| 0110    | 01110   |
| 0111    | 01111   |
| 1000    | 10010   |
| 1001    | 10011   |
| 1010    | 10110   |
| 1011    | 10111   |
| 1100    | 11010   |
| 1101    | 11011   |
| 1110    | 11100   |
| 1111    | 11101   |
- **Performance:** Smallest reliable sampling time: 2ms
- **TODO:** Implement NRZI and use control signals




---

## TODO
- Implement **error detection/correction** (e.g., parity bits, checksums)
- Explore other signal encoding methods: color, brightness, timing, on-off combinations  
- Transmitter improvements:
  - Use laser
  - Different colors and brightness levels  
- Receiver improvements:
  - Replace solar panel with phototransistor for higher sensitivity  
- Optimize threshold calculation and sampling time for new setups  

---

## References
- https://charliegerard.dev/blog/lifi-sending-data-light/
- https://www.irjet.net/archives/V12/i4/IRJET-V12I4197.pdf
- https://electronicsworkshops.com/2024/02/09/data-transmission-through-li-fi-using-arduino/#google_vignette
- https://www.instructables.com/LiFi-Experiment-Audio-Transmission-Through-Light/
- https://ieeexplore.ieee.org/document/7916619
- https://www.scirp.org/journal/paperinformation?paperid=80019