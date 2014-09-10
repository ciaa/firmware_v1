std::string
interpretePgnsCl2Fs( PtrDataFrame_t at_ptrFrame )
{
  uint16_t i_pathNameLength = 0, i_destPathNameLength = 0;

  std::ostringstream out;
  out << "SA: " << std::setw(2)<<std::setfill('0') << uint32_t(at_ptrFrame->sourceAddress()) << " DA: " << std::setw(2)<<std::setfill('0') << uint32_t(at_ptrFrame->destinationAddress()) << " length Data: " << std::setw(4)<<std::setfill('0') << uint32_t(at_ptrFrame->dataSize());
  switch (uint32_t(at_ptrFrame->dataOctet(0)))
  {
    case 0x00: out << "\tClient Connection Maintenance";
      break;

    case 0x01: out << "\tGet File Server Properties Request ";
      break;

    case 0x10: out << "\tGet Current Directory Request TAN: " << std::setw(2) << std::hex << uint32_t(at_ptrFrame->dataOctet(1));
      break;

    case 0x11: out << "\tChange Current Directory Request TAN: " << std::setw(2) << std::hex << uint32_t(at_ptrFrame->dataOctet(1));
               i_pathNameLength = uint16_t((at_ptrFrame->dataOctet(3)<<8) | (at_ptrFrame->dataOctet(2)));
               out << " Path Name Length : " << i_pathNameLength;
               out << " Path Name : ";
              for (uint16_t i = 0; i < i_pathNameLength; i++)
              {
                out << uint8_t(at_ptrFrame->dataOctet(4 + i));
              }
      break;

    case 0x20: out << "\tOpen File Request TAN: " << std::setw(2) << std::hex << uint32_t(at_ptrFrame->dataOctet(1));

              if (at_ptrFrame->dataOctet(2) == 0) out << " Open file for reading only";
              if (at_ptrFrame->dataOctet(2) == 1) out << " Open file for writing only";
              if (at_ptrFrame->dataOctet(2) == 2) out << " Open file for reading and writing only";
              if (at_ptrFrame->dataOctet(2) == 3) out << " Open directory";

              (at_ptrFrame->dataOctet(2) & 4)? out << " Create a new File if not existing": out << " Open an existing File(fails if non-existent file)";

              (at_ptrFrame->dataOctet(2) & 8)? out << " Open file for appending data the end of the file": out << " Open file for random access";

              (at_ptrFrame->dataOctet(2) & 16)? out << " Open file with exclusive access(fails if already open)": out << " Open file for shared access";

              i_pathNameLength = uint16_t((at_ptrFrame->dataOctet(4)<<8) | (at_ptrFrame->dataOctet(3)));

               out << " Path Name Length: " << std::hex << i_pathNameLength;
               out << " Path Name: ";
              for (uint16_t i = 0; i <   i_pathNameLength; i++)
              {
                out << uint8_t(at_ptrFrame->dataOctet(5 + i));
              }
      break;

    case 0x21: out << "\tSeek File Request TAN: " << std::setw(2) << std::hex << static_cast<uint32_t>(at_ptrFrame->dataOctet(1));
               out << " Handle: " << std::hex << static_cast<uint32_t>(at_ptrFrame->dataOctet(2));
               out << " Position Mode: " << std::hex << static_cast<uint32_t>(at_ptrFrame->dataOctet(3));
               out << " Offset: " << std::hex << ( (static_cast<uint32_t>(at_ptrFrame->dataOctet(7))<<24) | (static_cast<uint32_t>(at_ptrFrame->dataOctet(6))<<16) | (static_cast<uint32_t>(at_ptrFrame->dataOctet(5))<<8) | (static_cast<uint32_t>(at_ptrFrame->dataOctet(4))) );
      break;

    case 0x22: out << "\tRead File Request TAN: " << std::setw(2) << std::hex << static_cast<uint32_t>(at_ptrFrame->dataOctet(1));
               out << " Handle: "     <<std::hex<<static_cast<uint32_t>(at_ptrFrame->dataOctet(2));
               out << " Count: "      <<std::hex<<( (static_cast<uint16_t>(at_ptrFrame->dataOctet(4))<<8) | (static_cast<uint16_t>(at_ptrFrame->dataOctet(3))) );
               if (at_ptrFrame->dataOctet(5) == 0x00) out << " Do not report hidden files a directory listing";
               if (at_ptrFrame->dataOctet(5) == 0x01) out << " Report hidden files in a directory listing";
               if (at_ptrFrame->dataOctet(5) == 0xFF) out << " Parameter ist not available, FS shall not report hidden in a directory listing";
               if ( (at_ptrFrame->dataOctet(5) >= 0x02) && (at_ptrFrame->dataOctet(5) <= 0xFE) ) out << " ERROR wrong Error Code";
      break;

    case 0x23: out << "\tWrite File Request TAN: " << std::setw(2) << std::hex << static_cast<uint32_t>(at_ptrFrame->dataOctet(1));
               out << " Handle: " << std::hex << static_cast<uint32_t>(at_ptrFrame->dataOctet(2));
               out << " Number of Byte written Data: " <<std::dec<<( (static_cast<uint16_t>(at_ptrFrame->dataOctet(4))<<8) | (static_cast<uint16_t>(at_ptrFrame->dataOctet(3))) );
               out << " Data: ";
               for (int i = 0; i < ( (static_cast<uint16_t>(at_ptrFrame->dataOctet(4))<<8) | (static_cast<uint16_t>(at_ptrFrame->dataOctet(3))) ); i++)
               {
                 out << "0x" << std::hex << static_cast<uint32_t>(at_ptrFrame->dataOctet(5 + i)) << " ";
               }
      break;

    case 0x24: out << "\tClose File Request TAN: " << std::setw(2) << std::hex << static_cast<uint32_t>(at_ptrFrame->dataOctet(1));
               out << " Handle: " << std::hex << static_cast<uint32_t>(at_ptrFrame->dataOctet(2));
      break;

    case 0x30: out << "\tMove / Copy / Rename File Request TAN: " << std::setw(2) << std::hex << static_cast<uint32_t>(at_ptrFrame->dataOctet(1));
               out << ((at_ptrFrame->dataOctet(2) && 0x04)? " recursive" : " not recursive");
               out << ((at_ptrFrame->dataOctet(2) && 0x02)? " force" : " not force");
               out << ((at_ptrFrame->dataOctet(2) && 0x01)? " copy" : " not copy");

               i_pathNameLength = static_cast<uint16_t>( (static_cast<uint16_t>(at_ptrFrame->dataOctet(4))<<8) |  at_ptrFrame->dataOctet(3) );

               out << " Source Path Name Length: "  << std::hex << i_pathNameLength;
               out << " Source Path Name: ";
               for (int i = 0; i < i_pathNameLength; i++) {
                 out << uint8_t(at_ptrFrame->dataOctet(7 + i));
               }

               i_destPathNameLength = static_cast<uint16_t>( (static_cast<uint16_t>(at_ptrFrame->dataOctet(6))<<8) | at_ptrFrame->dataOctet(5) );

               out << " Destination Path Name Length: " << std::hex << i_destPathNameLength;
               out << " Destination Path Name: ";
               for (int i = 0; i < i_destPathNameLength; i++) {
                 out << uint8_t(at_ptrFrame->dataOctet(7 + i_pathNameLength + i));
               }
      break;

    case 0x31: out << "\tDelete File Request TAN: " << std::setw(2) << std::hex << static_cast<uint32_t>(at_ptrFrame->dataOctet(1));

               out << ((at_ptrFrame->dataOctet(2) && 0x04)? " recursive" : " not recursive");
               out << ((at_ptrFrame->dataOctet(2) && 0x02)? " force" : " not force");
               out << ((at_ptrFrame->dataOctet(2) && 0x01)? " copy" : " not copy");

               i_pathNameLength = static_cast<uint16_t>( (static_cast<uint16_t>(at_ptrFrame->dataOctet(4))<<8) | at_ptrFrame->dataOctet(3) );

               out << " Path Name Length: " << std::hex << i_pathNameLength;
               out << " Path Name: ";
              for (uint16_t i = 0; i <  i_pathNameLength; i++)
              {
                out << uint8_t(at_ptrFrame->dataOctet(5 + i));
              }

      break;
    case 0x32: out << "\tGet Files Attributes Request TAN: " << std::setw(2) << std::hex << static_cast<uint32_t>(at_ptrFrame->dataOctet(1));

               i_pathNameLength = static_cast<uint16_t>( (static_cast<uint16_t>(at_ptrFrame->dataOctet(3))<<8) | at_ptrFrame->dataOctet(2) );

               out << " Path Name Length: " << std::hex << i_pathNameLength;
               out << " Path Name: " ;
               for (int i = 0; i < i_pathNameLength; i++) {
                 out << uint8_t(at_ptrFrame->dataOctet(4 + i));
               }
      break;
    case 0x33: out << "\tSet File Attributes Request TAN: " << std::setw(2) << std::hex << static_cast<uint32_t>(at_ptrFrame->dataOctet(1));

               if (at_ptrFrame->dataOctet(2) == 0x1C) out << " Clear read-only attribute";
               if (at_ptrFrame->dataOctet(2) == 0x1D) out << " Set read-only attribute";
               if (at_ptrFrame->dataOctet(2) == 0x13) out << " Clear hidden attribute";
               if (at_ptrFrame->dataOctet(2) == 0x17) out << " Set hidden attribute";

               i_pathNameLength = static_cast<uint16_t>( (static_cast<uint16_t>(at_ptrFrame->dataOctet(4))<<8) | at_ptrFrame->dataOctet(3) );

               out << " Path Name Length: " << std::hex << i_pathNameLength;
               out << " Path Name: " ;
               for (int i = 0; i < i_pathNameLength; i++) {
                 out << uint8_t(at_ptrFrame->dataOctet(5 + i));
               }
      break;
    case 0x34: out << "\tGet File Date & File Time Request TAN: " << std::setw(2) << std::hex << static_cast<uint32_t>(at_ptrFrame->dataOctet(1));

               i_pathNameLength = static_cast<uint16_t>( (static_cast<uint16_t>(at_ptrFrame->dataOctet(3))<<8) | at_ptrFrame->dataOctet(2) );

               out << " Path Name Length: " << std::hex << i_pathNameLength;
               out << " Path Name: " ;
               for (int i = 0; i < i_pathNameLength; i++) {
                 out << uint8_t(at_ptrFrame->dataOctet(4 + i));
               }
      break;
    case 0x40: out << "\tInitialize Volume Request TAN: " << std::setw(2) << std::hex << static_cast<uint32_t>(at_ptrFrame->dataOctet(1));
               out << " Space : "        <<std::hex<< ((static_cast<uint32_t>(at_ptrFrame->dataOctet(5)) << 24)
			                                    | (static_cast<uint32_t>(at_ptrFrame->dataOctet(4)) << 16)
												| (static_cast<uint32_t>(at_ptrFrame->dataOctet(3)) <<  8)
												| (static_cast<uint32_t>(at_ptrFrame->dataOctet(2))      ));
               if (at_ptrFrame->dataOctet(6) == 0x00) out <<" create  a new volume using all available space";
               if (at_ptrFrame->dataOctet(6) == 0x02) out <<" create  a new volume using space specified";
               if (at_ptrFrame->dataOctet(6) == 0x01) out <<" Overwrite the existing volume";

               i_pathNameLength = static_cast<uint16_t>( static_cast<uint16_t>(at_ptrFrame->dataOctet(8)<<8) | at_ptrFrame->dataOctet(7) );

               out << " Path Name Length: " << std::hex << i_pathNameLength;
               out << " Path Name: " ;
               for (int i = 0; i < i_pathNameLength; i++) {
                 out << uint8_t(at_ptrFrame->dataOctet(9 + i));
               }
      break;
    default: return "";
  }

  return out.str();
}


