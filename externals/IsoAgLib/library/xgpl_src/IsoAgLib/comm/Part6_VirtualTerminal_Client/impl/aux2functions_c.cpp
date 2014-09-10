/*
  aux2functions_c.h - 

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "aux2functions_c.h"
#include "vtclient_c.h"
#include "vtclientconnection_c.h"

#include <IsoAgLib/comm/Part6_VirtualTerminal_Client/ivtclient_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isomonitor_c.h>


namespace __IsoAgLib {

// this structure is used as map key for collecting all preferred AUX2 assignments
// (sorted by isoname and model identification of the preferred AUX2 input device)
struct Aux2PreferredAssignmentInputDevice_s
{ // operator< is necessary for using it as map key
  bool operator<(const Aux2PreferredAssignmentInputDevice_s& ref) const
  { 
    if (ui16_model == ref.ui16_model)
      return (isoName < ref.isoName);
    else
      return (ui16_model < ref.ui16_model);
  }

  IsoName_c isoName;
  uint16_t ui16_model;
};


struct Aux2PreferredAssignmentObjects_s
{
  uint16_t ui16_input;
  uint16_t ui16_function;
};


Aux2Functions_c::Aux2Functions_c(VtClientConnection_c& vtClientConnection )
  : SchedulerTask_c( -1, false )
  , m_vtConnection( vtClientConnection )
  , m_state(State_WaitForPoolUploadSuccessfully)
  , mb_learnMode(false)
{
}


Aux2Functions_c::~Aux2Functions_c()
{
  if( isRegistered() )
    getSchedulerInstance().deregisterTask( *this );
}


void Aux2Functions_c::loadAssignment() {
#ifdef USE_VTOBJECT_auxiliaryfunction2
  IsoAgLib::iAux2Assignment_c assigment;
  m_vtConnection.getVtClientDataStorage().loadPreferredAux2Assignment( assigment );
  if( ! assigment.empty() ) { 
    for( IsoAgLib::iAux2AssignmentIterator_c i = assigment.begin(); i != assigment.end(); ++i ) {
      STL_NAMESPACE::map<uint16_t, vtObjectAuxiliaryFunction2_c*>::iterator iter = m_aux2Function.find( i->functionUid );
      if( iter != m_aux2Function.end() ) {
        iter->second->setPreferredAssignedInput( i->input.name, i->input.modelIdentificationCode, i->input.uid );
      }
    }
  }
#endif
}

void
Aux2Functions_c::notifyOnAux2InputStatus(
  const CanPkgExt_c& arc_data, 
  IsoAgLib::iVtClientObjectPool_c& arc_pool )
{
#ifdef USE_VTOBJECT_auxiliaryfunction2
  if (mb_learnMode || ((arc_data.getUint8Data(8-1) & 0x3) != 0))
    return; // do not respond to input status messages in learn mode or learn mode bits are set in received message

  const IsoName_c& c_inputIsoName = arc_data.getISONameForSA();
  uint16_t const cui16_inputObjectId = arc_data.getUint16Data(2-1);

  IsoName_c c_assignedIsoName;
  uint16_t ui16_assignedInputUid = 0xFFFF;

  for (STL_NAMESPACE::map<uint16_t, vtObjectAuxiliaryFunction2_c*>::iterator iter = m_aux2Function.begin(); iter != m_aux2Function.end(); ++iter)
  {
    iter->second->getAssignedInput(c_assignedIsoName, ui16_assignedInputUid);

    if ( (0xFFFF != ui16_assignedInputUid) &&
         (IsoName_c::IsoNameUnspecified() != c_assignedIsoName) &&
         (cui16_inputObjectId == ui16_assignedInputUid) &&
         (c_inputIsoName == c_assignedIsoName) )
    { // notify application on this new Input Status!
      const uint16_t cui16_value1 = arc_data.getUint16Data (4-1);
      const uint16_t cui16_value2 = arc_data.getUint16Data (6-1);
      const uint8_t cui8_operatingState = arc_data.getUint8Data (8-1);
      arc_pool.eventAuxFunction2Value (iter->first, cui16_value1, cui16_value2, cui8_operatingState);
    }
  }
#else
  (void)arc_data;
  (void)arc_pool;
#endif
}

void
Aux2Functions_c::notifyOnAux2InputMaintenance( const CanPkgExt_c& arc_data )
{
#ifdef USE_VTOBJECT_auxiliaryfunction2
  bool b_sendPreferredAssignments = false;

  const int32_t i32_now = HAL::getTime();
  const IsoName_c c_inputIsoName = arc_data.getISONameForSA();
  const uint16_t ui16_modelIdentificationCode = arc_data.getUint16Data(2-1);


  const uint8_t ui8_status = arc_data.getUint8Data(4-1);
  if (1 != ui8_status)
  {
    // @todo: maintenance with initializing should not cause a timeout
    return; // inputs with status not "ready" should not trigger sendPreferredAux2Assignments()
  }

  switch (m_state)
  {
    case State_WaitForPoolUploadSuccessfully:
      return; // nothing to do

    case State_WaitForFirstInputMaintenanceMessage:
      // we have some preferred assignments (otherwise this state would not be set in objectPoolUploadedSuccessfully
      // => let's wait for some time to receive input maintenance messages from different devices to send a more complete preferred assignment message
      m_state = State_CollectInputMaintenanceMessage;
      break;
    case State_CollectInputMaintenanceMessage:
    case State_Ready:
      break; // handle these cases in next switch, because m_state might have been modified in the above cases.
  }
    
  bool initializingToReady = false;
  if (c_inputIsoName.isSpecified()) {
    initializingToReady = ( mmap_receivedInputMaintenanceData.find( c_inputIsoName ) == mmap_receivedInputMaintenanceData.end() );
    mmap_receivedInputMaintenanceData[c_inputIsoName] = InputMaintenanceDataForIsoName_s(ui16_modelIdentificationCode, i32_now );
  }

  // first one or we removed the last one
  if( initializingToReady && ( mmap_receivedInputMaintenanceData.size() == 1 ) ) {
    getSchedulerInstance().registerTask( *this, m_vtConnection.getVtClientDataStorage().getAux2DeltaWaitBeforeSendingPreferredAssigment() );
  }

  IsoName_c c_prefAssignedIsoName;
  uint16_t ui16_prefAssignedModelIdentificationCode;
  uint16_t ui16_prefAssignedInputUid;

  switch (m_state)
  {
    case State_WaitForPoolUploadSuccessfully:
    case State_WaitForFirstInputMaintenanceMessage:
      break; // handled above

    case State_CollectInputMaintenanceMessage:
    case State_Ready:
      if( initializingToReady ) {
        for (STL_NAMESPACE::map<uint16_t, vtObjectAuxiliaryFunction2_c*>::iterator iter = m_aux2Function.begin(); iter != m_aux2Function.end(); ++iter)
        {
          if(iter->second->getMatchingPreferredAssignedInputReady())
            continue; // this input device was already found in our preferred assignments => skip it
          
          iter->second->getPreferredAssignedInput(c_prefAssignedIsoName, ui16_prefAssignedModelIdentificationCode, ui16_prefAssignedInputUid);
          if ( (ui16_prefAssignedModelIdentificationCode == ui16_modelIdentificationCode) && (c_prefAssignedIsoName == c_inputIsoName) ) { 
            iter->second->setMatchingPreferredAssignedInputReady(true);
            b_sendPreferredAssignments = true;
          }
        }
      }
      break;
  }

  if ( ( m_state == State_Ready ) && b_sendPreferredAssignments )
    sendPreferredAux2Assignments();
#else
  (void)arc_data;
#endif
}

void
Aux2Functions_c::objectPoolUploadedSuccessfully()
{
#ifdef USE_VTOBJECT_auxiliaryfunction2
  // if we don't have any preferred assignments => send empty preferred assignment msg
  bool b_preferredAssignmentFound = false;
  for (STL_NAMESPACE::map<uint16_t, vtObjectAuxiliaryFunction2_c*>::iterator iter = m_aux2Function.begin(); iter != m_aux2Function.end(); ++iter)
  {
    if( iter->second->hasPreferredAssigment() ) {
      b_preferredAssignmentFound = true;
      break;
    }
  }

  if (!b_preferredAssignmentFound)
  {
    sendPreferredAux2Assignments();
    m_state = State_Ready;    
  }
  else
  { // we have preferred assignments => wait for first input maintenance message
    m_state = State_WaitForFirstInputMaintenanceMessage;
  }
#endif
}

uint8_t
Aux2Functions_c::storeAux2Assignment(
  Stream_c& arc_stream, 
  uint16_t& rui16_functionObjId, 
  IsoAgLib::iVtClientObjectPool_c& arc_pool )
{
#ifdef USE_VTOBJECT_auxiliaryfunction2
  const uint16_t ui16_totalstreamsize = arc_stream.getByteTotalSize();
  
  if (ui16_totalstreamsize != 14)
    return (1<<0); // message should always have 14 bytes

  // first byte already consumed from stream! --> Byte-offset -2 instead of -1!
  STL_NAMESPACE::vector<uint8_t> c_buffer;
  for (uint16_t i = 0; i < arc_stream.getByteTotalSize(); i++)
  {
    c_buffer.push_back( arc_stream.get() );
  }

  const bool b_preferredAssignment = !(c_buffer[10-2] & (1 << 7));
  uint8_t funcType = (c_buffer[10-2] & 0x1F);
  uint16_t inputObjId = (c_buffer[11-2] | (c_buffer[12-2] << 8));
  // set reference of function object ID (is needed for response in caller)
  rui16_functionObjId = (c_buffer[13-2] | (c_buffer[14-2] << 8));

  bool b_preferredAssignedInputsChanged = false;

  if (0xFFFF == rui16_functionObjId)
  { // unassign ALL functions
    for (STL_NAMESPACE::map<uint16_t, vtObjectAuxiliaryFunction2_c*>::iterator iter = m_aux2Function.begin(); iter != m_aux2Function.end(); ++iter)
    {
      if( iter->second->setAssignedInput(IsoName_c::IsoNameUnspecified(), 0xFFFF) )
        arc_pool.aux2AssignmentChanged( *( static_cast<IsoAgLib::iVtObjectAuxiliaryFunction2_c*>( iter->second ) ) );

      if( b_preferredAssignment )
        b_preferredAssignedInputsChanged |= iter->second->setPreferredAssignedInput(IsoName_c::IsoNameUnspecified(), 0xFFFF /* default value */, 0xFFFF);
    }
  }
  else
  { // (un)assign given function
    STL_NAMESPACE::map<uint16_t, vtObjectAuxiliaryFunction2_c*>::iterator iter = m_aux2Function.find( rui16_functionObjId );
    if( iter != m_aux2Function.end() )
    {
      bool unassignName = true;
      for( int i=0; i<8; ++i )
      {
        if( c_buffer[2-2+i] != 0xFF )
        {
          unassignName = false;
          break;
        }
      }

      const bool assign = (funcType != 0x1F) && (inputObjId != 0xFFFF) && (!unassignName);
      
      IsoName_c c_inputIsoName; // defaults to unspecified
      uint16_t ui16_modelIdentificationCode = 0xFFFF;
      
      if( assign )
      {
        c_inputIsoName = IsoName_c( &c_buffer[2-2] );
        STL_NAMESPACE::map<IsoName_c,InputMaintenanceDataForIsoName_s>::const_iterator iter_map = mmap_receivedInputMaintenanceData.find(c_inputIsoName);
        if( iter_map == mmap_receivedInputMaintenanceData.end() )
          // @todo also check for READY?
          return (1<<0); // we did not yet receive an input maintenance message for this isoname => unknown
        // get model identification code from mmap_receivedInputMaintenanceData
        // (sort of a "backdoor" method because we do not get the code in the message!)
        ui16_modelIdentificationCode = iter_map->second.mui16_inputModelIdentificationCode;
      }
      else
      { // unassign
        // make sure to set all values to the "unassign"-values, even if only one was set so.
        funcType = 0x1F;
        inputObjId = 0xFFFF;
        //c_inputIsoName is still unspecified!
      }

      /// @todo We'd need to check if the Input is existing!!!      
      if( iter->second->setAssignedInput( c_inputIsoName, inputObjId ) )
        arc_pool.aux2AssignmentChanged( *( static_cast<IsoAgLib::iVtObjectAuxiliaryFunction2_c*>( iter->second ) ) );

      if (b_preferredAssignment)
        b_preferredAssignedInputsChanged |= iter->second->setPreferredAssignedInput(c_inputIsoName, ui16_modelIdentificationCode, inputObjId);
    }
    else
    {
      return (1<<0); // function object not found.
    }
  }

  if (b_preferredAssignedInputsChanged)
  {
    IsoAgLib::iAux2Assignment_c assignment;

    for (STL_NAMESPACE::map<uint16_t, vtObjectAuxiliaryFunction2_c*>::iterator iter = m_aux2Function.begin(); iter != m_aux2Function.end(); ++iter) {
      if( iter->second->hasPreferredAssigment() ) {
        IsoAgLib::iAux2FunctionInputAssignment_t a;
        a.functionUid = iter->first;
        iter->second->getPreferredAssignedInput( a.input.name, a.input.modelIdentificationCode, a.input.uid );
        assignment.push_front( a );
      }
    }
 
    m_vtConnection.getVtClientDataStorage().storePreferredAux2Assignment( assignment );
  }

