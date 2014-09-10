std::string
interpreteWheelBasedSpeedDist( PtrDataFrame_t at_ptrFrame )
{
  if (at_ptrFrame->dataSize() != 8)
    return "*** ILLEGAL - THIS PGN *MUST* HAVE 8 DATABYTES ***";

  std::ostringstream out;

  // Wheel-based implement speed
  out << std::dec << "WBISp: " << ((at_ptrFrame->dataOctet(1) << 8) | at_ptrFrame->dataOctet(0)) * 0.001 << " m/s ";

  // Wheel-based distance
  out << "WBIDs: " << static_cast<uint32_t>( (static_cast<uint32_t>(at_ptrFrame->dataOctet(5)) << 24)
                                                 | (static_cast<uint32_t>(at_ptrFrame->dataOctet(4)) << 16)
                                                 | (static_cast<uint32_t>(at_ptrFrame->dataOctet(3)) << 8)
                                                 |  static_cast<uint32_t>(at_ptrFrame->dataOctet(2))
                                                 ) * 0.001 << " m ";

  // Maximum time of tractor power
  out << "MaxTimeTP: " << static_cast<uint16_t>(at_ptrFrame->dataOctet(6)) << " min ";

  // Operator direction reversed
  switch (at_ptrFrame->dataOctet(7) >> 6) {
      case 0x0: out << "Operator direction: NotReversed "; break;
      case 0x1: out << "Operator direction: Reversed "; break;
      case 0x2: out << "Operator direction: Error "; break;
      case 0x3: out << "Operator direction: Not available "; break;
  }

  // Start/Stop state
  switch ((at_ptrFrame->dataOctet(7) >> 4) & 0x03) {
    case 0x0: out << "St/Sp state: stop "; break;
    case 0x1: out << "St/Sp state: start "; break;
    case 0x2: out << "St/Sp state: error "; break;
    case 0x3: out << "St/Sp state: n/a "; break;
  }

  // Key switch state
  switch ((at_ptrFrame->dataOctet(7) >> 2) & 0x03) {
    case 0x0: out << "Key sws: off "; break;
    case 0x1: out << "Key sws: not off "; break;
    case 0x2: out << "Key sws: error "; break;
    case 0x3: out << "Key sws: n/a "; break;
  }

  // Ground based direction
  switch (at_ptrFrame->dataOctet(7) & 0x03) {
    case 0x0: out << "WB Dir: Reverse "; break;
    case 0x1: out << "WB Dir: Forward "; break;
    case 0x2: out << "WB Dir: Error "; break;
    case 0x3: out << "WB Dir: N/A "; break;
  }

  return out.str();
}


std::string
interpreteGroundBasedSpeedDist( PtrDataFrame_t at_ptrFrame )
{
  if (at_ptrFrame->dataSize() != 8)
    return "*** ILLEGAL - THIS PGN *MUST* HAVE 8 DATABYTES ***";

  std::ostringstream out;

  // Wheel-based implement speed
  out << std::dec << "GBISp: " << ((at_ptrFrame->dataOctet(1) << 8) | at_ptrFrame->dataOctet(0)) * 0.001 << " m/s ";

  // Wheel-based distance
  out << "GBIDs: " << static_cast<uint32_t>( (static_cast<uint32_t>(at_ptrFrame->dataOctet(5)) << 24)
                                                 | (static_cast<uint32_t>(at_ptrFrame->dataOctet(4)) << 16)
                                                 | (static_cast<uint32_t>(at_ptrFrame->dataOctet(3)) << 8)
                                                 |  static_cast<uint32_t>(at_ptrFrame->dataOctet(2))
                                                 ) * 0.001 << " m ";

  // Byte 7 reserved.

  // Bit 8 to 3 reserved

  // Ground based direction
  switch (at_ptrFrame->dataOctet(7) & 0x03) {
    case 0x0: out << "GB Dir: Reverse "; break;
    case 0x1: out << "GB Dir: Forward "; break;
    case 0x2: out << "GB Dir: Error "; break;
    case 0x3: out << "GB Dir: N/A "; break;
  }

  return out.str();
}

