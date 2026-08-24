import subprocess
import re
import serial
ser = serial.Serial('COM3', 115200, timeout=0)

def read_controls(device):
    pattern = re.compile(
        r"Event: time ([0-9.]+), type ([0-9]+) "
        r"\(([^)]+)\), code ([0-9]+) "
        r"\(([^)]+)\), value (-?[0-9]+)"
    )

    process = subprocess.Popen(
        ["sudo", "evtest", device],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True
    )

    try:
        while True:
            line = process.stdout.readline()

            if not line:
                continue

            match = pattern.search(line)

            if match:
                print(
                    match.group(5),
                    match.group(6),
                    flush=True
                )
                ser.write(f"{match.group(5)},{match.group(6)}\n".encode())
                ser.flush()

    except KeyboardInterrupt:
        process.terminate()


read_controls("/dev/input/event11")