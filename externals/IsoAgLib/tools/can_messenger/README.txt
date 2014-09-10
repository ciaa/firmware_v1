can_server_messenger
--------------------

Martin Wodok <m.wodok@osb-ag.de>

Testtool to inject CAN packages in a running can_server_bus.

Usage:

can_server_messenger <options>

options are:
  -i <CAN-Identifier as HEX>
  -n <Repeat-Count>
  -c <CAN-Channel>
  -d <Databytes (up to 8) as HEX> (for compatibility also -s can be used)
  -p <Period in ms>
  -x use eXtended Identifier

Example: CanServerMessenger -x -n 1 -c 0 -i 1ceafffe -d a1b2c3d4e5f6affe