std::string
interpreteMaintainPower( PtrDataFrame_t at_ptrFrame )
{
  if (at_ptrFrame->dataSize() != 8)
    return "*** ILLEGAL - THIS PGN *MUST* HAVE 8 DATABYTES ***";
  std::ostringstream out;
  switch ((at_ptrFrame->dataOctet(0) >> 6) & 0x03) {
      case 0x0: out << " No req for ECU_PWR "; break;
      case 0x1: out << " 2s more of ECU_PWR "; break;
      case 0x2: out << " Reserved "; break;
      case 0x3: out << " Don't care "; break;
  }
  switch ((at_ptrFrame->dataOctet(0) >> 4) & 0x03) {
      case 0x0: out << " No req for ACT_PWR "; break;
      case 0x1: out << " 2s more of ACT_PWR "; break;
      case 0x2: out << " Reserved "; break;
      case 0x3: out << " Don't care "; break;
  }
  switch ((at_ptrFrame->dataOctet(1) >> 6) & 0x03) {
      case 0x0: out << "tp state: Impl may not be transported "; break;
      case 0x1: out << "tp state: Impl may be transported "; break;
      case 0x2: out << "tp state: Error "; break;
      case 0x3: out << "tp state: N/A "; break;
  }
  switch ((at_ptrFrame->dataOctet(1) >> 4) & 0x03) {
      case 0x0: out << "park state: Impl may not be disconnected "; break;
      case 0x1: out << "park state: Impl may be disconnected "; break;
      case 0x2: out << "park state: Error "; break;
      case 0x3: out << "park state: N/A "; break;
  }
  switch ((at_ptrFrame->dataOctet(1) >> 2) & 0x03) {
      case 0x0: out << "work state: Impl is not ready for field work "; break;
      case 0x1: out << "work state: Impl is ready for field work "; break;
      case 0x2: out << "work state: Error "; break;
      case 0x3: out << "work state: N/A "; break;
  }
  // Bytes 3 to 8 are reserved
  return out.str();
}

std::string
interpreteRearPTOstate( PtrDataFrame_t at_ptrFrame )
{
  if (at_ptrFrame->dataSize() != 8)
    return "*** ILLEGAL - THIS PGN *MUST* HAVE 8 DATABYTES ***";

  std::ostringstream out;

  // Rear PTO output shaft speed
  out << std::dec << "Rear PTO output shaft speed: " << static_cast<uint16_t>( (static_cast<uint16_t>(at_ptrFrame->dataOctet(1)) << 8) | static_cast<uint16_t>(at_ptrFrame->dataOctet(0)) ) * 0.125 << " 1/min ";

  // Rear PTO output shaft speed setpoint
  out << std::dec << "Rear PTO output shaft speed setpoint: " << static_cast<uint16_t>( (static_cast<uint16_t>(at_ptrFrame->dataOctet(3)) << 8) | static_cast<uint16_t>(at_ptrFrame->dataOctet(2)) ) * 0.125 << " 1/min ";

  // Rear PTO engagement
  switch (at_ptrFrame->dataOctet(4) >> 6) {
      case 0x0: out << "RPTO eng: disen "; break;
      case 0x1: out << "RPTO eng: en "; break;
      case 0x2: out << "RPTO eng: Error "; break;
      case 0x3: out << "RPTO eng: n/a "; break;
  }

  // Rear PTO mode
  switch ((at_ptrFrame->dataOctet(4) >> 4) & 0x03) {
    case 0x0: out << "RPTO mod: 540 "; break;
    case 0x1: out << "RPTO mod: 1000 "; break;
    case 0x2: out << "RPTO mod: Error "; break;
    case 0x3: out << "RPTO mod: n/a "; break;
  }

  // Rear PTO economy mode
  switch ((at_ptrFrame->dataOctet(4) >> 2) & 0x03) {
    case 0x0: out << "RPTO ec mod: disen "; break;
    case 0x1: out << "RPTO ec mod: en "; break;
    case 0x2: out << "RPTO ec mod: error "; break;
    case 0x3: out << "RPTO ec mod: n/a "; break;
  }

  // Rear PTO mode request status
  switch (at_ptrFrame->dataOctet(4) & 0x03) {
    case 0x0: out << "RPTO mod rq st: disen "; break;
    case 0x1: out << "RPTO mod rq st: en "; break;
    case 0x2: out << "RPTO mod rq st: Error "; break;
    case 0x3: out << "RPTO mod rq st: N/A "; break;
  }

  // Rear PTO mode
  switch (at_ptrFrame->dataOctet(5) >> 6) {
      case 0x0: out << "RPTO mode: external request "; break;
      case 0x1: out << "RPTO mode: controll override "; break;
      case 0x2: out << "RPTO mode: Error "; break;
      case 0x3: out << "RPTO mode: n/a "; break;
  }

  // Rear PTO Economy mode request status
  switch ((at_ptrFrame->dataOctet(5) >> 4) & 0x03) {
    case 0x0: out << "RPTO economy mode request status: external request "; break;
    case 0x1: out << "RPTO economy mode request status: controll override "; break;
    case 0x2: out << "RPTO economy mode request status: Error "; break;
    case 0x3: out << "RPTO economy mode request status: n/a "; break;
  }

  // Rear PTO shaft speed limit status
  switch ((at_ptrFrame->dataOctet(5) >> 1) & 0x07) {
    case 0x0: out << "RPTO shaft speed limit status: no limit "; break;
    case 0x1: out << "RPTO shaft speed limit status: op limit "; break;
    case 0x2: out << "RPTO shaft speed limit status: lim high "; break;
    case 0x3: out << "RPTO shaft speed limit status: lim low "; break;
    case 0x4: out << "RPTO shaft speed limit status: reserved "; break;
    case 0x5: out << "RPTO shaft speed limit status: reserved "; break;
    case 0x6: out << "RPTO shaft speed limit status: nr fault "; break;
    case 0x7: out << "RPTO shaft speed limit status: n/a "; break;
  }

  // Bits 1 is reserved
  // Bytes 7 and 8 are reserved

  return out.str();
}


