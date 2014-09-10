/*
  ivtclientobjectpool.h: Class for managing an object pool with
    softkey/button activation event handling

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IVTCLIENTOBJECTPOOL_H
#define IVTCLIENTOBJECTPOOL_H

#include "ivttypes.h"
#include <supplementary_driver/driver/datastreams/streaminput_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/iisoname_c.h>
#include <IsoAgLib/comm/Part3_DataLink/impl/stream_c.h>
#include <utility>

struct localSettings_s;


namespace IsoAgLib
{
  class iVtObjectAuxiliaryFunction2_c;
  class iMultiSendStreamer_c;
/**
  @brief This class is needed to handle Terminal KeyCodes (SoftKey or Button) and Numeric Value Changes and also
  gives you the possibility to perform some action right after successfull objectpool uploading.

  For how to specify an ISO VT Object Pool please refer to \ref XMLspec .
*/

class iVtClientObjectPool_c
{
public:

  /**
    data structure for callback after pool upload failure
  */  
  enum UploadError
  {
    UploadError_NoError,
    UploadError_OutOfMemoryError,
    UploadError_VtVersionError,
    UploadError_InvalidLanguageError,
    UploadError_EoopError
  };

  struct UploadErrorData
  {
    UploadError error;
    uint8_t vtFunctionInstance;

    UploadErrorData(UploadError a_error,
                          uint8_t a_vtFunctionInstance)
      : error(a_error),
        vtFunctionInstance(a_vtFunctionInstance)
    {}

    UploadErrorData()
      : error(UploadError_NoError),
        vtFunctionInstance(0xFF)
    {}
  };
    
  /**
    hook function that gets called after the ISO_Terminal_c instance
    receives a "Soft Key Activation" / "Button Activation" Message
    @param aui8_keyActivationCode 0, 1 or 2. For convenience, the following defines can be used:
             For Latchable Buttons:   [BUTTON_HAS_BEEN_LATCHED, BUTTON_HAS_BEEN_UNLATCHED]
             For Unlatchable Buttons: [BUTTON_HAS_BEEN_PRESSED, BUTTON_HAS_BEEN_RELEASED, BUTTON_IS_STILL_HELD]
             For Softkeys:            [KEY_HAS_BEEN_PRESSED, KEY_HAS_BEEN_RELEASED, KEY_IS_STILL_HELD]
    @param aui16_objId ObjectID of the vtObjectButton / vtObjectSoftKey object
    @param aui16_objIdMask ObjectID of the Mask that contains the vtObjectButton / vtObjectSoftKey object
    @param aui8_keyCode KeyCode as defined in the vtObjectButton / vtObjectSoftKey object
    @param ab_wasButton true if it was a button object, false if it was a soft key
  */
  virtual void eventKeyCode (uint8_t aui8_keyActivationCode, uint16_t aui16_objId, uint16_t aui16_objIdMask, uint8_t aui8_keyCode, bool ab_wasButton)=0;

  /**
    hook function that gets called after the ISO_Terminal_c instance
    receives a "Pointing Event" Message
    <!--@param aui16_xPosition of where the user has clicked (besides buttons and input objects) or touched
    @param aui16_yPosition of where the user has clicked (besides buttons and input objects) or touched-->
  */
  virtual void eventPointingEvent (uint16_t /*aui16_xPosition*/, uint16_t /*aui16_yPosition*/) {}

  /**
    hook function that gets called after the ISO_Terminal_c instance
    receives a "VT Change Numeric Value" Message
    @param aui16_objId of the object where the user changed the value
    @param aui8_value the value in 8bit
    @param aui32_value the (same) value in 32bit
  */
  virtual void eventNumericValue (uint16_t aui16_objId, uint8_t aui8_value, uint32_t aui32_value)=0;

  /**
    hook function that gets called after the ISO_Terminal_c instance
    receives a "Input String Value" Message. The function is called after
    the last TP package is received.
    @param aui16_objId of the object where the user changed the value
    @param aui8_length the length in 8bit
    @param rc_streaminput reference to streaminput
    @param aui8_unparsedBytes
    @param b_isFirst -> OBSOLETE. Always true
    @param b_isLast -> OBSOLETE. Always true
  */
  virtual void eventStringValue (uint16_t aui16_objId, uint8_t aui8_length, StreamInput_c &rc_streaminput, uint8_t aui8_unparsedBytes, bool b_isFirst, bool b_isLast)=0;

  /**
    OBSOLETE : no on-the-fly parsing anymore
  */
  virtual void eventStringValueAbort() {}

