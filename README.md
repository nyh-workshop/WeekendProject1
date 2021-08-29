# WeekendProject1

### A simple 5x7 LED matrix project.

This is a simple project that drives the 5x7 LED display (Kingbright 451688: https://uk.rs-online.com/web/p/led-displays/4516688/) using a PIC16F1579.

Each column is light up for 1ms intervals by using the Timer 0 interrupt. The entire frame takes about 5ms to render.

In the sample program it displays a bitmap that is generated randomly using basic LFSR.

To display a frame for a specified time = multiply the number by 5. Example, to show an emoticon frame for 1 second, 200 is used in the parameter:
> displayFrame(buffer, (unsigned char*)emoticon_0, 200);

The project is compiled using MikroC Necto 1.8.1. Other PIC-based compilers can run fine with few modifications on the code.

Schematic:
![alt_tag](https://github.com/nyh-workshop/WeekendProject1/blob/main/schematic.png)
