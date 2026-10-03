# Smart RC Car

This project is an experimental RC car controller built around a Linux host computer, a Python joystick listener, and Arduino-based signal processing for steering and throttle control.

The current codebase is a prototype and includes both the Linux-side input capture and several Arduino sketches for testing RC pulse widths and servo/ESC output behavior.

## Overview

The system captures gamepad or controller input from Linux input devices, translates the relevant axes into serial messages, and sends those values to an Arduino. The Arduino then interprets the incoming data and drives steering and throttle outputs.

The project is designed around the idea of using a standard controller to drive an RC car by reading joystick axes and converting them into RC-compatible PWM signals.

## Repository contents

- `main.py` – Python script that reads Linux input events, filters the relevant controller axes, and sends JSON payloads over serial.
- `ESC.h` – Arduino helper for reading throttle pulse width from an RC signal and exposing it as a value.
- `sketch_sep06a/` – experimental Arduino sketch and headers for steering/throttle signal handling.
- `events.txt` – captured Linux input event metadata for the controller.
- `steering.txt` – notes on steering signal ranges and calibration.
- `text.txt` – additional notes or prototype references.

## Hardware and software assumptions

- A Linux system with access to joystick/gamepad input events.
- `evtest` installed for reading raw input events.
- Python 3 with `pyserial` installed.
- Arduino-compatible board (for example Uno/Nano) connected over USB serial.
- RC steering servo and ESC or motor controller connected to the Arduino outputs.

## How the system works

### 1. Linux input capture

`main.py` opens a serial connection to the Arduino and reads from a Linux input event device such as `/dev/input/event6`.

It watches for joystick axis events and extracts values like:

- `ABS_X` for steering
- `ABS_Z` and `ABS_RZ` for throttle input

Each incoming event is converted into a small JSON payload such as:

```json
{"ABS_X": 1234}
```

and then written to the serial port.

### 2. Serial communication

The Python script sends the captured values to the microcontroller over a serial link. This lets a controller input device act as the user-facing control layer while the Arduino handles the actuator signaling.

### 3. Arduino decoding and actuation

The Arduino sketches in the repo focus on measuring RC-like pulse widths and mapping them to steering and throttle values. The code uses interrupts to detect signal changes and track the pulse width in microseconds.

The calibration notes in the project suggest ranges such as:

- steering neutral region around `1531–1537`
- left steering around `1537–1984`
- right steering around `1039–1531`
- forward throttle around `1496–1996`
- reverse throttle around `996–1492`

These ranges were used as reference values while tuning the controller mapping.

## Setup

### Install dependencies

On Linux:

```bash
sudo apt-get install evtest
python3 -m pip install pyserial
```

### Configure the serial port

In `main.py`, adjust the serial port and device path as needed:

```python
ser = serial.Serial('COM3', 9600, timeout=1)
proc = subprocess.Popen(["evtest", "/dev/input/event6"], stdout=subprocess.PIPE, text=True, bufsize=1)
```

Use the correct serial port for your Arduino and the correct Linux event device for your controller.

### Flash the Arduino sketch

Open the Arduino sketch in the `sketch_sep06a/` folder and upload it to your board using the Arduino IDE or compatible tooling.

### Run the application

```bash
python3 main.py
```

This starts the joystick listener and sends data to the connected Arduino.

## Notes and caveats

- The repository contains multiple experimental files and sketches, so not all code is unified into one final implementation.
- Input device names can vary across systems; `/dev/input/event6` may need to be changed.
- Serial parameters, pin assignments, and RC signal ranges may need calibration for your specific hardware.
- Some files are notes or prototypes rather than the final software architecture.

## Development direction

This project is best viewed as a foundation for:

- controller-to-vehicle input translation
- Arduino servo and ESC control
- tuning for reliable steering and throttle behavior
- integration of additional safety checks and better input filtering

## Suggested next steps

1. Confirm the correct gamepad event device on the Linux host.
2. Verify the Arduino serial port and baud rate.
3. Test steering and throttle output ranges with a bench setup before driving the car.
4. Standardize one final Arduino sketch and one final control pipeline for the vehicle.