std::string
fs_decodeErrorCode( uint8_t i_errorCode )
{
  std::ostringstream out;

  switch (uint32_t(i_errorCode)) {
    case 0x00:
      out << " Success" ;
      break;
    case 0x01:
      out << " Access Denied" ;
      break;
    case 0x02:
      out << " Invalid Access" ;
      break;
    case 0x03:
      out << " Too many files open" ;
      break;
    case 0x04:
      out << " File or path not found" ;
      break;
    case 0x05:
      out << " Invalid Handle" ;
      break;
    case 0x06:
      out << " Invalid given source name" ;
      break;
    case 0x07:
      out << " Invalid given destination name" ;
      break;
    case 0x08:
      out << " Out of free space" ;
      break;
    case 0x09:
      out << " Failure during write operation" ;
      break;
    case 0x0a:
      out << " Volume is possibly not initialized" ;
      break;
    case 0x0b:
      out << " Failure during a read operation" ;
      break;
    case 0x0c:
      out << " Function not supported" ;
      break;
    case 0x2A:
      out << " Invalid request length" ;
      break;
    case 0x2B:
      out << " Out of memory" ;
      break;
    case 0x2C:
      out << " Any other error" ;
      break;
    case 0x2D:
      out << " End of file reached" ;
      break;
    default:
      out << " Reserved" ;
      break;
  }

  return out.str();
}


