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
    setattr(data, 'index', 65)
    setattr(data, 'PosX', 1.0)
    setattr(data, 'PosY', 1.0)
    setattr(data, 'PosZ', 1.0)

    shm.seek(0)
    shm.write(bytes(data));


shm = mmap.mmap(0, 1024*1, "ipc0")
set_data()