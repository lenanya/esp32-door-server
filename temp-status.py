#!/bin/python

import requests
from sys import argv
from time import sleep 

def get_temp():
    while True:
        try: 
           with open("/var/www/arduino/temp", "r") as f:
               temp = f.read()
               return temp
        except Exception:
            continue
        
if len(argv) < 2:
    print("token dumbass")
    
token = argv[1]

url = "https://discord.com/api/v10/users/@me/settings"
headers = {"Authorization": token, "Content-Type": "application/json"}

while True:
    temp = get_temp()
    payload = {"custom_status": {"text": f"My room is {temp}°C rn lol"}}
    try:
        resp = requests.patch(url=url, headers=headers, json=payload)
    except Exception:
        continue
    print(resp.status_code)
    sleep(20)
    