import threading
import mmap
from time import sleep
import numpy as np
import ctypes

class IPC_Data(ctypes.Structure):
    _fields_ = [
        # 'A' control byte
        ("index", ctypes.c_byte),

        ('PosX', ctypes.c_float),
        ('PosY', ctypes.c_float),
        ('PosZ', ctypes.c_float),
    ]

def set_data():
    data = IPC_Data()

    shm.seek(0)
    shm.write(bytes(data));

def track():
    for i in range(0, 1000):
        sleep(1.0 / 60.0)
        set_data()

shm = mmap.mmap(0, 1024*8, "weebking_ipc")

c = threading.Thread(target=track)
c.start()