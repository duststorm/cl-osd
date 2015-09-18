# Prerequisites #

  1. Download and install [AVR Studio 5](http://i.amniels.com/avr-studio-4-and-5-download-links)
  1. Download and install [WinAVR](http://sourceforge.net/projects/winavr/files/latest/download?source=files)
  1. (or: Download and install [eXtreme Burner- AVR](http://extremeelectronics.co.in/avr-tutorials/gui-software-for-usbasp-based-usb-avr-programmers/))
  1. Download and install [TortoiseSVN](http://tortoisesvn.net/)

# Getting the code #

  1. Go to a folder where you want the code.
  1. Press right button in folder
  1. Select "SVN Checkout"
  1. Enter url of cl-osd: "http://cl-osd.googlecode.com/svn/trunk/"
  1. Press OK.

# Compile #

(If you just want to do some initial testing you don't need to do this step. Just use the pre-built hex and eep file. Note that this might not always work since you might need to do some configuration in the code.)

  1. Open "cl-osd.avrgccproj" inside the "cl-osd" folder you created in the last step.
  1. I the top you see a list with Debug, E-OSD etc. Select the board you whant to build for.
  1. If you are using Avr Studio 5.1.208 or higher you need to do the following steps:
    * Go to Project->cl-osd Properties...
    * Toolchain->Avr/GNU C Linker->Memory Settings
    * In "Initial Stack Address (hex)" clear that box.
  1. Press Build -> Clean cl-osd. (Only needed first time!)
  1. Press Build -> Build cl-osd. (First time you might be asked to save a solutions file. Place this in the same folder as the other files)
  1. In your folder cl-osd/X-OSD (where X is your target board) there should be a cl-osd.hex and a cl-osd.eep. That's the flash file and the eeprom file.

Continue to [Programming](Programming.md).