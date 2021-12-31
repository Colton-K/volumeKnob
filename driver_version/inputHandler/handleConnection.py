#!/usr/bin/python3

import serial
from pynput.keyboard import Key, Controller
import time
import os
#  from pycaw.pycaw import AudioUtilities, ISimpleAudioVolume

class MediaKnob:
    def __init__(self, port="/dev/ttyACM0", baudrate=9600):
        self._port = port
        self._baudrate = baudrate
        self._timeout = 1
        
        self._conn = serial.Serial()
        self._conn.port = self._port
        self._conn.baudrate = self._baudrate
        self._conn.timeout = self._timeout
        self.connect()

        self._kb = Controller()

    def connect(self):
        while True:
            try:
                self._conn.open()
                break
            except:
                time.sleep(1)
                pass


        while True:
            #  try:
            #  print("attempting to connect...")
            self._conn.write(b'Are these cowboy times?\n')
            if self.getInput() == "hello cowboy":
                #  print("Connected")
                return True
            #  except:
            #     time.sleep(1)
            #     pass

    def disconnect(self):
        self._conn.close()

    def getInput(self):
        return self._conn.readline().decode('utf-8').rstrip()

    def sendVolume(self):
        try:
            os.system("/home/colton/work/volumeKnob/driver_version/inputHandler/getVolume.sh")
            with open("/tmp/volume", 'r') as fh:
                volume = fh.readline().rstrip()
            #  print(volume)
            self._conn.write(f"{volume}\n".encode('utf-8'))
        except:
            self.disconnect()
            self.connect()

def main():
    k = MediaKnob()

    while True:
        k.sendVolume()
        time.sleep(.1)

if __name__ == "__main__":
    main()

