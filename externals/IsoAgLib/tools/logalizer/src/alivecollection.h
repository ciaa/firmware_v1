/*
  alivecollection.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef ALIVECOLLECTION_H
#define ALIVECOLLECTION_H

#include <logenvirons.h>
#include <vector>


enum msgType_e
{
  msgTypePeriodic,
  msgTypeCommand,
  msgTypeResponse,
  msgTypeRTS,
  msgTypeCTS,
  msgTypeDATA,
  msgTypeDPO,
  msgTypeEOMACK,
  msgTypeCONNABORT,
  msgTypeSetpoint, // TC->Client
  msgTypeMeasurement // Client->TC
};


class AliveCollection_c {
public:
  enum Category_e {
    categoryBegin,
    categoryVtServerClientPeriodic = categoryBegin,
    categoryTcServerClientPeriodic,
    categoryNotAcknowledge,
    categoryVtCommandResponse,
    categoryEtpTpCommunicationTimes,
    categoryProcessData,
    categoryEnd
  };

  AliveCollection_c();

  std::vector< uint64_t > &alives(
    Category_e ae_category,
    uint8_t ui8_address);

  std::vector< uint64_t > const &alives(
    Category_e ae_category,
    uint8_t ui8_address) const;

  std::vector< std::pair< msgType_e, std::string > > &response(
      Category_e ae_category,
      uint8_t ui8_address);

  std::vector< std::pair< msgType_e, std::string > > const &response(
      Category_e ae_category,
      uint8_t ui8_address) const;

  int32_t alivePeriod(Category_e ae_category) const;

  char const *name(Category_e ae_category) const;

  void report( std::ostream& ) const;

private:
  static Category_e nextCategory(Category_e ae_category);

  void setAlivePeriod(Category_e ae_category, int32_t ai32_alivePeriod);

  void setName(Category_e ae_category, char const *pch_name);

  struct Category_s {
    std::vector<uint64_t> svec_alives[256];
    std::vector<std::pair<msgType_e, std::string> > svec_response[256];
    const char* name;
    int32_t alivePeriod; // ==0 => single Event - >0 ==> periodic Event - <0 ==> handshaking Event
    Category_s();
  } marr_categories[categoryEnd];
};


inline
AliveCollection_c::Category_e
AliveCollection_c::nextCategory( Category_e ae_category )
{
  return (categoryEnd == ae_category) ?
    categoryEnd :
    Category_e(ae_category + 1);
}


inline
AliveCollection_c::Category_s::Category_s() :
  svec_alives(),
  svec_response(),
  name(""),
  alivePeriod(0)
{
}


inline
std::vector< uint64_t > &
AliveCollection_c::alives(
    Category_e ae_category,
    uint8_t ui8_address)
{
  return marr_categories[ae_category].svec_alives[ui8_address];
}


inline
std::vector< uint64_t > const &
AliveCollection_c::alives(
    Category_e ae_category,
    uint8_t ui8_address) const
{
  return marr_categories[ae_category].svec_alives[ui8_address];
}


inline
std::vector< std::pair< msgType_e, std::string > > &
AliveCollection_c::response(
    Category_e ae_category,
    uint8_t ui8_address)
{
  return marr_categories[ae_category].svec_response[ui8_address];
}


inline
std::vector< std::pair< msgType_e, std::string > > const &
AliveCollection_c::response(
    Category_e ae_category,
    uint8_t ui8_address) const
{
  return marr_categories[ae_category].svec_response[ui8_address];
}


inline 
int32_t
AliveCollection_c::alivePeriod( Category_e ae_category ) const
{
  return marr_categories[ae_category].alivePeriod;
}


inline
void
AliveCollection_c::setAlivePeriod( Category_e ae_category, int32_t ai32_alivePeriod )
{
  marr_categories[ae_category].alivePeriod = ai32_alivePeriod;
}


inline
char const *
AliveCollection_c::name( Category_e ae_category ) const
{
  return marr_categories[ae_category].name;
}


inline
void
AliveCollection_c::setName( Category_e ae_category, char const *pch_name )
{
  marr_categories[ae_category].name = pch_name;
}


#endif
