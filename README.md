3DSController ![](/3DS/cxi/icon48x48.png?raw=true)
===
A 3DS homebrew application which allows you to use your 3DS as a wireless controller for Windows.

### Download
The latest release will always be downloadable from [here](https://github.com/CTurt/3DSController/releases/).

If you are updating to 0.6 from an older version, you will need to make sure you update vJoy to the recommended version.

### Setup and Usage
Firstly, if you want to be able to register the circle pad or touch screen as a joystick you will need to install [vJoy (version 2.0.5-120515 is preferable)](http://sourceforge.net/projects/vjoystick/files/Beta%202.x/2.0.5-120515/vJoy_205_050515.exe/download). However, if you just want to use keyboard buttons, this is not necessary.

Extract the archive and copy the executable in the `3DS` directory with the extension that applies to your loader: `3DSController.3dsx` and `3DSController.smdh` for Ninjhax, `3DSController.3ds` for flashcards, or `3DSController.cia` for CFWs, into your 3DS's SD card or flashcard's micro SD card.

Copy the file `3DS/3DSController.ini` to the root of your 3DS's SD card, and change the line that says `IP: 192.168.0.4` to match your computer's local IP.

If you are unsure of your local IP address, run `3DSController.exe` and it will tell you.

Run `3DSController.exe` on your computer. If you are prompted, make sure to allow it through your firewall.

Start the application on your 3DS, there is no GUI, it will automatically try to connect to the IP address you put in `3DSController.ini`.

If it wasn't able to read the IP from `3DSController.ini`, it will notify you and quit.

Otherwise, you should just see a black screen, this is a good sign. To see if it works, open Notepad and press some buttons on the 3DS, they should show up. You can also test if the joystick works by going to Configure USB Game Controllers in Control Panel, it shows up as vJoy.

If using version 0.4 or above you can press L, R and X to bring up the keyboard. Press L, R and X again to close it.

If using version 0.6 or above, up to 16 joystick buttons are available. If you wish to use more than 8, you need to configure vJoy. Search in your start menu for vJoyConfig and set buttons to 16.

If using version 0.7 or above, the output vJoy device is configurable. To output to a device other than 1 it must first be enabled in vJoyConfig.

If using Ninjhax press Start and Select to return to the Homebrew Loader, otherwise you can just exit with the Home button.

### Configuration
Find the line `Port: 8889` and change it to your desired port, do this for both the 3DS's `3DSController.ini` and the PC's `3DSController.ini`.

To use custom key bindings, just change the PC's `3DSController.ini` file, it should be straight forward.

### Troubleshooting
- Make sure that you are using the 3DS and PC application from the same release,
- Make sure your 3DS has internet access (turn on the switch on the side of the 3DS so that an orange light shows) and is on the same network as your PC,
- Make sure that the `3DSController.ini` is in the root of your 3DS's SD card (not flashcard micro SD),
- Make sure that the `3DSController.ini` has the local IP of your computer, not your public IP,
- Make sure your firewall isn't blocking the application,
- Try using a different port (change the port for both the 3DS and PC's .ini file),