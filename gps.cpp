/**
 * Author: Trevor Yu
 * Email: trevor.tyu@gmail.com
 * Date: October 24, 2015
 * Summary: Code to configure and read data from a GPS sensor.
 *
 * Code primarily taken from:
 * http://stackoverflow.com/questions/18108932/linux-c-serial-port-reading-writing
 */

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>

using namespace std;

int main( void ) {
   const char* USB_PATH = "/dev/ttyACM0";

   /* Open file descriptor */
   int USB = open( USB_PATH, O_RDWR | O_NOCTTY );

   /* Error handling */
   if ( USB < 0 ) {
      cout << "Error " << errno << " opening " << USB_PATH << ": " \
      << strerror( errno ) << endl;
   }

   /* Configure port */
   struct termios tty;
   memset( &tty, 0, sizeof tty );

   /* Error handling */
   if( tcgetattr( USB, &tty ) != 0 ) {
      cout << "Error " << errno << " from tcgetattr: " << \
      strerror( errno ) << endl;
   }

   /* Set baud rate speed */
   cfsetospeed( &tty, (speed_t)B115200 );
   cfsetispeed( &tty, (speed_t)B115200 );

   /* Set other port stuff */
   tty.c_cflag &= ~PARENB;
   tty.c_cflag &= ~CSTOPB;
   tty.c_cflag &= ~CSIZE;
   tty.c_cflag |= CS8;

   tty.c_cflag &= ~CRTSCTS;
   tty.c_cc[VMIN] = 1;
   tty.c_cc[VTIME] = 5;
   tty.c_cflag |= CREAD | CLOCAL;

   /* Make raw */
   cfmakeraw( &tty );

   /* Flush port, then apply attributes */
   tcflush( USB, TCIFLUSH );
   if( tcsetattr( USB, TCSANOW, &tty ) != 0 ) {
      cout << "Error " << errno << " from tcsetattr" << endl;
   }

   /**
    * Write logic (used to configure the GPS update rate).
    * The GPS uses the PMTK protocol to control and change
    * various attributes and settings.
    * Documentation for this protocol can be found:
    * https://www.adafruit.com/datasheets/PMTK_A08.pdf
    */

   /*char cmd[256];
   memset(cmd, 0, 256);*/

   const char *str = "$PMTK869,1,0*34\r\n";
   write( USB, str, sizeof( str ) - 1 );
   const char *str2 = "$PMTK220,200*2C\r\n";
   write( USB, str2, sizeof( str2 ) - 1 );

   /*char *ptr = cmd;
   *ptr++ = *str++;

   uint8_t crc = 0;
   uint32_t tx = 0;
   for(int i = 1; *str && i < (sizeof(cmd) - 4); i++) {
      crc ^= *str;
      *ptr++ = *str++;
      tx++;
   }
   sprintf(ptr, "*%02X", crc);
   tx += 5; // '*XX<CR><LF>

   write( USB, ptr, sizeof( ptr ) );*/

   /* Read stuff */
   int n = 0,
       spot = 0;
   char buf = '\0';

   /* Whole response */
   char response[1024];
   memset( response, '\0', sizeof( response ) );

   while ( true ) {
      spot = 0;
      do {
         n = read( USB, &buf, 1 );
         if ( buf == '\n' ) {
            continue;
         }
         sprintf( &response[spot], "%c", buf );
         spot += n;
      } while( buf != '\r' && n > 0 );

      if ( n < 0 ) {
         std::cout << "Error reading: " << strerror( errno ) << std::endl;
      } else if ( n == 0 ) {
         std::cout << "Read nothing!" << std::endl;
      } else {
         std::cout << "Response: " << response << std::endl;
      }
   }

   return 0;
}
