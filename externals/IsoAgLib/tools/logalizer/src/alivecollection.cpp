/*
  alivecollection.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include <logenvirons.h>
#include <alivecollection.h>
#include <iomanip>
#include <string>


AliveCollection_c::AliveCollection_c()
{
  setName(categoryVtServerClientPeriodic, "VT Server<->Client Periodic Alive");
  setAlivePeriod(categoryVtServerClientPeriodic, 1000);

  setName(categoryTcServerClientPeriodic, "TC Server<->Client Periodic Alive");
  setAlivePeriod(categoryTcServerClientPeriodic, 2000);

  setName(categoryNotAcknowledge, "NOT Acknowledge (NACK)");
  setAlivePeriod(categoryNotAcknowledge, 0); // means NO alivePeriod!

  setName(categoryVtCommandResponse, "VT Command-Response times");
  setAlivePeriod(categoryVtCommandResponse, -1500); // means Handshaking!

  setName(categoryEtpTpCommunicationTimes, "(E)TP-communication times");
  setAlivePeriod(categoryEtpTpCommunicationTimes, -500); // means Handshaking! (damn, we have mixed periods here...)

  setName(categoryProcessData, "TC-Process Data");
  setAlivePeriod(categoryProcessData, -500); // well, there's not time defined actually...
}


void
AliveCollection_c::report( std::ostream& out ) const
{
  for (int i=0; i<256; i++) {
    for (Category_e categoryIter = categoryBegin;
         categoryIter != categoryEnd;
         categoryIter = nextCategory(categoryIter))
    {
      const int32_t ci32_alivePeriod = alivePeriod(categoryIter);
      if (alives(categoryIter, static_cast<uint8_t>( i )).size()>0)
      {
        if (ci32_alivePeriod > 0)
        { // we have a periodic event!
          out << std::endl << "ISOBUS node with SA="<<std::hex<<i<<std::dec<<" had the following alive-times for ["<<name(categoryIter)<<"] with alive-periods of "<<alivePeriod(categoryIter)<<" ms:"<<std::endl;
        }
        else if (ci32_alivePeriod < 0)
        { // we have a handshaking event!
          out << std::endl << "ISOBUS node with SA="<<std::hex<<i<<std::dec<<" had the following alive-times for ["<<name(categoryIter)<<"] with alive-periods of "<<(-alivePeriod(categoryIter))<<" ms:"<<std::endl;
        }
        else
        { // single events!! "== 0"
          out << std::endl << "ISOBUS node with SA="<<std::hex<<i<<std::dec<<" sent out ["<<name(categoryIter)<<"] at the following times:"<<std::endl;
        }
        std::vector<std::pair<msgType_e,std::string> >::const_iterator type_iter=response(categoryIter, static_cast<uint8_t>( i ) ).begin();
        for (std::vector<uint64_t>::const_iterator iter=alives(categoryIter, static_cast<uint8_t>( i )).begin();
             iter != alives(categoryIter, static_cast<uint8_t>( i )).end();
             iter++)
        {
          out << std::setfill (' ');
          out << "absolute time: "<<std::setw(10)<<(*iter/1000)<<"."<<std::setw(3)<<std::setfill('0')<<(*iter%1000)<<std::setfill(' ');
          if (iter != alives(categoryIter, static_cast<uint8_t>( i )).begin())
          {
            const uint64_t cui32_delta = ( *(iter) - *(iter-1) );
            out<< "  relative time: "<<std::setw(10)<<cui32_delta;

            if (ci32_alivePeriod > 0)
            { // print out the alivePeriod-deviation!
              out<<" deviation: ";
              int deviation = int ((double (int32_t (cui32_delta)-ci32_alivePeriod) / ci32_alivePeriod) * 100);
              uint8_t deviation_bias = (deviation > 0) ? '+' : '-';
              deviation = (deviation < 0) ? -deviation : deviation;
              if (deviation > 100)
              {
                out << "EXTREME DEVIATION(!!) OF " << std::setw(10) << deviation << "0";
              }
              else
              {
                while (deviation > 10)
                {
                  out << deviation_bias;
                  deviation -= 10;
                }
              }
            }
            else if (ci32_alivePeriod < 0)
            { // Handshaking
              if (type_iter == response(categoryIter,static_cast<uint8_t>( i )).end())
                exit_with_error("No direction/msgType set but is expected. System failure.");

              int32_t i32_alivePeriodSpecial;
              switch ((*type_iter).first)
              {
              case msgTypeResponse:   out << " Response  "; i32_alivePeriodSpecial = -ci32_alivePeriod; break;
              case msgTypeCommand:    out << " Command   "; i32_alivePeriodSpecial = 0; break; // no timing-requirement here!
              case msgTypeRTS:        out << " (E)TP-CONN: Request to Send (RTS)         "; i32_alivePeriodSpecial = 0; break; // no timing-requirement here!
              case msgTypeCTS:        out << " (E)TP-CONN: Clear to Send (CTS)           "; i32_alivePeriodSpecial = 1250; break;
              case msgTypeDPO:        out << " (E)TP-CONN: Data Packet Offset (DPO)      "; i32_alivePeriodSpecial = 1250; break; /// @todo SOON-260: set the correct values here!
              case msgTypeEOMACK:     out << " (E)TP-CONN: End of Message ACK (EoMACK)   "; i32_alivePeriodSpecial = 1250; break;
              case msgTypeDATA:       out << " (E)TP-DATA                                "; i32_alivePeriodSpecial = 250; break;
              case msgTypeCONNABORT:  out << " (E)TP-CONN: Connection Abort (CONNABORT)  "; i32_alivePeriodSpecial = -1; break; // doesn't come normally!
              case msgTypeSetpoint:   out << " (TC->Client) Setpoint "; i32_alivePeriodSpecial = 0; break;
              case msgTypeMeasurement:out << " (Client->TC) Measrmnt "; i32_alivePeriodSpecial = 0; break;
              default:                out << " ??? "; i32_alivePeriodSpecial = 0; break;
              }
              if ( ((*type_iter).first == msgTypeResponse) && ((*(type_iter-1)).first == msgTypeResponse) )
                out << " - RESPONSE FOLLOWING A RESPONSE! (check if there was a TP command though)";
              if ( ((*type_iter).first == msgTypeCommand)  && ((*(type_iter-1)).first == msgTypeCommand)  )
                out << " - COMMAND FOLLOWING A COMMAND! (check if there was a TP response though)";
              if (i32_alivePeriodSpecial > 0)
              { // print out the time it took!
                if (cui32_delta > (unsigned int) (i32_alivePeriodSpecial))
                  out << " *** !!! TIMEOUT - Check relative time!!!! ***";
                else
                {
                  int32_t time = int32_t ((cui32_delta*100) / i32_alivePeriodSpecial);
                  out <<std::setw(2)<<time<< " percent of timeout ("<<std::setw(4)<<i32_alivePeriodSpecial<<"): (one '%' shows 10%) ";
                  while (time > 10)
                  {
                    out << "%";
                    time -= 10;
                  }
                }
              }
              else if (i32_alivePeriodSpecial < 0)
              { // unsolicited messages (like CONNABORT)
                out << "*** UNEXPECTED/UNSOLICITED MESSAGE ***";
              }
              out << (*type_iter).second;
            }
          }
          out << std::endl;
          if (type_iter != response(categoryIter, static_cast<uint8_t>( i )).end())
            ++type_iter;
        }
      }
    }
  }
}