  /**
    *  the client received message vt select input object
    *  @param uint16_t aui16_objectId The object ID of the selected object
    *  @param uint8_t aui8_hasFocus byte 4 of the message, 0 means the object lost focus, 1 means the object received focus
    *  @param uint8_t aui8_selected byte 5 of the message, 0 means the object is not open/selected for input, 1  means the object is open/selected for input
    */
  virtual void eventVtSelectInputObject(uint16_t /*aui16_objectId*/, uint8_t /*aui8_hasFocus*/, uint8_t /*aui8_selected*/) { }

  /**
    hook function that gets called immediately after the
    "End Of Object Pool Response"/"Load Version Response
    Message was received for complete initial uploads
    or automatical language update partial uploads.
    @param ab_wasLanguageUpdate true if the object pool was updated while it was already active/being displayed.
                                false if the object pool was initially uploaded
    @param ai8_languageIndex -1 if a non-supported language was selected (and hence the default language (index 0) has been uploaded/updated)
                             0..(n-1) for the index to the supported languages. 0 is the first (=default) language. 1 the second, etc.
    @param aui16_languageCode the language code of the uploaded language (one of your supported languages!)
  */
  virtual void eventObjectPoolUploadedSuccessfully (bool ab_wasLanguageUpdate, int8_t ai8_languageIndex, uint16_t aui16_languageCode)=0;

  /**
    hook function that gets called immediately after the
    "End Of Object Pool Response" Message was received
    for user-application triggered partial pool updates.
  */
  virtual void eventPartialPoolUploadedSuccessfully() {}

  /**
    This function is called right before a language update is being sent to the VT,
    so the application has a chance to e.g. switch to a "Wait while updating language..." datamask
    before the object pool is being updated...
    <!--@param ai8_languageIndex -1 if a non-supported language was selected (and hence the default language (index 0) will be uploaded/updated)
                             0..(n-1) for the index to the supported languages. 0 is the first (=default) language. 1 the second, etc.
    @param aui16_languageCode the language code of the afterwards being uploaded language (one of your supported languages!)-->
  */
  virtual void eventPrepareForLanguageChange (int8_t /*ai8_languageIndex*/, uint16_t /*aui16_languageCode*/) {}

  /**
    hook function that gets called immediately after recognizing
    the loss of the VT (VT Status Msg not coming for >= 3secs.)
  */
  virtual void eventEnterSafeState()=0;

#ifdef CONFIG_VT_CLIENT_ON_MISSING_RESPONSE_CONTINUE
  virtual void eventAllRetriesFailedNowContinuing()=0;
#endif

  /**
    hook function that gets called immediately after recognizing the success/error
    of a command-response message. please keep the implementation short as
    this is directly called from VtClient_c's processMsg();
  */
  virtual void eventCommandResponse(uint8_t /*aui8_responseCommandError*/, const uint8_t /*apui8_responseDataBytes*/[8]) {}

  /**
    hook function that gets called immediately after recognizing an incoming LANGUAGE_PGN.
    react on any change by adjusting your pool to the new UNITS.
    please keep the implementation short as this is directly called from VtClient_c's processMsg();
    for changing the LANGUAGE please refer to "eventObjectPoolUploadedSuccessfully".
    VERY IMPORTANT: THIS FUNCTION CALL MAY OCCUR PRIOR TO AN SUCCESSFULLY UPLOADED POOL !!!!!!!
  */
  virtual void eventLanguagePgn(const localSettings_s& /*ars_localSettings*/) {}

  /**
    hook function that gets called immediately after recognizing an incoming
    VT status message.
  */
  virtual void eventVtStatusMsg() {}

  /**
    hook function that gets called immediately after recognizing an incoming
    VT ESC.
   */
  virtual void eventVtESC(uint16_t /*aui16_ObjectId*/) {}

  /**
    hook function that gets called immediately after recognizing an incoming
    Auxiliary Input Status message - but only if you have a function assigned to it!
   */
  virtual void eventAuxFunctionValue (uint16_t /*mui16_functionUid*/,
                                      uint16_t /*cui16_inputValueAnalog*/,
                                      uint16_t /*cui16_inputValueTransitions*/,
                                      uint8_t  /*cui8_inputValueDigital*/) {}

  /**
    hook function that gets called immediately after recognizing an incoming
    Auxiliary Input Type 2 Status message - but only if you have a function assigned to it!
   */
  virtual void eventAuxFunction2Value (uint16_t /*aui16_functionUid*/,
                                       uint16_t /*aui16_inputValue1*/,
                                       uint16_t /*aui16_inputValue2*/,
                                       uint8_t  /*aui8_operatingState*/) {}