std::string
interpreteRearHitch( PtrDataFrame_t at_ptrFrame )
{
  if (at_ptrFrame->dataSize() != 8)
    return "*** ILLEGAL - THIS PGN *MUST* HAVE 8 DATABYTES ***";

  std::ostringstream out;

  // Rear hitch position in %
  out << std::dec << "RearHitchPos: " << at_ptrFrame->dataOctet(0) * 0.4 << "% ";

  // Rear hith in-work indication.
  switch (at_ptrFrame->dataOctet(1) >> 6) {
      case 0x0: out << "RH in-wo ind: out-of-work "; break;
      case 0x1: out << "RH in-wo ind: in-work "; break;
      case 0x2: out << "RH in-wo ind: error "; break;
      case 0x3: out << "RH in-wo ind: n/a "; break;
  }

  // Rear hitch position limit status
  switch ((at_ptrFrame->dataOctet(1) >> 3) & 0x07) {
    case 0x0: out << "RH pos lim st: no limit "; break;
    case 0x1: out << "RH pos lim st: op limit "; break;
    case 0x2: out << "RH pos lim st: lim high "; break;
    case 0x3: out << "RH pos lim st: lim low "; break;
    case 0x4: out << "RH pos lim st: reserved "; break;
    case 0x5: out << "RH pos lim st: reserved "; break;
    case 0x6: out << "RH pos lim st: nr fault "; break;
    case 0x7: out << "RH pos lim st: n/a "; break;
  }

  // Rear nominal lover link force
  out << std::dec << "Rear nom low link force: " << int8_t(at_ptrFrame->dataOctet(2)) * 0.8 << "% ";

  // Rear draft
  out << std::dec << "Rear draft: " << static_cast<uint16_t>( (static_cast<uint16_t>(at_ptrFrame->dataOctet(4)) << 8) | static_cast<uint16_t>(at_ptrFrame->dataOctet(3)) ) * 10 - 320000 << "N ";

  // Bits 6 to 8 are reserved

  return out.str();
}


std::string
interpretePgnLanguage( PtrDataFrame_t at_ptrFrame )
{
  if (at_ptrFrame->dataSize() != 8)
    return "*** ILLEGAL - THIS PGN *MUST* HAVE 8 DATABYTES ***";

  std::ostringstream out;

  // Language Code
  out << "LC:" << at_ptrFrame->asciiDataOctet(0) << at_ptrFrame->asciiDataOctet(1) << " ";

  // Decimal Point
  switch (at_ptrFrame->dataOctet(2) >> 6) {
    case 0x0: out << "dp:, "; break;
    case 0x1: out << "dp:. "; break;
    case 0x2: out << "dp:R "; break;
    case 0x3: out << "dp:N "; break;
  }
  // Time Format
  switch ((at_ptrFrame->dataOctet(2) >> 4) & 0x03) {
    case 0x0: out << "t:24 "; break;
    case 0x1: out << "t:12 "; break;
    case 0x2: out <<"t:R  "; break;
    case 0x3: out << "t:N  "; break;
  }
  // Date Format
  switch (at_ptrFrame->dataOctet(3)) {
    case 0x0: out << "d:ddmmyyyy "; break;
    case 0x1: out << "d:ddyyyymm "; break;
    case 0x2: out << "d:mmyyyydd "; break;
    case 0x3: out << "d:mmddyyyy "; break;
    case 0x4: out << "d:yyyymmdd "; break;
    case 0x5: out << "d:yyyyddmm "; break;
    default:  out << "d:UNSPECIF "; break;
  }
  // Distance
  switch ((at_ptrFrame->dataOctet(4) >> 6) & 0x3) {
    case 0x0: out << "d:M  "; break;
    case 0x1: out << "d:IU "; break;
    case 0x2: out << "d:R  "; break;
    case 0x3: out << "d:N  "; break;
  }
  // Area
  switch ((at_ptrFrame->dataOctet(4) >> 4) & 0x3) {
    case 0x0: out << "a:M  "; break;
    case 0x1: out << "a:IU "; break;
    case 0x2: out << "a:R  "; break;
    case 0x3: out << "a:N  "; break;
  }
  // Volume
  switch ((at_ptrFrame->dataOctet(4) >> 2) & 0x3) {
    case 0x0: out << "v:M "; break;
    case 0x1: out << "v:I "; break;
    case 0x2: out << "v:U "; break;
    case 0x3: out << "v:N "; break;
  }
  // Mass
  switch ((at_ptrFrame->dataOctet(4) >> 0) & 0x3) {
    case 0x0: out << "m:M "; break;
    case 0x1: out << "m:I "; break;
    case 0x2: out << "m:U "; break;
    case 0x3: out << "m:N "; break;
  }
  // Temperature
  switch ((at_ptrFrame->dataOctet(5) >> 6) & 0x3) {
    case 0x0: out << "t:M  "; break;
    case 0x1: out << "t:IU "; break;
    case 0x2: out << "t:R  "; break;
    case 0x3: out << "t:N  "; break;
  }
  // Pressure
  switch ((at_ptrFrame->dataOctet(5) >> 4) & 0x3) {
    case 0x0: out << "p:M  "; break;
    case 0x1: out << "p:IU "; break;
    case 0x2: out << "p:R  "; break;
    case 0x3: out << "p:N  "; break;
  }
  // Force
  switch ((at_ptrFrame->dataOctet(5) >> 2) & 0x3) {
    case 0x0: out << "f:M  "; break;
    case 0x1: out << "f:IU "; break;
    case 0x2: out << "f:R  "; break;
    case 0x3: out << "f:N  "; break;
  }
  // System
  switch ((at_ptrFrame->dataOctet(5) >> 0) & 0x3) {
    case 0x0: out << "s:M "; break;
    case 0x1: out << "s:I "; break;
    case 0x2: out << "s:U "; break;
    case 0x3: out << "s:N "; break;
  }
  return out.str();
}


