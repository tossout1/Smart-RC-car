import subprocess
import re

proc = subprocess.Popen(["evtest", "/dev/input/event6"], stdout=subprocess.PIPE, text=True, bufsize=1)
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

        print(f"Timestamp: {timestamp}, Type: {event_type} ({event_type_name}), Code: {event_code} ({event_code_name}), Value: {event_value}")