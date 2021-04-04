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