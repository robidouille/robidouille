#!/usr/bin/python
import pygame
import time
import datetime
import serial

pygame.mixer.init()
sounds = []
for ii in range(0, 11):
    sounds.append(pygame.mixer.Sound("%d.wav" % ii))

idx = 13

now = datetime.datetime.now()

target = datetime.datetime(2014, 1, 1, 0, 0, 0) - datetime.timedelta(seconds = idx)
#target = datetime.datetime(2013, 12, 30, 21, 35, 0) - datetime.timedelta(seconds = idx)
#target = now + datetime.timedelta(seconds = 2, microseconds = 1000000-now.microsecond)

if now > target:
    print "Target is in the past"
    exit(0)

arduino = None
while True:
    now = datetime.datetime.now()

    td = target - now
    seconds = (td.microseconds + (td.seconds + td.days * 24.0 * 3600) * 10**6) / 10**6
    if seconds > 60:
        seconds = 60
    print "%s: Sleep %f" % (now, seconds)
    time.sleep(seconds)
    now = datetime.datetime.now()
    print "%s: Back %d" % (now, idx)
    if now < target:
        continue

    if idx <= 10:
        sounds[idx].play()

    if idx == 12:
        arduino.write("99m")
        arduino.flush()
    elif idx == 10:
        arduino.write("9c98m")
        arduino.flush()
    elif idx > 0 and idx < 10:
        arduino.write("%dc" % (idx-1))
        arduino.flush()
    elif idx == 0:
        arduino.write("97m")
        arduino.flush()

    if arduino == None:
        arduino = serial.Serial("/dev/ttyACM0", 115200)

    target += datetime.timedelta(seconds = 1)
    idx -= 1
    if idx < 0:
        break

arduino.close()
time.sleep(2)
