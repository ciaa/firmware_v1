/*
  vt2iso.hpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef VT2ISO_H
#define VT2ISO_H

#define DEF_MAX_OBJECTS (10000)

#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/dom/DOMBuilder.hpp>
#include <xercesc/util/XMLString.hpp>
#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <list>
#include <bitset>

#include "vt2iso-defines.hpp"
#include "vt2iso-globals.hpp"

// Would like to use wchar_t but C standard does not guarantee that wchar_t has at least
//  16 bits, so lets be most portable definition with unsigned short for the incoming UCS-2
typedef unsigned short UCS2; /* fixed 16 bits */

XERCES_CPP_NAMESPACE_USE

//  Simple error handler deriviative to install on parser
class DOMCountErrorHandler : public DOMErrorHandler
{
public:
    //  Constructors and Destructor
    DOMCountErrorHandler();
    ~DOMCountErrorHandler();

    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    bool getSawErrors() const;

    // -----------------------------------------------------------------------
    //  Implementation of the DOM ErrorHandler interface
    // -----------------------------------------------------------------------
    bool handleError(const DOMError& domError);
    void resetErrors();

private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    DOMCountErrorHandler(const DOMCountErrorHandler&);
    void operator=(const DOMCountErrorHandler&);

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fSawErrors
    //      This is set if we get any errors, and is queryable via a getter
    //      method. Its used by the main code to suppress output if there are
    //      errors.
    // -----------------------------------------------------------------------
    bool    fSawErrors;
};


// ---------------------------------------------------------------------------
//  This is a simple class that lets us do easy (though not terribly efficient)
//  trancoding of XMLCh data to local code page for display.
// ---------------------------------------------------------------------------
class StrX
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    StrX(const XMLCh* const toTranscode)
    {
        // Call the private transcoding method
        fLocalForm = XMLString::transcode(toTranscode);
    }

    ~StrX()
    {
        XMLString::release(&fLocalForm);
    }

    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    const char* localForm() const
    {
        return fLocalForm;
    }

private :
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fLocalForm
    //      This is the local code page form of the string.
    // -----------------------------------------------------------------------
    char*   fLocalForm;
};

inline std::ostream& operator<<(std::ostream& target, const StrX& toDump)
{
    target << toDump.localForm();
    return target;
}

inline bool DOMCountErrorHandler::getSawErrors() const
{
    return fSawErrors;
}


// forward declarations
#ifdef USE_SPECIAL_PARSING_PROP
  class SpecialParsingUsePropTag_c;
#else
  class SpecialParsingBasePropTag_c;
#endif

#ifdef USE_SPECIAL_PARSING
  class SpecialParsingUse_c;
#else
  class SpecialParsingBase_c;
#endif

class OneAttribute_c
{
public:
  void clear();
  void set (const std::string& newString);
  const std::string& get();
  void setIfNotGiven (const std::string& newString);
  void setIfNotGivenOrNull (const std::string& newString);
  bool isGiven();
  int getLength();
  int getIntValue();
  char getCharacterAtPos(int index);
  void setCharacterAtPos(int index, char character);
  void pushBack(char ch);
  bool isNull();
  std::string getObjectReference();
  std::string getObjectReferencePrefixed (const std::string& arstr_prefix);

private:
  std::string attrString;
  bool attrIsGiven;
};


class vt2iso_c : public DOMCountErrorHandler
{
public:
  vt2iso_c();
  ~vt2iso_c();

  struct Path_s
  {
    Path_s() : b_relativePath(false) {}
    std::string str_pathName;
    bool b_relativePath;
  };

  enum action_en
  {
    actionMarkIds,
    actionProcessElement
  };

  bool setCommandElement(unsigned int& commandType, DOMNode  *child,  /*DOMNamedNodeMap *pAttributes,*/ DOMNode *n, unsigned int& objChildCommands, std::string &commandMessage);

  void markIds (DOMNode *n);

  bool processElement (DOMNode *n, uint64_t ombType/* const char* rpcc_inKey, const char* rpcc_inButton, */);

  bool processPointElements(unsigned int& r_objChildPoints, DOMNode *r_n, bool ab_outputEnabled = true);

  bool processMacroElements(unsigned int& r_objMacros, DOMNode *r_n, bool ab_outputEnabled = true);

  bool processChildElements(unsigned int& r_objChildren, DOMNode *r_n, bool xyNeeded, bool ab_outputEnabled = true);

  void clean_exit(const char* error_message = NULL);

  bool getIsOPDimension() const { return is_opDimension; }
  bool getIsOPAdditionallyRequiredObjects() const { return is_opAdditionallyRequiredObjects; }
  bool getIsSKWidth() const { return is_skWidth; }
  bool getIsSKHeight() const { return is_skHeight; }