  /**
    gets called after
    - VT changes one or more assigned AUX2 input in the AUX2 functions
    - an AUX2 input maintenance timeout is detected by the client
   */
  virtual void aux2AssignmentChanged ( iVtObjectAuxiliaryFunction2_c& ) {}

  /**
    hook function that gets called immediately after recognizing an incoming
    Vt Status Msg with display state change (Active <-> Hidden)
    Please note that there's no Inactive state until Version 4
    is completely implemented.
   */
  virtual void eventDisplayActivation() {}

  /**
     Gets called after recognizing an incoming VT proprietary message (SP/(E)TP)
   */
  uint8_t eventProprietaryCommand(iIsoName_c const &acr_isoname, const uint8_t* data_8bytes)
  { return doEventProprietaryCommand(acr_isoname, data_8bytes); };
  uint8_t eventProprietaryCommand (iIsoName_c const &acr_isoname, uint8_t aui8_commandByte, __IsoAgLib::Stream_c& arc_stream)
  { return doEventProprietaryCommand(acr_isoname, aui8_commandByte, arc_stream); };

  /**
    Select this VtServer. This function is called when OP is of type RegisterPoolMode_MasterToSpecificVt.
    It is attempted to be used for proprietary matter only.
   */
  virtual bool selectVtServer(iIsoName_c const &) const { return true; }

  /**
    Will not be called before eventObjectPoolUploadedSuccessfully:
    
    state = TRUE: at least two active VTs are detected on the bus                  
    state = FALSE: less than two active VTs are detected on the bus
   */
  virtual void multipleActiveVts(bool /* a_state */) {}

  /**
    Gets called after a pool upload failure
   */
  virtual void UploadError(UploadErrorData /* a_errorData */) {}

  
  enum RegisterPoolMode_en
  {
    RegisterPoolMode_MasterToAnyVt,
    RegisterPoolMode_MasterToPrimaryVt,
    RegisterPoolMode_MasterToSpecificVt,
    RegisterPoolMode_Slave
  };

  enum ObjectPoolVersion_en
  {
    ObjectPoolVersion2 = 2, // use init values because these enum is compared against VtServerInstance_c::getVtIsoVersion() (return uint8_t)
    ObjectPoolVersion3 = 3,
    ObjectPoolVersion4 = 4
  };

  struct ObjectPoolSettings_s
  {
    ObjectPoolSettings_s(ObjectPoolVersion_en a_version, uint16_t a_dimension, uint16_t a_skWidth, uint16_t a_skHeight)
      : version(a_version), dimension(a_dimension), skWidth(a_skWidth), skHeight(a_skHeight) {}

    ObjectPoolVersion_en version;
    uint16_t dimension;
    uint16_t skWidth;
    uint16_t skHeight;
  };


  /**
    this init function has to be idempotent! use "b_initAllObjects" for this reason, it's initialized to false at construction time.
  */
  virtual void initAllObjectsOnce(MULTITON_INST_PARAMETER_DEF)=0;

  iVtClientObjectPool_c(iVtObject_c*HUGE_MEM** a_iVtObjects, uint16_t a_numObjects, uint16_t a_numObjectsLang, ObjectPoolSettings_s a_objectPoolSettings)
    : iVtObjects (a_iVtObjects)
    , numObjects (a_numObjects)
    , numObjectsLang (a_numObjectsLang)
    , version(a_objectPoolSettings.version)
    , dimension (a_objectPoolSettings.dimension)
    , skWidth (a_objectPoolSettings.skWidth)
    , skHeight (a_objectPoolSettings.skHeight)
    , b_initAllObjects (false)
    , numLang( 0 )
  {
    iVtObject_c*HUGE_MEM** iter = a_iVtObjects+1; // skip first entry (should be the general object pool part!)
    while (*iter++ != NULL)
      ++numLang;
  };

   virtual ~iVtClientObjectPool_c() {}

  /** Default implementation for convertColour which can also be used
   *  by custom color conversion implementations.
   */
   static uint8_t convertColourDefault(uint8_t colorValue, uint8_t colorDepth, IsoAgLib::iVtObject_c *obj, IsoAgLib::e_vtColour whichColour);

