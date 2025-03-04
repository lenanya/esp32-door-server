#!/bin/python

from sys import argv
from time import time 
import json

def stohms(seconds):
    if seconds >= 3600:
        hours = seconds // 3600
        minutes = (seconds % 3600) // 60
        seconds = seconds - hours * 3600 - minutes * 60
        return f"{hours}h {minutes}m {seconds}s"
    elif seconds >= 60:
        hours = seconds // 3600
        minutes = (seconds % 3600) // 60
        seconds = seconds - hours * 3600 - minutes * 60
        return f"{minutes}m {seconds}s"
    else:
        hours = seconds // 3600
        minutes = (seconds % 3600) // 60
        seconds = seconds - hours * 3600 - minutes * 60
        return f"{seconds}s"

current_time = time()

data = json.loads(argv[1])

data_new = []

for i in data:
    data_new.append({'time': i['time'], 'difference': i['difference'], 'since': stohms(int((current_time - i['unix']) // 1))})
    
print(json.dumps(data_new))