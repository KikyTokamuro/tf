# tf
Tiny program for fan control Thinkpad notebooks.

Tested on L420. Be carefull. Absolutly no warrantly!

### Requirements:
Add "options thinkpad_acpi fan_control=1" on "/etc/modprobe.d/thinkpad_acpi.conf".

Install "libconfig":
```sh
sudo pacman -S libconfig
```

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
Edit "/etc/tf.cfg" and restart tf service.

```
temperature = "/sys/devices/platform/thinkpad_hwmon/hwmon/hwmon1/temp1_input";

fan = "/proc/acpi/ibm/fan";

sleep = 6;

levels = {
  level0 = { 
    min =  0; 
    max = 55; 
  };

  level1 = {
    min = 48; 
    max = 60; 
  };

  level2 = { 
    min = 50; 
    max = 61; 
  };

  level3 = { 
    min = 52; 
    max = 63; 
  };
  
  level4 = { 
    min = 56; 
    max = 65; 
  };
  
  level5 = { 
    min = 59; 
    max = 66; 
  };
  
  level6 = { 
    min = 62; 
    max = 68; 
  };
  
  level7 = { 
    min = 65; 
    max = 74; 
  };
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