std::string
fs_decodeAttributes( uint8_t i_attributes ) 
{
  std::ostringstream out;

  (i_attributes & 128)? out << " Volume is case-sensitive"  : out << " Volume is case-insensitive" ;
  (i_attributes & 64)? out << " Volume is not removable"  : out << " Volume is removable" ;
  (i_attributes & 32)? out << " Volume does support long filenames"  : out << " Volume does not support long filenames" ;
  (i_attributes & 16)? out << " Handle specifies a directory"  : out << " Handle does not specify a directory" ;
  (i_attributes & 8)? out << " Handle specifies a volume"  : out << " Handle does not specify a volume" ;
  (i_attributes & 2)? out << " Hidden attribute is set"  : out << " Hidden attribute is not set" ;
  (i_attributes & 1)? out << " Read-only attribute is set"  : out << " Read-only attribute is not set" ;

  return out.str();
}


std::string
fs_decodeDate( uint16_t i_date )
{
  std::ostringstream out;
  out << " " << std::setw(4) << std::dec << uint32_t(1980 + ((i_date >> 9) & 0x7F)) << "-" << std::setw(2) << std::dec << uint32_t((i_date >> 5) & 0xF) << "-" << std::setw(2) << std::dec << uint32_t((i_date) & 0x1F);
  return out.str();
}