#else
  (void)arc_stream;
  (void)rui16_functionObjId;
  (void)arc_pool;
#endif

  return 0x00;
}


void
Aux2Functions_c::timeEvent()
{
#ifdef USE_VTOBJECT_auxiliaryfunction2
  const int32_t i32_now = HAL::getTime();
  int32_t next = i32_now + 300;

  // 1. check for timed out inputs
  for (STL_NAMESPACE::map<IsoName_c,InputMaintenanceDataForIsoName_s>::iterator iter_map = mmap_receivedInputMaintenanceData.begin();
      iter_map != mmap_receivedInputMaintenanceData.end();)
  {
    const int32_t timeout = iter_map->second.mi32_timeLastAux2Maintenance + 300;
    if ( i32_now > timeout )
    {
      // timeout => unassign all functions with matching isoname
      for (STL_NAMESPACE::map<uint16_t, vtObjectAuxiliaryFunction2_c*>::const_iterator iter_function = m_aux2Function.begin();
          iter_function != m_aux2Function.end(); ++iter_function)
      {
        if( iter_function->second->unassignInputIfIsoNameMatches(iter_map->first) ) {
          // allow sending of preferred assignment if matching input appears again
          iter_function->second->setMatchingPreferredAssignedInputReady(false);
            
          m_vtConnection.getPool().aux2AssignmentChanged( *( static_cast<IsoAgLib::iVtObjectAuxiliaryFunction2_c*>( iter_function->second ) ) );
        }
      }
      mmap_receivedInputMaintenanceData.erase(iter_map++);
    } else {
      if( timeout < next ) {
        next = timeout;
      }
      ++iter_map;
    }
  }
  if( ! mmap_receivedInputMaintenanceData.empty() ) {
    setNextTriggerTime( next );
  } // else the task is deregistered cause of a period < 0. see scheduler

  // 2. send preferred assignment
  if( ! mmap_receivedInputMaintenanceData.empty() && ( m_state == State_CollectInputMaintenanceMessage ) ) {
    sendPreferredAux2Assignments();
    m_state = State_Ready;
  }
#endif
}


