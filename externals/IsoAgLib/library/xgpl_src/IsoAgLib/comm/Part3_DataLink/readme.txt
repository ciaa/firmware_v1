/*!
\page DataLinkPage Multi-Packet Messages with ISO 11783 (Extended) Transport Protocol

ISO 11783 defines extensions to send/receive data streams of more than 8 Data Bytes,
which don't fit into one CAN packet.

Transport Protocol handles 9..1785 bytes whereas Extended Transport Protocol handles
messages even larger than 1785 bytes.

The handling in detail is not relevant for the application-programmer, because it is
abstracted by IsoAgLib and its several modules. It's function is only of interest for
ISOAgLib-internal-developers. (For examples of how to use (E)TP, those developers should
refer to the modules' internal use.)
  
*/