std::string 
interpreteLightingCommand( PtrDataFrame_t at_ptrFrame )
{
  if (at_ptrFrame->dataSize() != 8)
    return "*** ILLEGAL - THIS PGN *MUST* HAVE 8 DATABYTES ***";

  std::ostringstream out;

  // Tractor high-beam head lights
  switch ((at_ptrFrame->dataOctet(0) >> 6) & 0x3) {
    case 0x0: out << "Tractor high-beam head lights: deactivate "; break;
    case 0x1: out << "Tractor high-beam head lights: activate "; break;
    case 0x2: out << "Tractor high-beam head lights: reserve "; break;
    case 0x3: out << "Tractor high-beam head lights: don't care "; break;
  }
  // Tractor low-beam head lights
  switch ((at_ptrFrame->dataOctet(0) >> 4) & 0x3) {
    case 0x0: out << "Tractor low-beam head lights: deactivate "; break;
    case 0x1: out << "Tractor low-beam head lights: activate "; break;
    case 0x2: out << "Tractor low-beam head lights: reserve "; break;
    case 0x3: out << "Tractor low-beam head lights: don't care "; break;
  }
  // Tractor alternate head lights
  switch ((at_ptrFrame->dataOctet(0) >> 2) & 0x3) {
    case 0x0: out << "Tractor alternate head lights: deactivate "; break;
    case 0x1: out << "Tractor alternate head lights: activate "; break;
    case 0x2: out << "Tractor alternate head lights: reserve "; break;
    case 0x3: out << "Tractor alternate head lights: don't care "; break;
  }
  // Daytime running lights
  switch ((at_ptrFrame->dataOctet(0) >> 0) & 0x3) {
    case 0x0: out << "Daytime running lights: deactivate "; break;
    case 0x1: out << "Daytime running lights: activate "; break;
    case 0x2: out << "Daytime running lights: reserve "; break;
    case 0x3: out << "Daytime running lights: don't care "; break;
  }

  // Left-turn signal lights
  switch ((at_ptrFrame->dataOctet(1) >> 6) & 0x3) {
    case 0x0: out << "Left-turn signal lights: deactivate "; break;
    case 0x1: out << "Left-turn signal lights: activate "; break;
    case 0x2: out << "Left-turn signal lights: reserve "; break;
    case 0x3: out << "Left-turn signal lights: don't care "; break;
  }
  // Right-turn signal lights
  switch ((at_ptrFrame->dataOctet(1) >> 4) & 0x3) {
    case 0x0: out << "Right-turn signal lights: deactivate "; break;
    case 0x1: out << "Right-turn signal lights: activate "; break;
    case 0x2: out << "Right-turn signal lights: reserve "; break;
    case 0x3: out << "Right-turn signal lights: don't care "; break;
  }
  // Beacon lights
  switch ((at_ptrFrame->dataOctet(1) >> 2) & 0x3) {
    case 0x0: out << "Beacon lights: deactivate "; break;
    case 0x1: out << "Beacon lights: activate "; break;
    case 0x2: out << "Beacon lights: reserve "; break;
    case 0x3: out << "Beacon lights: don't care "; break;
  }
  // Front fog lights
  switch ((at_ptrFrame->dataOctet(1) >> 0) & 0x3) {
    case 0x0: out << "Front fog: deactivate "; break;
    case 0x1: out << "Front fog: activate "; break;
    case 0x2: out << "Front fog: reserve "; break;
    case 0x3: out << "Front fog: don't care "; break;
  }

  // Left stop lights
  switch ((at_ptrFrame->dataOctet(2) >> 6) & 0x3) {
    case 0x0: out << "Left stop lights: deactivate "; break;
    case 0x1: out << "Left stop lights: activate "; break;
    case 0x2: out << "Left stop lights: reserve "; break;
    case 0x3: out << "Left stop lights: don't care "; break;
  }
  // Right stop lights
  switch ((at_ptrFrame->dataOctet(2) >> 4) & 0x3) {
    case 0x0: out << "Right stop lights: deactivate "; break;
    case 0x1: out << "Right stop lights: activate "; break;
    case 0x2: out << "Right stop lights: reserve "; break;
    case 0x3: out << "Right stop lights: don't care "; break;
  }
  // Center stop lights
  switch ((at_ptrFrame->dataOctet(2) >> 2) & 0x3) {
    case 0x0: out << "Center stop lights: deactivate "; break;
    case 0x1: out << "Center stop lights: activate "; break;
    case 0x2: out << "Center stop lights: reserve "; break;
    case 0x3: out << "Center stop lights: don't care "; break;
  }
  // Back up light and alarm horn
  switch ((at_ptrFrame->dataOctet(2) >> 0) & 0x3) {
    case 0x0: out << "Back up light and alarm horn: deactivate "; break;
    case 0x1: out << "Back up light and alarm horn: activate "; break;
    case 0x2: out << "Back up light and alarm horn: reserve "; break;
    case 0x3: out << "Back up light and alarm horn: don't care "; break;
  }

  // Tractor marker lights
  switch ((at_ptrFrame->dataOctet(3) >> 6) & 0x3) {
    case 0x0: out << "Tractor marker lights: deactivate "; break;
    case 0x1: out << "Tractor marker lights: activate "; break;
    case 0x2: out << "Tractor marker lights: reserve "; break;
    case 0x3: out << "Tractor marker lights: don't care "; break;
  }
  // Implement marker lights
  switch ((at_ptrFrame->dataOctet(3) >> 4) & 0x3) {
    case 0x0: out << "Implement marker lights: deactivate "; break;
    case 0x1: out <<"Implement marker lights: activate "; break;
    case 0x2: out << "Implement marker lights: reserve "; break;
    case 0x3: out << "Implement marker lights: don't care "; break;
  }
  // Tractor clearance lights
  switch ((at_ptrFrame->dataOctet(3) >> 2) & 0x3) {
    case 0x0: out << "Tractor clearance lights: deactivate "; break;
    case 0x1: out << "Tractor clearance lights: activate "; break;
    case 0x2: out << "Tractor clearance lights: reserve "; break;
    case 0x3: out << "Tractor clearance lights: don't care "; break;
  }
  // Implement clearance lights
  switch ((at_ptrFrame->dataOctet(3) >> 0) & 0x3) {
    case 0x0: out << "Implement clearance lights: deactivate "; break;
    case 0x1: out << "Implement clearance lights: activate "; break;
    case 0x2: out << "Implement clearance lights: reserve "; break;
    case 0x3: out << "Implement clearance lights: don't care "; break;
  }

  // Tractor rear hight mounted work lights
  switch ((at_ptrFrame->dataOctet(4) >> 6) & 0x3) {
    case 0x0: out << "Tractor rear hight mounted work lights: deactivate "; break;
    case 0x1: out << "Tractor rear hight mounted work lights: activate "; break;
    case 0x2: out << "Tractor rear hight mounted work lights: reserve "; break;
    case 0x3: out << "Tractor rear hight mounted work lights: don't care "; break;
  }
  // Tractor rear low mounted work lights
  switch ((at_ptrFrame->dataOctet(4) >> 4) & 0x3) {
    case 0x0: out << "Tractor rear low mounted work lights: deactivate "; break;
    case 0x1: out << "Tractor rear low mounted work lights: activate "; break;
    case 0x2: out << "Tractor rear low mounted work lights: reserve "; break;
    case 0x3: out << "Tractor rear low mounted work lights: don't care "; break;
  }
  // Tractor underside work lights
  switch ((at_ptrFrame->dataOctet(4) >> 2) & 0x3) {
    case 0x0: out << "Tractor underside work lights: deactivate "; break;
    case 0x1: out << "Tractor underside work lights: activate "; break;
    case 0x2: out << "Tractor underside work lights: reserve "; break;
    case 0x3: out << "Tractor underside work lights: don't care "; break;
  }
  // Rear fog light
  switch ((at_ptrFrame->dataOctet(4) >> 0) & 0x3) {
    case 0x0: out << "Rear fog light: deactivate "; break;
    case 0x1: out << "Rear fog light: activate "; break;
    case 0x2: out << "Rear fog light: reserve "; break;
    case 0x3: out << "Rear fog light: don't care "; break;
  }

  // Tractor front high-mounted work lights
  switch ((at_ptrFrame->dataOctet(5) >> 6) & 0x3) {
    case 0x0: out << "Tractor front high-mounted work lights: deactivate "; break;
    case 0x1: out << "Tractor front high-mounted work lights: activate "; break;
    case 0x2: out << "Tractor front high-mounted work lights: reserve "; break;
    case 0x3: out << "Tractor front high-mounted work lights: don't care "; break;
  }
  // Tractor front low-mounted work lights
  switch ((at_ptrFrame->dataOctet(5) >> 4) & 0x3) {
    case 0x0: out << "Tractor front low-mounted work lights: deactivate "; break;
    case 0x1: out << "Tractor front low-mounted work lights: activate "; break;
    case 0x2: out << "Tractor front low-mounted work lights: reserve "; break;
    case 0x3: out << "Tractor front low-mounted work lights: don't care "; break;
  }
  // Tractor side high-mounted work lights
  switch ((at_ptrFrame->dataOctet(5) >> 2) & 0x3) {
    case 0x0: out << "Tractor side high-mounted work lights: deactivate "; break;
    case 0x1: out << "Tractor side high-mounted work lights: activate "; break;
    case 0x2: out << "Tractor side high-mounted work lights: reserve "; break;
    case 0x3: out << "Tractor side high-mounted work lights: don't care "; break;
  }
  // Tractor side low-mounted work lights
  switch ((at_ptrFrame->dataOctet(5) >> 0) & 0x3) {
    case 0x0: out << "Tractor side low-mounted work lights: deactivate "; break;
    case 0x1: out << "Tractor side low-mounted work lights: activate "; break;
    case 0x2: out << "Tractor side low-mounted work lights: reserve "; break;
    case 0x3: out << "Tractor side low-mounted work lights: don't care "; break;
  }

  // Implement left forward work lights
  switch ((at_ptrFrame->dataOctet(6) >> 6) & 0x3) {
    case 0x0: out << "Implement left forward work lights: deactivate "; break;
    case 0x1: out << "Implement left forward work lights: activate "; break;
    case 0x2: out << "Implement left forward work lights: reserve "; break;
    case 0x3: out << "Implement left forward work lights: don't care "; break;
  }
  // Implement right forward work lights
  switch ((at_ptrFrame->dataOctet(6) >> 4) & 0x3) {
    case 0x0: out << "Implement right forward work lights: deactivate "; break;
    case 0x1: out << "Implement right forward work lights: activate "; break;
    case 0x2: out << "Implement right forward work lights: reserve "; break;
    case 0x3: out << "Implement right forward work lights: don't care "; break;
  }
  // Implement OEM option 1 light
  switch ((at_ptrFrame->dataOctet(6) >> 2) & 0x3) {
    case 0x0: out << "Implement OEM option 1 light: deactivate "; break;
    case 0x1: out << "Implement OEM option 1 light: activate "; break;
    case 0x2: out << "Implement OEM option 1 light: reserve "; break;
    case 0x3: out << "Implement OEM option 1 light: don't care "; break;
  }
  // Implement OEM option 2 light
  switch ((at_ptrFrame->dataOctet(6) >> 0) & 0x3) {
    case 0x0: out << "Implement OEM option 2 light: deactivate "; break;
    case 0x1: out << "Implement OEM option 2 light: activate "; break;
    case 0x2: out << "Implement OEM option 2 light: reserve "; break;
    case 0x3: out << "Implement OEM option 2 light: don't care "; break;
  }

  // Implement rear work lights
  switch ((at_ptrFrame->dataOctet(7) >> 6) & 0x3) {
    case 0x0: out << "Implement rear work lights: deactivate "; break;
    case 0x1: out << "Implement rear work lights: activate "; break;
    case 0x2: out << "Implement rear work lights: reserve "; break;
    case 0x3: out << "Implement rear work lights: don't care "; break;
  }
  // Implement left-facing work lights
  switch ((at_ptrFrame->dataOctet(7) >> 4) & 0x3) {
    case 0x0: out << "Implement left-facing work lights: deactivate "; break;
    case 0x1: out << "Implement left-facing work lights: activate "; break;
    case 0x2: out << "Implement left-facing work lights: reserve "; break;
    case 0x3: out << "Implement left-facing work lights: don't care "; break;
  }
  // Implement right-facing work lights
  switch ((at_ptrFrame->dataOctet(7) >> 2) & 0x3) {
    case 0x0: out << "Implement right-facing work lights: deactivate "; break;
    case 0x1: out << "Implement right-facing work lights: activate "; break;
    case 0x2: out << "Implement right-facing work lights: reserve "; break;
    case 0x3: out << "Implement right-facing work lights: don't care "; break;
  }
  // Lighting data message request
  switch ((at_ptrFrame->dataOctet(7) >> 0) & 0x3) {
    case 0x0: out << "Lighting data message request: deactivate "; break;
    case 0x1: out << "Lighting data message request: activate "; break;
    case 0x2: out << "Lighting data message request: reserve "; break;
    case 0x3: out << "Lighting data message request: don't care "; break;
  }

  return out.str();
}


