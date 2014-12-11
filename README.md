3DSController ![](/3DS/cxi/icon48x48.png?raw=true)
===

A 3DS homebrew application which allows you to use your 3DS as a wireless controller for Windows.

### Download
The latest release will always be downloadable from [here](https://github.com/CTurt/3DSController/releases/).

### Setup and Usage
Firstly, if you want to be able to use the circle pad as a joystick you will need to install [vJoy](http://vjoystick.sourceforge.net/site/index.php/download-a-install). However, if you just want to use the buttons, this is not necessary.

Extract the archive and copy the executable in the `3DS` directory with the extension that applies to your loader: `3DSController.3dsx` and `3DSController.smdh` for Ninjhax, `3DSController.3ds` for flashcards, or `3DSController.cia` for CFWs, into your 3DS's SD card or flashcard's micro SD card.

Create a file called `3DSController.ini` on the root of your 3DS's SD card containing the local IP address of your PC with nothing else, no new lines, etc, for example `192.168.0.4`. If you are unsure of your local IP address, run `3DSController.exe` and it will tell you.

Run `3DSController.exe` on your computer. If you are prompted, make sure to allow it through your firewall.

Start the application on your 3DS, there is no GUI, it will automatically try to connect to the IP address you put in `3DSController.ini`.

If it wasn't able to read the IP from `3DSController.ini`, it will notify you and quit.

Otherwise, you should just see a black screen, this is a good sign. To see if it works, open Notepad and press some buttons on the 3DS, they should show up. You can also test if the joystick works by going to Configure USB Game Controllers in Control Panel, it shows up as vJoy.

If using Ninjhax press Start and Select to return to the Homebrew Loader, otherwise you can just exit with the Home button.

### Configuration
To change the port, add `:` followed by the desired port to your 3DS's `3DSController.ini`, for example it should contain, `192.168.0.4:8889` to use port 8889. On the PC's `3DSController.ini`, find the line that says `Port: 8888` and change it to match the port you chose for the 3DS.

To use custom key bindings, just change the PC's `3DSController.ini` file, it should be straight forward.

### Troubleshooting
- Make sure your 3DS has internet access (turn on the switch on the side of the 3DS so that an orange light shows) and is on the same network as your PC,
- Make sure that the `3DSController.ini` is in the root of your 3DS's SD card (not flashcard micro SD),
- Make sure that the `3DSController.ini` has the local IP of your computer, not your public IP,
- Make sure your firewall isn't blocking the application,
- Try using a different port,