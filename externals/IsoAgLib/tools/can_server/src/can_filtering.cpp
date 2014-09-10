/*
  can_filtering.cpp: Module to handle filtering on CAN-Packets.

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "can_filtering.h"

#include <list>
#include <map>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cstring>

#if defined(_MSC_VER)
#pragma warning( disable : 4996 )
#endif


namespace can_filtering
{

struct CanFilter
{
  bool active;

  bool doPgn;
  uint32_t pgn;

  bool doDa;
  uint8_t da;

  bool doSa;
  uint8_t sa;

  // to be extended by DLC / DataBytes  
};

static std::map<unsigned,std::list<CanFilter> > showFilters;
static std::map<unsigned,std::list<CanFilter> > hideFilters;

// the bus used for all the commands!
static unsigned filterCmdBus=0;


bool
canFilterMatch( const CanFilter &filter, uint32_t id, unsigned dlc, uint8_t *databytes )
{
  (void)dlc;
  (void)databytes;

  uint32_t idPgn = (id & 0x3FFFF00) >> 8;
  uint8_t idDa = 0xFF; // default to broadcast for PDU2
  uint8_t idSa = uint8_t( id );
  if( (idPgn & 0x0FF00) <= 0x0EF00 )
  { // PDU1, with DA
    idDa = uint8_t( idPgn );
    idPgn &= 0x3FF00;
  }

  return
       (!filter.doPgn || (filter.pgn == idPgn) )
    && (!filter.doDa || (filter.da == idDa) )
    && (!filter.doSa || (filter.sa == idSa) );
}


bool
pass( unsigned bus, uint32_t id, unsigned dlc, uint8_t *databytes )
{
  bool pass = true;
  if( !showFilters[bus].empty() )
  { // Check SHOW
    bool show = false;
    bool activeFilters = false;

    for( std::list<CanFilter>::iterator iter = showFilters[bus].begin();
      iter != showFilters[bus].end(); ++iter )
    {
      if( iter->active )
      {
        activeFilters = true;

        if( canFilterMatch( *iter, id, dlc, databytes ) )
        {
          show = true;
          break;
        }
      }
    }

    if( !show && !activeFilters )
      show = true;

    pass = show;
  }

  if( pass )
  { // Check HIDE
    for( std::list<CanFilter>::iterator iter = hideFilters[bus].begin();
      iter != hideFilters[bus].end(); ++iter )
    {
      if( iter->active )
      {
        if( canFilterMatch( *iter, id, dlc, databytes ) )
        {
          pass = false;
          break;
        }
      }
    }
  }

  return pass;
}


std::vector<std::string>
tokenize( std::string input )
{
  std::vector<std::string> result;
  char *inputCopy = strdup( input.c_str() );

  char *p = strtok( inputCopy, " " );
  while (p)
  {
    result.push_back( p ) ;
    p = strtok(NULL, " ");
  }

  free( inputCopy );
  return result;
}


std::string
listFilters( std::map<unsigned,std::list<CanFilter> > &filters )
{
  std::ostringstream oss;

  unsigned index=1;
  oss << "List of Filters for bus " << filterCmdBus << ": \n   PGN   DA SA Active?\n";
  for( std::list<CanFilter>::const_iterator iter = filters[filterCmdBus].begin();
    iter != filters[filterCmdBus].end(); ++iter, ++index )
  {
    oss << std::dec << std::setfill(' ') << std::setw(2) << index << " ";
    if( iter->doPgn )
      oss << std::hex << std::setfill('0') << std::setw(5) << iter->pgn << " "; 
    else
      oss << "xxxxx ";

    if( iter->doDa )
      oss << std::hex << std::setfill('0') << std::setw(2) << unsigned(iter->da) << " "; 
    else
      oss << "xx ";

    if( iter->doSa )
      oss << std::hex << std::setfill('0') << std::setw(2) << unsigned(iter->sa) << " "; 
    else
      oss << "xx ";

    if( iter->active )
      oss << "active";
    else
      oss << "inactive";

    oss << std::endl;
  }

  return oss.str();
}


std::string
listShow()
{
  return std::string( "\nSHOW " + listFilters( showFilters ) );
}

std::string
listHide()
{
  return std::string( "\nHIDE " + listFilters( hideFilters ) );
}

std::string
clearShow()
{
  showFilters[filterCmdBus].clear();
  return "Cleared SHOW Filters.\n";
}

std::string
clearHide()
{
  hideFilters[filterCmdBus].clear();
  return "Cleared HIDE Filters.\n";
}


std::string
load( std::vector<std::string> tokens )
{
  std::string fileName( "default.filters" );
  if( tokens.size() >= 2 )
    fileName = std::string( tokens[ 1 ] );
    
  std::ifstream file;
  file.open( fileName.c_str() );
  if( !file.is_open() )
    return std::string( "Cannot open file '" ) + fileName + std::string( "'...\n" );

  std::string line;
  std::string result;
  while( getline( file, line ) )
  {
    result += config( line );
  }
  file.close();

  return std::string( "Filters loaded from file '" ) + fileName + std::string( "':\n" ) + result;
}

std::string
saveFilters( const std::list<CanFilter> &filters, const char* showHide )
{
  std::ostringstream oss;

  for( std::list<CanFilter>::const_iterator iter = filters.begin();
    iter != filters.end(); ++iter )
  {
    oss << showHide << " add ";
    if( iter->doPgn )
      oss << std::hex << std::setfill('0') << std::setw(5) << iter->pgn << " "; 
    else
      oss << "xxxxx ";

    if( iter->doDa )
      oss << std::hex << std::setfill('0') << std::setw(2) << unsigned(iter->da) << " "; 
    else
      oss << "xx ";

    if( iter->doSa )
      oss << std::hex << std::setfill('0') << std::setw(2) << unsigned(iter->sa) << " "; 
    else
      oss << "xx";

    oss << std::endl;
  }

  return oss.str();
}

std::string
changeBus( std::vector<std::string> tokens )
{
  if ((tokens.size() < 2) || (tokens[ 1 ].empty()) )
    filterCmdBus = 0;
  else
  {
    std::istringstream iss( tokens[1] );
    iss >> filterCmdBus;
    if( iss.fail() )
      return std::string( "Wrong input. Can't switch bus.\n" );
  }

  std::ostringstream oss;
  oss << std::string( "Switched to bus " );
  oss << filterCmdBus;
  oss << std::string( ".\n");
  return oss.str();
}

std::string
save( std::vector<std::string> tokens )
{
  std::string fileName( "default.filters" );
  if( tokens.size() >= 2 )
    fileName = std::string( tokens[ 1 ] );
    
  std::ofstream file;
  file.open( fileName.c_str() );
  if( !file.is_open() )
    return std::string( "Cannot open file '" ) + fileName + std::string( "'...\n" );

  for( std::map<unsigned,std::list<CanFilter> >::iterator iter = showFilters.begin();
       iter != showFilters.end(); ++iter )
  {
    if( iter->second.empty() )
      continue;
    file << "bus " << iter->first << "\n";
    file << saveFilters( iter->second, "show" );
  }
  for( std::map<unsigned,std::list<CanFilter> >::iterator iter = hideFilters.begin();
       iter != hideFilters.end(); ++iter )
  {
    if( iter->second.empty() )
      continue;
    file << "bus " << iter->first << "\n";
    file << saveFilters( iter->second, "hide" );
  }
  file.close();

  return std::string( "Filters saved to file '" ) + fileName + std::string( "'...\n" );
}


bool
getFilter( CanFilter &filter, const std::vector<std::string> &tokens, int startIndex )
{
  if( tokens.size() <= size_t(startIndex+2) )
    return false;

  unsigned pgn, da, sa;

  std::istringstream iss( tokens[startIndex] );
  iss.setf ( std::ios::hex, std::ios::basefield );
  iss >> pgn;
  std::istringstream iss2( tokens[startIndex+1] );
  iss2.setf ( std::ios::hex, std::ios::basefield );
  iss2 >> da;
  std::istringstream iss3( tokens[startIndex+2] );
  iss3.setf ( std::ios::hex, std::ios::basefield );
  iss3 >> sa;

  filter.active = true;
  filter.pgn = pgn;
  filter.da = da;
  filter.sa = sa;
  filter.doPgn = !iss.fail();
  filter.doDa = !iss2.fail();
  filter.doSa = !iss3.fail();

  return true;
}


std::string
addShowHide( std::map<unsigned,std::list<CanFilter> > &filters, const std::vector<std::string> &tokens, const char* showHide )
{
  CanFilter filter;
  if( !getFilter( filter, tokens, 2 ) )
    return "Wrong filter-format!\n";

  filters[filterCmdBus].push_back( filter );
  std::ostringstream oss;
  oss << showHide << " Filter Nr. " << filters[filterCmdBus].size() << " added.\n";
  return oss.str();
}


std::string
activateShowHide( std::map<unsigned,std::list<CanFilter> > &filters, const std::vector<std::string> &tokens, bool activate )
{
  bool allFilters = (tokens.size() < 3) || (tokens[ 2 ].empty());
  
  int indexPlus1;
  if( !allFilters )
  {
    std::istringstream iss( tokens[2] );
    iss >> indexPlus1;
  }

  int index = 1;
  std::list<CanFilter>::iterator iter = filters[filterCmdBus].begin();
  while( iter != filters[filterCmdBus].end() )
  {
    if( allFilters )
    {
      iter->active = activate;
    }
    else
    {
      if( index == indexPlus1 )
      {
        iter->active = activate;
        return "Activation changed.\n";
      }
    }
    ++index;
    ++iter;
  }
  if( allFilters )
    return "All activations changed.\n";
  else
    return "Filter not found!\n";
}


std::string
removeShowHide( std::map<unsigned,std::list<CanFilter> > &filters, const std::vector<std::string> tokens )
{
  if( tokens.size() < 3)
    return "Invalid filter index.\n";

  std::istringstream iss( tokens[2] );
  int indexPlus1;
  iss >> indexPlus1;

  int index = 1;
  std::list<CanFilter>::iterator iter = filters[filterCmdBus].begin();
  while( iter != filters[filterCmdBus].end() )
  {
    if( index == indexPlus1 )
    {
      filters[filterCmdBus].erase( iter );
      return "Removed filter.\n";
    }
    ++index;
    ++iter;
  }
  return "Filter not found!\n";
}


std::string
config( const std::string &cmd )
{
  std::vector<std::string> tokens = tokenize( cmd );

  if( !tokens.empty() )
  {
    if (tokens[ 0 ] == "show") {
      if( tokens.size() >= 2 ) {
        if (tokens[ 1 ] == "list") {
          return listShow();
        } else if (tokens[ 1 ] == "clear") {
          return clearShow();
        } else if (tokens[ 1 ] == "add") {
          return addShowHide( showFilters, tokens, "SHOW" );
        } else if (tokens[ 1 ] == "remove") {
          return removeShowHide( showFilters, tokens );
        } else if (tokens[ 1 ] == "on") {
          return activateShowHide( showFilters, tokens, true );
        } else if (tokens[ 1 ] == "off") {
          return activateShowHide( showFilters, tokens, false );
        }
      }
    } else if (tokens[ 0 ] == "hide") {
      if( tokens.size() >= 2 ) {
        if (tokens[ 1 ] == "list") {
          return listHide();
        } else if (tokens[ 1 ] == "clear") {
          return clearHide();
        } else if (tokens[ 1 ] == "add") {
          return addShowHide( hideFilters, tokens, "HIDE" );
        } else if (tokens[ 1 ] == "remove") {
          return removeShowHide( hideFilters, tokens );
        } else if (tokens[ 1 ] == "on") {
          return activateShowHide( hideFilters, tokens, true );
        } else if (tokens[ 1 ] == "off") {
          return activateShowHide( hideFilters, tokens, false );
        }
      }
    } else if (tokens[ 0 ] == "list") {
      return listShow() + listHide();
    } else if (tokens[ 0 ] == "clear") {
      return clearShow() + clearHide();
    } else if (tokens[ 0 ] == "load") {
      return load( tokens );
    } else if (tokens[ 0 ] == "save") {
      return save( tokens );
    } else if (tokens[ 0 ] == "on") {
      return activateShowHide( showFilters, tokens, true ) + activateShowHide( hideFilters, tokens, true );
    } else if (tokens[ 0 ] == "off") {
      return activateShowHide( showFilters, tokens, false) + activateShowHide( hideFilters, tokens, false);
    } else if (tokens[ 0 ] == "bus") {
      return changeBus( tokens );
    }
  }

  return 
    "Help on filtering:\n"
    "  bus <busnr>                         switch the bus-number the following\n"
    "                                      commands apply to. defaults to 0.\n"
    "  show|hide list                      list all show or hide filters.\n"
    "  show|hide clear                     clears the show or hide filter lists.\n"
    "  show|hide add <pgn> <da> <sa>       given values in HEX or 'x'(don't care)\n"
    "  show|hide remove <index>            given index refers to the list.\n"
    "  show|hide on <index>                activates the given filter.\n"
    "  show|hide off <index>               deactivates the given filter.\n"
    "                        <index>-Note: If no index given, all will be done.\n"
    "  on                                  activates all show & hide filters.\n"
    "  off                                 deactivates all show & hide filters.\n"
    "  list                                lists all filters.\n"
    "  clear                               clears show & hide filters.\n"
    "  load <filename>                     load & execute the set of commands.\n"
    "  save <filename>                     saves current filter set to the file.\n"
    "                     <filename>-Note: Leaving the <filename> empty will use\n"
    "                                      the default filename 'default.filters'\n"
    ;
}

}
