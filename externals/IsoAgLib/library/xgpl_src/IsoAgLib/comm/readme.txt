/** \page CommOverPage Communication Protocol Services Overview
The implementation of all standardized protocol services is located in the directory
<tt>xgpl_src/IsoAgLib/comm/</tt> and its subdirectories. The directories are structured after the different parts of the <a href="http://www.isobus.com">ISOBUS</a> standard.
Those are at the moment: <tt>Part3_DataLink</tt>, <tt>Part5_NetworkManagement</tt>, <tt>Part6_VirtualTerminal_Client</tt>,
<tt>Part7_ApplicationLayer</tt>, <tt>Part7_ProcessData</tt>, <tt>Part10_TaskController_Client</tt> as well as <tt>Scheduler</tt> and <tt>ext</tt>.

\section ServiceDependency Dependencies of Protocol Services
Most of the protocol features can be selected independent from other functions for
a specific project. The dependencies are described in the following overview diagram. The file storage sheme of the services is also 
shown by the analogous distribution in the diagram.

  <img src="images/commOverview.png" alt="Communication Protocol Overview" title="Communication Protocol Overview">
  For details check the information on the individual item pages:
  <ul ><li> \ref SchedulerPage
  </li><li> \ref NetworkMgmtPage
  </li><li> \ref AppLayerPage
  </li><li> \ref DataLinkPage
  </li><li> \ref VTClient
  </li><li> \ref ProcDataPage
  </li></ul>

<!--\dot
  digraph CommServDepend {
    fontname=ARIALN;
    fontpath="/usr/X11R6/lib/X11/fonts/drakfont/ttf:/usr/share/fonts/ttf:/usr/X11R6/lib/X11/fonts/TTF";
    node     [/*shape=record, */fontname=ARIALN, fontsize=8, style=filled, fontcolor=blue, color=greenyellow];
    edge     [fontname=ARIALN, fontsize=8, dir="back"];

		can         [ label="CAN Communication" ];
		eeprom      [ label="EEPROM" ];
		root        [ label="Communication Services ISO 11783" ];
		scheduler   [ label="Scheduler\nFunction: Coordination of all services", URL="\ref SchedulerPage" ];
		network_mgmt [ label="Network Management (Part 5)\nFunction: Monitor lists of all nodes on network", URL="\ref NetworkMgmtPage" ];
		app_layer   [ label="Application Layer (Part 7)\nFunction: Periodically sent base information mostly tractor", URL="\ref AppLayerPage" ];
		data_link   [ label="Data Link (Part 3)\nFunction: Send stream of more than 8-Byte", URL="\ref DataLinkPage" ];
		vt_client   [ label="Virtual Terminal Client (Part 6)\nFunction: Upload, layout and control of virtual terminal", URL="\ref VTClient" ];
		process     [ label="Process Data (Part 7)\nFunction: Process-Data", URL="\ref ProcDataPage" ];

		can         -> root ;
		root        -> scheduler ;
		scheduler   -> network_mgmt ;
		network_mgmt -> app_layer ;
		network_mgmt -> data_link ;
		network_mgmt -> process ;
		eeprom      -> process [ label="store and reload accumulated values" ];
		network_mgmt -> data_link ;
		data_link   -> vt_client ;

  }
\enddot-->


\section CommOverLbs Overview on Scheduler
<b>Detailed Description:</b> \ref SchedulerPage<br>
The class Scheduler_c works as a central scheduler and data coordinator for all \isoaglib components.
The application has mostly only contact to the main scheduling function IsoAgLib::iScheduler_c::timeEvent
which is responsible to dedicate periodically execution time to all components with the
guarantee to return at the requested time.

\section CommOverIsoSystem Overview on ISO 11783 Monitor List
<b>Detailed Description:</b> \ref NetworkMgmtPage<br>
The class IsoAgLib::iIsoMonitor_c has a monitor list of all ISO 11783 network nodes.
It is mostly used by the application to search for a device by its device type - and to get its SA.
It is also used internally to claim address and manage local ISO 11783 identities \ref IsoAgLib::iIsoItem_c.

\section CommOverIsoTerminal Overview on ISO 11783 Virtual Terminal
<b>Detailed Description:</b> \ref VTClient<br>
 provides a complete toolchain for user interface management based
on ISO 11783 virtual terminal. The layout with including definition of handles
for data exchange and display control is based on XML. This approach is comparable
to user interface resource description of other PC operating systems.
The individual DTD data type description for ISO 11783 in \isoaglib is
described in \ref VTClient . The usage of this mechanism is best demonstrated with an
example like imiIsoMaskupload.cpp .
 contains the build utilities which are needed to generate the constant
variable array definitions which are used as data source to upload the object pool.
These utilities are tested with Linux, but as they use only some pure C libraries without
any GUI part, they should be easily ported to another development operating system.

\section CommOverIsoProc Overview on ISO Format
<b>Detailed Description:</b> \ref ProcDataPage<br>
 can automatically detect, based on the active protocol, how the
process data messages must be formatted. As ISO 11783 Process Data was at least till
Mid 2003 compatible to DIN 9684, the API is 100% independent from the protocol.<br>

<b>Possible Problems of Current ISO Activities:</b><br>
As the process data part of ISO 11783 is under heavy change,  can't guarantee
the compatibility for the final - whenever it comes - ISO 11783 release.<br><br>
<i>The main problem:</i><br>
The \isoaglib process data handling strategy is based on the object oriented approach, which
was defined by the creator of the corresponding DIN 9684 part. This is also comparable to
the ISO 11783 virtual terminal part.
This means, that each process data instance has a standard API for doing jobs like:
- setting a excat, minimum, maximum and percentual setpoint for control
- retrieving current setpoint exact, min, max, percentual) and measurement values
- access to derived measurement values like MIN, MAX, AVG which is best calculated at the measuring ECU,
  so that less data is sent on the BUS
- use of measurement programs to get automatic value updates
As soon as no standard - for all process data types useable - algorithm can be defined to
access all these functions,  has to try to use several builtin lookup tables
to connect them in a uniform way.
This would cause runtime and space overhead.
The other problem of this approach is the fact, that not all access services are created for
each process data, so that some ECUs which would for example only need to limit a process data
by setting a MAX setpoint, are then forced to send an exact setpoint.
This induces very tight dependencies, which aren't needed, and will probably cause resource conflicts.
Please contact <a href="mailto:a.spangler_at_osb-ag:de">Achim Spangler</a> if you have questions on the problems of the currently planned changes.
Please have also a look at the various tutorial examples which provide some initial insight in the topic,
and look at the topic \ref InfAgentNetwork .

*/
