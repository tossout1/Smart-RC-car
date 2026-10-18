import subprocess, json, re, serial
try:
    ser = serial.Serial('/dev/serial0', 38400, timeout=1)
except serial.SerialException as e:
    print("Error opening serial port:", e)
    ser = None
proc = subprocess.Popen(["evtest", "/dev/input/event11"], stdout=subprocess.PIPE, text=True, bufsize=1)
pattern = r"Event:\s+time\s+([\d\.]+),\s+(?:type\s+(\d+)\s+\(([^)]+)\),\s+code\s+(\d+)\s+\(([^)]+)\),\s+value\s+(-?\d+)|-+\s+(SYN_REPORT)\s+-+)"

for line in proc.stdout:

    match = re.search(pattern, line)
    if match is None:
        pass
    elif match.group(7) is not None:
        pass
    elif match.group(1):
        timestamp = match.group(1)
        event_type = match.group(2)
        event_type_name = match.group(3)
        event_code = match.group(4)
        event_code_name = match.group(5)
        event_value = match.group(6)

        payload_bytes = (json.dumps({event_code_name: int(event_value)})+"\n").encode('utf-8')
        if ser:
            ser.write(payload_bytes)
            ser.flush()
        print("Sent data to serial port:", payload_bytes)
