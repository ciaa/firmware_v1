/*
  diagnosticfunctionalities_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "diagnosticfunctionalities_c.h"

#include <IsoAgLib/comm/Part5_NetworkManagement/impl/identitem_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isorequestpgn_c.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/multisend_c.h>
#include <IsoAgLib/comm/impl/isobus_c.h>
#include <IsoAgLib/util/iassert.h>
#include <IsoAgLib/util/impl/util_funcs.h>
#include <stdlib.h>

#include <functional>
#include <numeric>


// not done as private member function because of recursive-include problems.
// IdentItem_c needs DiagnosticPgnHandler_c because of the enums.
// Actually those enums should then just be moved out into its own file
// then the problems would vanish and it would be clean. But the define is
// okay for this case here, as the singletons are currently all done by define.
#define getMultitonInst() mrc_identItem.getMultitonInst()


namespace __IsoAgLib
{

DiagnosticFunctionalities_c::DiagnosticFunctionalities_c( IdentItem_c& arc_identItem )
  : mrc_identItem( arc_identItem )
  , mt_multiSendEventHandler()
  , m_currentFunctionalities( NULL )
  , mui16_arrayLength( 0 )
  , m_functionalities()
  , m_functionalitiesDirty( true )
{
}


DiagnosticFunctionalities_c::~DiagnosticFunctionalities_c()
{
  if (m_currentFunctionalities)
    CNAMESPACE::free (m_currentFunctionalities);
}


void
DiagnosticFunctionalities_c::init()
{
  addFunctionalitiesMinimumControlFunction( 1, MinimumControlFunctionOptionsBitMask_t() );

  getIsoRequestPgnInstance4Comm().registerPGN ( *this, CONTROL_FUNCTION_FUNCTIONALITIES_PGN );
}


void
DiagnosticFunctionalities_c::close()
{
  getIsoRequestPgnInstance4Comm().unregisterPGN ( *this, CONTROL_FUNCTION_FUNCTIONALITIES_PGN );
}


void
DiagnosticFunctionalities_c::updatePackageIfNeeded()
{
  isoaglib_assert(!mt_multiSendEventHandler.m_isMultiSendRunning);

  if( !m_functionalitiesDirty )
    return;

  m_functionalitiesDirty = false;

  // destroy previously constructed message
  if (m_currentFunctionalities)
    CNAMESPACE::free (m_currentFunctionalities);

  // compute new length
  mui16_arrayLength = 2; // header size
  for (FunctionalitiesArrIter iter = m_functionalities.begin();
       iter != m_functionalities.end(); ++iter )
  {
    // Functionality - generation - number_of_option_bytes - option_1 - ... - option_n
    mui16_arrayLength = uint16_t(mui16_arrayLength + 3 + iter->second.number_of_option_bytes);
  }
  if (mui16_arrayLength < 8)
    mui16_arrayLength = 8;

  // create array for the message
  m_currentFunctionalities = (uint8_t *) CNAMESPACE::malloc (sizeof (uint8_t) * mui16_arrayLength);
  
  // fill the array
  m_currentFunctionalities[0] = 0xFF;
  uint16_t byte_counter = 1;
  m_currentFunctionalities[byte_counter++] = static_cast<uint8_t>(m_functionalities.size());
  for (FunctionalitiesArrIter iter = m_functionalities.begin();
       iter != m_functionalities.end(); ++iter )
  {
    m_currentFunctionalities[byte_counter++] = static_cast<uint8_t>(iter->first);
    m_currentFunctionalities[byte_counter++] = iter->second.generation;
    m_currentFunctionalities[byte_counter++] = iter->second.number_of_option_bytes;
    isoaglib_assert(iter->second.number_of_option_bytes <= MAX_OPTION_BYTES);
    for (uint8_t option = 0; option < iter->second.number_of_option_bytes; ++option)
      m_currentFunctionalities[byte_counter++] = iter->second.options_bytes[option];
  }
 
  // in case size is less than 8 -> fill rest of the message with 0xFF
  for (; byte_counter < 8; ++byte_counter)
  {
    m_currentFunctionalities[byte_counter] = 0xFF;
  }
  
  // control size in debug mode
  isoaglib_assert(mui16_arrayLength == byte_counter);
}


void
DiagnosticFunctionalities_c::MultiSendEventHandlerProxy_c::reactOnStateChange(const SendStream_c& sendStream)
{
  switch (sendStream.getSendSuccess())
  {
  case SendStream_c::SendAborted:
  case SendStream_c::SendSuccess:
    m_isMultiSendRunning = false;
    break;
  case SendStream_c::Running:
    break;
  }
}


bool
DiagnosticFunctionalities_c::processMsgRequestPGN ( uint32_t rui32_pgn, IsoItem_c* apc_isoItemSender, IsoItem_c* rpc_isoItemReceiver, int32_t )
{
  if( !mrc_identItem.isClaimedAddress() )
    return false;

  // we're not Network Management, so don't answer requests from 0xFE
  if( apc_isoItemSender == NULL )
    return false;

  if( ( rpc_isoItemReceiver != NULL ) && ( mrc_identItem.getIsoItem() != rpc_isoItemReceiver ) )
    return false; // request not adressed to us!

  switch( rui32_pgn )
  {
  case CONTROL_FUNCTION_FUNCTIONALITIES_PGN:
    if (!mt_multiSendEventHandler.m_isMultiSendRunning)
    {
      updatePackageIfNeeded();

      // send package
      if (mui16_arrayLength <= 8)
      {
        sendSinglePacket( m_currentFunctionalities, CONTROL_FUNCTION_FUNCTIONALITIES_PGN );
        return true;
      }
      else
      {
        if ( getMultiSendInstance4Comm().sendIsoBroadcast(
              mrc_identItem.isoName(),
              m_currentFunctionalities,
              mui16_arrayLength,
              CONTROL_FUNCTION_FUNCTIONALITIES_PGN,
              &mt_multiSendEventHandler) )
        { // Message successfully transmitted to multisend -> return true
          mt_multiSendEventHandler.m_isMultiSendRunning = true;
          return true;
        }
      }
    }
    // all "else"s: CannotRespondNow below!
    break;

  default:
    isoaglib_assert( !"Internal ISOAgLib-failure: Not registered for answering this PGN." );
  }

#if DEBUG_DIAGNOSTICPGN
  INTERNAL_DEBUG_DEVICE << "Couldn't response to RequestPGN with PGN=" << rui32_pgn << ". " << INTERNAL_DEBUG_DEVICE_ENDL;
#endif

  // something wrong happend - answer with CannotRespondNow
  //  (couldn't multisend or Identification not yet ready)
  getIsoRequestPgnInstance4Comm().answerRequestPGNwithACK ( *mrc_identItem.getIsoItem(), 0x03 ); // CannotRespondNow ACKNOWLEDGE
  return true;
}


void
DiagnosticFunctionalities_c::sendSinglePacket (const HUGE_MEM uint8_t* rhpb_data, int32_t ai32_pgn)
{
  CanPkgExt_c pkg;
  pkg.setIsoPri (6);
  pkg.setMonitorItemForSA (mrc_identItem.getIsoItem());
  pkg.setLen (8);
  pkg.setIsoPgn(ai32_pgn);
  for (uint8_t ui = 0 ; ui < 8; ++ui)
    pkg.setUint8Data (ui, rhpb_data[ui]);

  getIsoBusInstance4Comm() << pkg;
}


uint8_t
DiagnosticFunctionalities_c::getGeneration(FunctionalitiesCharacteristics_t functionality, uint8_t version) const
{
  switch (functionality)
  {
    case MinimumControlFunction:
      if (version == 1) return 1;
      break;
    case UniversalTerminal:
    case UniversalTerminalWorkingSet:
      if (version == 2) return 1;
      if (version == 3) return 2;
      if (version == 4) return 3;
      break;
    case AuxOInputs:
    case AuxOFunctions:
      if (version == 2) return 1;
      break;
    case AuxNInputs:
    case AuxNFunctions:
      if (version == 3) return 1;
      break;
    case TaskControllerBasic:
    case TaskControllerBasicWorkingSet:
    case TaskControllerGeo:
    case TaskControllerGeoWorkingSet:
    case TaskControllerSectionControl:
    case TaskControllerSectionControlWorkingSet:
      if (version == 3) return 1;
      break;
    case BasicTractorECU:
    case BasicTractorECUImplement:
      if (version == 1) return 1;
      break;
    case StopAllImplementOperations:
    case StopAllImplementOperationsImplement:
      if (version == 1) return 1;
      break;
#if 0
    case AdvanceTractorECU:
    case AdvanceTractorECUImplement:
      if (version == 1) return 1;
      break;
    case FileServer:
    case FileServerClient:
      if (version == 2) return 1;
      if (version == 3) return 2;
      break;
    case SequenceControlServer:
    case SequenceControlClient:
      if (version == 1) return 1;
      break;
    case DiagnosticTool:
    case DiagnosticECU:
      if (version == 1) return 1;
      break;
#endif
    case Functionality_ReservedForISO:
      break;
  }
  isoaglib_assert(false);
  return 0;
}


bool DiagnosticFunctionalities_c::addFunctionality(FunctionalitiesCharacteristics_t functionality, const Functionality_s& functionality_description)
{
  if ( m_functionalities.find(functionality) == m_functionalities.end() )
  {
    m_functionalities.insert( FunctionalitiesArrPair(functionality, functionality_description) );
    m_functionalitiesDirty = true;
    return true;
  }
  return false;
}

bool DiagnosticFunctionalities_c::addFunctionalitiesMinimumControlFunction(uint8_t version, const MinimumControlFunctionOptionsBitMask_t& options)
{
  FunctionalitiesCharacteristics_t functionality = MinimumControlFunction;
  return fillStructure(functionality, version, options);
}

bool DiagnosticFunctionalities_c::addFunctionalitiesUniversalTerminal(bool implement, uint8_t version, const UniversalTerminalOptionsBitMask_t& options)
{
  FunctionalitiesCharacteristics_t functionality = ( implement ? UniversalTerminalWorkingSet : UniversalTerminal );
  return fillStructure(functionality, version, options);
}

bool DiagnosticFunctionalities_c::addFunctionalitiesAuxO(bool function, uint8_t version, const AuxOOptionsBitMask_t& options)
{
  FunctionalitiesCharacteristics_t functionality = ( function ? AuxOFunctions : AuxOInputs );
  return fillStructure(functionality, version, options);
}

bool DiagnosticFunctionalities_c::addFunctionalitiesAuxN(bool function, uint8_t version, const AuxNOptionsBitMask_t& options)
{
  FunctionalitiesCharacteristics_t functionality = ( function ? AuxNFunctions : AuxNInputs );
  return fillStructure(functionality, version, options);
}

bool DiagnosticFunctionalities_c::addFunctionalitiesTaskControllerBasic(bool implement, uint8_t version, const TaskControllerBasicOptionsBitMask_t& options)
{
  FunctionalitiesCharacteristics_t functionality = ( implement ? TaskControllerBasicWorkingSet : TaskControllerBasic );
  return fillStructure(functionality, version, options);
}

bool DiagnosticFunctionalities_c::addFunctionalitiesTaskControllerGeo(bool implement, uint8_t version, uint8_t numberOfChannels, const TaskControllerGeoOptionsBitMask_t& options)
{
  isoaglib_assert( numberOfChannels >= 1 );

  FunctionalitiesCharacteristics_t functionality = ( implement ? TaskControllerGeoWorkingSet : TaskControllerGeo );
  
  // fill structure
  Functionality_s functionality_description;
  functionality_description.generation = getGeneration(functionality, version);

  functionality_description.options_bytes[0] = numberOfChannels;
  functionality_description.number_of_option_bytes = 1;
  if( options.getByte( 0 ) )
  {
    functionality_description.options_bytes[1] = options.getByte( 0 );
    functionality_description.number_of_option_bytes++;
  }
  else
    functionality_description.options_bytes[1] = 0xDA; // dummy, just to avoid "may be used uninitialized" warning

  return addFunctionality(functionality, functionality_description);
}

bool DiagnosticFunctionalities_c::addFunctionalitiesTaskControllerSectionControl(bool implement, uint8_t version, uint8_t numberOfBooms, uint8_t numberOfSections)
{
  FunctionalitiesCharacteristics_t functionality = ( implement ? TaskControllerSectionControlWorkingSet : TaskControllerSectionControl );
  
  // fill structure
  Functionality_s functionality_description;
  functionality_description.generation = getGeneration(functionality, version);
  functionality_description.number_of_option_bytes = 2;
  functionality_description.options_bytes[0] = numberOfBooms;
  functionality_description.options_bytes[1] = numberOfSections;  

  return addFunctionality(functionality, functionality_description);
}

bool DiagnosticFunctionalities_c::addFunctionalitiesBasicTractorECU(bool implement, uint8_t version, const BasicTractorECUOptionsBitMask_t& options)
{
  FunctionalitiesCharacteristics_t functionality = ( implement ? BasicTractorECUImplement : BasicTractorECU );
  return fillStructure(functionality, version, options);
}

bool DiagnosticFunctionalities_c::addFunctionalitiesStopAllImplementOperations(bool implement, uint8_t version, const StopAllImplementOperationsOptionsBitMask_t& options)
{
  FunctionalitiesCharacteristics_t functionality = ( implement ? StopAllImplementOperationsImplement : StopAllImplementOperations );
  return fillStructure(functionality, version, options);
}

#if 0
bool DiagnosticFunctionalities_c::addFunctionalitiesAdvanceTractorECU(bool implement, uint8_t version, const AdvanceTractorECUOptionsBitMask_t& options)
{
  FunctionalitiesCharacteristics_t functionality = ( implement ? AdvanceTractorECUImplementSet : AdvanceTractorECU );
  return fillStructure(functionality, version, options);
}

bool DiagnosticFunctionalities_c::addFunctionalitiesSequenceControl(bool implement, uint8_t version, const SequenceControlOptionsBitMask_t& options)
{
  FunctionalitiesCharacteristics_t functionality = ( implement ? SequenceControlClient : SequenceControlServer );
  return fillStructure(functionality, version, options);
}

bool DiagnosticFunctionalities_c::addFunctionalitiesFileServer(bool implement, uint8_t version, const FileServerOptionsBitMask_t& options)
{
  FunctionalitiesCharacteristics_t functionality = ( implement ? FileServerClient : FileServer );
  return fillStructure(functionality, version, options);
}

bool DiagnosticFunctionalities_c::addFunctionalitiesDiagnosticTool()
{
  FunctionalitiesCharacteristics_t functionality = DiagnosticTool;
  Functionality_s functionality_description;
  functionality_description.generation = getGeneration(functionality, 1); // TBD DiagnosticTool version ?
  functionality_description.number_of_option_bytes = 0;
  return addFunctionality(functionality, functionality_description);
}

bool DiagnosticFunctionalities_c::addFunctionalitiesDiagnosticECU()
{
  FunctionalitiesCharacteristics_t functionality = DiagnosticECU;
  Functionality_s functionality_description;
  functionality_description.generation = getGeneration(functionality, 1); // TBD DiagnosticECU version ?
  functionality_description.number_of_option_bytes = 0;
  return addFunctionality(functionality, functionality_description);
}
#endif

bool
DiagnosticFunctionalities_c::remFunctionalities(FunctionalitiesCharacteristics_t functionality)
{
  FunctionalitiesArrIter iter = m_functionalities.find(functionality);
  if ( iter != m_functionalities.end() )
  {
    m_functionalities.erase( iter );
    m_functionalitiesDirty = true;
    return true;
  }
  return false; // not found
}


}