  /**
    hook function that gets called every time a color-value
    has to be adapted to VT's color-depth (as it violates the color-range!).
    --> Standard implementation will simply clip all color-values to
    BLACK (Color 0) besides the background/transparency colors to WHITE (Color 1)
    Please overload this function if other behaviour is wanted
    <!--@param colorValue The color-value that was originally defined in the object
    @param colorDepth 0 for 1bit-color depth (2-colored VT, black/white)
                      1 for 4bit-color depth (16-colored VT)
                      2 for 8bit-color depth (256-colored VT)
    @param obj Reference to the object that's color's to be converted, use it for distinguishing a little more...-->
    @param whichColour Type of colour: BackgroundColour, LineColour, NeedleColour, etc. (See IsoAgLib::e_vtColour)
  */
  virtual uint8_t convertColour(uint8_t colorValue, uint8_t colorDepth, IsoAgLib::iVtObject_c * obj, IsoAgLib::e_vtColour whichColour)
  {
    return convertColourDefault(colorValue, colorDepth, obj, whichColour);
  }

  /**
    hook function that gets called immediately after recognizing the success of a command-response message
    for Get Attribute Value command
  */
  virtual void eventAttributeValue (IsoAgLib::iVtObject_c* /*obj*/, uint8_t /*ui8_attributeValue*/, uint8_t* /*pui8_value*/) {}

  /** This function enables the application to add proprietary
   * (language independent) IOP-data to the object pool upload
   * CAUTION: Use only if you know what you're doing,
   *          this is not needed for standard VT-Clients!
   * @return Size of that Objectpool (IOP)-part (including the 0x11 byte)
   *         paired with a pointer to your iMultiSendStreamer_c instance
   *         that handles streaming of the proprietary IOP-data.
   */
  virtual STL_NAMESPACE::pair<uint32_t, iMultiSendStreamer_c*> getAppSpecificFixPoolData() { return STL_NAMESPACE::pair<uint32_t,iMultiSendStreamer_c*>(0, (iMultiSendStreamer_c*)NULL); }

  /** This function enables the application to add proprietary
   * (language dependent) IOP-data to the object pool upload
   * CAUTION: Use only if you know what you're doing,
   *          this is not needed for standard VT-Clients!
   * @return Size of that Objectpool (IOP)-part (including the 0x11 byte)
   *         paired with a pointer to your iMultiSendStreamer_c instance
   *         that handles streaming of the proprietary IOP-data.
   */
  virtual STL_NAMESPACE::pair<uint32_t, iMultiSendStreamer_c*> getAppSpecificLangPoolData (int8_t /*ai8_languageIndex*/, uint16_t /*aui16_languageCode*/) { return STL_NAMESPACE::pair<uint32_t,iMultiSendStreamer_c*>(0, (iMultiSendStreamer_c*)NULL); }

private:
  /**
     hook functions that get called after recognizing
     an incoming VT proprietary message (SP/(E)TP)
   */
  virtual uint8_t doEventProprietaryCommand(iIsoName_c const &/*acr_isoname*/, const uint8_t* /*data_8bytes*/) { return 0; }
  virtual uint8_t doEventProprietaryCommand(iIsoName_c const &/*acr_isoname*/, uint8_t /*aui8_commandByte*/, __IsoAgLib::Stream_c& /*arc_stream*/)  { return 0; }

protected:
  iVtObject_c*HUGE_MEM** iVtObjects;
  uint16_t numObjects;
  uint16_t numObjectsLang;
  ObjectPoolVersion_en version;
  uint16_t dimension;
  uint16_t skWidth;
  uint16_t skHeight;
  bool b_initAllObjects;
  uint8_t numLang;

public:
  iVtObject_c*HUGE_MEM**
                        getIVtObjects()     const { return iVtObjects; }
  uint16_t              getNumObjects()     const { return numObjects; }
  uint16_t              getNumObjectsLang() const { return numObjectsLang; }
  ObjectPoolVersion_en  getVersion()        const { return version; }
  // method overrideVersion is only for special proprietary use.
  void overrideVersion(ObjectPoolVersion_en override) { version = override; }
  uint16_t              getDimension()      const { return dimension; }
  uint16_t              getSkWidth()        const { return skWidth; }
  uint16_t              getSkHeight()       const { return skHeight; }
  uint8_t               getNumLang()        const { return numLang; }
  bool                  multiLanguage()     const { return getNumLang() > 0; }

  iVtObjectWorkingSet_c&
  getWorkingSetObject() const { return *(iVtObjectWorkingSet_c*)(**iVtObjects); }
};

} // IsoAgLib

#endif
