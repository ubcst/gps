# gps
Logic and related files for the GPS sensor.

## Steps to Setup Executables and Such
1. `git clone https://github.com/ubcst/gps.git`
2. `cd gps`
3. `make`

## Steps to Run GPS
1. Enter the following command: `sudo gpsd /dev/ttyACM0 -F /var/run/gpsd.sock`
2. Run `./gps` (may need sudo permissions... looking into this)
3. Wait ~30 seconds. The GPS performs a full cold start, meaning that we clear system/user configurations at re-start.