std::string
interpreteEngineSpeedMsg( PtrDataFrame_t at_ptrFrame )
{
  if (at_ptrFrame->dataSize() != 8)
    return "*** ILLEGAL - THIS PGN *MUST* HAVE 8 DATABYTES ***";

  std::ostringstream out;

  // Engine speed
  out << "Engine speed:" << static_cast<uint16_t>( (static_cast<uint16_t>(at_ptrFrame->dataOctet(4)) << 8) | static_cast<uint16_t>(at_ptrFrame->dataOctet(3)) ) * 0.125 << " ";

  return out.str();
}



void handleVehiclePosition( uint64_t, double, double );
void handleVehicleSpeed( uint64_t, double, double, double, double );


std::string
interpreteVehicleSpeed( PtrDataFrame_t at_ptrFrame )
{
  if (at_ptrFrame->dataSize() != 8)
    return "*** ILLEGAL - THIS PGN *MUST* HAVE 8 DATABYTES ***";

  // Vehicle speed
  // J1939-71
  // 2 bytes Compass bearing 1/128 deg/bit, 0 offset, range 0 to 501.99deg
  // 2 bytes Naviagion based vehicle speed 1/256 km/h per bit, 0 offset, datarange 0 to 250.996 km/h
  // 2 bytes pitch 1/128 deg/bit, -200 deg offset, -200 to 301.99 deg
  // 2 bytes altitude 0.125 m/bit, -2500 m offset, -2500 to 5531.875 m
  std::ostringstream out;
  const double bearing = static_cast<double>( (static_cast<uint16_t>(at_ptrFrame->dataOctet(1)) << 8) | static_cast<uint16_t>(at_ptrFrame->dataOctet(0)) ) / 128;
  const double speedkmh = static_cast<double>( (static_cast<uint16_t>(at_ptrFrame->dataOctet(3)) << 8) | static_cast<uint16_t>(at_ptrFrame->dataOctet(2)) ) / 256;
  const double speedms = (static_cast<double>( (static_cast<uint16_t>(at_ptrFrame->dataOctet(3)) << 8) | static_cast<uint16_t>(at_ptrFrame->dataOctet(2)) ) * 10 ) / 9216;

  const double pitch = (static_cast<double>( (static_cast<uint16_t>(at_ptrFrame->dataOctet(5)) << 8) | static_cast<uint16_t>(at_ptrFrame->dataOctet(4)) ) / 128) - 200;
  const double altitude = (static_cast<double>( (static_cast<uint16_t>(at_ptrFrame->dataOctet(7)) << 8) | static_cast<uint16_t>(at_ptrFrame->dataOctet(6)) ) * 0.125 ) - 2500;

  handleVehicleSpeed( at_ptrFrame->time(), bearing, speedms, pitch, altitude );

  out << "Compass bearing:" << bearing << "° "
       << "Navigation based vehicle speed:" << speedkmh << " km/h "
       << "Pitch:" << pitch << "° "
       << "Altitude:" << altitude << " m ";
  
  return out.str();
}


