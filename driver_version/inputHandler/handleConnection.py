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
            print("attempting to connect...")
            self._conn.write(b'Are these cowboy times?\n')
            if self.getInput() == "hello cowboy":
                print("Connected")
                return True
            #  except:
            #     time.sleep(1)
            #     pass

    def disconnect(self):
        self._conn.close()

    def getInput(self):
        return self._conn.readline().decode('utf-8').rstrip()

    def processInput(self):
        try:
            update = self.getInput()

            #  print(update)
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
                # undo single press first registered
                self._kb.press(Key.media_play_pause)
                self._kb.release(Key.media_play_pause)
                # do double press activity
                self._kb.press(Key.media_volume_mute)
                self._kb.release(Key.media_volume_mute)
            if update == "Reversing song":
                # undo single press first registered
                self._kb.press(Key.media_play_pause)
                self._kb.release(Key.media_play_pause)

                self._kb.press(Key.media_previous)
                self._kb.release(Key.media_previous)
            if update == "Skipping song":
                # undo single press first registered
                self._kb.press(Key.media_play_pause)
                self._kb.release(Key.media_play_pause)

                self._kb.press(Key.media_next)
                self._kb.release(Key.media_next)
            if update == "Are these cowboy times?":
                self._conn.write("hello cowboy\n")
            if update == " Long press":
                self._conn.write(f"0\n".encode('utf-8'))
            if update == "getVolume":
                os.system("./getVolume.sh")
                with open("volume", 'r') as fh:
                    volume = fh.readline().rstrip()
                    #  print(volume)
                self._conn.write(f"{volume}\n".encode('utf-8'))

        except Exception as ex: 
            print("Connection broken...", ex)
            self.disconnect()
            self.connect()
            self.processInput()
            return

def main():
    k = MediaKnob()

    while True:
        k.processInput()

if __name__ == "__main__":
    main()

