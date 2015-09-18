# Programmer #

You need a AVR programmer in order to program the chip on the board.
There are several different types of programmers and in this case I will focus on USBASP since that's what I use and it's cheap.

# Soldering #

There are six pads on the boards that you need to either solder cables to or add a socket to.

The pitch of the pads are 2.0 mm which means that we need a special socket that has 2.0 mm pitch. If you don't have those at hand you can use a normal 2.45 mm pitch socket as Parity has done here:

[http://static.rcgroups.net/forums/attachments/3/7/6/4/4/5/t4278685-70-thumb-g-osd\_byParity.jpg?d=1316005018](http://static.rcgroups.net/forums/attachments/3/7/6/4/4/5/a4278685-209-g-osd_byParity.jpg)

Then you can use some sort of jumper cable sutch as these:

![![](http://dlnmh9ip6v2uc.cloudfront.net/images/products/JumperWire-Male-01-L_i_ma.jpg)](http://dlnmh9ip6v2uc.cloudfront.net/images/products/JumperWire-Male-01-L.jpg)

# Connecting board and programmer #

Now it's time to connect the board with the programmer.
Connect each of the cables together with jumper cables according to the following images:

## E-OSD programming pins (by kristaps\_r) ##

[http://static.rcgroups.net/forums/attachments/3/1/3/1/1/2/t4051434-191-thumb-E-OSD.jpg?d=1306862619](http://static.rcgroups.net/forums/attachments/3/1/3/1/1/2/a4051434-140-E-OSD.jpg)

## G-OSD programming pins ##

[http://static.rcgroups.net/forums/attachments/3/5/5/0/1/6/t4218208-28-thumb-2011-08-18\_00-48-00\_681%20copy.jpg?d=1313621628](http://static.rcgroups.net/forums/attachments/3/5/5/0/1/6/a4218208-52-2011-08-18_00-48-00_681%20copy.jpg)

## AVR ISP programming header ##

![http://iharis.com/assets/images/AVR_ISP_.png](http://iharis.com/assets/images/AVR_ISP_.png)

**Note that this pinout is for the header and not the cable.**
Just to make everything a bit more complicated. I could not find a pinout image for the cable.

VTG should be connected to +5 Volt.

See below for the difference a between the cable and the header:

![http://iharis.com/assets/images/AVR_ISP_HEADER02.jpg](http://iharis.com/assets/images/AVR_ISP_HEADER02.jpg)

# Get avrdude #

I use avrdude to program my boards but there are alot of other options.
For example some programmers such as the STK500 are able to program using the AVR Studio.

But in this example we use avrdude, get it [here](http://www.nongnu.org/avrdude/).

Note: If you have an atmega88PA instead of an atmega88 read [this page](http://www.avrfreaks.net/index.php?name=PNphpBB2&file=printview&t=100557&start=0).

# Programming #

**NOTE: Do not change the fuses on the AVR unless you really need to! You might brick you chip.**

**NOTE2: After these steps you will not be able to return to the original software!**

  * Press Start button -> Run -> type cmd -> OK
  * Type avrdude and press enter to make sure avrdude is installed correctly.
  * Move to the cl-osd dir (example "cd c:\save\cl-osd")
  * Type the following line: avrdude.exe -p m88 -c usbasp -U flash:w:cl-osd.hex -U eeprom:w:cl-osd.eep

For more info on avrdude see [this](http://www.ladyada.net/learn/avr/avrdude.html) page.