import time
import requests
import subprocess

TOKEN = ""
BASE  = "https://blynk.cloud/external/api"

PIN_SPOTIFY = 0   # V0
PIN_YT      = 1   # V1
PIN_STATUS  = 4   # V4

COOLDOWN_S  = 1.5
_last = {"spotify": 0.0, "youtube": 0.0}

def blynk_get(pin: int):
    r = requests.get(f"{BASE}/get?token={TOKEN}&V{pin}", timeout=8)
    t = r.text.strip().strip('"').strip()
    return t if t else None

def blynk_update(pin: int, value: str):
    requests.get(f"{BASE}/update?token={TOKEN}&V{pin}={value}", timeout=8)

def push_status(msg: str):
    blynk_update(PIN_STATUS, msg)

def can_run(key: str):
    now = time.time()
    if now - _last[key] < COOLDOWN_S:
        return False
    _last[key] = now
    return True

def open_spotify():
    if not can_run("spotify"): return
    subprocess.run('cmd /c start "" "spotify:"', shell=True)
    push_status("Spotify dibuka")

def open_youtube():
    if not can_run("youtube"): return
    subprocess.run('cmd /c start "" "https://www.youtube.com"', shell=True)
    push_status("YouTube dibuka")

print("Agent siap — V0=Spotify, V1=YouTube (Toggle).")
push_status("Agent mulai")

while True:
    v0 = blynk_get(PIN_SPOTIFY) 
    v1 = blynk_get(PIN_YT)
    print(f"DBG V0={v0} V1={v1}", end="\r")

    if v0 == "1":
        open_spotify()
        blynk_update(PIN_SPOTIFY, "0")

    if v1 == "1":
        open_youtube()
        blynk_update(PIN_YT, "0")

    time.sleep(0.25)
