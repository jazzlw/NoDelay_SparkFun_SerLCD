NoDelay Sparkfun SerLCD Library
===========================================================

This is a fork of the [Sparfun SerLCD library](https://github.com/sparkfun/SparkFun_SerLCD_Arduino_Library) that adds functionality for interacting with the LCD without the blocking `delay` calls that are so common in the original library. This allows using the LCD without wasting tons of processor time on these delay calls, but it requires the caller to wait the appropriate amount of time between calls to the LCD.

The original library uses 10ms after all commands, with an extra 10ms after things like `clear`, `setFastBacklight`, anything with system messages etc.  It also delays 50ms after special commands such as `setCursor`, and anything that changes modes like `noDisplay`, or `leftToRight`.  I've found these to generally be overkill, but some structure of delay between commands is essential for the display to work correctly, and the calling program must take care of this.

The huge benefit of this is that the programmer gets all of that time back to do other things.  For example, with the original library, setting the cursor to the first line, printing a line, setting it to the second line, and printing a second line would go something like this, with delay times shown in comments.

```C
lcd.setCursor(0, 0); // delay(50);
lcd.print(line0);    // delay(10);
lcd.setCursor(0, 1); // delay(50);             
lcd.print(line1);    // delay(10);
```

Thus this simple task, which includes about 4ms of actual communication over a 100 kHz I2C, would lock up your micro for over 120 ms!  Instead, with this small change, you can manage the time yourself and get all of that back.

To use this, simply use the overloaded constructor for the `SerLCD` class that takes a boolean argument `noDelay`.  If this is set to false, the library will behave as normal, or if true, all calls to `delay` are removed, eg

```C
SerLCD lcd = SerLCD(true);
```

![SparkFun SerLCD](https://cdn.sparkfun.com//assets/parts/1/1/9/2/7/14074-SparkFun_20x4_SerLCD_-_Black_on_RGB_3.3V-05.jpg)

[*SparkFun 20x4 SerLCD - Black on RGB 3.3V (LCD-14074)*](https://www.sparkfun.com/products/14074)

The SparkFun SerLCD is an AVR-based, serial enabled LCD that provides a simple and cost effective solution for adding a 20x4 Black on RGB Liquid Crystal Display into your project. We’ve overhauled the design to include an ATmega328P that handles all of the screen control, meaning a backpack is no longer needed! This display can now accept three different types of communication protocols: serial, I2C, and SPI. This simplifies the number of wires needed and allows your project to display all kinds of text and numbers.

The SerLCD is Qwiic compatible! We recommend adding a [Qwiic Adapter](https://www.sparkfun.com/products/14495) to the back of the SerLCD to get it onto the Qwiic bus.

The on-board ATmega328P AVR microcontroller utilizes 11.0592 MHz crystal for greater communication accuracy with adjustable baud rates of 1200 through 1000000 but is default set at 9600. The firmware for this SerLCD is fully opensource and allows for any customizations you may need.

Library written by Gaston Williams and Nathan Seidle ([SparkFun](http://www.sparkfun.com)).

Repository Contents
-------------------

* **/examples** - Example sketches for the library (.ino). Run these from the Arduino IDE.
* **/src** - Source files for the library (.cpp, .h).
* **keywords.txt** - Keywords from this library that will be highlighted in the Arduino IDE.
* **library.properties** - General library properties for the Arduino package manager.

Documentation
--------------

* **[Installing an Arduino Library Guide](https://learn.sparkfun.com/tutorials/installing-an-arduino-library)** - Basic information on how to install an Arduino library.

License Information
-------------------

This product is ***open source***!

Various bits of the code have different licenses applied. Anything SparkFun wrote is beerware; if you see me (or any other SparkFun employee) at the local, and you've found our code helpful, please buy us a round!

Please use, reuse, and modify these files as you see fit. Please maintain attribution to SparkFun Electronics and release anything derivative under the same license.

Distributed as-is; no warranty is given.

* Your friends at SparkFun.
