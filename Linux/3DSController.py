#!/usr/bin/env python
# Compatible with both Python 2.7.6 and 3.4.3

from __future__ import print_function
import socket, struct, time
import Xlib, Xlib.display, Xlib.XK
LMouse = []; RMouse = []
Button = []; MouseAbs = []; MouseRel = []; MouseAbsClick = []; MouseRelClick = []

##########################################################
# CONFIGURABLE REGION START - Don't touch anything above #
##########################################################
port = 8889

#This tells what the touch screen does if touched.
#Valid values: Button, MouseAbs, MouseRel, MouseRelClick, MouseRelClick
#Button sends the Tap button.
#MouseAbs moves your mouse to the same part of the screen as the touch screen was touched.
#MouseRel moves your mouse by the same distance as you drag across the touch screen.
#MouseAbsClick and MouseRelClick send the primary mouse button event if the screen is tapped, not held.
touch = MouseRelClick

mouse_speed = 4
# The number of pixels on each side of the 3DS screen which are ignored, since you can't reach the outermost corners.
abs_deadzone = 10

#Valid values can be found in any of these locations on your Linux system (some may not exist):
# /usr/include/X11/keysymdef.h
# /usr/lib/python3/dist-packages/Xlib/keysymdef/
# /usr/lib/python2.7/dist-packages/Xlib/keysymdef/
#Additionally, LMouse and RMouse can also be used on any button.
btn_map = {
	"A": "A",
	"B": "B",
	"X": "X",
	"Y": "Y",
	"L": "L",
	"R": "R",
	"ZL": "Q",
	"ZR": "W",
	"Left": Xlib.XK.XK_Left,
	"Right": Xlib.XK.XK_Right,
	"Up": Xlib.XK.XK_Up,
	"Down": Xlib.XK.XK_Down,
	"Start": Xlib.XK.XK_Return,
	"Select": Xlib.XK.XK_BackSpace,
	"Tap": LMouse,
}
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
	chars = ("!1234567890\x08"+
	         "QWERTYUIOP\13\13"+
	         "ASDFGHJKL-\13\13"+
	         "ZXCVBNM,.?\13\13"+
	         "\0\0\0     \0\0\0\0")
	
	if x>=1 and x<=312 and y>=78 and y<=208:
		xi = int(x*12/320)
		yi = int((y-78)*12/320)
		return chars[yi*12 + xi]
	else: return None

def key_to_keysym(key):
	if not key: return 0
	
	if isinstance(key,str):
		if key=="\x08": return Xlib.XK.XK_BackSpace
		if key=="\13": return Xlib.XK.XK_Return
		if key==" ": return Xlib.XK.XK_space
		return Xlib.XK.string_to_keysym(key)
	
	return key

def action_key(key, action):
	x_action = Xlib.X.ButtonRelease
	x_action2 = Xlib.X.KeyRelease
	if action:
		x_action = Xlib.X.ButtonPress
		x_action2 = Xlib.X.KeyPress
    
	if key is LMouse or key is RMouse:
		if key is LMouse: button = 1
		if key is RMouse: button = 3
		button = disp.get_pointer_mapping()[button-1] # account for left-handed mice
		disp.xtest_fake_input(x_action, button)
		disp.sync()
		return
	
	keysym = key_to_keysym(key)
	if not keysym: return
	
	keycode = disp.keysym_to_keycode(keysym)
	disp.xtest_fake_input(x_action2, keycode)
	disp.sync()
	
def press_key(key):
	action_key(key,True)
        
def release_key(key):
	action_key(key,False)

def move_mouse(x,y):
	x=int(x)
	y=int(y)
	if not x and not y: return
	
	disp.warp_pointer(x,y)
	disp.sync()

def move_mouse_abs_frac(x,y):
	root = disp.screen().root
	geom = root.get_geometry()
	
	root.warp_pointer(int(x*geom.width), int(y*geom.height))
	disp.sync()

disp = Xlib.display.Display()

touch_click = (touch is MouseAbsClick or touch is MouseRelClick)
if touch is MouseAbsClick: touch = MouseAbs
if touch is MouseRelClick: touch = MouseRel

if touch is MouseAbs and disp.screen_count()!=1:
	print("Sorry, but MouseAbs only supports a single monitor. I'll use MouseRel instead.")
	touch = MouseRel

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
		#print(data)
		
		newkeys = data["keys"] & ~prevkeys
		oldkeys = ~data["keys"] & prevkeys
		prevkeys = data["keys"]
		
		for btnid in range(16):
			if newkeys & (1<<btnid):
				press_key(btn_map[keynames[btnid]])
			if oldkeys & (1<<btnid):
				release_key(btn_map[keynames[btnid]])
		if newkeys & keys.Tap:
			if data["keyboardActive"]:
				keyboard_prevkey = currentKeyboardKey(data["touchX"], data["touchY"])
				press_key(keyboard_prevkey)
			elif touch is Button:
				press_key(btn_map["Tap"])
			touch_start = time.time()
		if prevkeys & keys.Tap:
			if data["keyboardActive"] & (keyboard_prevkey != currentKeyboardKey(data["touchX"], data["touchY"])):
				release_key(keyboard_prevkey)
				keyboard_prevkey = currentKeyboardKey(data["touchX"], data["touchY"])
				press_key(keyboard_prevkey)
		if oldkeys & keys.Tap:
			if data["keyboardActive"]:
				release_key(keyboard_prevkey)
			elif touch is Button:
				release_key(btn_map["Tap"])
		if data["keys"] & keys.Tap:
			if touch is MouseAbs:
				x = (data["touchX"]-abs_deadzone) / (320.0-abs_deadzone*2)
				y = (data["touchY"]-abs_deadzone) / (240.0-abs_deadzone*2)
				move_mouse_abs_frac(x, y)
			if touch is MouseRel and not newkeys & keys.Tap:
				x = (data["touchX"]-touch_last_x) * mouse_speed
				y = (data["touchY"]-touch_last_y) * mouse_speed
				move_mouse(x, y)
			touch_last_x = data["touchX"]
			touch_last_y = data["touchY"]
		
		if oldkeys & keys.Tap and touch_click and time.time()-touch_start < 0.1:
			press_key(LMouse)
			release_key(LMouse)
		
		if abs(data["circleX"])>=16 or abs(data["circleY"])>=16:
			move_mouse(data["circleX"]*mouse_speed/32.0, -data["circleY"]*mouse_speed/32.0)
	
	if rawdata[0]==command.SCREENSHOT:
		pass # unused by both 3DS and PC applications
