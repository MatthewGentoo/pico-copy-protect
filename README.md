# pico-copy-protect

A silly little demo that prevents trivial cloning of RP2040 programs
by tying the flash contents to the flash chip's unique id.

### Process

The serial NOR flash used by the Pico has a unique identifier burned
in, separate to the contents of the flash chip. On first startup, the
demo application stores this unique identifier into flash. Subsequent
starts can then compare the stored identifier to the measured
identifier. A mismatch indicates that the contents of the flash chip
have been dumped and copied onto a new board.

After a mismatch is identified, the application can show an error, or
generally misbehave.

### Demo Application

The application in this repository will blink the Pico's onboard LED
slowly if it is happy, and will blink rapidly if it determines that it
has been cloned.

### Caveats

It would be trivial for an attacker to disassemble the application and
identify what is happening. From there they could simply erase the
unique id, or modify the program to patch out the check.

Not a replacement for a chip with a proper secure element and trusted
boot process.

### License

Released under the terms of the 3-clause BSD license, the same as pico-sdk.
