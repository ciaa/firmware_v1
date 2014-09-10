PCAN Driver
~~~~~~~~~~~

The driver is used in IsoAgLib at least for
  tools/can_server (make can_server_msq_pcan can_server_socket_pcan)
  bin/can_server_msq_pcan
  bin/can_server_socket_pcan

---------------------------------------

For Windows, since ISOAgLib 2.6.2 please download the driver from
  http://www.peak-system.com/fileadmin/media/files/pcan-basic.zip

and place all folders in this directory!

---------------------------------------

For Linux, please visit the website
  http://p103112.typo3server.info/fileadmin/media/linux/index.htm

and download/build/install the regarding driver for your interface.
For building the PCAN-driver, be sure to have the kernel headers
installed and linked to /usr/src/linux (e.g.
ln -s /usr/src/linux-headers-2.6.31-19-generic/ /usr/src/linux).

Pay attention if you have a kernel with SocketCAN support or not.
If you have, be sure to build the driver with
	make NET=NO

Install the driver with
	modprobe pcan

(If you can send but not receive, you have a built a SocketCAN variant)
