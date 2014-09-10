std::string
partiallyInterpretePgnsVtEcu( PtrDataFrame_t at_ptrFrame, bool response )
{
  if (at_ptrFrame->dataSize() < 8)
    return "*** ILLEGAL - THIS PGN *MUST* HAVE AT LEAST 8 DATABYTES ***";

  std::ostringstream out;

  if (at_ptrFrame->dataOctet(0) < 0x10)
    out << "Activation ";
  if ((at_ptrFrame->dataOctet(0) >=0x10) && (at_ptrFrame->dataOctet(0) < 0xF0))
    out << "Command ";

  int8_t i8_uidBytePos = -1;

  switch (at_ptrFrame->dataOctet(0))
  {
    case 0x00: out << "Soft Key"; i8_uidBytePos = 3; break;
    case 0x01: out << "Button"; i8_uidBytePos = 3; break;
    case 0x02: out << "Pointing Event"; break;
    case 0x03: out << "VT Select Input Object"; i8_uidBytePos = 2; break;
    case 0x04: out << "VT ESC"; i8_uidBytePos = 2; break;
    case 0x05: out << "VT Change Numeric Value"; i8_uidBytePos = 2; break;
    case 0x06: out << "VT Change Active Mask"; i8_uidBytePos = 2; break;
    case 0x07: out << "VT Change Soft Key Mask"; i8_uidBytePos = 4; break;
    case 0x08: out << "Input String Value"; i8_uidBytePos = 2; break;

    case 0x11: out << "Object Pool Transfer"; break;
    case 0x12: out << "End of Object Pool"; break;

    case 0x92: out << "ESC"; i8_uidBytePos = 2; break;

    case 0xC0: out << "Get Memory"; break;
    case 0xC2: out << "Get Number Of Soft Keys"; break;
    case 0xC3: out << "Get Text Font Data"; break;
    case 0xC7: out << "Get Hardware"; break;

    case 0xD0: out << "Store Version"; break;
    case 0xD1: out << "Load Version"; break;
    case 0xD2: out << "Delete Version"; break;

    case 0xA0: out << "Hide/Show Object"; i8_uidBytePos = 2; if (at_ptrFrame->dataOctet(4-1)) out << " -show-"; else out << " -hide-"; break;
    case 0xA1: out << "Enable/Disable Object"; i8_uidBytePos = 2; if (at_ptrFrame->dataOctet(4-1)) out << " -enable-"; else out << " -disable-"; break;
    case 0xA2: out << "Select Input Object"; i8_uidBytePos = 2; break;
    case 0xA3: out << "Control Audio Device"; break;
    case 0xA4: out << "Set Audio Volume"; break;
    case 0xA5: out << "Change Child Location"; i8_uidBytePos = 4; break;
    case 0xA6: out << "Change Size"; i8_uidBytePos = 2; break;
    case 0xA7: out << "Change Background Colour"; i8_uidBytePos = 2; break;
    case 0xA8: out << "Change Numeric Value"; i8_uidBytePos = 2; break;
    case 0xA9: out << "Change End Point"; i8_uidBytePos = 2; break;
    case 0xAA: out << "Change Font Attributes"; i8_uidBytePos = 2; break;
    case 0xAB: out << "Change Line Attributes"; i8_uidBytePos = 2; break;
    case 0xAC: out << "Change Fill Attributes"; i8_uidBytePos = 2; break;
    case 0xAD: out << "Change Active Mask"; i8_uidBytePos = 4; break;
    case 0xAE: out << "Change Soft Key Mask"; i8_uidBytePos = 5; break;
    case 0xAF: out << "Change Attribute"; i8_uidBytePos = 2; out << " AID="; out << uint16_t(at_ptrFrame->dataOctet(4-1)); break;

    case 0xB0: out << "Change Priority"; i8_uidBytePos = 2; break;
    case 0xB1: out << "Change List Item"; i8_uidBytePos = 2; break;
    case 0xB2: out << "Delete Object Pool"; break;
    case 0xB3: out << "Change String Value"; i8_uidBytePos = response ? 4 : 2; break;
    case 0xB4: out << "Change Child Position"; i8_uidBytePos = 4; break;

    case 0xFE: out << "VT Status Message: Active WS-Master's SA: $"; out << std::hex << uint16_t(at_ptrFrame->dataOctet(1)); break;
    case 0xFF: out << "Working Set Maintenance"; break;
  }
  if (i8_uidBytePos > 0)
    out << std::dec << " ObjectID=" <<((uint16_t(at_ptrFrame->dataOctet(i8_uidBytePos+1-1)<<8)) | (uint16_t(at_ptrFrame->dataOctet(i8_uidBytePos-1))));

  return out.str();
}


std::string
interpretePgnsVtFromEcu( PtrDataFrame_t at_ptrFrame )
{
  std::string result(
    partiallyInterpretePgnsVtEcu(at_ptrFrame, false) );

  bool const cb_response = at_ptrFrame->dataOctet(0) < 0x10;
  if (cb_response)
    result += " Response";

  if( gs_main.mb_storeIop && ( at_ptrFrame->data()[0] == 0x12 ) ) {
    iopStore( at_ptrFrame->sourceAddress() );
  }

  return result;
}


std::string
interpretePgnsVtToEcu( PtrDataFrame_t at_ptrFrame )
{
  bool const cb_response =
    (at_ptrFrame->dataOctet(0) >=0x10) &&
    (at_ptrFrame->dataOctet(0) < 0xF0);

  std::string result(
    partiallyInterpretePgnsVtEcu(at_ptrFrame, cb_response) );

  if (cb_response)
    result += " Response";

  return result;
}
