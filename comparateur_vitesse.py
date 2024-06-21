

import sys, os, subprocess
name1 = sys.argv[1]
name2 = sys.argv[2]

first = []
second = []

def run_and_get(name):
    pipe = subprocess.Popen([f"time ./{name} > /dev/null"], shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    _, err = pipe.communicate()
    r = err.decode("utf-8")
    time = r.split()[1]
    time = time.split("m")
    time[1] = time[1][:-1]
    return float(time[0]) * 60 + float(time[1])

for i in range(100):
    first.append(run_and_get(name1))
    second.append(run_and_get(name2))
    
print(f"{name1} : {sum(first)/len(first):.6f}")
print(f"{name2} : {sum(second)/len(second):.6f}")