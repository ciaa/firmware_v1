/** @page AppLayerPage Base Data handling
\anchor BaseDataPage
The \isoaglib stores the information of several base information,
so that they can be easily evaluated from the application, independent
from the time relation between CAN-receive time and access time.
Additionally the Base_c class can be configured to send,
these data types. In this case the application can update the
data for send, independent from the individual send time on BUS.
Thus the afford for implementation of the correct send intervals
is completely taken from application to the \isoaglib.

@section BaseDataTypeList Overview on the handled PGN data for ISO 11783
As the ISO 11783 specifies a lot of information types
which can be periodically sent, the \isoaglib has the following
classes:
- TimePosGps_c:
  - year/month/day/hour/minute/second from calendar setting
  - check if at least one calendar data string was received
- TracGeneral_c:
  - key switch state tractor
  - maximum power time of tractor [min]
  - last receive time of maintain power request
  - check for request of maintenance for:
    - ECU power
    - actuator power
    - implement in transport state
    - implement in park state
    - implement in work
  - draft force on front and rear hitch
  - lower link force of front and rear hitch

- TracMove_c:
  - front and rear hitch position
  - real and gear based driving speed of tractor
  - real and gear based driving distance of tractor

- TracPto_c:
  - front and rear PTO RPM
  - tractor engine RPM
  - activation state of front and rear PTO
  - activation state of 100 n/min feature of front and rear PTO
  - activation state of economy mode in front and rear PTO

- iTracAux_c:
  handles management of axiliary valves of tractor (control and information)
  
- iTracCert_c:
  handles certification data information

- iTracGuidance_c and iTracGuidanceCommand_c:
  handle the machine steering system and guidance system

- iTracLight_c:
  handles lightning information and control

- iTracMoveSetPoint_c:
  handles control of tractor moving parameters by implement

- iTracPTOSetPoint_c:
  handles control of PTO settings by implement


@section BaseDataPowerMaintenance Request of Power Maintenance
The \isoaglib provides the possibility to request the maintenance of tractor.

@section BaseDataSetting Setting of values for active sender
The functions for setting the base data values for send are
defined in relation to the corresponding reading functions.
If a read function is named speedReal(), the function to set the
current driving speed is named setSpeedReal( int16_t ).

*/
