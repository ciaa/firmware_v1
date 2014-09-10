/*
  specialparsingbase_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef SPECIALPARSINGBASE_C_H
#define SPECIALPARSINGBASE_C_H

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMDocumentType.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMNodeIterator.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMText.hpp>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLUni.hpp>

#ifndef WIN32
#include <stdint.h>
#else
  typedef unsigned char uint8_t;
  typedef signed char int8_t;
  typedef unsigned short uint16_t;
  typedef short int16_t;
  typedef unsigned int uint32_t;
  typedef int int32_t;
  typedef unsigned long long int uint64_t;
  typedef long int long int64_t;
#endif

#include <string>

XERCES_CPP_NAMESPACE_USE

class SpecialParsingBase_c
{
 public:
  /** that function tries to parse an already known tag from vt2iso which contains
    * unknown attribute tags
    * @return returns 0 if successful else anything unequal to 0
    * @param pNode pointer to element in xml file
    * @param objectType objectType of known xml tag
    * @param objName name of object
    * @param pui_objID pointer to variable of object ID
    * @param pb_isObjID pointer to flag if object is wished
    * @param pcch_SKM pointer to softkeymask if object is a datamask or alarmmask
    * @param pcch_varRef pointer to variable reference if given
  */
  virtual bool parseKnownTag (DOMNode* pNode, const uint8_t objectType,
                              const char* objName, unsigned int* pui_objID,
                              bool* pb_isObjID, const char* pcch_SKM="NULL",
                              const char* pcch_varRef="NULL") = 0;

  /** check if found tag is invalid at this position */
  virtual bool checkTag (DOMNode* pNode, unsigned int objectType, uint64_t ombType) = 0;

  /** that function tries to parse an unknown tag from vt2iso
    * @return returns 0 if successful else anything unequal to 0
    * @param pNode pointer to element in xml file
    * @param objectType object type of pNode
    * @param pb_isObjID pointer to boolean
  */
  virtual bool parseUnknownTag (DOMNode* pNode, unsigned int objectType, bool* pb_isObjID, unsigned int aui_objID) = 0;

  /// returns the object type from a given DOMNode if type is known and sets it
  virtual uint16_t getObjType (const char* node_name) { return 0xFFFF; }

  /// if a tag has unknown attributes, set that flag
  virtual void setUnknownAttributes (bool b_hasUnknownAttr) {}

  /** that functions creates all necessary files and prints out all collected data */
  virtual bool outputCollectedData2Files() { return true; }

  /** that functions writes all necessary file includes into a given file */
  virtual void addFileIncludes(FILE* p_includeFile, const char* fileName) {}

  /** check if the found wish ID is already used as resource ID
    * @return if true, the found ID is NOT used as resource ID
  */
  virtual bool checkUseOfResourceID (unsigned int objID) { return true; }

  /// returns if the current parsing module contains proprietary object types which can be handled like basic object types
  virtual bool checkForProprietaryOrBasicObjTypes (unsigned int objType) = 0;

  virtual const char* getResourceName (uint16_t ui16_resourceID) { return NULL; }

  virtual ~SpecialParsingBase_c() {}

};

#endif
