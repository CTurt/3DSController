3DSController
=============
A 3DS homebrew application which allows you to use your 3DS as a wireless controller for Windows.

### Setup and Usage
Firstly, if you want to be able to use the circle pad as a joystick you will need to install [vJoy](http://vjoystick.sourceforge.net/site/index.php/download-a-install). However, if you just want to use the buttons, this is not necessary.

Copy the executable with the extension that applies to your loader: `3DSController.3dsx` for Ninjhax, `3DSController.3ds` for flashcards, or `3DSController.cia` for CFWs, into your 3DS's SD card or flashcard's micro SD card.

Create file called `3DSController.ini` on the root of your 3DS's SD card containing the local IP address of your PC with nothing else, no new lines, etc, for example `192.168.0.4`. Refer [here](http://www.helpfulpctools.com/HowToCheckYourLocalIP.php) if you are unsure of how to find your local IP address.

Run `3DSController.exe` on your computer. If you are prompted, make sure to allow it through your firewall.

Start the application on your 3DS, there is no GUI, it will automatically try to connect to the IP address you put in `3DSController.ini`.

To make sure it worked, open Notepad and press some buttons on the 3DS, they should show up.

To return back to the Homebrew Loader, hold down Start and Select.

### Troubleshooting
- Make sure your 3DS has internet access and is on the same network as your PC,
- Make sure that the `3DSController.ini` is in the root of your 3DS's SD card,
- Make sure that the `3DSController.ini` has the local IP of your computer, not your public IP,
- Make sure your firewall isn't blocking the application,
