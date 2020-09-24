# tf
Tiny program for fan control Thinkpad notebooks.

Tested on L420. Be carefull. Absolutly no warrantly!

### Prequesite:
Add "options thinkpad_acpi fan_control=1" on "/etc/modprobe.d/thinkpad_acpi.conf"

### Build and Install:
```sh
$ make
$ make install
```

### Uninstall:
```sh
$ make uninstall
```

### Usage:
```sh
$ sudo systemctl enable tf
$ sudo systemctl start tf
```

### Configuration:
Edit "tf.h" and rebuild and reinstall

```c
// CPU temp
#define F_TEMP "/sys/devices/platform/thinkpad_hwmon/hwmon/hwmon1/temp1_input"

// thinkpad_acpi fan interface
#define F_FAN "/proc/acpi/ibm/fan"

// Config (Change to suit your needs)
// {MIN, MAX}
config_i config[] = {
    {0,  46}, // Level 0
    {38, 51}, // Level 1
    {45, 56}, // Level 2
    {51, 61}, // Level 3
    {55, 64}, // Level 4
    {60, 66}, // Level 5
    {63, 68}, // Level 6
    {65, 74}  // Level 7
};
```

### License:
MIT License

Copyright (c) 2020 Daniil A. (Kiky Tokamuro)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
