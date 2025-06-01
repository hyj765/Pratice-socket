import subprocess
import threading

proc = subprocess.Popen(
    ["python", "server.py"],
    stdout=subprocess.PIPE,
    stderr=subprocess.PIPE,
    text=True
)
print("Server Start ====================")

def stream_reader(stream, prefix):
    for line in stream:
        print(f"[{prefix}] {line.strip()}")

threading.Thread(target=stream_reader, args=(proc.stdout, "SERVER"), daemon=True).start()
threading.Thread(target=stream_reader, args=(proc.stderr, "ERROR"), daemon=True).start()

try:
    proc.wait()
except KeyboardInterrupt:
    print("Terminating server...")
    proc.terminate()
    proc.wait()