bool
Aux2Functions_c::sendPreferredAux2Assignments()
{
#ifdef USE_VTOBJECT_auxiliaryfunction2
  if( m_aux2Function.empty() )
    return true;

  static SendUpload_c msc_tempSendUpload;
  Aux2PreferredAssignmentInputDevice_s key;
  Aux2PreferredAssignmentObjects_s objIds;
  STL_NAMESPACE::map<Aux2PreferredAssignmentInputDevice_s, STL_NAMESPACE::list<Aux2PreferredAssignmentObjects_s> > map_Inputs;

  for (STL_NAMESPACE::map<uint16_t, vtObjectAuxiliaryFunction2_c*>::const_iterator iter = m_aux2Function.begin(); iter != m_aux2Function.end(); ++iter)
  {
    if (! iter->second->getMatchingPreferredAssignedInputReady())
      continue; // skip preferred assignments for which we didn't detect a input maintenance message with ready state

    objIds.ui16_function = iter->first;

    iter->second->getPreferredAssignedInput(key.isoName, key.ui16_model, objIds.ui16_input);
      
    // only send preferred assignments for inputs that are valid/claimed
    if (key.isoName.isSpecified() && getIsoMonitorInstance( m_vtConnection.getMultitonInst() ).item( key.isoName, true ) )
    {
      map_Inputs[key].push_back(objIds);
    }
  }

  msc_tempSendUpload.vec_uploadBuffer.clear();

  // calculate needed size for vec_uploadBuffer
  uint16_t msgSize = 2; // minimum size for no assignments
  uint8_t noInputUnits = 0;

  for (STL_NAMESPACE::map<Aux2PreferredAssignmentInputDevice_s, STL_NAMESPACE::list<Aux2PreferredAssignmentObjects_s> >::const_iterator iter = map_Inputs.begin(); iter != map_Inputs.end(); ++iter)
  {
    noInputUnits++;
    msgSize += 11;
    msgSize += iter->second.size() * 2 * sizeof(uint16_t);
  }

  msc_tempSendUpload.vec_uploadBuffer.reserve( (msgSize < 8) ? 8 : msgSize );

  msc_tempSendUpload.vec_uploadBuffer.push_back (0x22);
  msc_tempSendUpload.vec_uploadBuffer.push_back (noInputUnits);

  for (STL_NAMESPACE::map<Aux2PreferredAssignmentInputDevice_s, STL_NAMESPACE::list<Aux2PreferredAssignmentObjects_s> >::const_iterator iter = map_Inputs.begin(); iter != map_Inputs.end(); ++iter)
  {
    const uint8_t* pui8_isoName = iter->first.isoName.outputString(); 
    for (uint8_t ui8_i = 0; ui8_i < 8; ui8_i++)
    {
      msc_tempSendUpload.vec_uploadBuffer.push_back(pui8_isoName[ui8_i]);
    }

    msc_tempSendUpload.vec_uploadBuffer.push_back(iter->first.ui16_model & 0xFF);
    msc_tempSendUpload.vec_uploadBuffer.push_back(iter->first.ui16_model >> 8);
    msc_tempSendUpload.vec_uploadBuffer.push_back(iter->second.size());
    
    for (STL_NAMESPACE::list<Aux2PreferredAssignmentObjects_s>::const_iterator iter_list = iter->second.begin(); iter_list != iter->second.end(); ++iter_list)
    {
      msc_tempSendUpload.vec_uploadBuffer.push_back(iter_list->ui16_function & 0xFF);
      msc_tempSendUpload.vec_uploadBuffer.push_back(iter_list->ui16_function >> 8);
      msc_tempSendUpload.vec_uploadBuffer.push_back(iter_list->ui16_input & 0xFF);
      msc_tempSendUpload.vec_uploadBuffer.push_back(iter_list->ui16_input >> 8);
    }
  }

  // pad with 0xFF in case there is only a 8 byte message
  for (uint16_t i = msgSize; i < 8; i++)
    msc_tempSendUpload.vec_uploadBuffer.push_back( 0xFF );
  
  return m_vtConnection.commandHandler().queueOrReplace (msc_tempSendUpload, true /* b_enableReplaceOfCmd */);
#else
  return true;
#endif
}


}
