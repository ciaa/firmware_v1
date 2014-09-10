/*! \page NetworkMgmtPage Device AddressClaim and Management of Information in Monitor Lists
\anchor SystemMgmtPage
\section NetworkMgmt Network Management
The IsoAgLib allows to claim address a local device (identity) with a single
function call at the BUS. Additionally it provides simple access to individual
information of other members, like source adress, name (64-bit ISO)
and device type. Thus an implement can simply check for available devices,
which could be used for optimisation of the own work process. In case more than
one implement of a device type is active, the IsoAgLib provides a simple
loop-search function, to check all implements of the searched type.

The IsoAgLib stores information of active BUS members in a monitor list,
so that the information retrieval is independent from the state messages
on the BUS.
This is also used for the address claim process, which results in the claim
of a free or static reserved source adress. The application can check all the time
for the activity and address claim state of the local own and remote devices.
This way the IsoAgLib provides selective access to devices which are currently
claiming address, or which has already claimed an address.

\subsection SystemMgmtPrjFiles Selection of Source Files for ISO 11783
The source files for system management are organised so that all protocol specific
stuff is placed in specific subdirectories. <br>
<b>Sources for any protocols</b><br>
- files in <tt>xgpl_src/IsoAgLib/comm/System</tt> (<b>no subdirectories</b>)
- everything in subdirectory <tt>xgpl_src/IsoAgLib/comm/System<b>/impl</b></tt>
<br>
<b>Sources for ISO 11783</b>
- everything in subdirectory <tt>xgpl_src/IsoAgLib/comm/System<b>/ISO11783</b></tt>
<br>
\subsection SystemMgmtDefines Project configuration for ISO 11783
As some parts in the IsoAgLib are dependent on the supported protocols, it is <b>very</b>
important to define the individual protocols to implement by some \#define's. This can
be achieved by the default settings in the central project configuration file
<tt>xgpl_src/IsoAgLib/isoaglib_config.h</tt> and by the compiler-defines in the Makefile.
The decision on the protocols is based on the availability of the following <tt>\#define</tt> clauses:
- <tt>\#define USE_ISO_11783</tt>
In case <tt>USE_xy</tt> is not defined, the config files
<tt>xgpl_src/IsoAgLib/isoaglib_config.h</tt> define a default setting.
Thus you can change the default rules for your individual needs in this file.

*/