std::string
interpreteVehiclePosition( PtrDataFrame_t at_ptrFrame )
{
  if (at_ptrFrame->dataSize() != 8)
    return "*** ILLEGAL - THIS PGN *MUST* HAVE 8 DATABYTES ***";

  // Vehicle position
  // J1939-71
  // 4 bytes Latitude  10^-7 deg/bit, -210 deg offset range from 10^-7 deg/bit, -210 deg offset
  // 4 bytes Longitude 10^-7 deg/bit, -210 deg offset range from 10^-7 deg/bit, -210 deg offset
  std::ostringstream out;
  const double lat = (static_cast<uint32_t>( (static_cast<uint32_t>(at_ptrFrame->dataOctet(3)) << 24)
                                           | (static_cast<uint32_t>(at_ptrFrame->dataOctet(2)) << 16)
                                           | (static_cast<uint32_t>(at_ptrFrame->dataOctet(1)) << 8)
                                           |  static_cast<uint32_t>(at_ptrFrame->dataOctet(0))
                                           ) * 0.0000001) - 210.;
  const double lon = (static_cast<uint32_t>( (static_cast<uint32_t>(at_ptrFrame->dataOctet(7)) << 24)
                                           | (static_cast<uint32_t>(at_ptrFrame->dataOctet(6)) << 16)
                                           | (static_cast<uint32_t>(at_ptrFrame->dataOctet(5)) << 8)
                                           |  static_cast<uint32_t>(at_ptrFrame->dataOctet(4))
                                           ) * 0.0000001) - 210.;

  handleVehiclePosition( at_ptrFrame->time(), lat, lon );

  out << "Latitude: " << lat << "° " << "Longitude: " << lon << "° ";
  return out.str();
}