std::string
fs_decodeTime( uint16_t i_time )
{
  std::ostringstream out;
  out << " " << std::setw(2) << std::dec << uint32_t(((i_time >> 11) & 0x1F)) << "-" << std::setw(2) << std::dec << uint32_t(((i_time >> 5) & 0x3F)) << "-" << std::setw(2) << std::dec << uint32_t(2 * ((i_time) & 0x1F));
  return out.str();
}


std::string
interpretePgnsFs2Cl( PtrDataFrame_t at_ptrFrame )
{
  uint32_t i_pathNameLength = 0;
  uint16_t msg_count = 0;
  uint16_t msg_offset = 0;

  std::ostringstream out;
  out << "SA: " << std::setw(2)<<std::setfill('0') << uint32_t(at_ptrFrame->sourceAddress()) << " DA: " << std::setw(2)<<std::setfill('0') << uint32_t(at_ptrFrame->destinationAddress()) << " length Data: " << std::setw(4)<<std::setfill('0') << uint32_t(at_ptrFrame->dataSize());

  switch (uint32_t(at_ptrFrame->dataOctet(0))) {
    case 0x00:
      out << "\tFile Server Status: ";
        (at_ptrFrame->dataOctet(1) & 0x2) ? out << "Busy writing" : out << "Not Busy writing";
        (at_ptrFrame->dataOctet(1) & 0x1) ? out << " Busy reading" : out << " Not Busy reading";
      out << " Number of Open Files: " << std::setw(2) << std::hex << uint32_t(at_ptrFrame->dataOctet(2));
      break; //File Server Status

    case 0x01:

      out << "\tGet File Server Properties Response Version Number:";
      switch (at_ptrFrame->dataOctet(1)) {
        case 0x00: out << " Draft version of the Standard"; break;
        case 0x01: out << " Final Draft version of the Standard"; break;
        case 0x02: out << " First published version of the Standard"; break;
      }
      out << " Maximum Number of Simuntaneously Open Files: " << std::hex << uint32_t(at_ptrFrame->dataOctet(2));
      out << " File Server Capabilities : ";
      (at_ptrFrame->dataOctet(3)) ? out << " File server supports multiple volumes"
                : out << " File server does not supports multiple volumes";
      break; //Get File Server Properties Response

    case 0x10:

      out << "\tGet Current Directory Response TAN: " << std::setw(2) << std::hex << uint32_t(at_ptrFrame->dataOctet(1));
      fs_decodeErrorCode(at_ptrFrame->dataOctet(2));
      out << " Total Space: " << std::setw(8) <<  ((static_cast<uint32_t>(at_ptrFrame->dataOctet(6)) << 24) |
		                                      (static_cast<uint32_t>(at_ptrFrame->dataOctet(5)) << 16) |
		                                      (static_cast<uint32_t>(at_ptrFrame->dataOctet(4)) << 8 ) |
		                                      (static_cast<uint32_t>(at_ptrFrame->dataOctet(3)))) << "(* 512 bytes)";
      out << " Free Space: " << std::setw(8) << ((static_cast<uint32_t>(at_ptrFrame->dataOctet(10)) << 24) |
		                                    (static_cast<uint32_t>(at_ptrFrame->dataOctet( 9)) << 16) |
		                                    (static_cast<uint32_t>(at_ptrFrame->dataOctet( 8)) << 8 ) |
		                                    (static_cast<uint32_t>(at_ptrFrame->dataOctet( 7)))) << "(* 512 bytes)";
      i_pathNameLength = (static_cast<uint16_t>(at_ptrFrame->dataOctet(12))<<8) | at_ptrFrame->dataOctet(11);
      out << " Path Name Length: " << std::setw(4) << i_pathNameLength;

      out << " Path Name: ";
      for (uint16_t i_count = 0; i_count < i_pathNameLength; i_count++)
      {
        out << uint8_t(at_ptrFrame->dataOctet(13 + i_count));
      }

      break; //Get Current Directory Response
    case 0x11:

      out << "\tChange Current Directory Response TAN: " << std::setw(2) << std::hex << uint32_t(at_ptrFrame->dataOctet(1));
      fs_decodeErrorCode(at_ptrFrame->dataOctet(2));

      break; //Chanege Current Directory Response
    case 0x20:

      out << "\tOpen File Response TAN: " << std::setw(2) << std::hex << uint32_t(at_ptrFrame->dataOctet(1));
      fs_decodeErrorCode(at_ptrFrame->dataOctet(2));
      out << " Handle: " << std::setw(2) << std::hex << uint32_t(at_ptrFrame->dataOctet(3));
      fs_decodeAttributes(at_ptrFrame->dataOctet(4));

      break; //Open File Response
    case 0x21:

      out << "\tSeek File Response TAN: " << std::setw(2) << std::hex << uint32_t(at_ptrFrame->dataOctet(1));
      fs_decodeErrorCode(at_ptrFrame->dataOctet(2));
      out << " Position : " << std::setw(8) << ((static_cast<uint32_t>(at_ptrFrame->dataOctet(7)) << 24) |
		                                   (static_cast<uint32_t>(at_ptrFrame->dataOctet(6)) << 16) |
		                                   (static_cast<uint32_t>(at_ptrFrame->dataOctet(5)) << 8 ) |
		                                   (static_cast<uint32_t>(at_ptrFrame->dataOctet(4))));
      break; //Seek File Response
    case 0x22:

      out << "\tRead File/Directory Response TAN: " << std::setw(2) << std::hex << uint32_t(at_ptrFrame->dataOctet(1));
      fs_decodeErrorCode(at_ptrFrame->dataOctet(2));

      msg_count = uint16_t((at_ptrFrame->dataOctet(4)<<8) | at_ptrFrame->dataOctet(3));
      msg_offset = 5;

      out << " Count: " << std::setw(4) << msg_count << " ";

      //case file-data
      if (at_ptrFrame->dataSize() == msg_count + 5u) {
        for (uint16_t loop = 0; loop < msg_count; loop++) {
          out << std::setw(2) << std::hex << uint32_t(at_ptrFrame->dataOctet(5 + loop)) << " ";
        }
      }
      //else directory entries
      else {
        for (uint16_t loop = 0; loop < msg_count; loop++) {
          uint8_t filenamelength = uint8_t(at_ptrFrame->dataOctet(msg_offset));
          if (filenamelength == 0xff)
            break;
          out << " Filename length: " << uint16_t(filenamelength) << " ";
          out << " Filename: ";
          uint8_t loopfilename = 0;
          for (; loopfilename < filenamelength; loopfilename++) {
            out << uint8_t(at_ptrFrame->dataOctet(msg_offset + 1 + loopfilename));
          }
          fs_decodeAttributes(at_ptrFrame->dataOctet(msg_offset + 1 + loopfilename));
          fs_decodeDate(uint16_t(at_ptrFrame->dataOctet(msg_offset + 3 + loopfilename) << 8 | uint16_t(at_ptrFrame->dataOctet(msg_offset + 2 + loopfilename))));
          fs_decodeTime(uint16_t(at_ptrFrame->dataOctet(msg_offset + 5 + loopfilename) << 8 | uint16_t(at_ptrFrame->dataOctet(msg_offset + 4 + loopfilename))));
          out << " FileSize: " << ((static_cast<uint32_t>(at_ptrFrame->dataOctet(msg_offset + 9 + loopfilename)) << 24) |
		                           (static_cast<uint32_t>(at_ptrFrame->dataOctet(msg_offset + 8 + loopfilename)) << 16) |
		                           (static_cast<uint32_t>(at_ptrFrame->dataOctet(msg_offset + 7 + loopfilename)) << 8 ) |
		                           (static_cast<uint32_t>(at_ptrFrame->dataOctet(msg_offset + 6 + loopfilename))));
          msg_offset = uint16_t(msg_offset + filenamelength + 10);
        }
      }

      break; //Read File/Directory Response
    case 0x23:

      out << "\tWrite File Response TAN: " << std::setw(2) << std::hex << uint32_t(at_ptrFrame->dataOctet(1));
      fs_decodeErrorCode(at_ptrFrame->dataOctet(2));
      out << " Count: " << std::setw(4) << uint16_t((at_ptrFrame->dataOctet(4)<<8) | at_ptrFrame->dataOctet(3));
      break; // Write File Response
    case 0x24:

      out << "\tClose File Response TAN: " << std::setw(2) << std::hex << uint32_t(at_ptrFrame->dataOctet(1));
      fs_decodeErrorCode(at_ptrFrame->dataOctet(2));
      break; // Close File Response
    case 0x30:

      out << "\tMove File Response TAN: " << std::setw(2) << std::hex << uint32_t(at_ptrFrame->dataOctet(1));
      fs_decodeErrorCode(at_ptrFrame->dataOctet(2));
      break; // Move File Response
    case 0x31:

      out << "\tDelete File Repsonse TAN: " << std::setw(2) << std::hex << uint32_t(at_ptrFrame->dataOctet(1));
      fs_decodeErrorCode(at_ptrFrame->dataOctet(2));
      break; // Delete File Repsonse
    case 0x32:

      out << "\tGet File Attribute Response TAN: " << std::setw(2) << std::hex << uint32_t(at_ptrFrame->dataOctet(1));
      fs_decodeErrorCode(at_ptrFrame->dataOctet(2));
      fs_decodeAttributes(at_ptrFrame->dataOctet(3));
      out << " Size: " << ((static_cast<uint32_t>(at_ptrFrame->dataOctet(7)) << 24) |
		                   (static_cast<uint32_t>(at_ptrFrame->dataOctet(6)) << 16) |
		                   (static_cast<uint32_t>(at_ptrFrame->dataOctet(5)) << 8 ) |
		                   (static_cast<uint32_t>(at_ptrFrame->dataOctet(4))));
      break; // Get File Attribute Response
    case 0x33:

      out << "\tSet File Attribute Response TAN: " << std::setw(2) << std::hex << uint32_t(at_ptrFrame->dataOctet(1));
      fs_decodeErrorCode(at_ptrFrame->dataOctet(2));
      break; // Set File Attribute Response
    case 0x34:

      out << "\tGet File Date & Time Response TAN: " << std::setw(2) << std::hex << uint32_t(at_ptrFrame->dataOctet(1));
      fs_decodeErrorCode(at_ptrFrame->dataOctet(2));
      fs_decodeDate(uint16_t((at_ptrFrame->dataOctet(4)<<8) | at_ptrFrame->dataOctet(3)));
      fs_decodeTime(uint16_t((at_ptrFrame->dataOctet(6)<<8) | at_ptrFrame->dataOctet(5)));
      break; // Get File Date & Time Response
    case 0x40:

      out << "\tInitialize Volume Response TAN: " << std::setw(2) << std::hex << uint32_t(at_ptrFrame->dataOctet(1));
      fs_decodeErrorCode(at_ptrFrame->dataOctet(2));
      fs_decodeAttributes(at_ptrFrame->dataOctet(3));
      break; // Initialize Volume Response
    default: ;
  }

  return out.str();
}

