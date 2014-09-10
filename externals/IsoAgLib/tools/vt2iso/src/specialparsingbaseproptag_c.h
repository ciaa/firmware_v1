/*
  specialparsingbaseproptag_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef SPECIALPARSINGBASEPROPTAG_C_H
#define SPECIALPARSINGBASEPROPTAG_C_H

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

#include "vt2iso-defines.hpp"
#include "vt2iso.hpp"

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

class SpecialParsingBasePropTag_c
{
 public:
  /** check if found tag is invalid at this position */
  virtual bool checkTag(DOMNode* ap_Node, unsigned int aui_objectType, uint64_t a_ui64_ombType) = 0;

  /** that function tries to parse an unknown tag from vt2iso
    * @return returns 0 if successful else anything unequal to 0
    * @param ap_Node pointer to element in xml file
    * @param aui_objectType object type of pNode
    * @param rpcch_objName pointer to object name character string
  */
  virtual bool parseUnknownTag (DOMNode* ap_Node, unsigned int aui_objectType, std::string& arstr_objName, vt2iso_c* pc_vt2iso) = 0;

  /// returns the object type from a given DOMNode if type is known and sets it
  virtual uint16_t getObjType (const char* node_name) { return ui8_objType; }

  /// if a tag has unknown attributes, set that flag
  virtual void setUnknownAttributes (bool b_hasUnknownAttr) {}

  /// simple setter function to set the object type
  void setObjType (uint8_t aui8_objType) { ui8_objType = aui8_objType; }

  /// set the object id for the current handled tag element
  virtual void setObjID (uint16_t aui16_objID) = 0;

  /** that functions creates all necessary files and prints out all collected data */
  virtual void outputCollectedData2Files(const OneAttribute_c[maxAttributeNames]) {}

  /** that functions writes to already existing files and prints out a record during runtime of vt2iso */
  virtual bool outputData2FilesPiecewise(const OneAttribute_c[maxAttributeNames], vt2iso_c* pc_vt2iso) {return true;}

  /// returns if the current parsing module contains proprietary object types which can be handled like basic object types
  virtual bool checkForProprietaryOrBasicObjTypes() = 0;

  virtual void setOmcType (uint64_t& apui64_omcType, const uint64_t* apui64_ombType, uint16_t aui16_objType)=0;

  virtual bool objHasArrayEventMacro (uint16_t objType) const =0;

  virtual bool objHasArrayObjectXY (uint16_t objType) const =0;

  virtual bool objHasArrayObject (uint16_t objType) const =0;

  virtual bool objHasArrayPoints (uint16_t objType) const =0;

  virtual ~SpecialParsingBasePropTag_c() {}

  SpecialParsingBasePropTag_c(): ui8_objType(0xFF) {}

 private:
  uint8_t ui8_objType;
};

#endif