/** For extend port measured flow, retract port measured flow, extend
  * port estimated flow, retract port estimated flow.
  */
inline int
tecu_portMeasuredOrEstimatedFlowPercent(uint8_t ui8_octet)
{
  return int(ui8_octet) - 125;
}


std::string
tecu_auxiliaryValveFailSaveModeMeasuredRepr(unsigned ui_2lsb)
{
  switch (ui_2lsb & 0x3u) {
  case 0u:
    return "block";
  case 1u:
    return "float";
  case 2u:
    return "error indication";
  case 3u:
    return "not available";
  default:
    return "(protocol error)";
  }
}


std::string
tecu_valveStateRepr(unsigned ui_4lsb)
{
  switch (ui_4lsb & 0xFu) {
  case 0u:
    return "blocked";
  case 1u:
    return "extend";
  case 2u:
    return "retract";
  case 3u:
    return "floating";
  default:
    return "reserved";
  case 14u:
    return "error indication";
  case 15u:
    return "not available";
  }
}


std::string
tecu_persistentLimitStatusRepr(unsigned ui_3lsb)
{
  switch (ui_3lsb & 0x7u) {
  case 0u:
    return "not limited";
  case 1u:
    return "operator limited/controlled";
  case 2u:
    return "limited high";
  case 3u:
    return "limited low";
  default:
    return "reserved";
  case 6u:
    return "non-recoverable fault";
  case 7u:
    return "not available";
  }
}