  void setIsOPDimension (bool b_isOPDimension) { is_opDimension = b_isOPDimension; }
  void setIsOPAdditionallyRequiredObjects (bool b_isOPAdditionallyRequiredObjects) { is_opAdditionallyRequiredObjects = b_isOPAdditionallyRequiredObjects; }
  void setIsSKWidth (bool b_isSKWidth) { is_skWidth = b_isSKWidth; }
  void setIsSKHeight (bool b_isSKHeight) { is_skHeight = b_isSKHeight; }

  unsigned int getOPDimension() const { return opDimension; }
  unsigned int getSKWidth() const { return skWidth; }
  unsigned int getSKHeight() const { return skHeight; }

  const std::string& getOPAdditionallyRequiredObjects() const { return opAdditionallyRequiredObjects; }

  void setOPDimension (unsigned int OPDimension) { opDimension = OPDimension; }
  void setSKWidth (unsigned int SKWidth) { skWidth = SKWidth; }
  void setSKHeight (unsigned int SKHeight) { skHeight = SKHeight; }

  void skRelatedFileOutput();

  void generateIncludeDefines();

  int getAmountXmlFiles() const {return vec_xmlFiles.size();}

  // don't keep this reference too long, xmlFiles may get changed.
  const std::string& getXmlFile (int index) { return vec_xmlFiles[index].str_pathName; }
  bool getXmlFileRelativePath (int index) { return vec_xmlFiles[index].b_relativePath; }

  const char* getAttributeValue (DOMNode* pc_node, const char* attributeName);

  void getKeyCode();

  bool init(
    const std::string& xmlFile,
    std::string* dictionary,
    bool ab_externalize,
    bool ab_disableContainmentRules,
    DOMBuilder* ap_parser,
    bool ab_verbose,
    const std::string& arcstr_outDir,
    const std::string& arcstr_namespace,
    bool ab_acceptUnknownAttributes,
    bool ab_silentMode,
    bool ab_pedanticMode,
    const std::string& str_outFileName,
    const std::string& arcstr_searchPath,
    const std::string& arcstr_langPrefix,
    const std::string& str_definesPrefix,
    const std::string& arcstr_baseClass,
    const std::string& arcstr_baseClassHdr );

  void parse();

  bool prepareFileNameAndDirectory (const std::string& astr_fileName);

  void convertIdReferenceToNameReference (int ai_attrType);

  signed long int idOrName_toi (const char* apc_string, bool ab_isMacro);

  void defaultAndConvertAttributes (unsigned int a_objType);

  void setParseModeWorkingSet (bool ab_parseOnlyWorkingSet) { mb_parseOnlyWorkingSet = ab_parseOnlyWorkingSet; }
  bool isParseModeWorkingSet() { return mb_parseOnlyWorkingSet;}

  bool processVtPresetFile(const std::string& pch_fileName);

private:
  signed int strlenUnescaped (const std::string& pcc_string);

  bool copyWithQuoteAndLength (std::string &dest, const std::string& src, unsigned int len);

  signed long int getID (const char* objName, bool b_isMacro, bool b_wishingID, unsigned int wishID);

  signed long int setID (const char* objName, unsigned int wishID);

  void convertIdReferencesToNameReferences();

  int languageCodeToIndex (char* lc);

  void setAttributeValue (int attrID);
  void cleanAttribute (int attrID);

  bool checkForFileOrFile148 (const char *tag);

  DOMNamedNodeMap* patched_getAttributes (DOMNode *n);

  bool getAttributesFromNode (DOMNode *n, bool treatSpecial);

  bool openDecodePrintOut (const std::list<Path_s>& rcl_bitmapPath, unsigned int &options, int& ref_maxDepth, int fixNr=-1);

  bool checkForAllowedExecution() const;

  std::string getObjectReference (int ai_attributeIndex) { return arrc_attributes [ai_attributeIndex].getObjectReference(); }

  void autoDetectLanguage (DOMNode *n);

  bool processProjectFile(const std::string& pch_fileName);

  unsigned int getFreeId (unsigned int& aui_objNextId);

  bool doAllFiles (action_en aen_action);

  bool isProjectFileMode (const std::string& astr_fileName);

  std::vector<std::string> getSearchPath( const std::string& arcstr_searchPathArg );

  static bool existsFile( const std::string& arcstr_fileWithPath );

  //! copies tmpFileName (the text-file without linewrapping) over to destFileName respecting the given mayLineLen
  void lineWrapTextFile( const std::string &destFileName, const std::string &tmpFileName, unsigned int maxLineLen );

  void diffFileSave( const std::string &destFileName, const std::string &tempFileName );

  std::list<std::string> scanLanguageFilesOS( language_s& a_lang );

public:
  bool isVerbose() { return mb_verbose; }

