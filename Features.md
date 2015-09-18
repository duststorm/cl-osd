# cl-osd features #

These features are the current progress for [r88](https://code.google.com/p/cl-osd/source/detail?r=88).

## Common ##
  * Run time
  * Support for current sensor (with additional hardware)
  * Global config options available in config.h.

## E-OSD ##
  * 2 Voltage inputs (one can be used for RSSI)
  * Possible to upgrade to GPS and home calculations but requires some advanced soldering.

## G-OSD ##
  * 3 Voltage inputs (one of two inputs can be used for RSSI)
  * GPS data
    * Lat, Long pos
    * Speed
    * Altitude
    * Time (UTC)
    * Date
    * Bearing
    * Satelites
    * Fix or not
  * Home calculations
    * Distance to home
    * Bearing to home
    * Altitude above home
    * Arrow home
    * Auto set home position (Also manual)
  * Support for imperial units