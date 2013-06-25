isdst
=====

Checks if DST (Daylight saving time) changed and displays a popup window.

Currently uses zenity as notification backend.

(still experimental)

### Requirements

* In order to compile you need basic gcc compiler. `build-essential` package in Ubuntu/Debian.

* In order to run you need `zenity` to show the notification.

### Usage

    make
    ./isdstd

### Notes

    The default file, where the latest check is saved, is ".isdst".
