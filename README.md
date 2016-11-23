# Multiplay-Controller V1.1 11/2016
Arduino code for Multiplay software controller

This software was designed to be used on a Arduino Leonardo board to control or emulate shortcut keyboard presses to run specific functions of the software.

To prevent accidental presses, the PLAY, FADE ALL and STOP functions require the UNSECURE button to be pressed in order to output the correspondent keystroke.

The ASCII code for each command must be adjusted either on the code or in the key binding preferences on the software according to the code established key code.

Olikraus U8GLib was used to drive the OLED display and should work with any display available on the U8G library. Changes must be made on the code for setting up coordinates and display type. More on https://github.com/olikraus/u8glib . 


