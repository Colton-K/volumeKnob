#!/usr/bin/python3

import serial
from pynput.keyboard import Key, Controller
import time
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
        self._conn.open()
        while True:
            print("attempting to connect...")
            self._conn.write(b'Are these cowboy times?\n')
            if self.getInput() == "hello cowboy":
                return True

    def disconnect(self):
        self._conn.close()

    def getInput(self):
        return self._conn.readline().decode('utf-8').rstrip()

    def processInput(self):
        update = self.getInput()
        print(update)
        if update == "Decreasing volume":
            self._kb.press(Key.media_volume_down)
            self._kb.release(Key.media_volume_down)
        if update == "Increasing volume":
            self._kb.press(Key.media_volume_up)
            self._kb.release(Key.media_volume_up)
        if update == " Single press":
            self._kb.press(Key.media_play_pause)
            self._kb.release(Key.media_play_pause)
        if update == " Double press":
            self._kb.press(Key.media_volume_mute)
            self._kb.release(Key.media_volume_mute)
        #  if update == " Long press":
        #      self._kb.press(Key.)
        #      self._kb.release(Key.)
        if update == "Reversing song":
            self._kb.press(Key.media_previous)
            self._kb.release(Key.media_previous)
        if update == "Skipping song":
            self._kb.press(Key.media_next)
            self._kb.release(Key.media_next)
        if update == "Are these cowboy times?":
            self._conn.write("hello cowboy\n")

        #  session = AudioUtilities.GetAllSessions()[0]
        #  volume = session._ctl.QueryInterface(ISimpleAudioVolume)
        #  print(volume.GetMasterVolume())
        # send update with volume info
        self._conn.write("50\n".encode('utf-8'))


def main():
    k = MediaKnob()

    while True:
        k.processInput()

if __name__ == "__main__":
    main()

