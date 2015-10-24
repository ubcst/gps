# gps
Logic and related files for the GPS sensor.

## Steps to Setup Executables and Such
1. `git clone https://github.com/ubcst/gps.git`
2. `cd gps`
3. `make`

## Steps to Run GPS
1. Run `./gps` (may need sudo permissions... looking into this)
2. Depending on when the GPS got its last fix (i.e. position), it may automatically perform a cold start. This means that it'll take about ~30 seconds to get a new fix. So don't be alarmed, if you aren't seeing anything within the first minute.

## Debugging Steps
The first step in debugging is to check if the GPS is actually getting a fix. You can do this through one of the command line tools (e.g. cgps, gpscat, gpsmon, gpspipe).
* If you are not getting a fix, check that the gpsd daemon is running:
  * `ps -ax | grep "gps"`
  * If the daemon is not running, execute the following to start the daemon:
    * `sudo gpsd /dev/ttyACM0 -F /var/run/gpsd.sock`