std::string
interpreteValveEstimatedFlow(PtrDataFrame_t at_ptrFrame)
{
  std::ostringstream out;

  // for different valves resp. different values of at_ptrFrame->pgn()
  out <<
    "Extend port estimated flow: " << tecu_portMeasuredOrEstimatedFlowPercent(at_ptrFrame->dataOctet(0)) << "% " <<
    "Retract port estimated flow: " << tecu_portMeasuredOrEstimatedFlowPercent(at_ptrFrame->dataOctet(1)) << "% " <<
    "Fail save mode - measured: " << tecu_auxiliaryValveFailSaveModeMeasuredRepr(unsigned(at_ptrFrame->dataOctet(2)) >> 6) << " " <<
    "Valve state: " << tecu_valveStateRepr(at_ptrFrame->dataOctet(2)) << " " <<
    "Limit status: " << tecu_persistentLimitStatusRepr(at_ptrFrame->dataOctet(3) >> 5);

  return out.str();
}


inline double
tecu_extendOrRetractPortPressureRepr(
    std::vector< uint8_t > const &ar_data,
    size_t t_offset)
{
  return 5e3 * (0x100u * ar_data[t_offset+1] + ar_data[t_offset]); // unit: Pa
}


inline double
tecu_returnPortPressureRepr(unsigned at_octet)
{
  return 16e3 * at_octet; // unit: Pa
}


std::string
interpreteValveMeasuredFlow(PtrDataFrame_t at_ptrFrame)
{
  std::ostringstream out;

  // for different valves resp. different values of at_ptrFrame->pgn()
  out <<
    "Extend port measured flow: " << tecu_portMeasuredOrEstimatedFlowPercent(at_ptrFrame->dataOctet(0)) << "% " <<
    "Retract port measured flow: " << tecu_portMeasuredOrEstimatedFlowPercent(at_ptrFrame->dataOctet(1)) << "% " <<
    "Extend port pressure: " << std::scientific << tecu_extendOrRetractPortPressureRepr(at_ptrFrame->data(), 2) << " Pa " <<
    "Retract port pressure: " << std::scientific << tecu_extendOrRetractPortPressureRepr(at_ptrFrame->data(), 4) << " Pa " <<
    "Return port pressure: " << std::scientific << tecu_returnPortPressureRepr(at_ptrFrame->dataOctet(6)) << " Pa " <<
    "Limit Status: " << tecu_persistentLimitStatusRepr(at_ptrFrame->dataOctet(3) >> 5);

  return out.str();
}


inline double
tecu_commandPortFlowPercent(uint8_t ui8_octet)
{
  return 0.4 * ui8_octet;
}


std::string
tecu_commandAuxiliaryFailSaveMode(unsigned ui_2lsb)
{
  switch (ui_2lsb & 0x3u) {
  case 0u:
    return "block";
  case 1u:
    return "float";
  case 2u:
    return "reserved";
  case 3u:
    return "don't care";
  default:
    return "(protocol error)";
  }
}


std::string
tecu_commandAuxiliaryStateRepr(unsigned ui_4lsb)
{
  switch (ui_4lsb & 0xFu) {
  case 0u:
    return "block";
  case 1u:
    return "extend";
  case 2u:
    return "retract";
  case 3u:
    return "float";
  default:
    return "reserved";
  case 15u:
    return "don't care";
  }
}


std::string
interpreteValveCommand(PtrDataFrame_t at_ptrFrame)
{
  std::ostringstream out;

  // for different valves resp. different values of at_ptrFrame->pgn()
  out <<
    "Auxiliary port flow - command: " << tecu_commandPortFlowPercent(at_ptrFrame->dataOctet(0)) << "% " <<
    "Auxiliary fail safe mode - command: " << tecu_commandAuxiliaryFailSaveMode(at_ptrFrame->dataOctet(2) >> 6) << " " <<
    "Auxiliary state - command: " << tecu_commandAuxiliaryStateRepr(at_ptrFrame->dataOctet(2));

  return out.str();
}
