#!/usr/bin/env python
# Compatible with both Python 2.7.6 and 3.4.3

from __future__ import print_function
import socket, struct, time
import uinput

##########################################################
# CONFIGURABLE REGION START - Don't touch anything above #
##########################################################
port = 8889
mouse_speed = 4

btn_gamepad_map = {
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

keboard_matrix = [
		uinput.KEY_GRAVE, uinput.KEY_1, uinput.KEY_2, uinput.KEY_3, uinput.KEY_4, uinput.KEY_5, uinput.KEY_6, uinput.KEY_7, uinput.KEY_8, uinput.KEY_9, uinput.KEY_0, uinput.KEY_BACKSPACE,
		uinput.KEY_Q, uinput.KEY_W, uinput.KEY_E, uinput.KEY_R, uinput.KEY_T, uinput.KEY_Y, uinput.KEY_U, uinput.KEY_I, uinput.KEY_O, uinput.KEY_P, uinput.KEY_ENTER, uinput.KEY_ENTER,
		uinput.KEY_A, uinput.KEY_S, uinput.KEY_D, uinput.KEY_F, uinput.KEY_G, uinput.KEY_H, uinput.KEY_J, uinput.KEY_K, uinput.KEY_L, uinput.KEY_MINUS, uinput.KEY_ENTER, uinput.KEY_ENTER,
		uinput.KEY_Z, uinput.KEY_X, uinput.KEY_C, uinput.KEY_V, uinput.KEY_B, uinput.KEY_N, uinput.KEY_M, uinput.KEY_COMMA, uinput.KEY_DOT, uinput.KEY_SLASH, uinput.KEY_ENTER, uinput.KEY_ENTER,
		uinput.KEY_SPACE, uinput.KEY_SPACE, uinput.KEY_SPACE, uinput.KEY_SPACE, uinput.KEY_SPACE, uinput.KEY_SPACE, uinput.KEY_SPACE, uinput.KEY_SPACE, uinput.KEY_SPACE, uinput.KEY_SPACE, uinput.KEY_SPACE, uinput.KEY_SPACE
]

btn_keyboard_map = {
	"A": uinput.KEY_A,
	"B": uinput.KEY_B,
	"X": uinput.KEY_X,
	"Y": uinput.KEY_TAB,
	"L": uinput.KEY_LEFTSHIFT,
	"R": uinput.KEY_RIGHTSHIFT,
	"ZL": uinput.KEY_Q,
	"ZR": uinput.KEY_W,
	"Left": uinput.KEY_LEFT,
	"Right": uinput.KEY_RIGHT,
	"Up": uinput.KEY_UP,
	"Down": uinput.KEY_DOWN,
	"Start": uinput.KEY_ENTER,
	"Select": uinput.KEY_ESC
}

gamepad_device = uinput.Device([
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

mouse_device = uinput.Device([
	uinput.REL_X,
	uinput.REL_Y,
	uinput.BTN_LEFT,
	uinput.BTN_RIGHT
	])

keyboard_device = uinput.Device(keboard_matrix + btn_keyboard_map.values())

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

def currentKeyboardKey(x, y):
	if x>=1 and x<=312 and y>=78 and y<=208:
		xi = int(x*12/320)
		yi = int((y-78)*12/320)
		return keboard_matrix[yi*12 + xi]
	else: return None

def action_key(key):
	if key in keboard_matrix:
		keyboard_device.emit_click(key)
		return

def press_key(key):
	keyboard_device.emit(key, 1)

def release_key(key):
	keyboard_device.emit(key,0)

def press_btn(btn):
	gamepad_device.emit(btn, 1)

def release_btn(btn):
	gamepad_device.emit(btn, 0)

def move_mouse(x,y):
	x=int(x)
	y=int(y)
	if not x and not y: return
	mouse_device.emit(uinput.REL_X, x)
	mouse_device.emit(uinput.REL_Y, y)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("", port))

prevkeys = 0
touch_start = 0
touch_last_x = 0
touch_last_y = 0
keyboard_prevkey = currentKeyboardKey(0, 0)

while True:
	rawdata, addr = sock.recvfrom(4096)
	rawdata = bytearray(rawdata)
	#print("received message", rawdata, "from", addr)

	if rawdata[0]==command.CONNECT:
		pass # CONNECT packets are empty

	if rawdata[0]==command.KEYS:
		fields = struct.unpack("<BBxxIhhHHhh", rawdata)

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
		}

		newkeys = data["keys"] & ~prevkeys
		oldkeys = ~data["keys"] & prevkeys
		prevkeys = data["keys"]

		if data["keyboardActive"]:

			for btnid in range(16):
				if newkeys & (1<<btnid):
					press_key(btn_keyboard_map[keynames[btnid]])
				if oldkeys & (1<<btnid):
					release_key(btn_keyboard_map[keynames[btnid]])

			if newkeys & keys.Tap:
				keyboard_prevkey = currentKeyboardKey(data["touchX"], data["touchY"])
				action_key(keyboard_prevkey)

			if prevkeys & keys.Tap:
				if keyboard_prevkey != currentKeyboardKey(data["touchX"], data["touchY"]):
					keyboard_prevkey = currentKeyboardKey(data["touchX"], data["touchY"])
					action_key(keyboard_prevkey)

		else:
			for btnid in range(16):
				if newkeys & (1<<btnid):
					press_btn(btn_gamepad_map[keynames[btnid]])
				if oldkeys & (1<<btnid):
					release_btn(btn_gamepad_map[keynames[btnid]])

			gamepad_device.emit(uinput.ABS_X, data["circleX"], syn=False)
			gamepad_device.emit(uinput.ABS_Y, 0-data["circleY"])
			gamepad_device.emit(uinput.ABS_RX, data["cstickX"], syn=False)
			gamepad_device.emit(uinput.ABS_RY, data["cstickY"])

			if newkeys & keys.Tap:
				touch_start = time.time()

			if data["keys"] & keys.Tap:
				if not newkeys & keys.Tap:
					x = (data["touchX"]-touch_last_x) * mouse_speed
					y = (data["touchY"]-touch_last_y) * mouse_speed
					move_mouse(x, y)
				touch_last_x = data["touchX"]
				touch_last_y = data["touchY"]

			if oldkeys & keys.Tap and time.time()-touch_start < 0.1:
				if newkeys & keys.L:
					mouse_device.emit(uinput.BTN_RIGHT, 1)
					mouse_device.emit(uinput.BTN_RIGHT, 0)
				else:
					mouse_device.emit(uinput.BTN_LEFT, 1)
					mouse_device.emit(uinput.BTN_LEFT, 0)


	if rawdata[0]==command.SCREENSHOT:
		pass # unused by both 3DS and PC applications
