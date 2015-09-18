## When will feature xxx be added? ##
I do this project for fun, so I will be added when I have time for it.

## What features are supported? ##
See page [Features](Features.md).

## How do I compile & program my board? ##
See page [CompileProgram](CompileProgram.md) and [Programming](Programming.md).

## I found a bug, what should I do? ##
Add it to the issues page with info about how to reproduce it etc. I will have a look at is when I can.

## I would like to help out, what should I do? ##

Learn how to compile and program the board and then check the issues and try to fix any of them. Or you could try to add more features.
Contact me and I will add your code to svn.

## Is there a schematics over E-OSD / G-OSD? ##

There is a schematic for E-OSD [here](http://static.rcgroups.net/forums/attachments/3/6/2/8/4/1/a4069376-238-e-osd.png).

No schematic for G-OSD but it's almost the same. Some pins on the µP has changed.

## How do I trim the voltages? ##

On G-OSD: Use the pots on the board. Two of them are for the inputs and the third does nothing right now.

On E-OSD: There is no simple tweek right now but here is one way:
In hardware.h look for ADC\_MULT and ADC\_DIV. These are arrays with values for all adc inputs. If your voltage is too high set it to example mult 9 and div 10. Or the other way around if voltage is too low. Here is a simplification of the equation used for the inputs:

```
Voltage = (IN[x] * ADC_MULT[x]) / ADC_DIV[x]
x = ADC input. In the case of E-OSD only 0 since 1 is used by RSSI.
```
**Note 1**: Dont use too high values or you might overflow the variable!

**Note 2**: Also only use integers!

An example:
If we see a too high voltage reading on voltage 1 then ADC\_MULT and  should be changed from:
```
#define ADC_MULT 1, 1
#define ADC_DIV 1, 1
```
to:
```
#define ADC_MULT 9, 1
#define ADC_DIV 10, 1
```
So if the measured voltage was 2 Volts it would show as (2 x 9) / 10 = 1,8 Volts on the screen.


## How do I change the screen update speed? ##
No quick fix for this either yet but [here](http://www.rcgroups.com/forums/showpost.php?p=19802356&postcount=755) is a post about it.

## How can I remove the grey text background ##
Look [here](http://www.rcgroups.com/forums/showpost.php?p=19511436postcount=462).

## How do I restore the original firmware? ##
It's not possible since the code on the G-OSD cannot be read since a code protection fuse is set. Sorry!