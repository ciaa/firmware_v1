void
checkAlives( PtrDataFrame_t at_ptrFrame )
{
  if ( ((at_ptrFrame->pgn() == VT_TO_ECU_PGN) && (at_ptrFrame->dataOctet (0) == 0xFE))
       ||
       ((at_ptrFrame->pgn() == ECU_TO_VT_PGN) && (at_ptrFrame->dataOctet (0) == 0xFF))
     )
  {
    gs_main.m_alive.alives(AliveCollection_c::categoryVtServerClientPeriodic, at_ptrFrame->sourceAddress()).push_back (at_ptrFrame->time());
  }

  if ( (at_ptrFrame->pgn() == PROCESS_DATA_PGN) && ((at_ptrFrame->dataOctet (0) & 0x0F) >= 0x0E) )
  {
    gs_main.m_alive.alives(AliveCollection_c::categoryTcServerClientPeriodic, at_ptrFrame->sourceAddress()).push_back (at_ptrFrame->time());
  }
}


void
checkSingles( PtrDataFrame_t at_ptrFrame )
{
  if (at_ptrFrame->pgn() == ACKNOWLEDGEMENT_PGN)
  {
    gs_main.m_alive.alives(AliveCollection_c::categoryNotAcknowledge, at_ptrFrame->sourceAddress()).push_back (at_ptrFrame->time());
  }
}


void
checkHandshakingsVtCommands( PtrDataFrame_t at_ptrFrame )
{
  if( at_ptrFrame->dataSize() == 0 )
    return;
  if( (at_ptrFrame->dataOctet(0) < 0x60) || (at_ptrFrame->dataOctet(0) >= 0xF0) )
    return; // some non-cmd

  AliveCollection_c::Category_e e_category; // no need to initialize.
  msgType_e msgType_direction;
  uint8_t ui8_saClient;
  if (at_ptrFrame->pgn() == VT_TO_ECU_PGN)
  {
    e_category = AliveCollection_c::categoryVtCommandResponse;
    msgType_direction = msgTypeResponse;
    ui8_saClient = at_ptrFrame->ps();
  }
  else if (at_ptrFrame->pgn() == ECU_TO_VT_PGN)
  {
    e_category = AliveCollection_c::categoryVtCommandResponse;
    msgType_direction = msgTypeCommand;
    ui8_saClient = at_ptrFrame->sourceAddress();
  }
  else return;

  gs_main.m_alive.response(e_category, ui8_saClient).push_back (std::make_pair< msgType_e, std::string >(msgType_direction, "")); // @todo VT commands
  gs_main.m_alive.alives(e_category, ui8_saClient).push_back (at_ptrFrame->time());
}


void
checkHandshakingTP( PtrDataFrame_t at_ptrFrame )
{
  if( at_ptrFrame->dataSize() == 0 )
    return;

  msgType_e mtype = msgTypeDATA; // default to avoid compiler warning. The code below should make sure that it is always initialized properly!
  uint8_t ui8_saClient=0xFE; // default to avoid compiler warning. The code below should make sure that it is always initialized properly!

  switch (at_ptrFrame->pgn())
  {
  case ETP_DATA_TRANSFER_PGN:
  case TP_DATA_TRANSFER_PGN:
    ui8_saClient = at_ptrFrame->sourceAddress();
    mtype = msgTypeDATA;
    break;

  case ETP_CONN_MANAGE_PGN:
  case TP_CONN_MANAGE_PGN:
    switch (at_ptrFrame->dataOctet (0)) {
    case 0x10:
    case 0x14:
      mtype = msgTypeRTS;
      ui8_saClient = at_ptrFrame->sourceAddress();
      break;
    case 0x11:
    case 0x15:
      mtype = msgTypeCTS;
      ui8_saClient = at_ptrFrame->destinationAddress();
      break;
    case 0x16:
      mtype = msgTypeDPO;
      ui8_saClient = at_ptrFrame->sourceAddress();
      break;
    case 0x13:
    case 0x17:
      mtype = msgTypeEOMACK;
      ui8_saClient = at_ptrFrame->destinationAddress();
      break;
    case 0xFF:
      mtype = msgTypeCONNABORT;
      ui8_saClient = at_ptrFrame->sourceAddress();
      // this message is special case! This aborts SA->DA and also DA->SA!
      gs_main.m_alive.response(AliveCollection_c::categoryEtpTpCommunicationTimes, at_ptrFrame->destinationAddress()).push_back (std::make_pair<msgType_e,std::string>(mtype, "") );
      gs_main.m_alive.alives(AliveCollection_c::categoryEtpTpCommunicationTimes, at_ptrFrame->destinationAddress()).push_back (at_ptrFrame->time());
      break; // CONN ABORT BY SENDER!
    }
    break;

  default:
    return;
  }

  gs_main.m_alive.response(AliveCollection_c::categoryEtpTpCommunicationTimes, ui8_saClient).push_back (std::make_pair<msgType_e,std::string>(mtype, ""));
  gs_main.m_alive.alives(AliveCollection_c::categoryEtpTpCommunicationTimes, ui8_saClient).push_back (at_ptrFrame->time());
}


void
checkHandshakingsProcData( PtrDataFrame_t at_ptrFrame )
{
  if (at_ptrFrame->pgn() != PROCESS_DATA_PGN)
    return;
  if( at_ptrFrame->dataSize() == 0 )
    return;

  AliveCollection_c::Category_e e_category( AliveCollection_c::categoryProcessData );

  msgType_e msgType_direction;
  uint8_t ui8_saClient;

  if( (at_ptrFrame->destinationAddress() != 0xFF) && ((at_ptrFrame->dataOctet(0) & 0x0F) < 0x0E) )
  { // ignore TC Status, Maintenance, etc.
    if( at_ptrFrame->sourceAddress() >= 0xF0 ) // assume it's the TC
    {
      msgType_direction = msgTypeSetpoint;
      ui8_saClient = at_ptrFrame->destinationAddress();
    }
    else
    {
      msgType_direction = msgTypeMeasurement;
      ui8_saClient = at_ptrFrame->sourceAddress();
    }
    gs_main.m_alive.response(e_category, ui8_saClient).push_back (std::make_pair<msgType_e,std::string>(
      msgType_direction, 
      interpretePgnData( at_ptrFrame )));
    gs_main.m_alive.alives(e_category, ui8_saClient).push_back (at_ptrFrame->time());
  }
}
