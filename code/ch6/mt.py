import threading
import time

def worker(name):
    time.sleep(120)

threads = []
for i in range(4):
    t = threading.Thread(target=worker, args=(i,), name="worker%d" % i)
    t.start()
    threads.append(t)

time.sleep(120)