  static FILE& save_fopen (const std::string& arcstr_fileName, const char* apcc_mode);

private:
  bool firstLineFileE;

  FILE *partFile_variables;
  FILE *partFile_variables_extern;
  FILE *partFile_attributes;
  FILE *partFile_attributes_extern;
  FILE *partFile_functions;
  FILE *partFile_functions_origin;
  FILE *partFile_defines;
  FILE *partFile_list;
  FILE *partFile_listAttributes;
//  FILE *partFile_list_extern;
  FILE *partFile_handler_direct;
  FILE *partFile_handler_derived;
  FILE *partFile_split_function;
  FILE *partFile_obj_selection;

  unsigned int ui_languages;

  std::string partFileName_attributes; // original destination filename for the attributes. stored for writing in the wrapped attributes at the end of program execution.
  std::string partFileName_obj_selection; // original destination filename for the obj-selection. stored for overwriting it at the end of program execution if new content is available.
  std::string mstr_sourceDir;
  std::string mstr_sourceDirAndProjectPrefix;
  std::string mstr_destinDirAndProjectPrefix;
  std::string mstr_outDirName; // overriding outdir as given by cmdline-parameter
  std::string mstr_outFileName; // overriding filename as given by cmdline-parameter
  std::string mstr_className;
  std::string mstr_projectName;
  std::string spc_autoLanguage;

  std::vector<Path_s> vec_xmlFiles;

  // search path
  std::vector<std::string> mvec_searchPath;
  static const char msc_searchPathSeperator = ':';

  static const char* mscp_langDetectionDoneAttributeName;
  
  std::bitset<65536> mbitset_objIdUsed;

  std::map<std::string, uint16_t> map_objNameIdTable;

  unsigned int objNextAutoID;
  unsigned int objNextMacroAutoID;
  unsigned int kcNextAutoID;
  unsigned int objNextUnnamedName;
  unsigned int opDimension;
  unsigned int skWidth;
  unsigned int skHeight;
  unsigned int mi_objectPoolVersion;
  std::string opAdditionallyRequiredObjects;
  bool is_opDimension;
  bool is_opAdditionallyRequiredObjects;
  bool is_skWidth;
  bool is_skHeight;

  bool b_externalize;
  bool mb_parseOnlyWorkingSet;
  bool mb_verbose;
  bool mb_acceptUnknownAttributes;
  bool mb_silentMode;
  bool mb_pedanticMode;


  OneAttribute_c arrc_attributes [maxAttributeNames];
  void clearAndSetElements (DOMNode *child, const std::vector <int> &avec);

  typedef std::map<uint16_t, std::string> ObjListEntry;

  std::map<int32_t, ObjListEntry> map_objNameAndID;
  static bool sb_WSFound;
  uint16_t ui16_WSObjID;

  language_s arrs_language [DEF_iso639entries];

  // Assuming an 8 bit per pixel bitmap.
  unsigned char *picBuffer;
  unsigned int ui_picBufferSize;

  std::string mstr_namespaceDeclarationBegin;       // namespace BLA {
  std::string mstr_namespaceDeclarationEnd;         // }
  std::string mstr_namespacePrefix;                 // BLA::
  std::string mstr_langPrefix;
  std::string mstr_definesPrefix;
  std::string mstr_baseClass;
  std::string mstr_baseClassHdr;

  std::string attr_name;
  std::string attr_value;
  std::string attr_value2;
  std::string filename;

  std::string m_objName;
  bool is_objName;
  unsigned int objID;
  bool is_objID;
  unsigned int objType;

  bool arrb_objTypes [ DEF_iso639entries ];

  bool b_hasUnknownAttributes;
  bool b_hasMoreThan6SoftKeys;
  bool b_disableContainmentRules;
  bool m_errorOccurred;

  // used in processElement(...)
  char* m_nodeName;

  DOMBuilder* parser;
#ifdef USE_SPECIAL_PARSING_PROP
  SpecialParsingUsePropTag_c* pc_specialParsingPropTag;
#else
  SpecialParsingBasePropTag_c* pc_specialParsingPropTag;
#endif

#ifdef USE_SPECIAL_PARSING
  SpecialParsingUse_c* pc_specialParsing;
#else
  SpecialParsingBase_c* pc_specialParsing;
#endif

  std::list<Path_s> l_stdBitmapPath;
  std::list<Path_s> l_fixedBitmapPath;

  std::list<Path_s> l_dictionaryPath;

  std::list<std::string> scanLanguageFiles( language_s& a_lang );
  void readLanguageFilesToBuffer( std::list<std::string> a_languageFiles, language_s& a_lang );
  static bool isEol( const char c );
  static UCS2* findWideChar(const UCS2 *s, UCS2 wc, size_t n);


};

#endif // VT2ISO_H