## Parts used
1. [Nintendo Switch Mario Kart Racing Wheel Pro Deluxe](amazon.com/Nintendo-Switch-Mario-Racing-Wheel-Deluxe/dp/B01C6XAO7M/ref=sr_1_4?crid=27JN4RVJPQXOY&dib=eyJ2IjoiMSJ9.QwQ_wC22JDxA7mkjpoeGTrBxq71Mey3iklzPPsX1WZDmUDcVPEvX27Dx9cmsFmB99iQBIQK0ooIkvLY1E5HsiD4uJyCaVny489SvEF4_PmO36gLJm_bxkRSBhJ_VeV_luM38JYKMBMcekRP1AdVdiKtvIugFUpp0CGJzCXRIX2c1y4-QIt2bl2gWsoBbqS_ZPNaYSaJtaWe7MoKyr1n_fS9TAQwxtQiIeooAFeg07Ks.oQHApkT-IBXJOm3bEOz_bws0mSAZ2DuqwkuF7RYYmok&dib_tag=se&keywords=mariokart+steering+wheel&qid=1791060067&sprefix=%2Caps%2C224&sr=8-4)
2. [Traxxas Slash 2WD kit (Prebuilt will also work)](https://www.amazon.com/Traxxas-Slash-2WD-Unassembled-Kit/dp/B0GW9H3BTF/ref=sr_1_9?crid=3N9TGSJGP3CMJ&dib=eyJ2IjoiMSJ9.xPq5Aj29y95n92J0yFttec0UGf8No-oInCZYgdj794xiLh8teZ2rDhx9RvzwFtlO1GyaqhWHtPFfLilTFOaoDBz9pUt9UgdpHLLNwdKfcMdRM33c96S9BmPeqs3c2Nr4aY6wTb61mWuxz5419qcIeBFqRgYyFolb0u773nRiPVnkBBMNuFAXWNxVvNglh6-STg6x7XqfOgZ36PbHrWEpq4NF7-5uyWmAVWSwDXCnZQ9D742OIDtGd09QizTpewj_dUbZllzYp6SS0yKbd9ky9dFENFIvzgmDkIndrzHp0y8.gf3dvIGJKj7a2AqGLLcxNjLF3sKfRDCVyzOeuCBoY-o&dib_tag=se&keywords=Traxxas+slash+2WD&qid=1791060184&sprefix=traxxas+slash+2wd%2Caps%2C202&sr=8-9)
3. [Arduino Uno R3](https://www.amazon.com/Arduino-A000066-ARDUINO-UNO-R3/dp/B008GRTSV6/ref=sr_1_3?crid=2BYFI14TVFY94&dib=eyJ2IjoiMSJ9.MazmhFfn-DF8W5oyX_S-tKdK1Vw_ErfhallV_3kz1V6zHbE7Kl1Enw8IxI_9U5DULZv03zfQbFBrypHp7Um6U9TNvb-PXEBE2NdVsbXFNjyHM72SyMbYfaLibIFztFI27zFJxvobUB6Zv90wW6Es1FkH53O5_CPdgRKMrg7GlbljPq8IF9mrwILKM5__W2X36QNjlXwjrmaxxkScmPj2d5MEXsRqSDI1FtFDOCeqvfA.fxVtOP9CHGBv4YUGoF8u-Py7z3nw_4--wCS8vxzGyh0&dib_tag=se&keywords=Arduino%2BUno%2BR3&qid=1791060443&sprefix=arduino%2Buno%2Br3%2Caps%2C177&sr=8-3&th=1)
4. [Crunchlabs Hackpack battery](https://www.crunchlabs.com/products/power-bank-replacement)
5. [USB-B to USB-A cable](https://www.amazon.com/UGREEN-Printer-Scanner-Brother-Lexmark/dp/B00P0FO1P0/ref=sr_1_3?crid=2RN6G8TFZ8G0Z&dib=eyJ2IjoiMSJ9.0pl8BL_1yUhugdKy06gDexnsBGOFyrPO4UDImETVBiFvfshf4OiAeM_Qyzj4CVjsJJ8q2Ie_uBMGXRLogUpS9duPW8Wg9v7E8Q19x3dSqZM-vZ9fmRNCZ3EdyjAFH09j5HFqkBvqziPNS3JaxRQgWCGrqVmMZYZo-VpOjrWosoDLJ65-6LKFsZsdcCyjtLEa0C07BBcCaEYDRk1cYs6vti0P_GJmYOY7xOdfhwnUmog.29wyJjTL0Tdnq6u8ZpLjTQtZzwoJI2ytTAXx89KRnO4&dib_tag=se&keywords=USB-B%2Bto%2BUSB-A(male%2Bto%2Bmale)&qid=1791060685&sprefix=usb-b%2Bto%2Busb-a%2Bmale%2Bto%2Bmale%2B%2Caps%2C182&sr=8-3&th=1)
6. [Rubber bands](https://www.amazon.com/AmazonBasics-Assorted-Color-Rubber-Bands/dp/B074B1KCXD/ref=sr_1_5?crid=3CDFTWWK3BK68&dib=eyJ2IjoiMSJ9.xe7ZQ2oigo6xfyW59xjigWOsZExFhevjnYeU6I2ByV6wxd_8JleatLyBT7ZOf_GbxmnQXf1FAREaJHUfEwwktj-R2gOi2pCUocT9qernWvX-du2qwDZXXg58OeetPr4S6JHDTtC_hGSps75HqRammnJhbxnxra9b4VzNgNrYeShwoWgNLgOPaMVbU8ObZWPjz7KDDCxEqGFMOAIRUevNUAPyOBf54XoOzyR88e9rmaEr3eZMukzCA08N95QNTi_7VY5HCzOX6fI7XSegs7VkPW5WoS8KcO6dQK9becOLK-U.naFaUGCd7MkskInksw9Pl09RbXI4ujwzBSYCLRPOIds&dib_tag=se&keywords=rubber+bands&qid=1791060761&sprefix=rubbe%2Caps%2C187&sr=8-5)
7. [Raspberry pi 5 8GB RAM](https://www.amazon.com/CanaKit-Raspberry-Starter-Kit-PRO/dp/B0CRSNCJ6Y/ref=sr_1_1?crid=20ZHRHJC1E88B&dib=eyJ2IjoiMSJ9.TzIAlcap-JN7BzO8wHpHCYXA_A6fwtcZpItKBOZUeAoOCRTkM7taiZYZ_w40BmiyBa1ymqUrEuzVze7glNtbevthvbUhBrZWRvBpBRhiI-jqYgb-Ye55_FdmnVizOQztNWAoVdUqm4pwkFS9j7y9gEwy0SJzgjZKD9zsjtPtPakg54YXMMzy3I-mTD7d93zMLRxO30wHLvUDz-EeXTjFJs8ab8DfUktlkFXT2f8dgW4.pzNlFvHKeohJFkgD86IC2y3LjlSWaYFVjpBER3qL4GI&dib_tag=se&keywords=raspberry%2Bpi%2B5%2B8gb%2Bkit&qid=1791061220&sprefix=Raspberry%2Bpi%2B5%2B8GB%2B%2Caps%2C183&sr=8-1&th=1)
8. [GPIO jumper wires](https://www.amazon.com/Elegoo-EL-CP-004-Multicolored-Breadboard-arduino/dp/B01EV70C78/ref=sr_1_1?dib=eyJ2IjoiMSJ9.15YUbS0PBcrafLDYuUDgt86ibtn-A51J--Im62JERL60XnpRZiGWEDIoOBPY8JGij8Cp4ct1pHZxddBDIukhx5qfbvCJG1h4tge-a1Rkkf3bsyEl2gDz7TeGXOU15pWzf30rIcTpGVBSO7KYn_kw82hkVUPHJTy_pWE0hGH365yr8i5IvA8O-iUhygmaM0xqCTvqVCQbE_5URjcegITJ_SqCUur6mS9M19_bf9IMxU0.TJGLv7OVs8cfR-iHaknmZ9OG1VKT8dpDD3cR1IrL0hI&dib_tag=se&keywords=GPIO%2Bjumper%2Bwires&qid=1791060918&sr=8-1&th=1)
9. [2.4GHz wireless transceiver (Part undecided)]
10. [Electrical Tape](amazon.com/Scotch-Super-33-Electrical-carton/dp/B0000AZ9HG/ref=sr_1_3?crid=2JTLAA5QR8CES&dib=eyJ2IjoiMSJ9.BbgJSeuFNdNT_y9SQi8UudiWY_zwOpeaqkfzWjDboMInOnI3tOt9LTMtBE30Uvr144btAkpG0A46h1H4NJiMDxzHYvCEzfqo-MHtwy9aOiUDaHRaalpDUvSbLlPhNS1fVYwIjOWyznVuPRthdnbExy27o6eBPKyNRnd5V_TXx4XVwqA5iUbG4RD2qyB9cLw_Q-4OtNYnjE3AaOZOmE9Q-XAAZDHtCf877_UbwPzK11Y.dTPkE21bBx8QKWwenJFgmZXRvSME8J-JUDuscJfgdT4&dib_tag=se&keywords=electrical+tape&qid=1791062425&sprefix=electrical+%2Caps%2C243&sr=8-3)

## Cirkit Wire Diagram: https://app.cirkitdesigner.com/project/d734c38f-9ae0-46d0-b47e-13db84d90372

## How to build
1. If you bought the Traxxas Slash 2WD kit then assemble it, if not move on to step 2.
2. Follow the Raspberry Pi operating system installation wizard and select Raspberry Pi OS (64-bit)
3. On your raspberry pi 5 OS open a terminal and run ```bash
git clone https://github.com/tossout1/Smart-RC-car```
4. Using the Arduino IDE open the folder "sketch_sep06a" and click upload
5. Using the wire diagram in Cirkit wire the jumper wires to their designated spots (Put the reciever vertical as leaving it horizontal can cause glitches)
6. Attatch the Arduino on top of the battery latch using rubber bands
7. Attatch the Crunchlabs battery on the very front of the RC car using rubber bands
8. Using the USB-B to USB-A cable connect the arduino and the crunchlabs battery to power the Arduino (To turn off unplug the USB-B cable from the Arduino)
9. wrap the jumper wires where they meet the ESC and steering servo cables in electrical tape

## How to run
1. On the Raspberry Pi 5 open a terminal and run ```bash
python main.py```
2. Connect the USB-B cable to the arduino
3. Plug in the LIPO or NIMH battery to your ESC
4. Click the ESC power button

## How to turn off
1. Click the ESC power button until the green light disappears
2. Unplug the battery from the ESC
3. Unplug the USB-B cable from the Arduino 
4. In the terminal you ran the run command click Ctrl+C on your keyboard

# IMPORTANT
1. Due to occasional glitches a few seconds after the ESC is powered occasionally the motor may spin automatically but only once in that power cycle. To prevent this from ruining anything simply hold the rear bumper off the ground so the tires are off the ground and wait 10 seconds to make sure a glitch doesn't happen.
2. If the controller is turned off while there are signals the motor will start slowing down to a stop and the steering will stay at it's last position.