#!/usr/bin/env python
# Compatible with both Python 2.7.6 and 3.4.3

from __future__ import print_function
import socket, struct, time
import uinput

##########################################################
# CONFIGURABLE REGION START - Don't touch anything above #
##########################################################
port = 8889

#Valid values can be found in any of these locations on your Linux system (some may not exist):
# /usr/include/linux/input-event-codes.h
#The virtual device is defined on the device variable and the mapping of the keys on btn_map
#RECAP keynames (DO NOT TOUCH) are the keys that we expect commming from the 3ds, device is
#the virtual device that we define and btn_map maps what we recieve with our virtual device
btn_map = {
	"A": uinput.BTN_A,
	"B": uinput.BTN_B,
	"X": uinput.BTN_X,
	"Y": uinput.BTN_Y,
	"L": uinput.BTN_TL,
	"R": uinput.BTN_TR,
	"ZL": uinput.BTN_THUMBL,
	"ZR": uinput.BTN_THUMBR,
	"Left": uinput.BTN_DPAD_LEFT,
	"Right": uinput.BTN_DPAD_RIGHT,
	"Up": uinput.BTN_DPAD_UP,
	"Down": uinput.BTN_DPAD_DOWN,
	"Start": uinput.BTN_START,
	"Select": uinput.BTN_SELECT,
	"Tap": uinput.BTN_MODE,
}

device = uinput.Device([
	uinput.ABS_X + (-159, 159, 0, 10),
	uinput.ABS_Y + (-159, 159, 0, 10),
	uinput.ABS_RX + (-146, 146, 0, 16),
	uinput.ABS_RY + (-146, 146, 0, 16),
	uinput.BTN_A,
	uinput.BTN_B,
	uinput.BTN_X,
	uinput.BTN_Y,
	uinput.BTN_TL,
	uinput.BTN_TR,
	uinput.BTN_THUMBL,
	uinput.BTN_THUMBR,
	uinput.BTN_DPAD_LEFT,
	uinput.BTN_DPAD_RIGHT,
	uinput.BTN_DPAD_UP,
	uinput.BTN_DPAD_DOWN,
	uinput.BTN_START,
	uinput.BTN_SELECT,
	uinput.BTN_MODE,
	])
########################################################
# CONFIGURABLE REGION END - Don't touch anything below #
########################################################

def pprint(obj):
	import pprint
	pprint.PrettyPrinter().pprint(obj)

class x: pass

command = x()
command.CONNECT = 0
command.KEYS = 1
command.SCREENSHOT = 2

keynames = [
	"A", "B", "Select", "Start", "Right", "Left", "Up", "Down",
	"R", "L", "X", "Y", None, None, "ZL", "ZR",
	None, None, None, None, "Tap", None, None, None,
	"CSRight", "CSLeft", "CSUp", "CSDown", "CRight", "CLeft", "CUp", "CDown",
]

keys = x()
keys.A       = 1<<0
keys.B       = 1<<1
keys.Select  = 1<<2
keys.Start   = 1<<3
keys.Right   = 1<<4
keys.Left    = 1<<5
keys.Up      = 1<<6
keys.Down    = 1<<7
keys.R       = 1<<8
keys.L       = 1<<9
keys.X       = 1<<10
keys.Y       = 1<<11
keys.ZL      = 1<<14 # (new 3DS only)
keys.ZR      = 1<<15 # (new 3DS only)
keys.Tap     = 1<<20 # Not actually provided by HID
keys.CSRight = 1<<24 # c-stick (new 3DS only)
keys.CSLeft  = 1<<25 # c-stick (new 3DS only)
keys.CSUp    = 1<<26 # c-stick (new 3DS only)
keys.CSDown  = 1<<27 # c-stick (new 3DS only)
keys.CRight  = 1<<28 # circle pad
keys.CLeft   = 1<<29 # circle pad
keys.CUp     = 1<<30 # circle pad
keys.CDown   = 1<<31 # circle pad
	
def press_key(key):
	device.emit(key, 1)
        
def release_key(key):
	device.emit(key,0)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("", port))

prevkeys = 0

touch_start = 0
touch_last_x = 0
touch_last_y = 0

while True:
	rawdata, addr = sock.recvfrom(4096)
	rawdata = bytearray(rawdata)
	#print("received message", rawdata, "from", addr)
	
	if rawdata[0]==command.CONNECT:
		pass # CONNECT packets are empty
	
	if rawdata[0]==command.KEYS:
		fields = struct.unpack("<BBxxIhhHHhhI", rawdata)
		
		data = {
			"command": fields[0],
			"keyboardActive": fields[1],
			"keys": fields[2],
			"circleX": fields[3],
			"circleY": fields[4],
			"touchX": fields[5],
			"touchY": fields[6],
			"cstickX": fields[7],
			"cstickY": fields[8],
			"volume": fields[9]
		}
		
		newkeys = data["keys"] & ~prevkeys
		oldkeys = ~data["keys"] & prevkeys
		prevkeys = data["keys"]
		
		for btnid in range(16):
			if newkeys & (1<<btnid):
				press_key(btn_map[keynames[btnid]])
			if oldkeys & (1<<btnid):
				release_key(btn_map[keynames[btnid]])
		if newkeys & keys.Tap:
			press_key(btn_map["Tap"])
		if oldkeys & keys.Tap:
			release_key(btn_map["Tap"])

		device.emit(uinput.ABS_X, data["circleX"], syn=False)
		device.emit(uinput.ABS_Y, 0-data["circleY"])
		device.emit(uinput.ABS_RX, data["cstickX"], syn=False)
		device.emit(uinput.ABS_RY, data["cstickY"])
	
	if rawdata[0]==command.SCREENSHOT:
		pass # unused by both 3DS and PC applications
