/*
  vt2iso.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

// Headers are order dependant  
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <sys/stat.h>

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#if defined WIN32 && !defined cygwin
  #include <windows.h>
  #include <ostream>
  static const char scc_dirSeparatorWrong = '/';
  static const char scc_dirSeparatorCorrect = '\\';
  #include "WinDirent.h"   // Gives me a POSIX API for Windows
#else
  #include <dirent.h>
  static const char scc_dirSeparatorWrong = '\\';
  static const char scc_dirSeparatorCorrect = '/';
#endif

#include <xercesc/parsers/AbstractDOMParser.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMLocator.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/dom/DOMAttr.hpp>
#include <xercesc/dom/DOMElement.hpp>

#include "vt2isoimagefreeimage_c.h"
#include "vt2iso.hpp"
#include "boost/format.hpp"

#ifdef USE_SPECIAL_PARSING_PROP
 #include <specialparsinguseproptag_c.h>
#else
  #include <specialparsingbaseproptag_c.h>
#endif

#ifdef USE_SPECIAL_PARSING
 #include <specialparsinguse_c.h>
#else
  #include <specialparsingbase_c.h>
#endif

#if defined(_MSC_VER)
#pragma warning( disable : 4996 )
#endif

using namespace std;

using boost::format;

extern BGR_s vtColourTable[256];

// special formatting with "%" for "str(format(...."
#define MACRO_16bitToLE(value) (value & 0xFF) % ((value >> 8) & 0xFF)
#define MACRO_32bitToLE(value) (value & 0xFF) % ((value >> 8) & 0xFF) % ((value >> 16) & 0xFF) %  ((value >> 24) & 0xFF)

// ### GLOBALS ###
char iso639table [DEF_iso639entries][2+1] = {{"aa"},{"ab"},{"af"},{"am"},{"ar"},{"as"},{"ay"},{"az"},{"ba"},{"be"},{"bg"},{"bh"},{"bi"},{"bn"},{"bo"},{"br"},{"ca"},{"co"},{"cs"},{"cy"},{"da"},{"de"},{"dz"},{"el"},{"en"},{"eo"},{"es"},{"et"},{"eu"},{"fa"},{"fi"},{"fj"},{"fo"},{"fr"},{"fy"},{"ga"},{"gd"},{"gl"},{"gn"},{"gu"},{"ha"},{"hi"},{"hr"},{"hu"},{"hy"},{"ia"},{"ie"},{"ik"},{"in"},{"is"},{"it"},{"iw"},{"ja"},{"ji"},{"jw"},{"ka"},{"kk"},{"kl"},{"km"},{"kn"},{"ko"},{"ks"},{"ku"},{"ky"},{"la"},{"ln"},{"lo"},{"lt"},{"lv"},{"mg"},{"mi"},{"mk"},{"ml"},{"mn"},{"mo"},{"mr"},{"ms"},{"mt"},{"my"},{"na"},{"ne"},{"nl"},{"no"},{"oc"},{"om"},{"or"},{"pa"},{"pl"},{"ps"},{"pt"},{"qu"},{"rm"},{"rn"},{"ro"},{"ru"},{"rw"},{"sa"},{"sd"},{"sg"},{"sh"},{"si"},{"sk"},{"sl"},{"sm"},{"sn"},{"so"},{"sq"},{"sr"},{"ss"},{"st"},{"su"},{"sv"},{"sw"},{"ta"},{"te"},{"tg"},{"th"},{"ti"},{"tk"},{"tl"},{"tn"},{"to"},{"tr"},{"ts"},{"tt"},{"tw"},{"uk"},{"ur"},{"uz"},{"vi"},{"vo"},{"wo"},{"xh"},{"yo"},{"zh"},{"zu"}};

const char* vt2iso_c::mscp_langDetectionDoneAttributeName = "__vt2iso_lang_detection_done";


//! for right now (legacy-XMLs, but vt-designer still writes this out!!)
//! special sequences in the XML's value= tag are:
//! \n
//! \r
//! length-wise they count as 1 character and are "converted" in the output to \n and \r, which is one "char".
//! other characters need to be escaped for c-string printout like " and \ ...
std::string escapedString (const std::string &instr)
{
  std::string outstr;
  for (unsigned i=0; i<instr.length(); ++i)
  {
    switch (instr[i])
    {
    case '\\':
      if( (i+1) < instr.length() )
      { 
        switch( instr[i+1] )
        {
        case 'n': // support for \n and \r is subject to change. currently:
        case 'r': // let \n and \r simply pass, vt-designer correctly calculated the length already, too!
          outstr += instr[i]; // copy \ ...
          break;
        default:
          outstr += "\\\\";
          break;
        }
      }
      else
      { // single \ at the end of string
        outstr += "\\\\";
      }
      break;
    case '"':  outstr += "\\\""; break;
    case '\n': outstr += "\\n"; break;
    case '\r': outstr += "\\r"; break;
    default: outstr += instr[i]; break;
    }
  }
  return outstr;
}

#define X(str) XStr(str).unicodeForm()
// ---------------------------------------------------------------------------
//  This is a simple class that lets us do easy (though not terribly efficient)
//  trancoding of char* data to XMLCh data.
// ---------------------------------------------------------------------------
class XStr
{
  public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    XStr(const char* const toTranscode)
    {
      // Call the private transcoding method
      fUnicodeForm = XMLString::transcode(toTranscode);
    }

    ~XStr()
    {
      XMLString::release(&fUnicodeForm);
    }

    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    const XMLCh* unicodeForm() const
    {
      return fUnicodeForm;
    }

  private :
    // -----------------------------------------------------------------------
    //  Private data members
    //  fUnicodeForm
    //   This is the Unicode XMLCh format of the string.
    // -----------------------------------------------------------------------
    XMLCh*   fUnicodeForm;
};

void utf16convert (const XMLCh* source, std::string &destin)
{
  utf16convert((const char*)source, destin);
}

int StringToInt(std::string stringValue)
{
    std::stringstream ssStream(stringValue);
    int iReturn=0;
    ssStream >> iReturn;

    return iReturn;
}

std::string IntToString(int iValue)
{
    std::stringstream ssStream;
    ssStream << iValue;
    return ssStream.str();
}

DOMCountErrorHandler::DOMCountErrorHandler()
  : fSawErrors(false) {}

DOMCountErrorHandler::~DOMCountErrorHandler() {}

// ---------------------------------------------------------------------------
//  DOMCountHandlers: Overrides of the DOM ErrorHandler interface
// ---------------------------------------------------------------------------
bool DOMCountErrorHandler::handleError(const DOMError& domError)
{
  fSawErrors = true;
  if (domError.getSeverity() == DOMError::DOM_SEVERITY_WARNING)
    std::cerr << "\nWarning at file ";
  else if (domError.getSeverity() == DOMError::DOM_SEVERITY_ERROR)
    std::cerr << "\nError at file ";
  else
    std::cerr << "\nFatal Error at file ";

  std::cerr << StrX(domError.getLocation()->getURI())
      << ", line " << domError.getLocation()->getLineNumber()
      << ", char " << domError.getLocation()->getColumnNumber()
      << "\n  Message: " << StrX(domError.getMessage()) << std::endl;

  return true;
}

void DOMCountErrorHandler::resetErrors()
{
  fSawErrors = false;
}

// ---------------------------------------------------------------------------
//  GLOBAL Bitmap instance
// ---------------------------------------------------------------------------
Vt2IsoImageFreeImage_c c_Bitmap;



// ---------------------------------------------------------------------------
//  void usage () --- Prints out usage text.
// ---------------------------------------------------------------------------
static void usage()
{
  std::cout << "\nvt2iso BUILD DATE: " << __DATE__ <<std::endl<<std::endl<<
    "Usage:\n"
    " vt2iso [options] <VTP file> [options]"<<std::endl<<std::endl<<
    "This program converts the input-files into ISOAgLib VT-Client Code-files."<<std::endl<<
    "Input files are processed in the order given in the VTP project-file."<<std::endl<<std::endl<<
    "Features:\n"
    " - auto_language=\"..\" attribute\n"
    " - support for auto-detecting language=\"..\" attribute if the value is given in the .values.xx.txt files."<<std::endl<<std::endl<<
    "Options:\n"
    "[Basic:]\n"
    " -o=dir Use the given Outputdirectory for the generated files instead of the directory where the XML/VTP-files are located.\n"
    " -a=pre Use the given Prefix instead of the Projectname as a prefix for the generated files.\n"
    " -a     (Not specifying a value for -a lets vt2iso use the name of the XML/VTP - think of it as Legacy-Mode!)\n"
    " -g=ns  Group the generated structures into the given namespace. if only -g is passed, the Project-Name will be used.\n"
    "[Extended:]\n"
    " -p     Output ISO11783-VT Palette to act-file.\n"
    " -e     Externalize. If you need to use the split-up version of the generated files, use this option.\n"
    " -m     More informative output (verbose mode). Will also print out warnings on overridden values with language-files.\n"
    " -r=pre Do not use iVtObjectID as id prefix, use pre instead.\n"
    " -u     User defined attributes accepted\n"
    " -l     Running in silent mode\n"
    " -c=xxx Specify a VT preset which contains proprietary colours (important for images with poprietary colours)\n"
    " -x=pre Use the given Prefix for Multilanguage objects. Will generate iVtObjectX_pre1 instead of iVtObjectX_1.\n"
    " -k     Pedantic mode during resolving values from translation files for inputstring, stringvariable and outputstring.\n"
    " -b=cl  Derive the object pool from the given base class. Defaults to IsoAgLib::iVtClientObjectPool_c.\n"
    " -bh=h  Specify the header file to include for the base class which was specified with the -b option. Defaults to none.\n"
    "[XML-Parser:]\n"
    " -v=xxx Validation scheme [always | never | auto]. Defaults to auto\n"
    " -n     Enable namespace processing. Defaults to off.\n"
    " -s     Enable schema processing. Defaults to off.\n"
    " -f     Enable full schema constraint checking. Defaults to off.\n"
    " -locale=ll_CC  specify the locale. Defaults to en_US.\n"
#if 0
    /*  This feature is not shown up in the help here in order to avoid confusion about. Furthermore it is not
     *  yet extensively testd FOB 06/19/2009
     */
    " -d     Specify a search path for xmls marked as relative in vtp project file. Seperate directories with colons.\n"
#endif 
    << std::endl;

#ifdef USE_SPECIAL_PARSING
    std::cout <<
    " -dict=xxx Specify the relative path to the resource dictionary"<<std::endl;
#endif
  std::cout << " -?    Show this help."<<std::endl<<std::endl;
}

template <class T> std::vector<T> make_vector ( const T& param1 )
{
  std::vector<T> vec;
  vec.push_back (param1);
  return vec;
}

template <class T> std::vector<T> make_vector ( const T& param1, const T& param2 )
{
  std::vector<T> vec;
  vec.push_back (param1);
  vec.push_back (param2);

  return vec;
}

template <class T> std::vector<T> make_vector ( const T& param1, const T& param2, const T& param3 )
{
  std::vector<T> vec;
  vec.push_back (param1);
  vec.push_back (param2);
  vec.push_back (param3);

  return vec;
}

template <class T> std::vector<T> make_vector ( const T& param1, const T& param2, const T& param3, const T& param4 )
{
  std::vector<T> vec;
  vec.push_back (param1);
  vec.push_back (param2);
  vec.push_back (param3);
  vec.push_back (param4);

  return vec;
}

template <class T> std::vector<T> make_vector ( const T& param1, const T& param2, const T& param3, const T& param4, const T& param5 )
{
  std::vector<T> vec;
  vec.push_back (param1);
  vec.push_back (param2);
  vec.push_back (param3);
  vec.push_back (param4);
  vec.push_back (param5);

  return vec;
}

template <class T> std::vector<T> make_vector ( const T& param1, const T& param2, const T& param3, const T& param4, const T& param5, const T& param6 )
{
  std::vector<T> vec;
  vec.push_back (param1);
  vec.push_back (param2);
  vec.push_back (param3);
  vec.push_back (param4);
  vec.push_back (param5);
  vec.push_back (param6);

  return vec;
}

template <class T> void processNestedNodesInProjectFile(DOMNode *child, T& r_container, const std::string& rstr_parentNode, const std::string& rstr_childNode, const std::string& rstr_pathAttribute)
{
  std::string local_attrName;
  std::string local_attrValue;

  if (strcmp (XMLString::transcode(child->getNodeName()), rstr_parentNode.c_str()) == 0)
  {
    DOMNode *innerChild;
    for (innerChild = child->getFirstChild(); innerChild != 0; innerChild=innerChild->getNextSibling())
    {
      if ((strcmp (XMLString::transcode(innerChild->getNodeName()), rstr_childNode.c_str()) == 0) && innerChild->hasAttributes())
      {
        vt2iso_c::Path_s s_filePath;

        DOMNamedNodeMap *pAttributes = innerChild->getAttributes();
        int nSize = pAttributes->getLength();
        for (int i=0;i<nSize;++i)
        {
          DOMAttr *pAttributeNode = (DOMAttr*) pAttributes->item(i);
          utf16convert (pAttributeNode->getName(), local_attrName);
          utf16convert (pAttributeNode->getValue(), local_attrValue);
          if (local_attrName == rstr_pathAttribute)
          {
            s_filePath.str_pathName = local_attrValue;
            // convert directory separators
            (void) std::replace (s_filePath.str_pathName.begin(),
                                 s_filePath.str_pathName.end(),
                                 scc_dirSeparatorWrong,
                                 scc_dirSeparatorCorrect);
          }
          if (local_attrName.compare("RelativePath") == 0)
          {
            if (local_attrValue.compare("1") == 0)
              s_filePath.b_relativePath = true;
          }
        }
        if (!s_filePath.str_pathName.empty())
          r_container.push_back(s_filePath);
      }
    }
  }
}

void OneAttribute_c::clear()
{
  attrString.clear();
  attrIsGiven = false;
}

void OneAttribute_c::set (const std::string& newString)
{
  attrIsGiven = true;
  attrString = newString;
}


const std::string& OneAttribute_c::get()
{
  return attrString;
}

void OneAttribute_c::setIfNotGiven (const std::string& newString)
{
  if (!isGiven())
  {
    set(newString);
  }
}

void OneAttribute_c::setIfNotGivenOrNull (const std::string& newString)
{
  if (!isGiven() || isNull())
  {
    set(newString);
  }
}

bool OneAttribute_c::isGiven()
{
  return attrIsGiven;
}

int OneAttribute_c::getLength()
{
  return attrString.length();
}

int OneAttribute_c::getIntValue()
{
  return StringToInt(attrString);
}

char OneAttribute_c::getCharacterAtPos(int index)
{
  return attrString[index];
}

void OneAttribute_c::setCharacterAtPos(int index, char character)
{
  attrString[index] = character;
}

void OneAttribute_c::pushBack(char ch)
{
  attrString.push_back(ch);
  attrIsGiven = true;
}

//! It's NULL either if
/// - attribute=""
/// - attribute="65535"
/// - attribute="NULL"
/// - attribute not given
bool OneAttribute_c::isNull()
{
  if (!isGiven())
    return true;

  if ( (getIntValue() == 65535) || (get().compare("NULL") == 0) || (getLength() == 0) )
    return true;

  return false;
}


std::string OneAttribute_c::getObjectReference()
{
  if (isNull())
    return "NULL";
  else
    return std::string ("&iVtObject") + get();
}


FILE& vt2iso_c::save_fopen (const std::string& arcstr_fileName, const char* apcc_mode)
{
  FILE* handle = fopen (arcstr_fileName.c_str(), apcc_mode);
  if (handle) return *handle;

  std::cerr << "vt2iso: Couldn't open '" << arcstr_fileName << "' (with mode '"<<apcc_mode<<"'). Terminating." << std::endl;
  exit (-1);
}


bool vt2iso_c::sb_WSFound = false;

void vt2iso_c::clean_exit (const char* error_message)
{
  std::string partFileName;

  // close all streams to files at the end of this function because someone may want
  // to write into these files in special parsing

  if (partFile_functions)
  {
    int extraLanguageLists = (ui_languages>0)?arrs_language[0].count : 0;

    fprintf (partFile_functions, "void iObjectPool_%s_c::initAllObjectsOnce (MULTITON_INST_PARAMETER_DEF)\n{\n", mstr_className.c_str());
    fprintf (partFile_functions, "  if (b_initAllObjects) return;   // so the pointer to the ROM structures are only getting set once on initialization!\n");
    fprintf (partFile_functions, "  int i_objCount = %d;\n", map_objNameIdTable.size() - extraLanguageLists);
    fprintf (partFile_functions, "  int i_listIndex = 0;\n");
    fprintf (partFile_functions, "  IsoAgLib::iVtObject_c::iVtObject_s* HUGE_MEM * pps_sROMs = %sall_sROMs;\n", mstr_namespacePrefix.c_str());
    fprintf (partFile_functions, "  do \n");
    fprintf (partFile_functions, "  {\n");
    fprintf (partFile_functions, "    IsoAgLib::iVtObject_c* HUGE_MEM * pc_objList = %sall_iVtObjectLists [i_listIndex];\n", mstr_namespacePrefix.c_str());
    fprintf (partFile_functions, "    while (i_objCount)\n");
    fprintf (partFile_functions, "    {\n");
    fprintf (partFile_functions, "      ((__IsoAgLib::vtObject_c*)(*pc_objList))->init (*pps_sROMs MULTITON_INST_PARAMETER_USE_WITH_COMMA);\n");
    fprintf (partFile_functions, "      ++pc_objList;\n");
    fprintf (partFile_functions, "      ++pps_sROMs;\n");
    fprintf (partFile_functions, "      --i_objCount;\n");
    fprintf (partFile_functions, "    }\n");
    fprintf (partFile_functions, "    i_objCount = %d;\n", extraLanguageLists);
    fprintf (partFile_functions, "    ++i_listIndex;\n");
    fprintf (partFile_functions, "  } while (%sall_iVtObjectLists[i_listIndex]);\n", mstr_namespacePrefix.c_str());

    fprintf (partFile_functions, "\n  #include \"%s-functions-origin.inc\"\n", mstr_outFileName.c_str());
    fprintf (partFile_functions, "\n  b_initAllObjects = true;");
    fprintf (partFile_functions, "\n}\n");
  }

  if (partFile_functions_origin)
  {
    fprintf (partFile_functions_origin, "\n  for (int i=0;i<numObjects; i++) {");
    fprintf (partFile_functions_origin, "\n    iVtObjects [0][i]->setOriginSKM (false);");
    fprintf (partFile_functions_origin, "\n    iVtObjects [0][i]->setOriginBTN (NULL);");
    fprintf (partFile_functions_origin, "\n  }\n");
  }

  if (partFile_defines)
  {
    fprintf (partFile_defines, "\nstatic const unsigned int vtKeyCodeACK = 0;\n");
    // OLD:  fprintf (partFile_defines, "\n#define vtObjectCount %d\n", objCount);
  }

  std::map<int32_t, ObjListEntry>::iterator mit_lang;
  std::map<uint16_t, std::string>::iterator mit_obj;

  FILE* fileList;
  bool *pb_firstLine;

  for (mit_lang = map_objNameAndID.begin(); mit_lang != map_objNameAndID.end(); mit_lang++)
  {
    if (mit_lang->first < 0x0) // negative index was defined for default objects
    {
      fileList = partFile_list;
      pb_firstLine = &firstLineFileE;

      // get workingset object and set as first item in list -> needed for pool upload/initialization
      if (sb_WSFound) // should always be the case!
      {
        mit_obj = mit_lang->second.find (ui16_WSObjID);
        *pb_firstLine = false;

        if (fileList) // first, print out the workingset object
          fprintf (fileList, "\n %s", mit_obj->second.c_str());
          fprintf (partFile_listAttributes, "\n (IsoAgLib::iVtObject_c::iVtObject_s*)%s_sROM", mit_obj->second.c_str());

        // delete from the map
        mit_lang->second.erase (mit_obj);
      }
    }
    else
    {
      fileList = arrs_language[mit_lang->first].partFile;
      pb_firstLine = &arrs_language[mit_lang->first].firstLine;
    }

    for (mit_obj = mit_lang->second.begin(); mit_obj != mit_lang->second.end(); mit_obj++)
    {
      if (*pb_firstLine)
      {
        *pb_firstLine = false;
        fprintf (partFile_listAttributes, ",");
      }
      else
      {
        fprintf (fileList, ",");
        fprintf (partFile_listAttributes, ",");
      }
      fprintf (fileList, "\n %s", mit_obj->second.c_str());
      fprintf (partFile_listAttributes, "\n (IsoAgLib::iVtObject_c::iVtObject_s*)%s_sROM", mit_obj->second.c_str());
    }
  }

  if (partFile_list)
  { // -list
    if (firstLineFileE)
    {
      firstLineFileE = false;
    }
    fputs ("\n};\n", partFile_list);
    // write implementation of handler class constructor into list
    // as there the list must be known
    // -> the handler decleration can be included from everywhere
    fprintf (partFile_list, "\nIsoAgLib::iVtObject_c* HUGE_MEM * all_iVtObjectLists [] = {" );
    fprintf (partFile_list, "\n  all_iVtObjects," );
    for (unsigned int i=0; i<ui_languages; i++)
      fprintf (partFile_list, "\n  all_iVtObjects%d,", i);
    fprintf (partFile_list, "\n  NULL // indicate end of list");
    fprintf (partFile_list, "\n};\n");
    fprintf ( partFile_list, "%s", mstr_namespaceDeclarationEnd.c_str());

    fclose(partFile_list);
  }

  if (partFile_listAttributes)
  { // -list_attributes.inc
    fputs ("\n};\n", partFile_listAttributes); 
    fprintf ( partFile_listAttributes, "%s", mstr_namespaceDeclarationEnd.c_str());
    fclose(partFile_listAttributes);
  }

//   if (partFile_list_extern);
//     fclose(partFile_list_extern);

  for (unsigned int i=0; i<ui_languages; i++)
  {
    if (arrs_language [i].firstLine)
    { // put in at least one entry so that compiler doesn't error out (gcc doesn't care, but others do!)
      fputs ("\n  NULL", arrs_language [i].partFile);
    }
    fputs ("\n};\n", arrs_language [i].partFile);
    // write implementation of handler class constructor into list
    // as there the list must be known
    // -> the handler decleration can be included from everywhere
    fputs (mstr_namespaceDeclarationEnd.c_str(), arrs_language [i].partFile);
    fclose (arrs_language [i].partFile);
  }


  if (partFile_handler_derived)
  { // handler class derived
  // NEW:
    fprintf (partFile_handler_derived, "#ifndef DECL_derived_iObjectPool_%s_c", mstr_className.c_str() );
    fprintf (partFile_handler_derived, "\n#define DECL_derived_iObjectPool_%s_c", mstr_className.c_str() );
    if( !mstr_baseClassHdr.empty() )
      fprintf (partFile_handler_derived, "\n\n#include \"%s\"\n", mstr_baseClassHdr.c_str() );
    fprintf (partFile_handler_derived, "\n%s",mstr_namespaceDeclarationBegin.c_str());
    fprintf (partFile_handler_derived, "\n// forward declaration");
    fprintf (partFile_handler_derived, "\nextern IsoAgLib::iVtObject_c* HUGE_MEM * all_iVtObjectLists [];");
    fprintf (partFile_handler_derived, "\n%s",mstr_namespaceDeclarationEnd.c_str());
    fprintf (partFile_handler_derived, "\nclass iObjectPool_%s_c : public %s {", mstr_className.c_str(), mstr_baseClass.c_str());
    fprintf (partFile_handler_derived, "\npublic:");
    fprintf (partFile_handler_derived, "\n  void initAllObjectsOnce(MULTITON_INST_PARAMETER_DEF);");
    int extraLanguageLists = (ui_languages>0)?arrs_language[0].count : 0;
    fprintf (partFile_handler_derived, "\n  iObjectPool_%s_c() : %s (%sall_iVtObjectLists, %d, %d,  ObjectPoolSettings_s(iVtClientObjectPool_c::ObjectPoolVersion%d, %d, %d, %d) ) {}\n",
             mstr_className.c_str(), mstr_baseClass.c_str(), mstr_namespacePrefix.c_str(), map_objNameIdTable.size() - extraLanguageLists, extraLanguageLists, mi_objectPoolVersion, opDimension, skWidth, skHeight);
    fprintf (partFile_handler_derived, "\n};\n");
    fprintf (partFile_handler_derived, "\n#endif\n" );
    fclose (partFile_handler_derived);
  }

  std::string extension;
  if (b_externalize)
    extension = "-extern";

  // Write Derived Includes (-cpp)
  FILE* partFile_derived = NULL;
  partFileName = mstr_destinDirAndProjectPrefix + "_derived-cpp.h";
  partFile_derived = &save_fopen (partFileName.c_str(),"wt");

  fprintf (partFile_derived, "#include \"%s-defines.inc\"\n", mstr_outFileName.c_str());
  fprintf (partFile_derived, "#include \"%s-variables%s.inc\"\n", mstr_outFileName.c_str(), extension.c_str());
  fprintf (partFile_derived, "#include \"%s-attributes%s.inc\"\n", mstr_outFileName.c_str(), extension.c_str());
  if (b_externalize)
  {
    fputs(mstr_namespaceDeclarationBegin.c_str(), partFile_derived);
    fprintf (partFile_derived, "extern IsoAgLib::iVtObject_c::iVtObject_s* HUGE_MEM all_sROMs [];\n");
    fprintf (partFile_derived, "extern IsoAgLib::iVtObject_c* HUGE_MEM * all_iVtObjectLists [];\n");
    fprintf (partFile_derived, "extern IsoAgLib::iVtObject_c* HUGE_MEM all_iVtObjects [];\n");
    for (unsigned int i=0; i<ui_languages; i++)
    {
      fprintf (partFile_derived, "extern IsoAgLib::iVtObject_c* HUGE_MEM all_iVtObjects%i [];\n", i);
    }
    fputs(mstr_namespaceDeclarationEnd.c_str(), partFile_derived);
  }
  else
  {
    for (unsigned int i=0; i<ui_languages; i++)
    {
      fprintf (partFile_derived, "#include \"%s-list%02d.inc\"\n", mstr_outFileName.c_str(), i);
    }
    fprintf (partFile_derived, "#include \"%s-list.inc\"\n", mstr_outFileName.c_str());
    fprintf (partFile_derived, "#include \"%s-list_attributes.inc\"\n", mstr_outFileName.c_str());
  }
  fprintf (partFile_derived, "#include \"%s-functions.inc\"\n", mstr_outFileName.c_str());

  if (pc_specialParsing)
  {
    pc_specialParsing->addFileIncludes(partFile_derived, mstr_outFileName.c_str());
  }
  fclose (partFile_derived);


  // Write Derived Includes (-h)
  partFileName = mstr_destinDirAndProjectPrefix + "_derived-h.h";
  partFile_derived = &save_fopen (partFileName.c_str(),"wt");

  fprintf (partFile_derived, "#include <IsoAgLib/comm/Part6_VirtualTerminal_Client/ivtincludes.h>\n");
  fprintf (partFile_derived, "#include \"%s-handler-derived.inc\"\n", mstr_outFileName.c_str());

  if (b_externalize)
  {
    FILE* partFileTmp;
    partFileName = mstr_destinDirAndProjectPrefix + "-variables.cpp";
    partFileTmp = &save_fopen (partFileName.c_str(), "wt");
    fprintf (partFileTmp, "#include <IsoAgLib/comm/Part6_VirtualTerminal_Client/ivtincludes.h>\n");
    fprintf (partFileTmp, "#include \"%s-variables.inc\"\n", mstr_outFileName.c_str());
    fclose (partFileTmp);

    partFileName = mstr_destinDirAndProjectPrefix + "-attributes.cpp";
    partFileTmp = &save_fopen (partFileName.c_str(), "wt");
    fprintf (partFileTmp, "#include <IsoAgLib/comm/Part6_VirtualTerminal_Client/ivtincludes.h>\n");
    fprintf (partFileTmp, "#include \"%s-variables-extern.inc\"\n", mstr_outFileName.c_str());
    fprintf (partFileTmp, "#include \"%s-attributes-extern.inc\"\n", mstr_outFileName.c_str());
    fprintf (partFileTmp, "#include \"%s-attributes.inc\"\n", mstr_outFileName.c_str());
    fclose (partFileTmp);

    partFileName = mstr_destinDirAndProjectPrefix + "-list.cpp";
    partFileTmp = &save_fopen (partFileName.c_str(), "wt");
    fprintf (partFileTmp, "#include <IsoAgLib/comm/Part6_VirtualTerminal_Client/ivtincludes.h>\n");
    fprintf (partFileTmp, "#include \"%s-attributes-extern.inc\"\n", mstr_outFileName.c_str());
    fprintf (partFileTmp, "#include \"%s-variables-extern.inc\"\n", mstr_outFileName.c_str());
    for (unsigned int i=0; i<ui_languages; i++)
    {
      fprintf (partFileTmp, "#include \"%s-list%02d.inc\"\n", mstr_outFileName.c_str(), i);
    }
    fprintf (partFileTmp, "#include \"%s-list.inc\"\n", mstr_outFileName.c_str());
    fclose (partFileTmp);

    partFileName = mstr_destinDirAndProjectPrefix + "-list_attributes.cpp";
    partFileTmp = &save_fopen (partFileName.c_str(), "wt");
    fprintf (partFileTmp, "#include <IsoAgLib/comm/Part6_VirtualTerminal_Client/ivtincludes.h>\n");
    fprintf (partFileTmp, "#include \"%s-attributes-extern.inc\"\n", mstr_outFileName.c_str());
    fprintf (partFileTmp, "#include \"%s-list_attributes.inc\"\n", mstr_outFileName.c_str());
    fclose (partFileTmp);
  }

  /// if USE_SPECIAL_PARSING is defined additional output is done
  if (pc_specialParsing)
    pc_specialParsing->outputCollectedData2Files();

  fprintf ( partFile_variables, "%s", mstr_namespaceDeclarationEnd.c_str());
  fprintf ( partFile_variables_extern, "%s", mstr_namespaceDeclarationEnd.c_str());
  fprintf ( partFile_attributes, "%s", mstr_namespaceDeclarationEnd.c_str());
  fprintf ( partFile_attributes_extern, "%s", mstr_namespaceDeclarationEnd.c_str());
  fprintf ( partFile_defines, "%s", mstr_namespaceDeclarationEnd.c_str());

  if (partFile_derived)           fclose (partFile_derived);
  if (partFile_variables_extern)  fclose (partFile_variables_extern);
  if (partFile_attributes)        fclose (partFile_attributes);
  if (partFile_attributes_extern) fclose (partFile_attributes_extern);
  if (partFile_defines)           fclose (partFile_defines);
  if (partFile_obj_selection)     fclose (partFile_obj_selection);

  if (partFile_functions)         fclose (partFile_functions);
  if (partFile_functions_origin)  fclose (partFile_functions_origin);

  if (partFile_attributes)
  { // if any "-attributes.inc.tmp" were written, copy them over wrapped to "-attributes.inc"
    partFileName = partFileName_attributes + ".tmp";
    lineWrapTextFile( partFileName_attributes, partFileName, 1022 );
  }

  if (partFile_obj_selection)
  { // if any "-objectselection.inc.tmp" were written, copy them over to "-objectselection.inc", but only if the content differs
    // this is to avoid an unneeded rebuild of IsoAgLib, as it includes the generated object-selection file.
    partFileName = partFileName_obj_selection + ".tmp";
    diffFileSave( partFileName_obj_selection, partFileName );
  }
  if (error_message != NULL)
    std::cout << error_message;

  if (b_hasMoreThan6SoftKeys)
    if (!mb_silentMode)
      std::cout
        << "*** WARNING *** WARNING *** WARNING *** WARNING *** WARNING *** WARNING *** WARNING *** WARNING" << std::endl << std::endl
        << "Your objectpool uses softkeymasks with MORE THAN 6 softkeys. Be aware that this pool may fail to upload on some VTs!!" << std::endl << std::endl
        << "*** WARNING *** WARNING *** WARNING *** WARNING *** WARNING *** WARNING *** WARNING *** WARNING" << std::endl << std::endl;
}

signed int vt2iso_c::strlenUnescaped (const std::string& pcc_string)
{
  int i_unescapedLength=0;
  char c_current;
  bool b_lastWasEscapeChar=false;
  const int c_orig_len = pcc_string.length();
  int orig_len = c_orig_len;
  int src_index=0;
  while (orig_len-- > 0)
  {
    c_current=pcc_string[src_index++];

    if ((c_current == '\\') && (!b_lastWasEscapeChar))
    { // this is the escape character
      b_lastWasEscapeChar=true;
    }
    else
    { // count as normal character
      i_unescapedLength++;
      if (b_lastWasEscapeChar &&(c_current == 'x'))
      { // That was an escaped number, with 2 digits
        src_index = src_index+2; // skip up to next
        orig_len = orig_len-2;
      }
      b_lastWasEscapeChar=false;
    }
  }

  if (b_lastWasEscapeChar)
    return -1;
  else
  {
    if( (c_orig_len >= 1) && ((0xff==pcc_string[0]) && (0xfe==pcc_string[1])) ) // BOM check, UniCode encoding, LE
    {
      if (i_unescapedLength & 0x01)   // thats odd, it should always be a pair for UCS-2
      {
        printf ("Odd countOf chars: %d in UnicodeString!\n", i_unescapedLength);
        return -1;
      }
    }

    return i_unescapedLength;
  }
}

bool vt2iso_c::copyWithQuoteAndLength (std::string &dest, const std::string& src, unsigned int len)
{
  dest = std::string ("const_cast<char*>(\"");

  signed int ret = strlenUnescaped(src);
  if (ret == -1)
  {
    printf ("Error in copyWithQuoteAndLength()! STOP PARSER! bye.\n\n");
    return false;
  }

  unsigned int i=0;
  int srcIndex = 0;
  bool b_lastWasEscapeChar=false;

  const bool unicode = ( ( ret >= 1 ) && ((0xff==src[0]) && (0xfe==src[1])) ); // BOM check, UniCode encoding, LE
  unsigned int take = ( (unsigned int)ret <= len) ? ret : len;
  for (; i<take; ++i)
  { // copy probably escaped string
    if ((src[srcIndex] == 'x') && (b_lastWasEscapeChar))
    {
      // \xAB (four input chars) -> one 8bit-char
      i -= 2; // one substracted below already on \ detection!
    }
    else if ((src[srcIndex] == '\\') && (!b_lastWasEscapeChar))
    {
      b_lastWasEscapeChar=true;
      // \A (two input chars) -> one 8bit-char
      --i;
    }
    else
    {
      b_lastWasEscapeChar=false;
    }
    dest.push_back(src[srcIndex++]);
  }
  
  // fill with spaces if necessary
  if( unicode )
  {
    for (; i<len; i+=2)
    {
      dest.push_back(' ');
      dest.push_back(0x00); // LE UCS-2 Space
    }
  }
  else
  {
    for (; i<len; i++)
      dest.push_back(' ');
  }
  dest.push_back ('\"');
  dest.push_back (')');
  return true;
}
// ---------------------------------------------------------------------------
//  signed long int getID (const char* objName, bool isMacro, bool widhingID, unsigned int wishID=0)
// ---------------------------------------------------------------------------
signed long int vt2iso_c::getID (const char* objName, bool b_isMacro, bool b_wishingID, unsigned int wishID)
{
  bool isThere = false;
  unsigned int foundID = 0;

  // Added the following check. This is necessary so that objects like input lists which can contain lists of the NULL object ID (65535)
  // do not assign object ids for this, and as a result count an additional object unnecessarily by incrementing objCount.
  if (wishID == 65535)
  {
    return wishID;
  }

  std::string str_objName = std::string (objName);
  std::map<std::string, uint16_t>::iterator iter = map_objNameIdTable.find (str_objName);
  // first check if ID is there already
  if (iter != map_objNameIdTable.end())
  {
    foundID = iter->second;
    isThere = true;
  }

  if (isThere)
  { // name already there
    if (b_wishingID)
    { // check if have conflicting IDs...
      if (wishID != foundID)
      {
        std::cerr << "REDEFINED OBJECT ID '" << foundID << "' with '" << wishID << "' for Object with name '" << objName << "'!!! STOPPING PARSER! bye."<<std::endl<<std::endl;
        return -1;
      }
    }
  }
  else
  { // !isThere
    // check what's the new ID to be
    if (b_wishingID)
    {
      // first check if ID is there already
      for (iter=map_objNameIdTable.begin(); iter != map_objNameIdTable.end(); ++iter)
      {
        // std::cout << "comparing " << wishID << " with " << objIDTable [i] << ""<<std::endl;
        if (iter->second == wishID)
        {
          if (pc_specialParsing)
            std::cerr << "DOUBLE USE OF RESOURCE NAME '" << pc_specialParsing->getResourceName (wishID) << "' WITH ID '"<< (uint16_t)wishID << "' ENCOUNTERED! STOPPING PARSER! bye."<<std::endl<<std::endl;
          else
            std::cerr << "DOUBLE USE OF OBJECT ID '" << wishID << "' ENCOUNTERED! STOPPING PARSER! bye."<<std::endl<<std::endl;
          return -1;
        }
      }
      // if we reach here that wishID is not assigned yet, so we can use it!
      foundID = wishID;
    }
    else if ( checkForAllowedExecution() )
    { /// only auto-decrement if the current object has a basic or proprietary object type
      if (b_isMacro)
      {
        foundID = getFreeId (objNextMacroAutoID);
      }
      else
      {
        foundID = getFreeId (objNextAutoID);
      }
    }

    /// only insert object if it is basic or proprietary
    if ( checkForAllowedExecution() )
    { // insert new name-id pair now!
#if DEBUG
      std::cout << "Inserting name-ID-pair (" << str_objName << ", " << foundID << ")" << std::endl;
#endif
      map_objNameIdTable.insert (pair<std::string, uint16_t>(str_objName, foundID));
    }
  }

  return (foundID);
}

unsigned int vt2iso_c::getFreeId (unsigned int& aui_objNextId)
{
  while (mbitset_objIdUsed.test (aui_objNextId))
    --aui_objNextId;
  return (aui_objNextId--);
}

signed long int vt2iso_c::setID (const char* objName, unsigned int wishID)
{
  b_hasUnknownAttributes = false;

  // Added the following check. This is necessary so that objects like input lists which can contain lists of the NULL object ID (65535)
  // do not assign object ids for this, and as a result count an additional object unnecessarily by incrementing objCount.
  if (wishID == 65535)
  {
    return wishID;
  }

  // first check if object is there already
  std::string str_objName = std::string (objName);
  std::map<std::string, uint16_t>::iterator iter=map_objNameIdTable.find (str_objName);
  {
    if (iter->second != wishID) // only set ID if different from wishID
      iter->second = wishID;

    return (wishID);
  }

  return getID (objName, false, true, wishID);
}

signed long int vt2iso_c::idOrName_toi(const char* apc_string, bool ab_isMacro)
{
  if (apc_string [0] == 0x00)
  {
    std::cerr << "*** ERROR *** idOrName_toi: Empty 'object_id' attribute!"<<std::endl<<std::endl;
    return -1;
  }

  if ((apc_string [0] >= '0') && (apc_string [0] <= '9'))
  {
    const int len = strlen (apc_string);
    for (int i = 1; i<len; ++i)
    {
      if ((apc_string[i] < '0') || (apc_string[i] > '9'))
      {
        std::cerr << "*** ERROR *** idOrName_toi: value starts with digit [0..9] but has other characters in it, too!"<<std::endl<<std::endl;
        return -1;
      }
    }
    // if reaching here, the "name" consists of digits only...
    return atoi (apc_string);
  }

  return getID (apc_string, ab_isMacro, false, 0);
}


void vt2iso_c::getKeyCode()
{
  arrc_attributes [attrKey_code].set (str(format("%d") % kcNextAutoID));
  kcNextAutoID--;
}


bool
vt2iso_c::init(
  const string& arcstr_cmdlineName,
  std::string* dictionary,
  bool ab_externalize,
  bool ab_disableContainmentRules,
  DOMBuilder* ap_parser,
  bool ab_verbose,
  const std::string& arcstr_outDirName,
  const std::string& arcstr_namespace,
  bool ab_acceptUnknownAttributes,
  bool ab_silentMode,
  bool ab_pedanticMode,
  const std::string& arcstr_outFileName,
  const std::string& arcstr_searchPath,
  const std::string& arcstr_langPrefix,
  const std::string& arcstr_definesPrefix,
  const std::string& arcstr_baseClass,
  const std::string& arcstr_baseClassHdr )
{
  parser = ap_parser;
  mb_verbose = ab_verbose;
  mb_acceptUnknownAttributes = ab_acceptUnknownAttributes;
  mb_silentMode = ab_silentMode;
  mb_pedanticMode = ab_pedanticMode;
  mstr_outDirName = arcstr_outDirName; // overriding parameters!
  mstr_outFileName = arcstr_outFileName; // overriding parameters!
  mstr_langPrefix = arcstr_langPrefix;
  mstr_definesPrefix = arcstr_definesPrefix;
  mstr_baseClass = arcstr_baseClass;
  mstr_baseClassHdr = arcstr_baseClassHdr;

  // pass verbose-level on to vt2isoimagebase_c
  if (!mb_verbose)
    c_Bitmap.resetOstream();

  /// Are we running in project-file mode? (check extension for .VTP/.vtp)
  if( !isProjectFileMode(arcstr_cmdlineName) )
  {
    std::cerr << "Not specifying a vt-designer project file (.vtp), but this legacy-mode is no longer supported. Please create a proper project-file for your project."<<std::endl;
    return false;
  }

  // Parse xml file search path if given
  mvec_searchPath = getSearchPath( arcstr_searchPath );

  // Will generate all needed (part)filenames/directories
  if (!prepareFileNameAndDirectory (arcstr_cmdlineName)) 
    return false;

  std::string str_namespace;
  if (arcstr_namespace == ":projectname:")
    str_namespace = mstr_className;
  else
    str_namespace = arcstr_namespace;

  if (!str_namespace.empty())
  {
    mstr_namespacePrefix = str_namespace + std::string("::");
    mstr_namespaceDeclarationBegin = std::string("namespace ") + str_namespace + "\n" + std::string("{\n");
    mstr_namespaceDeclarationEnd = std::string("}\n");
  }

  b_externalize = ab_externalize;
  b_disableContainmentRules = ab_disableContainmentRules;

  partFile_variables = NULL;
  partFile_variables_extern = NULL;
  partFile_attributes = NULL;
  partFile_attributes_extern = NULL;
  partFile_functions = NULL;
  partFile_functions_origin = NULL;
  partFile_defines = NULL;
  partFile_obj_selection = NULL;
  partFile_list = NULL;
  partFile_listAttributes = NULL;
#if 0
  partFile_handler_direct = NULL;
#endif
  partFile_handler_derived = NULL;

  std::string partFileName;

  for (int i=0; i<=DEF_iso639entries; i++)
  {
    arrb_objTypes[i] = false;
  }


  /// Let's go....
  if(!mb_silentMode)
  {
    std::cout << std::endl
        << "Generation Settings:" << std::endl
        << "--> Project's Dir: " << mstr_sourceDir << std::endl
        << "--> Project-Name:  " << mstr_projectName << std::endl
        << "--> Class-Name:    " << mstr_className << std::endl
        << "--> Namespace:     " << ((mstr_namespacePrefix.empty()) ? std::string("[--NONE--]") : mstr_namespacePrefix) << std::endl
        << "--> Output Files:  " << mstr_destinDirAndProjectPrefix << "[...]" << std::endl
        << "--> Filelist:" << std::endl;

    for (size_t dex=0; dex < vec_xmlFiles.size(); ++dex)
      std::cout << "    - " << vec_xmlFiles[dex].str_pathName << std::endl;

    std::cout << std::endl;
  }


  partFileName = mstr_destinDirAndProjectPrefix + "-variables.inc";
  partFile_variables = &save_fopen (partFileName.c_str(),"wt");
  fprintf (partFile_variables, "%s", mstr_namespaceDeclarationBegin.c_str());

  partFileName = mstr_destinDirAndProjectPrefix + "-variables-extern.inc";
  partFile_variables_extern = &save_fopen (partFileName.c_str(),"wt");
  fprintf (partFile_variables_extern, "%s", mstr_namespaceDeclarationBegin.c_str());

  partFileName_attributes = mstr_destinDirAndProjectPrefix + "-attributes.inc"; // store original file-name for later wrap-copying over
  partFileName = partFileName_attributes + ".tmp";
  partFile_attributes = &save_fopen (partFileName.c_str(),"wt");
  fprintf (partFile_attributes, "%s", mstr_namespaceDeclarationBegin.c_str());

  partFileName = mstr_destinDirAndProjectPrefix + "-attributes-extern.inc";
  partFile_attributes_extern = &save_fopen (partFileName.c_str(),"wt");
  fprintf (partFile_attributes_extern, "%s", mstr_namespaceDeclarationBegin.c_str());

  partFileName = mstr_destinDirAndProjectPrefix + "-functions.inc";
  partFile_functions = &save_fopen (partFileName.c_str(),"wt");

  partFileName = mstr_destinDirAndProjectPrefix + "-functions-origin.inc";
  partFile_functions_origin = &save_fopen (partFileName.c_str(),"wt");

  partFileName = mstr_destinDirAndProjectPrefix + "-defines.inc";
  partFile_defines = &save_fopen (partFileName.c_str(),"wt");
  fprintf (partFile_defines, "%s", mstr_namespaceDeclarationBegin.c_str());

  partFileName_obj_selection = mstr_destinDirAndProjectPrefix + "-objectselection.inc";
  partFileName = partFileName_obj_selection + ".tmp";
  partFile_obj_selection = &save_fopen (partFileName.c_str(),"wt");

  partFileName = mstr_destinDirAndProjectPrefix + "-list.inc";
  partFile_list = &save_fopen (partFileName.c_str(),"wt");
  fprintf (partFile_list, "%s", mstr_namespaceDeclarationBegin.c_str());
  fprintf (partFile_list, "IsoAgLib::iVtObject_c* HUGE_MEM all_iVtObjects [] = {");

  partFileName = mstr_destinDirAndProjectPrefix + "-list_attributes.inc";
  partFile_listAttributes = &save_fopen (partFileName.c_str(),"wt");
  fprintf (partFile_listAttributes, "%s", mstr_namespaceDeclarationBegin.c_str());
  fprintf (partFile_listAttributes, "IsoAgLib::iVtObject_c::iVtObject_s* HUGE_MEM all_sROMs [] = {");

  partFileName = mstr_destinDirAndProjectPrefix + "-handler-derived.inc";
  partFile_handler_derived = &save_fopen (partFileName.c_str(),"wt");

#ifdef USE_SPECIAL_PARSING_PROP
  pc_specialParsingPropTag = new SpecialParsingUsePropTag_c (arcstr_cmdlineName,
      arcstr_definesPrefix,
      partFile_variables,
      partFile_variables_extern,
      partFile_attributes,
      partFile_attributes_extern,
      partFile_functions,
      partFile_defines);
#else
  pc_specialParsingPropTag = NULL;
#endif

#ifdef USE_SPECIAL_PARSING
  pc_specialParsing = new SpecialParsingUse_c (arcstr_cmdlineName, mstr_sourceDir, dictionary, ""); // "" was the PoolIdent before
#else
  pc_specialParsing = NULL;
#endif
  return true;
}

void vt2iso_c::defaultAndConvertAttributes (unsigned int a_objType)
{
  arrc_attributes[attrSelectable].setIfNotGiven("yes");
  arrc_attributes[attrSoft_key_mask].setIfNotGiven("NULL");
  arrc_attributes[attrHidden].setIfNotGiven("no");
  arrc_attributes[attrBorder_colour].setIfNotGiven("black");
  arrc_attributes[attrHorizontal_justification].setIfNotGiven("left");
  arrc_attributes[attrVertical_justification].setIfNotGiven("top");
  arrc_attributes[attrActivate_for_editing].setIfNotGiven("false");
  arrc_attributes[attrFont_style].setIfNotGiven("none");
  arrc_attributes[attrVariable_reference].setIfNotGivenOrNull("NULL");
  arrc_attributes[attrTarget_value_variable_reference].setIfNotGivenOrNull("NULL");
  arrc_attributes[attrScale].setIfNotGiven("1");
  arrc_attributes[attrRle].setIfNotGiven("auto");
  arrc_attributes[attrLine_suppression].setIfNotGiven("none");

  switch (a_objType)
  { // Check for special handling on Input Objects (options, enabled, option2, etc..)
  case otInputboolean:
    arrc_attributes[attrEnabled].setIfNotGiven("yes");
    break;

  case otInputstring:
    arrc_attributes[attrOptions].setIfNotGiven("none");
    arrc_attributes[attrEnabled].setIfNotGiven("yes");
    break;

  case otInputnumber:
    arrc_attributes[attrOptions].setIfNotGiven("none");

    /// Convert (old v2) attrEnabled to attrInputObjectOptions (in case someone is still
    /// using enabled="..")
    if (!arrc_attributes[attrEnabled].isGiven() && !arrc_attributes[attrInputObjectOptions].isGiven())
    { // none of both set, so default to options2="enabled" (aka inputobject_options="..")
      arrc_attributes[attrInputObjectOptions].set ("enabled");
    }
    else
    { // One or both set, so if enabled set, merge it to attrInputObjectOptions
      // note: if conflicting values for the "enabled" state are set, this is ignored
      if (arrc_attributes [attrEnabled].isGiven())
      { // old enabled=".." given...
        const signed int retEnabled = booltoi (arrc_attributes [attrEnabled].get().c_str());
        if (retEnabled == -1)
        {
          std::cerr << "Error in booltoi() from 'enabled=\"" << arrc_attributes [attrEnabled].get() << "\"'! STOPPING PARSER! bye."<<std::endl<<std::endl;
          clean_exit();
          exit (-1);
        }
        if (retEnabled == 1)
        { // merge over to attrInputObjectOptions
          if (arrc_attributes[attrInputObjectOptions].get().length() > 0)
            arrc_attributes[attrInputObjectOptions].pushBack ('+');
          arrc_attributes[attrInputObjectOptions].set (arrc_attributes[attrInputObjectOptions].get() + "enabled");
        }
        arrc_attributes[attrInputObjectOptions].setIfNotGiven("none");
        // for clearness, unset the "enabled" flag, because it's already converted and hence obsolete.
        arrc_attributes [attrEnabled].clear();
      }
      // else: enabled not set, no need to merge.
    }
    // from now on "attrEnabled" can be ignored for otInputlist, only "attrInputObjectOptions" needs to be checked.
    // (attrEnabled got merged into attrInputObjectOptions)
    break;

  case otInputlist:
    /// Convert (old v2) attrEnabled to attrOptions (as vt-designer always
    /// writes out options=".." regardless of version 2 or 4).
    if (!arrc_attributes[attrEnabled].isGiven() && !arrc_attributes[attrOptions].isGiven())
    { // None of both set, so default to options="enabled"
      arrc_attributes[attrOptions].set ("enabled");
    }
    else
    { // One or both set, so if enabled set, merge it to attrOptions
      // note: if conflicting values for the "enabled" state are set, this is ignored
      if (arrc_attributes [attrEnabled].isGiven())
      { // old enabled=".." given...
        const signed int retEnabled = booltoi (arrc_attributes [attrEnabled].get().c_str()); 
        if (retEnabled == -1)
        {
          std::cerr << "Error in booltoi() from 'enabled=\"" << arrc_attributes [attrEnabled].get() << "\"'! STOPPING PARSER! bye."<<std::endl<<std::endl;
          clean_exit();
          exit (-1);
        }
        if (retEnabled == 1)
        { // merge over to attrOptions
          if (arrc_attributes[attrOptions].get().length() > 0)
            arrc_attributes[attrOptions].pushBack ('+');
          arrc_attributes[attrOptions].set (arrc_attributes[attrOptions].get() + "enabled");
        }
        arrc_attributes[attrOptions].setIfNotGiven("none");
        // for clearness, unset the "enabled" flag, because it's already converted and hence obsolete.
        arrc_attributes [attrEnabled].clear();
      }
      // else: enabled not set, no need to merge.
    }
    // from now on "attrEnabled" can be ignored for otInputlist, only "attrOptions" needs to be checked.
    // (attrEnabled got merged into attrOptions)
    break;

  default:
    arrc_attributes[attrOptions].setIfNotGiven("none");
    break;
  }

  if (a_objType == otAuxiliaryControlDesignatorObjectPointer)
  {
    if( arrc_attributes[attrValue].isGiven() )
      {
      std::cerr << "Invalid (obsolete) attribute 'value=\"..\"' given in Auxiliary Control Designator Object Poiniter. Please use the correct 'auxiliary_object_id=\"..\"' instead!" << std::endl;
      exit (-1);
    }
  }

  if (a_objType == otButton)
  {
      arrc_attributes[attrOptions].setIfNotGiven("0");
  }
  // I think it's better to let the user specify, 8bit as default would blow up the pool, and if only b/w was wanted to be used.. too bad then.
  // if (!attrIsGiven [attrFormat]) {
  //  sprintf (attrString [attrFormat], "8bit");
  //  attrIsGiven [attrFormat] = true;
  // }

  if ((a_objType == otStringvariable) || (a_objType == otOutputstring))
  {
    if (!arrc_attributes [attrLanguage].isGiven() && (spc_autoLanguage.length()) >0)
    {
      arrc_attributes [attrLanguage].set(spc_autoLanguage);
    }
  }
  if (a_objType == otGraphicsContext)
  {
    arrc_attributes[attrCursorX].setIfNotGiven("0");
    arrc_attributes[attrCursorY].setIfNotGiven("0");
  }
  if (a_objType != otGraphicsContext)
  {
    arrc_attributes[attrBackground_colour].setIfNotGiven("white");
  }
}

void vt2iso_c::convertIdReferenceToNameReference(int ai_attrType)
{
//  arrc_attributes[ai_attrType].isGiven();
  if (arrc_attributes[ai_attrType].isGiven())
  {
    if(arrc_attributes[ai_attrType].getLength() == 0)
      return; // do nothing if string empty

    char firstChar = arrc_attributes[ai_attrType].getCharacterAtPos(0);
    if (firstChar >= '0' && firstChar <= '9')
    { // ID is given, so lookup the name for it!
      int uid = arrc_attributes[ai_attrType].getIntValue();
      // first check if ID has an associated name
      for (std::map<std::string, uint16_t>::iterator iter=map_objNameIdTable.begin(); iter != map_objNameIdTable.end(); ++iter)
      {
        if (int(iter->second) == uid)
        {
          arrc_attributes [ai_attrType].set (iter->first);
        }
      }
    }
  }
}

void vt2iso_c::convertIdReferencesToNameReferences()
{
  convertIdReferenceToNameReference (attrActive_mask);
  convertIdReferenceToNameReference (attrSoft_key_mask);
  convertIdReferenceToNameReference (attrVariable_reference);
  convertIdReferenceToNameReference (attrFont_attributes);
  convertIdReferenceToNameReference (attrInput_attributes);
  convertIdReferenceToNameReference (attrLine_attributes);
  convertIdReferenceToNameReference (attrFill_attributes);
  convertIdReferenceToNameReference (attrTarget_value_variable_reference);
}

int vt2iso_c::languageCodeToIndex (char* lc)
{
  for (unsigned int index = 0; index<ui_languages; index++)
  {
    if ( (arrs_language[index].code [0] == lc[0])
          && (arrs_language[index].code [1] == lc[1])
       )
    {
      return index;
    }
  }
  return -1; // negative number to indicate language code not found
}

/* sets the passed attribute if name matches id */
void vt2iso_c::setAttributeValue(int attrID)
{
  if (attr_name.compare(attrNameTable[attrID]) == 0)
  {
    arrc_attributes [attrID].set(attr_value);
  }
}

bool vt2iso_c::checkForFileOrFile148 (const char *tag)
{
  std::string errMsg;
  if (!arrc_attributes [attrFile].isGiven())
  {
    switch (colourdepthtoi (arrc_attributes [attrFormat].get().c_str()))
    {
      case 0: if (!(arrc_attributes [attrFile0].isGiven()))
         errMsg = str(format("YOU NEED TO SPECIFY THE file= OR THE file0= ATTRIBUTE FOR THE <%s> OBJECT! STOPPING PARSER! bye.\n\n") % tag);
        break;
      case 1: if (!(arrc_attributes [attrFile0].isGiven() && arrc_attributes [attrFile1].isGiven()))
        errMsg = str(format("YOU NEED TO SPECIFY THE file= OR ALL fileX= (x=0,1) ATTRIBUTES FOR THE <%s> OBJECT! STOPPING PARSER! bye.\n\n") % tag);
        break;
      case 2: if (!(arrc_attributes [attrFile0].isGiven() && arrc_attributes [attrFile2].isGiven()))
        errMsg = str(format("YOU NEED TO SPECIFY THE file= OR MINIMUM fileX= (x=0,2) ATTRIBUTES FOR THE <%s> OBJECT! STOPPING PARSER! bye.\n\n") % tag);
        break;
      case -1:
        return false;
    }
    if (errMsg.length() > 0)
    {
      std::cerr << errMsg;
      return false;
    }
  }
  return true;
}

DOMNamedNodeMap* vt2iso_c::patched_getAttributes(DOMNode *n)
{
  std::string local_attrName;
  std::string local_attrValue;
  DOMNamedNodeMap *pAttributes = n->getAttributes();

  if (n->hasAttributes())
  { // parse through all attributes
    int nSize = pAttributes->getLength();
    for (int i=0;i<nSize;++i)
    {
      DOMAttr *pAttributeNode = (DOMAttr*) pAttributes->item(i);
      utf16convert (pAttributeNode->getName(), local_attrName);
      utf16convert (pAttributeNode->getValue(), local_attrValue);
      if (local_attrName.compare("latchable") == 0)
      {
        // remove in any case, if latchable="no" removing and no addition is fine...
        ((DOMElement *)n)->removeAttributeNode (pAttributeNode);
        if ( (local_attrValue.compare("yes") == 0)
          || (local_attrValue.compare("true") == 0)
          || (local_attrValue.compare("on") == 0)
          || (local_attrValue.compare("show") == 0)
          || (local_attrValue.compare("enable") == 0)
           )
        {
          std::cout << "*** WARNING: Replaced latchable='"<<local_attrValue<<"' by the new options='latchable'. Please adapt your XML files... ***"<<std::endl;
          local_attrValue = "latchable";
          ((DOMElement *)n)->setAttribute (X("options"), X(local_attrValue.c_str()));
        }
        break;
      }
    }
  }
  return pAttributes;
}


bool vt2iso_c::getAttributesFromNode(DOMNode *n, bool treatSpecial)
{
  DOMNamedNodeMap *pAttributes;
  if (n->hasAttributes())
  { // parse through all attributes
    pAttributes = patched_getAttributes(n);
    int nSize = pAttributes->getLength();
    // empty all possible attribute-values...
    for (int j=0; j<maxAttributeNames; j++)
    {
      arrc_attributes [j].clear();
    }

    // now get all attributes listed in the <tag ...> element
    for (int i=0;i<nSize;++i)
    {
      DOMAttr *pAttributeNode = (DOMAttr*) pAttributes->item(i);
      utf16convert (pAttributeNode->getName(), attr_name);
      utf16convert (pAttributeNode->getValue(), attr_value);

      if (attr_name.compare(mscp_langDetectionDoneAttributeName) == 0)
      {
          continue;
      }
      
      if (treatSpecial)
      { // get 'name=', 'id=' and all other possible attributes
        if (attr_name.compare("name") == 0)
        {
          m_objName = attr_value;
          is_objName = true;
          continue;
        }

        if (attr_name.compare("id") == 0)
        {
          objID = StringToInt(attr_value);
          is_objID = true;
          continue;
        }
      }
      int l;
      for (l=0; l<maxAttributeNames; l++)
      {
        if (attr_name.compare("soft_key_mask") == 0)
        {
          arrc_attributes [attrSoft_key_mask].set (attr_value);
          break;
        }
        else if (attr_name.compare("active_mask") == 0)
        {
          arrc_attributes [attrActive_mask].set (attr_value);
          break;
        }

        if (attr_name.compare(attrNameTable [l]) == 0)
        {
          arrc_attributes [l].set( attr_value );
    // DEBUG-OUT
//          std::cout << "FOUND ATTR: IND " << l << ":= " << attrNameTable [l] << " -> " << attrString[l] << ":"
//                    << attrIsGiven [l] << ""<<std::endl;
          break;
        }
      }

      /// if USE_SPECIAL_PARSING is NOT defined, output an error warning and stop here, if attribute is unknown
      // ERROR: We didn't match a possible attribute name
      if ((pc_specialParsingPropTag || pc_specialParsing) && (l == maxAttributeNames))
      {
        /// set a flag to sign if any attribute is unknown
        if (pc_specialParsing)
          pc_specialParsing->setUnknownAttributes (true);

        if (pc_specialParsingPropTag)
          pc_specialParsingPropTag->setUnknownAttributes (true);

        b_hasUnknownAttributes = true;
      }
      else if (l == maxAttributeNames)
      {
        if (mb_acceptUnknownAttributes)
        {
          if(!mb_silentMode){
            std::cout << "\n\nIGNORING UNKNOWN ATTRIBUTE " << attr_name << "=" << attr_value << " IN TAG <" << XMLString::transcode(n->getNodeName())
                      << std::endl;
          }
        }
        else
        {
          std::cerr << "\n\nUNKNOWN ATTRIBUTE " << attr_name << "=" << attr_value << " IN TAG <" << XMLString::transcode(n->getNodeName())
                    << "> ENCOUNTERED! STOPPING PARSER! bye."<<std::endl<<std::endl;
          return false;
        }
      }
    }
  }

  if (treatSpecial) {
    // If no 'name=' given, add 'name=Unnamed%d' attribute
    if (is_objName == false)
    {
      m_objName = str(format("Unnamed%d") % objNextUnnamedName);
      ((DOMElement *)n)->setAttribute (X("name"), X(m_objName.c_str()));
      objNextUnnamedName++;
      is_objName = true;
    }
  }
  return true;
}

bool vt2iso_c::openDecodePrintOut (const std::list<Path_s>& rcl_stdBitmapPath, unsigned int &options, int& ref_maxDepth, int fixNr)
{
  if (arrc_attributes [attrRle].isGiven())
  {
    if (picturegraphicrletoi (arrc_attributes [attrRle].get().c_str()) == -1)
      return false;

    // set rle stuff
    unsigned int rle = picturegraphicrletoi (arrc_attributes [attrRle].get().c_str());
    // replace possible rle='auto' by 'rle1+rle4+rle8'
    if (rle & (uint64_t(1)<<3)) rle = (uint64_t(1)<<0) | (uint64_t(1)<<1) | (uint64_t(1)<<2);
    // and merge to options byte
    options |= rle<<2;
  }

  std::string arr_file[3];
  bool b_absPath[3] = {false, false, false};

  // fill up missing fileX with default file
  if (arrc_attributes[attrFile].isGiven())
  {
    if (!arrc_attributes[attrFormat].isGiven())
    {
      std::cerr << "Attribute \"file\" needs also attribute \"format\"" <<std::endl;
      return false;
    }

    int depthFromFormat = colourdepthtoi (arrc_attributes [attrFormat].get().c_str());
    if ( (depthFromFormat < 0) || (depthFromFormat > 2))
    {
      std::cerr << "Invalid format " << arrc_attributes [attrFormat].get() << "!"<<std::endl;
      return false;
    }

    for (int depth = 0; depth <= depthFromFormat; depth++)
    {
      arr_file[depth] = arrc_attributes[attrFile].get();
      if (arrc_attributes [attrAbsolutePath].isGiven())
        b_absPath[depth] = (arrc_attributes [attrAbsolutePath].get().compare("1") == 0);
    }
  }

  for (int i=0; i<=2; i++)
  {
    if (arrc_attributes[attrFile0 + i].isGiven())
    {
      arr_file[i] = arrc_attributes[attrFile0 + i].get(); // arrc_attributes[attrFile0 + i] can be empty => empty arr_file[i] will not used for bitmap generation
      if (arrc_attributes[attrAbsolutePath1 + i].isGiven())
        b_absPath[i] = (arrc_attributes [attrAbsolutePath1 + i].get().compare("1") == 0);
      else
        b_absPath[i] = false; // no absPath attribute given => set to default relative path
    }
  }

  if (arr_file[0].empty() && (fixNr == -1))
  { // bitmap for depth 0 must be available (allowed for fixedbitmap)
    std::cerr << "no bitmap for colour depth 0! This is only allowed for fixedbitmap"<<std::endl;
    return false;
  }

  ref_maxDepth=0;
  if (!arr_file[1].empty()) ref_maxDepth=1;
  if (!arr_file[2].empty()) ref_maxDepth=2;

  // generate all lower depth-bitmaps...
  for (int actDepth=0; actDepth <= ref_maxDepth; actDepth++)
  {
    if (arr_file[actDepth].empty())
      continue;

    if (fixNr == -1) fprintf (partFile_attributes, "const HUGE_MEM uint8_t iVtObject%s_aRawBitmap%d [] = {", m_objName.c_str(), actDepth);
    else /* -fix- */ fprintf (partFile_attributes, "const HUGE_MEM uint8_t iVtObject%s_aRawBitmap%dFixed%d [] = {", m_objName.c_str(), actDepth, fixNr);

    // find matching path path for file
    struct stat s_stat;

    std::string str_concat = " "; str_concat[0] = scc_dirSeparatorCorrect;
    std::list<Path_s>::const_iterator iter = rcl_stdBitmapPath.begin();
    for (; iter != rcl_stdBitmapPath.end(); ++iter)
    {
      std::string str_tmpWorkDir = mstr_sourceDir;
      if (!iter->b_relativePath)
        str_tmpWorkDir.clear();

      if (b_absPath[actDepth])
        filename = arr_file[actDepth]; // absolute path in picture graphic object
      else
        filename = str_tmpWorkDir + iter->str_pathName + str_concat + arr_file[actDepth];

      if (stat(filename.c_str(), &s_stat) == 0)
        // bitmap in file system found
        break;
    }

    if (iter == rcl_stdBitmapPath.end())
    { // bitmap not found in file system
      std::cerr << "Loading of " << filename << " failed (not found in file system)!"<<std::endl;
      return false;
    }

    // Open Bitmap
    if ( c_Bitmap.openBitmap( filename.c_str() ) )
    {
      if (mb_verbose) std::cout << "Loaded successfully! ";
    }
    else
    {
      std::cerr << "Loading failed!"<<std::endl;
      return false;
    }
    if (ui_picBufferSize < c_Bitmap.getWidth() * c_Bitmap.getHeight())
    {
      delete[] picBuffer;
      ui_picBufferSize = c_Bitmap.getWidth() * c_Bitmap.getHeight();
      picBuffer = new unsigned char[ui_picBufferSize];
    }
    // Decode bitmap to buffer!
    switch (actDepth)
    {
      case 0: // 1 bit colour (monochrome) = 2 colours (black/white)
        c_Bitmap.write1BitBitmap( picBuffer, ui_picBufferSize );
        break;
        case 1: // 4 bit colour = 16 colours
          c_Bitmap.write4BitBitmap( picBuffer, ui_picBufferSize );
          break;
          case 2: // 8 bit colour = 256 colours
            c_Bitmap.write8BitBitmap( picBuffer, ui_picBufferSize );
            break;
    } // switch

    if (c_Bitmap.objRawBitmapBytes [actDepth] == 0)
    {
      std::cerr << "===> Bitmap with size 0. Terminating!"<<std::endl<<std::endl;
      return false;
    }
    if (c_Bitmap.objRawBitmapBytes [actDepth] < 0)
    {
      std::cerr << "===> Bitmap palette did not match. Terminating!"<<std::endl<<std::endl;
      exit(1);
    }

    // Is RLE wanted/sensible? ("rle[actDepth]" set?)
    if (options & (uint64_t(1)<<(2+actDepth))) {
      unsigned int offset=0;
      unsigned int rleBytes=0;
      while (offset < c_Bitmap.objRawBitmapBytes [actDepth]) {
        int cnt=1;
        while (((offset+cnt) < c_Bitmap.objRawBitmapBytes [actDepth]) && (picBuffer [offset] == picBuffer [offset+cnt]) && (cnt < 255)) cnt++;
        offset += cnt;
        rleBytes += 2;
      }
      // If RLE-size is larger than uncompressed size, clear the RLE1/4/8 flag!
      if (rleBytes >= c_Bitmap.objRawBitmapBytes [actDepth]) {
        options &= -1-(uint64_t(1)<<(2+actDepth));
        if (mb_verbose)
          std::cout << m_objName << "' actDepth012='" << actDepth
              << "' has RLE size of " << rleBytes << " but uncompressed size of "
              << c_Bitmap.objRawBitmapBytes [actDepth] << ". Turning RLE off!"<<std::endl;
      }
    }

    static bool b_largeObject = false;
    bool b_oldLarge = b_largeObject;
    // Print out raw data or rle_data?
    if (options & (uint64_t(1)<<(2+actDepth))) { // rle1, rle4 or rle8 "STILL" set?
      //// +++ RLE-COMPRESSED OUTPUT +++
      unsigned int offset=0;
      int rleBytes=0;
      int PixelCount = 0;
      while (offset < c_Bitmap.objRawBitmapBytes [actDepth]) {
        unsigned int cnt=1;
        while (((offset+cnt) < c_Bitmap.objRawBitmapBytes [actDepth]) && (picBuffer [offset] == picBuffer [offset+cnt]) && (cnt < 255)) cnt++;
        if (offset == 0) fprintf (partFile_attributes, " /* RLE-Encoded Raw-Bitmap Data */ %d, %d", cnt, picBuffer [offset]);
        else /* ----- */ fprintf (partFile_attributes, ",%d, %d", cnt, picBuffer [offset]);
        offset += cnt;
        rleBytes += 2;
        PixelCount += cnt;
      }
      if (mb_verbose)
        std::cout << m_objName << "' actDepth012='" << actDepth
            << "' has RLE size of " << rleBytes << " but uncompressed size of "
            << c_Bitmap.objRawBitmapBytes [actDepth]
            << "#Pixels: " << PixelCount << ", Width: " << c_Bitmap.getWidth() << ", Height: "
            << c_Bitmap.getHeight() << std::endl;
      c_Bitmap.objRawBitmapBytes [actDepth] = rleBytes;
      if (rleBytes > 65000) b_largeObject = true; // normally 65535-restAttributesSize would be enough, but don't care, go save!
    } else {
      //// +++ NORMAL UNCOMPRESSED OUTPUT +++
      if (actDepth == 1)
      { // nur der lesbarkeit halber!!
        fprintf (partFile_attributes, " /* 16-Colour Raw-Bitmap Data */ (%d << 4) | %d", picBuffer [0] >> 4, picBuffer [0] & 0xF);
        for (unsigned int i=1; i<c_Bitmap.objRawBitmapBytes [actDepth]; i++)
        {
          fprintf (partFile_attributes, ", (%d << 4) | %d", picBuffer [i] >> 4, picBuffer [i] & 0xF);
        }
      }
      else
      {
        fprintf (partFile_attributes, " /* Raw-Bitmap Data */ %d", picBuffer [0]);
        for (unsigned int i=1; i<c_Bitmap.objRawBitmapBytes [actDepth]; i++)
        {
          fprintf (partFile_attributes, ", %d", picBuffer [i]);
        }
      }
      if (c_Bitmap.objRawBitmapBytes [actDepth] > 65000) b_largeObject = true; // normally 65535-restAttributesSize would be enough, but don't care, go save!
    }
    fprintf (partFile_attributes, "};\n");
    if (!b_oldLarge && b_largeObject)
    {
      fprintf (partFile_attributes, "#ifndef USE_OBJECTS_LARGER_THAN_64K\n");
      fprintf (partFile_attributes, "  #error \"Your objectpool has large object(s) so you need to compile your project with the   USE_OBJECTS_LARGER_THAN_64K   define set.\"\n");
      fprintf (partFile_attributes, "#endif\n");
    }
  } // for actDepth...

  return true;
}

bool vt2iso_c::checkForAllowedExecution() const
{
  // no module is wished
  if (!pc_specialParsing && !pc_specialParsingPropTag)
    return true;

  // if parsing of proprietary tags/attributes is wished, check here for correct proprietary type
  if (pc_specialParsingPropTag && pc_specialParsingPropTag->checkForProprietaryOrBasicObjTypes())
    return true;

  // if parsing for lookup tables is wished, check here for correct additional types
  if (pc_specialParsing && pc_specialParsing->checkForProprietaryOrBasicObjTypes (objType))
    return true;

  return false;
}

void vt2iso_c::autoDetectLanguage (DOMNode *n)
{
  if( ((DOMElement*)n)->hasAttribute( X(mscp_langDetectionDoneAttributeName) ) ) {
    return;
  }

  std::string newLanguageValue;
  std::string searchName;
  searchName = getAttributeValue (n, "name");
  if (searchName[0] == 0x00)
  { // no name, so we can't search for the
    searchName = getAttributeValue(n, "id");
  }

  if (searchName[0] == 0x00)
  { // no id, so we can't search for the value...
    ((DOMElement *)n)->setAttribute (X(mscp_langDetectionDoneAttributeName), X(""));
    return;
  }

  if (getAttributeValue (n, "language")[0] != 0x00)
  { // language found, no need to auto-detection
#if DEBUG_LANGUAGE_AUTO_DETECT
    std::cout << "AUTO-DETECT-LANGUAGE: language=\""<< getAttributeValue (n, "language") << "\" directly specified for ["<< searchName <<"]."<<std::endl;
#endif
    ((DOMElement *)n)->setAttribute (X(mscp_langDetectionDoneAttributeName), X(""));
    return;
  }

   char MBBuffer[ maxTagLength ];

// #if DEBUG_LANGUAGE_AUTO_DETECT
//   std::cout << "AUTO-DETECT-LANGUAGE: searching language file for ["<<searchName<<"]... ";
// #endif
// language not explicitly given, check if it should be given implicitly
  for (unsigned int curLang=0; curLang<ui_languages; curLang++)
  {
      /// Check if object is in language-value-file
    if (arrs_language[curLang].valueBuffer != NULL)
    {
      char* bufferCur = arrs_language[curLang].valueBuffer;
      char* bufferEnd = bufferCur + arrs_language[curLang].valueBufferLen;
      while (bufferCur < bufferEnd)
      { // check this line (\n and \r has been previously converted to 0x00
        std::string pc_id;
        int numWideChars;
        char* firstChar=bufferCur;
        while (*firstChar == ' ')
          firstChar++;
        if ( (*firstChar == 0x00)
             || (strstr (bufferCur, "//") && (strstr (bufferCur, "//") == firstChar))
           )
        { // ignore line
        }
        else
        {
          if (arrs_language[curLang].unicode)
          {
            bufferCur += 2;  // start me at the beginning of the tag 
            numWideChars = (bufferEnd - bufferCur) / 2;
            UCS2* commaLoc = findWideChar ((const UCS2*)bufferCur, 0x2C, numWideChars );
            if (commaLoc)   // if not valid, we are done looking..
            {
              int charCnt = ((char*)commaLoc - bufferCur)/2;
              wcstombs(MBBuffer, (const wchar_t*)bufferCur, charCnt );
              MBBuffer[charCnt] = 0;  // theres our identifier..
              pc_id = MBBuffer;
              if (pc_id.compare(searchName) == 0)
              { /// add this language to the language=".." attribute!
                newLanguageValue += arrs_language [curLang].code;
                break;
              }
            }
            else
            { // no comma found, although it was not a commentary line :(
              std::cout << "No COMMA in a non-comment line in the " << arrs_language [curLang].code << ".vtu language file!"<<std::endl;
              return;
            }
          }
          else //  This is good ol null terminated strings, so can use those standard i/o calls ------------
          {
            char* comma = strchr (bufferCur, ',');
            if (comma)
            {
              *comma = 0x00;
              pc_id = bufferCur;
              *comma = ','; // revert comma-separator
              if (pc_id.compare(searchName) == 0)
              { /// add this language to the language=".." attribute!
                newLanguageValue += arrs_language [curLang].code;
                break;
              }
            }
            else
            { // no comma found, although it was not a commentary line :(  - or we could have been at the end...
              if ((bufferEnd-bufferCur) > 5) {   // There really should be a comma
                std::cout << "No COMMA in a non-comment line in the " << arrs_language [curLang].code << ".vtl language file!"<<std::endl;
                return;
              }
            }
          }
        }

        // advance to next line --  of-course a wideChar and the byteSize is available
        if (arrs_language[curLang].unicode)
        {
          UCS2* newStart = findWideChar ((const UCS2*)bufferCur, 0x00, numWideChars );
          if (newStart) 
          {
            newStart++;  // theres another 0x0000 out there...
            bufferCur = (char*)newStart;
          } 
          else
          {
            bufferCur = bufferEnd + 1;   // Jane, stop this crazy thing
          }
        }
        else  // - Yup good ol strlen employed here  ---------
        {
          int lineLen = strlen (bufferCur)+1; // include terminating 0x00
          bufferCur += lineLen;
        }
      }
    }
  }
  ((DOMElement *)n)->setAttribute (X("language"), X(newLanguageValue.c_str()));
#if DEBUG_LANGUAGE_AUTO_DETECT
  if (newLanguageValue[0] != 0x00)
    std::cout << "AUTO-DETECT-LANGUAGE: FOUND: ["<<newLanguageValue <<"] for "<<searchName<<"."<<std::endl;
#endif
  ((DOMElement *)n)->setAttribute (X(mscp_langDetectionDoneAttributeName), X(""));
}

std::string
formatAsFloat( const std::string& in )
{
  bool dotFound=false;
  for( std::size_t i=0; i<in.length(); ++i )
    if( in[ i ] == '.' )
      dotFound = true;

  bool eFound=false;
  for( std::size_t i=0; i<in.length(); ++i )
    if( ( in[ i ] == 'e' ) || ( in[ i ] == 'E' ) )
      eFound = true;

  std::string out( in );

  if( !dotFound && !eFound )
    out += ".";

  out += "f";

  return out;
}


// ---------------------------------------------------------------------------
//
//  static void processElement (DOMNode *n, unsigned int omb) --- Recursively process all ELEMENT XML-Tags...
// we're in an element here, not text or something else...
//
// ---------------------------------------------------------------------------
/// const char* rpcc_inKey, const char* rpcc_inButton
/// NOT USED atm for recursive uses, as the recursion is done in the setOrigin code!!!!
bool
vt2iso_c::processElement (DOMNode *n, uint64_t ombType /*, const char* rpcc_inKey, const char* rpcc_inButton*/)
{
  if (!n)
  {
    printf ("processElement(): 'DOMNode == NULL'. STOPPING PARSER! bye.\n\n");
    return false;
  }

  // first set some member variables for all the helper functions
  m_nodeName = XMLString::transcode(n->getNodeName());

  #define maxFixedBitmaps 1000

  is_objName = false;
  is_objID = false;

  objType = objectIsType (m_nodeName); // returns 0..(maxObjectTypes-1)
  int commandType = commandIsType (m_nodeName); // returns 0..(maxCommandsToCompare-1)

  // only parse WorkingSet (and objectpool) when in "mb_parseOnlyWorkingSet"
  if (mb_parseOnlyWorkingSet)
  {
    if ((objType != otObjectpool) && (objType != otWorkingset))
     return true;
  }
  else
  { // !mb_parseOnlyWorkingSet
    if (objType == otWorkingset)
      return true; // has already been parsed!
  }

  if (isVerbose())
  {
    std::cout << " processElement ("<<m_nodeName<<", ombType = 0x"<<std::hex<< ombType <<")" << std::endl;
  }

  /// if USE_SPECIAL_PARSING is defined the objType will also be tested if it is a valid additional object type
  if ( ( pc_specialParsing || pc_specialParsingPropTag) && (objType == 0xFFFF) && (commandType == 0xFFFF))
  {
    if ( pc_specialParsing )
      objType = pc_specialParsing->getObjType(m_nodeName);

    if ( pc_specialParsingPropTag && objType == 0xFFFF )
      objType = pc_specialParsingPropTag->getObjType (m_nodeName);
  }

  // ERROR: Wrong <TAG>
  if ((objType == 0xFFFF) && (commandIsType (m_nodeName) == 0xFFFF))
  {
    std::cerr << "\n\nUNKNOWN TAG <"<< m_nodeName <<"> ENCOUNTERED! STOPPING PARSER! bye."<<std::endl<<std::endl;
    return false;
  }

  /// NOW GO AND DO SOMETHING FOR THIS ELEMENT
  DOMNode *child;
  DOMNamedNodeMap *pAttributes;
  if (objType >= otObjectpool)
  {
    if (objType == otObjectpool)
    { // expect (datamask) dimension here
      if (n->hasAttributes())
      { // parse through all attributes
        /// ATTRIBUTES IN THIS ELEMENT GOT >>obsolete<< WHEN USING WITH A PROJECT FILE!!!
        clean_exit ("You're running in project-mode but are using an XML-file that has values in the <objectpool> tag (e.g. \"dimension\"). Remove all attributes please and make sure the values in your project-file are set up correctly! STOPPING PARSER! bye.\n\n");
        exit(-1);
      }
    }
    if (pc_specialParsing && (objType == otIncludeobject))
    {
      m_objName = str(format("%s") % (getAttributeValue (n, "name")));
      if (!pc_specialParsing->parseKnownTag(n, objType, m_objName.c_str(), &objID, &is_objID))
        return false;
    }
  }
  else
  { /// Standard ISO Object
#if DEBUG
  fprintf(stderr, "  objType = %d\n", objType);
  fprintf(stderr, "  maxObjectTypes = %d\n", maxObjectTypes);
#endif

    if ( objType < maxObjectTypes && ( !b_disableContainmentRules && ( ( (uint64_t(1)<<objType) & ombType) == 0 )) )
    {
      // ERROR: Unallowed <TAG> here?!
      std::cerr << "\n\nENCOUNTERED WRONG TAG AT THIS POSITION!\nENCOUNTERED: <" << m_nodeName << "> '" << getAttributeValue (n, "name") << "' objType: "
                << objType << " ombType: " << std::dec << ombType << " (0x" << std::hex << ombType << std::dec << ")\nPOSSIBLE TAGS HERE WOULD BE: ";
      for (int j=0; j<maxObjectTypes; j++)
      {
        if ((uint64_t(1)<<j) & ombType)
        {
          std::cerr << " <" << otCompTable [j] << ">  ";
        }
      }
      std::cerr << std::endl << std::endl;
      return false;
    }

    /// if USE_SPECIAL_PARSING is defined, check here if found tag is valid at this position
    bool b_unknownTag = false;
    if (pc_specialParsing && ((objType >= maxObjectTypes) && !pc_specialParsing->checkTag(n, objType, ombType)))
    {
      b_unknownTag = true;
    }
    if (!b_unknownTag && pc_specialParsingPropTag && ((objType >= maxObjectTypes) && !pc_specialParsingPropTag->checkTag (n, objType, ombType)))
    {
      std::cerr << "Unknown tag for enhanced parsing modules! STOP PARSER! bye." << std::endl << std::endl;
      return false;
    }

    static bool sb_firstObject=true;
    if (sb_firstObject)
    { // first object has to be WorkingSet
      if (objType != otWorkingset)
      {
        std::cerr << "\n\nFIRST ELEMENT HAS TO BE <workingset>! Stopping PARSER. Please put your workingset-object at the top in your <objectpool>!"<<std::endl<<std::endl;
        return false;
      }
      sb_firstObject = false; // check is only valid for first element!
    }

    autoDetectLanguage (n);
    if (!getAttributesFromNode (n, true))
    {
      std::cerr << "\n\nCOULDN'T GET ATTRIBUTES FROM NODE!"<<std::endl<<std::endl;
      return false; // true: read name= and id=
    }

    // set all non-set attributes to default values (as long as sensible, like bg_colour etc.)
    defaultAndConvertAttributes (objType);

    // get a new ID for this object if not yet done
    signed long int checkObjID = getID (m_objName.c_str(), (objType == otMacro), is_objID, objID);

    if (checkObjID == -1)
    {
      std::cerr << "Error in getID() from object <" << m_nodeName << "> '" << m_objName << "'! STOP PARSER! bye." << std::endl << std::endl;
      return false;
    }
    else
      objID = (unsigned int)checkObjID;

    if (pc_specialParsingPropTag && (objType >= maxObjectTypes))
    {
      if (!pc_specialParsingPropTag->parseUnknownTag (n, objType, m_objName, this))
        return false;
    }

    arrb_objTypes[ objType ] = true; /// Remember which tag has been used - this will later be used to save some overhead in ivtincludes.

    /** if USE_SPECIAL_PARSING is defined then a special parsing follows here
      * either to parse the additional attributes in the known tags
      * or to fully parse an unknown tag
    */
    if (pc_specialParsing)
    {
      /// first check if the wished id (attribute id="" is given) is a valid resource id
      if (is_objID && !pc_specialParsing->checkUseOfResourceID (objID))
      {
        const char* pcch_tmpName = (is_objName) ? m_objName.c_str() : "unnamed object";
        std::cerr << "\n\nMISUSE OF RESOURCE ID '" << objID << "' AS WISH OBJECT ID IN OBJECT <" << m_nodeName << "> with name '"
            << pcch_tmpName << "' STOPPING PARSER! bye."<<std::endl<<std::endl;
        return false;
      }

      if (objType >= maxObjectTypes) /// that tag is unknown for basic vt2iso
      {
        if (!pc_specialParsing->parseUnknownTag(n, objType, &is_objID, objID))
          return false;
        b_hasUnknownAttributes = false;
      }
      else
      {
        switch (objType)
        {
          case otDatamask:
          case otAlarmmask:
            /// attribute for reference to softkeymask
            if (!pc_specialParsing->parseKnownTag(n, objType, m_objName.c_str(), &objID, &is_objID, arrc_attributes[attrSoft_key_mask].get().c_str()))
              return false;
            break;
          default:
            /// attribute for variable reference
            if (!pc_specialParsing->parseKnownTag(n, objType, m_objName.c_str(), &objID, &is_objID, "NULL", arrc_attributes[attrVariable_reference].get().c_str()))
              return false;
            break;
        }
      }
      if (b_hasUnknownAttributes)
      // set object ID here to ensure that the resource ID is used as object ID
        setID (m_objName.c_str(), objID);
    }

    // Completely parsed <tag ...> now, start writing out to the files!
    // ______________________________________________________________________________________________________________________________
    // we got objName, objID, objType here now! - print out standard definitions in A, B and C right after the array definition lines
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    bool objHasArrayEventMacro = false;
    if (pc_specialParsingPropTag && (objType >= maxObjectTypes))
    {
      objHasArrayEventMacro = pc_specialParsingPropTag->objHasArrayEventMacro (objType);
    }
    switch (objType)
    {
      case otWorkingset:     case otDatamask:       case otAlarmmask:       case otContainer:  case otSoftkeymask:  case otKey:  case otButton:
      case otInputboolean:   case otInputstring:    case otInputnumber:     case otInputlist:
      case otOutputstring:   case otOutputnumber:   case otOutputlist:
      case otLine:           case otRectangle:      case otEllipse:         case otPolygon:
      case otMeter:          case otLinearbargraph: case otArchedbargraph:
      case otPicturegraphic:
      case otFontattributes: case otLineattributes:  case otFillattributes:  case otInputattributes:
        objHasArrayEventMacro = true; // only variables and object pointer have no events/macros
        break;
    }

    bool objHasArrayObject = false;
    if (pc_specialParsingPropTag && (objType >= maxObjectTypes))
    {
      objHasArrayObject = pc_specialParsingPropTag->objHasArrayObject (objType);
    }
    switch (objType)
    {
      case otSoftkeymask:
      case otInputlist:
      case otOutputlist:
        objHasArrayObject = true;
        break;
    }

    bool objHasArrayObjectXY = false;
    if (pc_specialParsingPropTag && (objType >= maxObjectTypes))
    {
      objHasArrayObjectXY = pc_specialParsingPropTag->objHasArrayObjectXY (objType);
    }
    std::string recursePass;
    //const char* rpcc_inKey=NULL;
    //const char* rpcc_inButton=NULL;
    switch (objType)
    {
      case otKey:
        recursePass = m_objName;
        //rpcc_inKey = recursePass.c_str();
        objHasArrayObjectXY = true;
        break;
      case otButton:
        recursePass = m_objName;
        //rpcc_inButton = recursePass.c_str();
        objHasArrayObjectXY = true;
        break;
      case otWorkingset:
      case otDatamask:
      case otAlarmmask:
      case otContainer:
      case otAuxiliaryfunction:
      case otAuxiliaryinput:
      case otAuxiliaryFunction2:
      case otAuxiliaryInput2:
        objHasArrayObjectXY = true;
        break;
    }

    bool objHasArrayLanguagecode = false;
    switch (objType)
    {
      case otWorkingset:
        objHasArrayLanguagecode = true;
        break;
    }

    bool objHasArrayMacroCommand = false;
    switch (objType)
    {
      case otMacro:
        objHasArrayMacroCommand = true;
        break;
    }

    bool objHasArrayPoints = false;
    if (pc_specialParsingPropTag && (objType >= maxObjectTypes))
    {
      objHasArrayPoints = pc_specialParsingPropTag->objHasArrayPoints (objType);
    }
    switch (objType)
    {
      case otPolygon:
        objHasArrayPoints = true;
        break;
    }

    static std::string duplicateForLanguages; // max for all languages

    /// Backup as those values are overriden by the multilanguage processing when getting values from the .xx.txt
    OneAttribute_c backupAttrStringValue = arrc_attributes [attrValue];
    OneAttribute_c backupAttrStringLength = arrc_attributes [attrLength];

    autoDetectLanguage (n);
    // ************* NEW: Language duplicates!! ************

    bool b_dupMode=false;
    if ((arrc_attributes [attrLanguage].isGiven()) && (arrc_attributes [attrLanguage].getLength() > 0) && (arrc_attributes [attrLanguage].getLength() != 2))
    { // "*" or multiple languages, so we need to loop!
      if ((arrc_attributes [attrLanguage].get().compare ("*")) == 0)
      { // build all languages we have defined in the working set
        duplicateForLanguages.clear();
        for (unsigned int ui=0; ui<ui_languages; ui++)
          duplicateForLanguages += arrs_language [ui].code;
      }
      else
      {
        duplicateForLanguages = arrc_attributes [attrLanguage].get();
      }
      b_dupMode=true;
    }


    const char* dupLangNext=duplicateForLanguages.c_str(); // if dup, then start with first one of course...
    do // language duplication loop!
    {
      if (b_dupMode)
      { // "fake-copy" next language to "attrString [attrLanguage]"
        if (*dupLangNext == '+') dupLangNext++; // skip "+"s!
        arrc_attributes [attrLanguage].clear();
        arrc_attributes [attrLanguage].pushBack (*dupLangNext++);
        arrc_attributes [attrLanguage].pushBack (*dupLangNext++);
        if (*dupLangNext == '+') dupLangNext++; // skip "+"s!

        /// Restore "value=" attribute, as it may have been changed by the multilanguage get from .xx.txt file!
        arrc_attributes [attrValue] = backupAttrStringValue;
        arrc_attributes [attrLength] = backupAttrStringLength;
      }
      // ##########################################
      // ### Print out Repeat Array definitions ###
      // ##########################################
      // ### Print out LANGUAGECODE array
      int deXcolourDepth(0);
      int deXtransCol(0);
      unsigned int objChildLanguages(0);
      std::string tempString;
      if (objHasArrayLanguagecode)
      {
        // Process all Child-Elements
        bool firstElement(true);
        for (child = n->getFirstChild(); child != 0; child=child->getNextSibling())
        {
          if ( (child->getNodeType() == DOMNode::ELEMENT_NODE) && (0 == strcmp (XMLString::transcode(child->getNodeName()), otCompTable [otLanguage])) )
          {
            /// Here we're in one <language code="xx"> element!!!
            // get 'code=' out of child
            if(child->hasAttributes())
            {
              // parse through all attributes
              pAttributes = patched_getAttributes(child);
              int nSize = pAttributes->getLength();

              arrc_attributes [attrCode].clear();

              for(int i=0;i<nSize;++i)
              {
                DOMAttr *pAttributeNode = (DOMAttr*) pAttributes->item(i);
                utf16convert (pAttributeNode->getName(), attr_name);
                utf16convert (pAttributeNode->getValue(), attr_value);

                if (attr_name.compare("code") == 0)
                {
                  arrc_attributes [attrCode].set( attr_value );
                }
                else
                {
                  std::cerr << "\n\nATTRIBUTE OTHER THAN 'code=' GIVEN IN <language ...> ! STOPPING PARSER! bye."<<std::endl<<std::endl;
                  return false;
                }
              }
            }
            if (firstElement)
            {
              fprintf (partFile_attributes, "const IsoAgLib::repeat_vtLanguage_s ivtObject%s_aLanguage [] = {", m_objName.c_str());
            }
            else
            {
              fprintf (partFile_attributes, ", ");
            }
            if (!(arrc_attributes [attrCode].isGiven()))
            {
              std::cerr << "\n\ncode ATTRIBUTE NEEDED IN <language ...> ! STOPPING PARSER! bye."<<std::endl<<std::endl;
              return false;
            }
            char languageCode[2];
            languageCode[0] = tolower (arrc_attributes [attrCode].getCharacterAtPos(0)); // getCharacterAtPos
            languageCode[1] = tolower (arrc_attributes [attrCode].getCharacterAtPos(1));

            // Check for correct LanguageCode!
            int lc;
            for (lc=0; lc<DEF_iso639entries; ++lc)
            {
              if ((iso639table[lc][0] == languageCode[0]) && (iso639table[lc][1] == languageCode[1])) break;
            }
            if (lc == DEF_iso639entries)
            { // language not found!
              std::cerr << "\n\nWARNING: <language code=\"" << languageCode[0] << languageCode[1] << "\" /> MAY NOT be conform to ISO 639. Please make sure it is correct."<<std::endl<<std::endl;
            }

            tempString = str(format("'%c', '%c'") % languageCode[0] % languageCode[1]);

            fprintf (partFile_attributes, "{{%s}}", tempString.c_str());
            objChildLanguages++;
            firstElement = false;

            /// Also add this language to the intern language-table!
            std::string langFileName;
            langFileName = str(format("%s-list%02d.inc") % mstr_destinDirAndProjectPrefix % ui_languages);
            arrs_language [ui_languages].partFile = &save_fopen (langFileName.c_str(), "wt");
            langFileName = str(format("%sIsoAgLib::iVtObject_c* HUGE_MEM all_iVtObjects%d [] = {") % mstr_namespaceDeclarationBegin % ui_languages);
            fputs (langFileName.c_str(), arrs_language [ui_languages].partFile);
            arrs_language [ui_languages].code[0] = languageCode[0];
            arrs_language [ui_languages].code[1] = languageCode[1];
            arrs_language [ui_languages].code[2] = 0x00;
            arrs_language [ui_languages].count = 0;
            arrs_language [ui_languages].firstLine = true;

            // scan for language files and read into buffer
            std::list<std::string> l_languageFiles = scanLanguageFiles( arrs_language[ui_languages] );
            if( l_languageFiles.empty() ) {
              std::cerr << "    no language files found for lang " << arrs_language[ui_languages].code << std::endl;
              return false;
            } else {
              readLanguageFilesToBuffer( l_languageFiles, arrs_language[ui_languages] );
            }


            ui_languages++;
          }
        }
        if (firstElement == false)
          fprintf (partFile_attributes, "};\n");
      }
      // ### Print out RAWBITMAP byte array
      unsigned int deXactualWidth(0);
      unsigned int deXactualHeight(0);
      unsigned int stdRawBitmapBytes [3];
      unsigned int fixNr(0);
      unsigned int fixBitmapCounter(0);
      unsigned int objBitmapOptions(0);
      unsigned int deXwidth=0;
      if (objType == otPicturegraphic)
      {
        if (!(arrc_attributes [attrWidth].isGiven() && arrc_attributes [attrTransparency_colour].isGiven()))
        {
          std::cerr << "YOU NEED TO SPECIFY THE width= AND transparency_colour= ATTRIBUTES FOR THE <picturegraphic> OBJECT " << m_objName << "! STOPPING PARSER! bye."<<std::endl<<std::endl;
          return false;
        }

        ///////////////////////////////////////////////////////
        /// ### +BEGIN+ -- FIRST -- process "standard" bitmap
        c_Bitmap.resetLengths();

#if 0
        if (!checkForFileOrFile148 ("picturegraphic"))
          return false;
#endif

        if (picturegraphicoptionstoi (arrc_attributes [attrOptions].get().c_str()) == -1)
        {
          std::cerr << "\n\nFAILED ON picturegraphicoptionstoi!"<<std::endl<<std::endl;
          return false;
        }

        objBitmapOptions = picturegraphicoptionstoi (arrc_attributes [attrOptions].get().c_str());
        // set deXcolourDepth per reference, according to the found file and fileX attributes
        if (!openDecodePrintOut (l_stdBitmapPath, objBitmapOptions, deXcolourDepth))
        {
          std::cerr << "\n\nCOULDN'T openDecodePrintOut!"<<std::endl<<std::endl;
          return false;
        }

        // copy values from c_Bitmap somewhere to have them when Print'ing out the array afterwards...
        deXwidth = arrc_attributes [attrWidth].getIntValue();

        deXtransCol = colourtoi (arrc_attributes [attrTransparency_colour].get().c_str());
        if (deXtransCol == -1)
        {
          std::cerr << "\n\nFAILED ON colourtoi!"<<std::endl<<std::endl;
          return false;
        }

        deXactualWidth = c_Bitmap.getWidth();
        deXactualHeight = c_Bitmap.getHeight();

        for (int i=0; i<3; i++) stdRawBitmapBytes [i] = c_Bitmap.objRawBitmapBytes [i];
        /// ### +END+ -- FIRST -- process "standard" bitmap
        ///////////////////////////////////////////////////////

        //////////////////////////////////////////////////////
        /// ### +BEGIN+ -- SECOND -- process "fixed" bitmaps
        fixNr = 0;
        fixBitmapCounter = 0;
        unsigned int fiXactualWidth [maxFixedBitmaps];
        unsigned int fiXactualHeight [maxFixedBitmaps];
        unsigned int fixRawBitmapBytes [maxFixedBitmaps] [3];
        unsigned int fixBitmapOptions [maxFixedBitmaps];
        for (child = n->getFirstChild(); child != 0; child=child->getNextSibling())
        {
          if ( (child->getNodeType() == DOMNode::ELEMENT_NODE) && (0 == strcmp (XMLString::transcode(child->getNodeName()), otCompTable [otFixedBitmap]) )) {
            if (!getAttributesFromNode(child, false))
            {
              std::cerr << "\n\nCOULDN'T getAttributesFromNode!"<<std::endl<<std::endl;
              return false; // false: DON'T read name= and id=
            }
            // no defaultAndConvertAttributes() needed here...

            c_Bitmap.resetLengths();
            if (!checkForFileOrFile148 ("fixedbitmap"))
            {
              std::cerr << "\n\nCOULDN'T checkForFileOrFile148!"<<std::endl<<std::endl;
              return false;
            }

            fixBitmapOptions [fixNr] = objBitmapOptions & 0x3; // keep flashing/transparency information from <pictureobject>
            int dummyMaxDepth;
            if (!openDecodePrintOut (l_fixedBitmapPath, fixBitmapOptions[fixNr], dummyMaxDepth, fixNr)) return false;

            // copy values from c_Bitmap somewhere in a temp array that will be printed afterwards............
            //fiXtransCol [fixNr] = colourtoi ( arrc_attributes [attrTransparency_colour]);
            fiXactualWidth [fixNr] = c_Bitmap.getWidth();
            fiXactualHeight [fixNr] = c_Bitmap.getHeight();
            for (int i=0; i<3; i++)
              fixRawBitmapBytes [fixNr] [i] = c_Bitmap.objRawBitmapBytes [i];
            fixNr++; //is count then!!
          }
        }
        // have there been any Fixed Bitmaps?
        if (fixNr > 0)
        {
          // 1st: Print out array here now...
          fprintf (partFile_attributes, "const IsoAgLib::repeat_rawData_rawBytes_actWidth_actHeight_formatoptions_s iVtObject%s_aFixedBitmaps [] = {", m_objName.c_str());
          bool firstEntry=true;
          for (unsigned int i=0; i<fixNr; i++)
          {
            for (int actDepth=0; actDepth<3; actDepth++)
            {
              // was this depth generated for this special bitmap?
              if (fixRawBitmapBytes[i] [actDepth] > 0)
              {
                if (!firstEntry) fprintf (partFile_attributes, ", ");
                unsigned int options = (fixBitmapOptions[i] & 0x3) | ( (fixBitmapOptions[i] & (uint64_t(1)<<(2+actDepth))) ? (uint64_t(1)<<2) : 0 );
                fprintf (partFile_attributes, "{iVtObject%s_aRawBitmap%dFixed%d, %d, %d, %d, (%d << 6) | %d}", m_objName.c_str(), actDepth, i, fixRawBitmapBytes[i] [actDepth], fiXactualWidth[i], fiXactualHeight[i], actDepth, options);

                fixBitmapCounter++;
                firstEntry = false;
              }
            }
          }
          fprintf (partFile_attributes, "};\n");
        }
        /// ### +END+ -- SECOND -- process "fixed" bitmaps
        //////////////////////////////////////////////////////
      }

      // ### Print out OBJECTID(_X_Y) array
      bool xyNeeded = false;
      if (objHasArrayObjectXY) xyNeeded = true;
      unsigned int objChildObjects(0);
      if (objHasArrayObjectXY || objHasArrayObject)
      {
        // Process all Child-Elements
        if (!processChildElements (objChildObjects, n, xyNeeded))
        {
          std::cerr << "\n\nCOULDN'T processChildElements!"<<std::endl<<std::endl;
          return false;
        }
      }

      // ### Print out EVENT_MACRO array
      unsigned int objChildMacros(0);
      if (objHasArrayEventMacro)
      { // Process all macro-Elements
        if (!processMacroElements (objChildMacros, n))
        {
          std::cerr << "\n\nCOULDN'T processMacroElements!"<<std::endl<<std::endl;
          return false;
        }
      }

      // ### Print out MACRO_COMMAND array
      unsigned int objChildCommands(0);
      if (objHasArrayMacroCommand)
      {
        int nBytes = StringToInt(arrc_attributes [attrNumber_of_bytes].get());
        if (nBytes <= 0) {
          std::cerr << "The macro's number of bytes to follow should be positive, but it isn't." << std::endl;
          return false;
        }
        fprintf (partFile_attributes, "const uint8_t iVtObject%s_aMacro_Commands [%d] = {", m_objName.c_str(), nBytes);

        // Process all Child-Elements
        bool isStillMissingCommand = true;
        for (child = n->getFirstChild(); child != 0; child=child->getNextSibling())
        {
          bool const isReallyCommandElement = child->getNodeType() == DOMNode::ELEMENT_NODE;
          if (isReallyCommandElement)  // May need to adjust this to be sure the child is really a command element and not something else to enforce integrity
          {
            char *command_name = XMLString::transcode(child->getNodeName());
            unsigned int commandTypeChild = commandIsType (command_name);
            std::string commandMessage;
            bool const isNotSet = !setCommandElement(commandTypeChild, child/*, pAttributes*/, n, objChildCommands, commandMessage);
            if (isNotSet)
            {
              std::cerr << "\n\nsetCommandElement WAS NOT SET!"<<std::endl<<std::endl;
              return false;
            }
            if (!isStillMissingCommand)
              fprintf (partFile_attributes, ", ");
            fprintf (partFile_attributes, "%s", commandMessage.c_str());
            isStillMissingCommand = false;
          }
        }
        if (isStillMissingCommand) {
          std::cerr << "The macro should have at least one command, but it hasn't." << std::endl;
          return false;
        }

        fprintf (partFile_attributes, "};\n");
      } // End of if (objHasArrayMacroCommand)
      unsigned int objChildPoints(0);
      // ### Print out point array for a Polygon Object
      if (objHasArrayPoints)
      {
        if ( !processPointElements(objChildPoints, n) )
        {
          std::cerr << "\n\nCOULDN'T processPointElements" << std::endl;
          return false;
        }
      }

      // ###################################################
      // ### Print out definition, values and init-calls ###
      // ###################################################
      std::string pc_postfix;
      /// MultiLanguage-Support: See which -list.inc file to write object to!
      if (arrc_attributes [attrLanguage].isGiven() && (arrc_attributes [attrLanguage].getLength() > 0))
      { // write to special language-list file if language is correct!
        // search language in list-file-array
        unsigned int curLang(0);
        for (; curLang<ui_languages; curLang++)
        {
          if (arrc_attributes [attrLanguage].get().compare(arrs_language[curLang].code) == 0)
            break; // found the language! => index is "curLang"
        }
        if (curLang==ui_languages)
        { // language not found!
          std::cerr << "\n\nYOU NEED TO SPECIFY A VALID LANGUAGE which you have also defined in the <workingset> object ("<<arrc_attributes [attrLanguage].get()<<" is not!)! STOPPING PARSER! bye."<<std::endl<<std::endl;
          return false;
        }
        //FILE *fileList = arrs_language[curLang].partFile;
        //bool *pb_firstLine = &arrs_language[curLang].firstLine;

        arrs_language[curLang].count++; // and do NOT count the normal list up!
        if (curLang > 0)
        {
          pc_postfix = str(format("_%s%d") % mstr_langPrefix % curLang);
          // Note: Someone said the langPrefix is wrong here, but this probably is just a merge error (where langPrefix was set to the defines-prefix!)
        }
        if ( checkForAllowedExecution() )
        {
          std::string str_objClassName = std::string("&iVtObject") + m_objName + pc_postfix;
          // first check if found language was inserted before into the map
          std::map<int32_t, ObjListEntry>::iterator mit_entry = map_objNameAndID.find (curLang);

          if (mit_entry != map_objNameAndID.end()) // success! language entry found
          { // only insert an further entry in the found language map
            //store the objectId - objectName pair in the appropriate language map
            mit_entry->second.insert (std::pair<uint16_t, std::string>(objID, str_objClassName));
          }
          else
          { // add a new map entry for a further language which includes the first entry in that language specific map
            ObjListEntry listEntry;
            listEntry.insert (std::pair<uint16_t, std::string>(objID, str_objClassName));
            map_objNameAndID.insert (std::pair<int32_t, ObjListEntry>(curLang, listEntry));
          }
        }



        // check whether the objecttype could have a translateable value - otherwise
        // we don't need to lookup in the parsed buffer
        bool b_hasTranslateableValue;
        switch( objType ) {
          case otOutputstring:
          case otInputstring:
          case otStringvariable:
            b_hasTranslateableValue = true;
            break;
          default:
            b_hasTranslateableValue = false;
            break;
        }


        /// Try to retrieve   value='...'   from language-value-file
        if ( b_hasTranslateableValue && ( arrs_language[curLang].valueBuffer != NULL) )
        {
          //std::cout << "searching language file for ["<<objName<<"]."<<std::endl;
          bool b_foundLanguageValue = false;
          char* bufferCur = arrs_language[curLang].valueBuffer;
          char* bufferEnd = bufferCur + arrs_language[curLang].valueBufferLen;
          std::string pc_foundValue;
          int numWideChars;
          char MBBuffer[ maxTagLength ];

          if (arrs_language[curLang].unicode)  // The first read of a UCS-2 has a BOM there
            bufferCur += 2;  // move me forward, start me at the beginning of the tag 

          while (bufferCur < bufferEnd)
          {
            // check this line (\n and \r has been previously converted to 0x00
            std::string pc_id;
            char* firstChar=bufferCur;
            while (*firstChar == ' ')
              firstChar++;
            if ( (*firstChar == 0x00)
                   || (strstr (bufferCur, "//") && (strstr (bufferCur, "//") == firstChar))
               )
            { // ignore line
            }
            else
            {
              if (arrs_language[curLang].unicode)  // This is the UCS-2   vtu file -  Unicode version
              {
                numWideChars = (bufferEnd - bufferCur) / 2;
                UCS2* commaLoc = findWideChar ((const UCS2*)bufferCur, 0x2C, numWideChars );
                if (commaLoc)   // if not valid, we are done looking..
                {
                  int charCnt = ((char*)commaLoc - bufferCur)/2;
                  wcstombs(MBBuffer, (const wchar_t*)bufferCur, charCnt );
                  MBBuffer[charCnt] = 0;  // theres our identifier..
                  pc_id = MBBuffer;
                  UCS2* firstQuote = findWideChar ((const UCS2*)commaLoc, 0x0022, numWideChars );
                  if (firstQuote)
                  {
                    UCS2* nextQuote = findWideChar ((const UCS2*)(firstQuote+1), 0x0022, numWideChars );
                    if (nextQuote)
                    { // extract text from in between

                      pc_foundValue.clear();

                      pc_foundValue.push_back((char)0xff); // Prepend the char string with the BOM
                      pc_foundValue.push_back((char)0xfe);

                      for (char* it=(char*)(firstQuote+1); it < (char*)nextQuote; it++)
                      {   
                        // We MUST handle a NULL (zero, 0) with a   '\x00' 
                        //   in addition, we may be considered paranoid, but that doesn't mean 
                        //      these "special" control chars are NOT out to get us!  ;-)

                        int incoming = *it & 0xff; // mask sign extension
                        int upDgt, lowDgt;
                        char pushChar;
                        pc_foundValue.push_back(0x5c);	// Prepend with '\x'  
                        pc_foundValue.push_back('x');

                        upDgt = incoming/16;
                        lowDgt = incoming%16;

                        // need chars 0 to 9 (0x30 to 0x39)  OR   a to f (0x61 to 0x66)
                        pushChar = (upDgt < 10) ? (upDgt+'0') : (upDgt-10+'a');
                        pc_foundValue.push_back(pushChar);
                        pushChar = (lowDgt < 10) ? (lowDgt+'0') : (lowDgt-10+'a');
                        pc_foundValue.push_back(pushChar);
                      } //  The for loop of all the chars
                    }
                    else
                    { // no closing quote
                      std::cerr << "No CLOSING quote in the " << arrs_language [curLang].code << ".vtu language file!"<<std::endl;
                      return false;
                    }
                  }
                  else
                  { // no opening quote
                    std::cerr << "No OPENING quote in the " << arrs_language [curLang].code << ".vtu language file!"<<std::endl;
                    return false;
                  }
                  /// Is this the tag we are looking for?   If so, we can break here
                  if ((pc_id).compare(m_objName) == 0)
                  { // set value and break
                    b_foundLanguageValue= true;
                    break;
                  }
                }
                else
                { // no comma found, although it was not a commentary line :(
                  std::cerr << "No COMMA in a non-comment line in the " << arrs_language [curLang].code << ".vtu language file!";
                  return false;
                }
              }  // end of working on the UCS-2  vtu file tag/string
              else  
              {  //  For *.vtl files we can use those standard i/o calls ------------ 
                char* comma = strchr (bufferCur, ',');
                if (comma)
                {
                  *comma = 0x00;
                  pc_id = bufferCur;
                  *comma = ','; // revert comma-separator
                  comma++; // goto next character following the comma
                  char* firstQuote = strchr (comma, '"');
                  if (firstQuote)
                  {
                    char* nextQuote = strrchr (firstQuote+1, '"');
                    if (nextQuote)
                    { // extract text from in between
                      pc_foundValue.clear();
                      for (char* it=firstQuote+1; it < nextQuote; it++)
                      { // extract byte by byte
                        pc_foundValue.push_back (*it);
                      }
                    }
                    else
                    { // no closing quote
                      std::cerr << "No CLOSING quote in the " << arrs_language [curLang].code << ".vtl language file!"<<std::endl;
                      return false;
                    }
                  }
                  else
                  { // no opening quote
                    std::cerr << "No OPENING quote in the " << arrs_language [curLang].code << ".vtl language file!"<<std::endl;
                    return false;
                  }
                  /// BREAK HERE TO WATCH GOTTEN IN THE DIFFERENT CASES!
                  if ((pc_id).compare(m_objName) == 0)
                  { // set value and break
                    //std::cout << "found language value for [" << objName << "]."<<std::endl;
                    b_foundLanguageValue= true;
                    break;
                  }
                }
                else
                { // no comma found, although it was not a commentary line :(
                  std::cerr << "No COMMA in a non-comment line in the " << arrs_language [curLang].code << ".vtl language file!";
                  return false;
                }
              }   // END OF  - 8bit wide character manipulation
            }

            // advance to next line --  of-course a wideChar and the byteSize is available
            if (arrs_language[curLang].unicode)
            {
              UCS2* newStart = findWideChar ((const UCS2*)bufferCur, 0x0000, numWideChars );
              if (newStart) 
              {
                newStart += 2;  // theres another 0x0000 out there...
                bufferCur = (char*)newStart;
              } 
              else
              {
                bufferCur = bufferEnd + 1;
              }
            }
            else
            {
              int lineLen = strlen (bufferCur)+1; // include terminating 0x00
              bufferCur += lineLen;
            } 
          }  // END OF -    while (bufferCur < bufferEnd)

          if (b_foundLanguageValue)
          {
            /// Do we have conflicting 'value='s now? Just put out a warning (if in verbose-mode)!
            if (arrc_attributes [attrValue].isGiven())
              if (mb_verbose) std::cout <<"Overriding value & length from ["<< m_objName <<"]!!" << std::endl;

            // anyway, override attrValue and clear length (so it gets auto-calculated below!)
            arrc_attributes [attrValue].set( pc_foundValue );
            arrc_attributes [attrLength].clear();
          } else {
            if( mb_pedanticMode ) {
              clean_exit( str(format("could not find a translated value for language %s, object %s (%d)\n") % arrs_language[curLang].code % m_objName % objID ).c_str() );
              exit( -1 );
            }
          }
        }
      }
      else
      {
        if ( checkForAllowedExecution() )
        {
          // was the standard map already created?
          // standard map get the index -1 to differ between "real" language map and normal object map
          std::map<int32_t, ObjListEntry>::iterator mit_entry = map_objNameAndID.find (0xFFFFFFFF);

          std::string str_objClassName = std::string("&iVtObject") + m_objName + pc_postfix;

          if (mit_entry != map_objNameAndID.end())
          {
            mit_entry->second.insert (std::pair<uint16_t, std::string>(objID, str_objClassName));
          }
          else
          {
            ObjListEntry listEntry;
            listEntry.insert (std::pair<uint16_t, std::string>(objID, str_objClassName));
            map_objNameAndID.insert (std::pair<int32_t, ObjListEntry>(0xFFFFFFFF, listEntry));
          }
        }
      }
      // if special parsing is active and the object type is greater than maxObjectType
      // the output to the files must be done separately
      if ( pc_specialParsingPropTag && (objType >= maxObjectTypes) )
      { // need to know the object id in special parsing when data is written to files
        pc_specialParsingPropTag->setObjID (objID);
        bool b_outputOK = pc_specialParsingPropTag->outputData2FilesPiecewise ( arrc_attributes, this);
        if ( !b_outputOK )
        {
          std::cerr << "Error in outputData2FilesPiecewise() from object <" << m_nodeName << "> '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
          return false;
        }
      }
      else if (objType < maxObjectTypes)
      {
        fprintf (partFile_variables, "IsoAgLib::iVtObject%s_c iVtObject%s%s;\n", otClassnameTable [objType], m_objName.c_str(), pc_postfix.c_str());
        fprintf (partFile_variables_extern,  "extern IsoAgLib::iVtObject%s_c iVtObject%s%s;\n", otClassnameTable [objType], m_objName.c_str(), pc_postfix.c_str());
        fprintf (partFile_attributes, "const IsoAgLib::iVtObject_c::iVtObject%s_s iVtObject%s%s_sROM = {%d", otClassnameTable [objType], m_objName.c_str(), pc_postfix.c_str(), objID);

        fprintf (partFile_attributes_extern, "extern const IsoAgLib::iVtObject_c::iVtObject%s_s iVtObject%s%s_sROM;\n", otClassnameTable [objType], m_objName.c_str(), pc_postfix.c_str());

        fprintf (partFile_defines, "static const unsigned int %s%s%s = %d;\n", mstr_definesPrefix.c_str(), m_objName.c_str(), pc_postfix.c_str(), objID);
      }

      /// Add explicit Button/Key includement
      if (arrc_attributes [attrInButton].isGiven())
      {
        fprintf (partFile_functions_origin, "  iVtObject%s%s.setOriginBTN (&iVtObject%s);\n", m_objName.c_str(), pc_postfix.c_str(), arrc_attributes [attrInButton].get().c_str());
      }
      if (arrc_attributes [attrInKey].isGiven())
      {
        signed int resultat = booltoi (arrc_attributes [attrInKey].get().c_str());
        if (resultat == -1)
        {
          std::cerr << "Error in booltoi() from object <" << m_nodeName << "> '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
          return false;
        }
        fprintf (partFile_functions_origin, "  iVtObject%s%s.setOriginSKM (%s);\n", m_objName.c_str(), pc_postfix.c_str(), resultat ? "true":"false");
      }
      if ((arrc_attributes [attrVariable_reference].get().compare("NULL") != 0) && (strncmp (arrc_attributes [attrVariable_reference].get().c_str(), "&iVtObject", strlen ("&iVtObject")) != 0))
      { // != 0 means an object reference is given, so add the "&iVtObject" prefix!!
        arrc_attributes [attrVariable_reference].set( getObjectReference(attrVariable_reference) );
      }
      if ((arrc_attributes [attrTarget_value_variable_reference].get().compare("NULL") != 0) && (strncmp (arrc_attributes [attrTarget_value_variable_reference].get().c_str(), "&iVtObject", strlen ("&iVtObject")) != 0))
      { // != 0 means an object reference is given, so add the "&iVtObject" prefix!!
        arrc_attributes [attrTarget_value_variable_reference].set ( getObjectReference(attrTarget_value_variable_reference) );
      }

      //! @todo ON REQUEST To be enabled when handling cases where only IDs are used in XMLs. Not 100% supported now, even if the
      //! following "convertIdReferencesToNameReferences();" call wouldn't be commented out!
      //convertIdReferencesToNameReferences();
      // ###########################################
      // ### Print out inidivual object stuff... ###
      // ###########################################
      switch (objType)
      {
        case otWorkingset:
        {
          signed int retSelectable = booltoi ( arrc_attributes [attrSelectable].get().c_str());
          if (retSelectable == -1)
          {
            std::cerr << "Error in booltoi() from object <" << m_nodeName << "> '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          if (!arrc_attributes [attrActive_mask].isGiven() && (retSelectable == 1))
          {
            std::cerr << "YOU NEED TO SPECIFY THE active_mask= ATTRIBUTE FOR THE <workingset> OBJECT (if selectable='yes'!)! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          if (colourtoi (arrc_attributes [attrBackground_colour].get().c_str()) == -1)
            return false;

          fprintf (partFile_attributes, ", %d, %d, %s", colourtoi (arrc_attributes [attrBackground_colour].get().c_str()), retSelectable, arrc_attributes [attrActive_mask].getObjectReference().c_str());

          if (!vt2iso_c::sb_WSFound)
          {
            vt2iso_c::sb_WSFound = true;
            ui16_WSObjID = objID;
          }
          break;
        }
        case otDatamask:
          if (colourtoi (arrc_attributes [attrBackground_colour].get().c_str()) == -1)
            return false;

          fprintf (partFile_attributes, ", %d, %s", colourtoi (arrc_attributes [attrBackground_colour].get().c_str()), getObjectReference(attrSoft_key_mask).c_str());
          break;

        case otAlarmmask:
        {
          signed long int retPrio = prioritytoi( arrc_attributes [attrPriority].get().c_str());
          signed long int retSignal = acousticsignaltoi ( arrc_attributes [attrAcoustic_signal].get().c_str());
          if ((retPrio == -1) || (retSignal == -1))
          {
            if (retPrio == -1)
              std::cerr << "Error in prioritytoi()  from object <" << m_nodeName << "> '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            else
              std::cerr << "Error in acousticsignaltoi()  from object <" << m_nodeName << "> '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }

          if (colourtoi ( arrc_attributes [attrBackground_colour].get().c_str()) == -1)
            return false;

          fprintf (partFile_attributes, ", %d, %s, %d, %d", colourtoi ( arrc_attributes [attrBackground_colour].get().c_str()), getObjectReference(attrSoft_key_mask).c_str(), (unsigned int)retPrio, (unsigned int)retSignal);
          break;
        }

        case otContainer:
        {
          if (!(arrc_attributes [attrWidth].isGiven() && arrc_attributes [attrHeight].isGiven()))
          {
            std::cerr << "YOU NEED TO SPECIFY THE width= AND height= ATTRIBUTES FOR THE <container> OBJECT '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          signed int retHidden = booltoi ( arrc_attributes [attrHidden].get().c_str());
          if (retHidden == -1)
          {
            std::cerr << "Error in booltoi()  from object <" << m_nodeName << "> '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }

          fprintf (partFile_attributes, ", %s, %s, %d", arrc_attributes [attrWidth].get().c_str(), arrc_attributes [attrHeight].get().c_str(), (unsigned int)retHidden);
          break;
        }

        case otSoftkeymask:
          if (colourtoi ( arrc_attributes [attrBackground_colour].get().c_str()) == -1)
            return false;

          fprintf (partFile_attributes, ", %d", colourtoi ( arrc_attributes [attrBackground_colour].get().c_str()));

          if (objChildObjects > 6)
          {
            if(!mb_silentMode)
              std::cout << "THE <softkeymask> OBJECT '" << m_objName << "' has more than 6 SoftKeys! Please be aware that maybe not all VTs handle SoftKeyMasks with more than 6 Softkeys as they don't have to!!!!"<<std::endl<<std::endl;
            b_hasMoreThan6SoftKeys = true;
          }
          break;

        case otKey:
          if (!arrc_attributes [attrKey_code].isGiven())
            getKeyCode ();
          if (colourtoi ( arrc_attributes [attrBackground_colour].get().c_str()) == -1)
            return false;

          fprintf (partFile_attributes, ", %d, %s", colourtoi ( arrc_attributes [attrBackground_colour].get().c_str()), arrc_attributes [attrKey_code].get().c_str());
          fprintf (partFile_defines, "static const unsigned int vtKeyCode%s = %d;\n", m_objName.c_str(),  arrc_attributes [attrKey_code].getIntValue()); // like in otButton
          break;

        case otButton:
          if (!arrc_attributes [attrKey_code].isGiven())
            getKeyCode ();
          if (!(arrc_attributes [attrWidth].isGiven() && arrc_attributes [attrHeight].isGiven()))
          {
            std::cerr << "YOU NEED TO SPECIFY THE width= AND height= ATTRIBUTES FOR THE <key> OBJECT '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          if (colourtoi ( arrc_attributes [attrBackground_colour].get().c_str()) == -1)
            return false;
          if (colourtoi ( arrc_attributes [attrBorder_colour].get().c_str()) == -1)
            return false;
          // we set version 4 if no version set, else we use information from vtp-file.
          if (buttonoptiontoi (arrc_attributes [attrOptions].get().c_str(), mi_objectPoolVersion) == -1)
            return false;
          fprintf (partFile_attributes, ", %s, %s, %d, %d, %s, %d", arrc_attributes [attrWidth].get().c_str(), arrc_attributes [attrHeight].get().c_str(), colourtoi ( arrc_attributes [attrBackground_colour].get().c_str()), colourtoi ( arrc_attributes [attrBorder_colour].get().c_str()), arrc_attributes [attrKey_code].get().c_str(), buttonoptiontoi (arrc_attributes [attrOptions].get().c_str(), mi_objectPoolVersion));
          fprintf (partFile_defines, "static const unsigned int vtKeyCode%s =  %d;\n", m_objName.c_str(), arrc_attributes  [attrKey_code].getIntValue()); // like in otKey
          break;

        case otInputboolean:
        {
          if (!(arrc_attributes [attrWidth].isGiven() && arrc_attributes [attrForeground_colour].isGiven()))
          {
            std::cerr << "YOU NEED TO SPECIFY THE width= AND foreground_colour= ATTRIBUTES FOR THE <inputboolean> OBJECT '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          arrc_attributes [attrValue].setIfNotGiven ("0");

          signed int retEnabled = booltoi ( arrc_attributes [attrEnabled].get().c_str());
          if (retEnabled == -1)
          {
            std::cerr << "Error in booltoi() from object <" << m_nodeName << "> '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          if (colourtoi ( arrc_attributes [attrBackground_colour].get().c_str()) == -1)
            return false;

          fprintf (partFile_attributes, ", %d, %s, %s, %s, %s, %d", colourtoi ( arrc_attributes [attrBackground_colour].get().c_str()), arrc_attributes [attrWidth].get().c_str(), getObjectReference (attrForeground_colour).c_str(), arrc_attributes [attrVariable_reference].get().c_str(), arrc_attributes [attrValue].get().c_str(), (unsigned int)retEnabled);
          break;
        }

        case otInputstring:
        {
          if (!arrc_attributes [attrValue].isGiven())
          { // no value given
            if (!(arrc_attributes [attrWidth].isGiven() && arrc_attributes [attrHeight].isGiven() && arrc_attributes [attrFont_attributes].isGiven() && arrc_attributes [attrLength].isGiven()))
            {
              std::cerr << "YOU NEED TO SPECIFY THE width= AND height= AND font_attributes= AND length= ATTRIBUTES FOR THE <inputstring> OBJECT '" << m_objName << "' IF NO VALUE IS GIVEN! STOPPING PARSER! bye."<<std::endl<<std::endl;
              return false;
            }
          /// @todo ON REQUEST: MAYBE WARN/FAIL HERE WHEN NO LANGUAGE IS GIVEN BUT NO ENTRY IS DEFINED????????
          /*
            if (arrs_language[i].valueBuffer == NULL)
            { // open failed.
            std::cout << "\n\nLanguage file for code=\"" << arrs_language[i].code[0] << arrs_language[i].code[1] << "\" in object ["<< objName <<"] not found! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }*/
            arrc_attributes [attrValue].set( "NULL" );
          }
          else
          { // value given
            signed int ret = strlenUnescaped (arrc_attributes [attrValue].get().c_str());
            if (ret == -1)
            {
              std::cerr << "Error in strlenUnescaped() from object <" << m_nodeName << "> '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
              return false;
            }
            //auto-calculate string length
            if (!arrc_attributes [attrLength].isGiven())
            {
              arrc_attributes [attrLength].set(str(format("%d") % ret));
            }
            if (!(arrc_attributes [attrWidth].isGiven() && arrc_attributes [attrHeight].isGiven() && arrc_attributes [attrFont_attributes].isGiven() && arrc_attributes [attrEnabled].isGiven()))
            {
              std::cerr << "YOU NEED TO SPECIFY THE width= AND height= AND font_attributes= AND enabled = ATTRIBUTES FOR THE <inputstring> OBJECT '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
              return false;
            }
            if (!copyWithQuoteAndLength (tempString, escapedString(arrc_attributes [attrValue].get()).c_str(), arrc_attributes [attrLength].getIntValue()))
            {
              return false;
            }
            arrc_attributes [attrValue].set(tempString);
          }
          signed int retEnabled = booltoi( arrc_attributes [attrEnabled].get().c_str());
          signed int retHorJustification = horizontaljustificationtoi (arrc_attributes [attrHorizontal_justification].get().c_str());
          signed int retVertJustification = verticaljustificationtoi (arrc_attributes [attrVertical_justification].get().c_str(), mi_objectPoolVersion); // we set version 4 if no version set, else we use information from vtp-file.
          signed int retJustification = retHorJustification | (retVertJustification << 2);
          if (retEnabled == -1) {
            std::cerr << "Error in booltoi() from object <" << m_nodeName << "> '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          if (retHorJustification == -1) {
            std::cerr << "Error in horizontaljustificationtoi() from object <" << m_nodeName << "> '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          if (retVertJustification == -1) {
            std::cerr << "Error in verticaljustificationtoi() from object <" << m_nodeName << "> '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          if (colourtoi ( arrc_attributes [attrBackground_colour].get().c_str()) == -1)
            return false;
          if (stringoptionstoi (arrc_attributes [attrOptions].get().c_str(), mi_objectPoolVersion) == -1)
            return false;

          fprintf (partFile_attributes, ", %s, %s, %d, %s, %s, %d, %s, %d, %s, %s, %d", arrc_attributes [attrWidth].get().c_str(), arrc_attributes [attrHeight].get().c_str(), colourtoi ( arrc_attributes [attrBackground_colour].get().c_str()), getObjectReference (attrFont_attributes).c_str(), getObjectReference (attrInput_attributes).c_str(), stringoptionstoi (arrc_attributes [attrOptions].get().c_str(), mi_objectPoolVersion), arrc_attributes [attrVariable_reference].get().c_str(), (unsigned int)retJustification, arrc_attributes [attrLength].get().c_str(), arrc_attributes [attrValue].get().c_str(), (unsigned int)retEnabled );
          break;
        }

        case otInputnumber:
        {
          if (!(arrc_attributes [attrWidth].isGiven() && arrc_attributes [attrHeight].isGiven() && arrc_attributes [attrFont_attributes].isGiven() && arrc_attributes [attrMin_value].isGiven() && arrc_attributes [attrMax_value].isGiven()
                && arrc_attributes [attrOffset].isGiven() && arrc_attributes [attrScale].isGiven() && arrc_attributes [attrNumber_of_decimals].isGiven() && arrc_attributes [attrFormat].isGiven()))
          {
            std::cerr << "YOU NEED TO SPECIFY THE width= AND height= AND font_attributes= AND min_value= AND max_value= "
                      << "AND offset= AND scale= AND number_of_decimals= AND format= ATTRIBUTES FOR THE <inputnumber> OBJECT "
                      << m_objName << "! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          arrc_attributes [attrValue].setIfNotGiven ("0");
          if (colourtoi ( arrc_attributes [attrBackground_colour].get().c_str()) == -1)
            return false;
          if (inputnumberoptionstoi (arrc_attributes [attrOptions].get().c_str(), mi_objectPoolVersion) == -1)
            return false;

          fprintf (partFile_attributes, ", %s, %s, %d, %s, %d, %s, %sUL, %sUL, %sUL", arrc_attributes [attrWidth].get().c_str(), arrc_attributes [attrHeight].get().c_str(), colourtoi ( arrc_attributes [attrBackground_colour].get().c_str()), getObjectReference (attrFont_attributes).c_str(), inputnumberoptionstoi (arrc_attributes [attrOptions].get().c_str(), mi_objectPoolVersion), arrc_attributes [attrVariable_reference].get().c_str(), arrc_attributes [attrValue].get().c_str(), arrc_attributes [attrMin_value].get().c_str(), arrc_attributes [attrMax_value].get().c_str());

          if ( arrc_attributes [attrOffset].get().find("L") != std::string::npos ) // != NULL
          { // contains already a number type specifier
            fprintf (partFile_attributes, ", %s", arrc_attributes [attrOffset].get().c_str());
          }
          else
          { // place "L" for type specifier
            fprintf (partFile_attributes, ", %sL", arrc_attributes [attrOffset].get().c_str());
          }
          // note that the enabled=".." attribute is converted into the
          // inputobject_options attribute in defaultAndConvertAttributes(..) already.
          signed int retInputOption = inputobjectoptiontoi (arrc_attributes [attrInputObjectOptions].get().c_str(), mi_objectPoolVersion);
          signed int retFormat = formattoi (arrc_attributes [attrFormat].get().c_str());
          signed int retHorJust = horizontaljustificationtoi (arrc_attributes [attrHorizontal_justification].get().c_str());
          signed int retVertJust = verticaljustificationtoi (arrc_attributes [attrVertical_justification].get().c_str(), mi_objectPoolVersion); // we set version 4 if no version set, else we use information from vtp-file.
          signed int retJust = retHorJust | (retVertJust << 2);
          if (retInputOption == -1) {
            std::cerr << "Error in inputobjectoptiontoi() from object <" << m_nodeName << "> '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          if (retFormat == -1) {
            std::cerr << "Error in formattoi() from object <" << m_nodeName << "> '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          if (retHorJust == -1) {
            std::cerr << "Error in horizontaljustificationtoi() from object <" << m_nodeName << "> '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          if (retVertJust == -1) {
            std::cerr << "Error in verticaljustificationtoi() from object <" << m_nodeName << "> '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }

          fprintf (partFile_attributes, ", %s, %s, %d, %d, %d", formatAsFloat(arrc_attributes [attrScale].get()).c_str(), arrc_attributes [attrNumber_of_decimals].get().c_str(), (unsigned int)retFormat, (unsigned int)retJust, (unsigned int)retInputOption);
          break;
        }

        case otInputlist:
        {
          if (!(arrc_attributes [attrWidth].isGiven() && arrc_attributes [attrHeight].isGiven()))
          {
            std::cerr << "YOU NEED TO SPECIFY THE width= AND height= ATTRIBUTES FOR THE <inputlist> OBJECT '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          arrc_attributes [attrValue].setIfNotGiven ("0");

          // note that the enabled=".." attribute is converted into the
          // options attribute in defaultAndConvertAttributes(..) already.
          signed int retOptions = inputobjectoptiontoi (arrc_attributes [attrOptions].get().c_str(), mi_objectPoolVersion);
          if (retOptions == -1)
          {
            std::cerr << "Error in itoinputobjectoptions() from object <" << m_nodeName << "> '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }

          fprintf (partFile_attributes, ", %s, %s, %s, %s, %d", arrc_attributes  [attrWidth].get().c_str(), arrc_attributes [attrHeight].get().c_str(), arrc_attributes  [attrVariable_reference].get().c_str(), arrc_attributes [attrValue].get().c_str(), retOptions);
          break;
        }

        case otOutputlist:
        {
          if (mi_objectPoolVersion < 4)
          {
            std::cerr << "YOU ARE NOT ALLOWED TO USE OUTPUTLISTS IN A POOL PRIOR TO VERSION 4! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          if (!(arrc_attributes [attrWidth].isGiven() && arrc_attributes [attrHeight].isGiven()))
          {
            std::cerr << "YOU NEED TO SPECIFY THE width= AND height= ATTRIBUTES FOR THE <outputlist> OBJECT '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          arrc_attributes [attrValue].setIfNotGiven ("0");

          fprintf (partFile_attributes, ", %s, %s, %s, %s", arrc_attributes [attrWidth].get().c_str(), arrc_attributes [attrHeight].get().c_str(), arrc_attributes [attrVariable_reference].get().c_str(), arrc_attributes [attrValue].get().c_str());
          break;
        }

        case otOutputstring:
        {
          if (!arrc_attributes [attrValue].isGiven())
          {
            // Variable Reference
            if (!(arrc_attributes [attrWidth].isGiven() && arrc_attributes [attrHeight].isGiven() && arrc_attributes [attrFont_attributes].isGiven() && arrc_attributes [attrLength].isGiven()))
            {
              std::cerr << "YOU NEED TO SPECIFY THE width= AND height= AND font_attributes= AND length= ATTRIBUTES FOR THE <outputstring> OBJECT  from object '" << m_objName << "' WHEN VALUE IS SET BY REFERENCE! STOPPING PARSER! bye."<<std::endl<<std::endl;
              return false;
            }
            arrc_attributes [attrValue].set( "NULL" );
          }
          else
          {
            // Direct Value
            if (!arrc_attributes [attrLength].isGiven())
            {
              signed int ret = strlenUnescaped (arrc_attributes [attrValue].get().c_str());
              if (ret == -1)
              {
                std::cout << "Error in strlenUnescaped() from object <" << m_nodeName << "> '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
                return false;
              }
              // Auto-calculate Length-field
              arrc_attributes [attrLength].set(str(format("%d") % ret));
            }
            if (!(arrc_attributes [attrWidth].isGiven() && arrc_attributes [attrHeight].isGiven() && arrc_attributes [attrFont_attributes].isGiven() && arrc_attributes [attrLength].isGiven()))
            {
              std::cout << "YOU NEED TO SPECIFY THE width= AND height= AND font_attributes= AND length= ATTRIBUTES FOR THE <outputstring> OBJECT '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
              return false;
            }
            if (!copyWithQuoteAndLength (tempString, escapedString(arrc_attributes [attrValue].get()).c_str(), arrc_attributes [attrLength].getIntValue()))
            {
              return false;
            }
            arrc_attributes [attrValue].set( tempString );
          }
          signed int retHorJust = horizontaljustificationtoi (arrc_attributes [attrHorizontal_justification].get().c_str());
          if (retHorJust == -1)
          {
            std::cout << "Error in horizontaljustificationtoi() from object <" << m_nodeName << "> '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          signed int retVertJust = verticaljustificationtoi (arrc_attributes [attrVertical_justification].get().c_str(), mi_objectPoolVersion); // we set version 4 if no version set, else we use information from vtp-file.
          if (retVertJust == -1)
          {
            std::cout << "Error in verticaljustificationtoi() from object <" << m_nodeName << "> '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          signed int retJust = retHorJust;
          retJust |= retVertJust << 2;
          if (colourtoi ( arrc_attributes [attrBackground_colour].get().c_str()) == -1)
            return false;
          if (stringoptionstoi (arrc_attributes [attrOptions].get().c_str(), mi_objectPoolVersion) == -1)
            return false;

          fprintf (partFile_attributes, ", %s, %s, %d, %s, %d, %s, %d, %s, %s", arrc_attributes [attrWidth].get().c_str(), arrc_attributes [attrHeight].get().c_str(), colourtoi ( arrc_attributes [attrBackground_colour].get().c_str()), getObjectReference (attrFont_attributes).c_str(), stringoptionstoi (arrc_attributes [attrOptions].get().c_str(), mi_objectPoolVersion), arrc_attributes [attrVariable_reference].get().c_str(), (unsigned int)retJust, arrc_attributes [attrLength].get().c_str(), arrc_attributes [attrValue].get().c_str());
          break;
        }

        case otOutputnumber:
        {
          if (!(arrc_attributes [attrWidth].isGiven() && arrc_attributes [attrHeight].isGiven() && arrc_attributes [attrFont_attributes].isGiven() && arrc_attributes [attrOffset].isGiven() && arrc_attributes [attrScale].isGiven()
                && arrc_attributes [attrNumber_of_decimals].isGiven() && arrc_attributes [attrFormat].isGiven()))
          {
            std::cout << "YOU NEED TO SPECIFY THE width= AND height= AND font_attributes= AND offset= AND scale= AND number_of_decimals= AND format= ATTRIBUTES FOR THE <outputnumber> OBJECT '" << m_objName << "''! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          arrc_attributes [attrValue].setIfNotGiven ("0");
          if (colourtoi ( arrc_attributes [attrBackground_colour].get().c_str()) == -1)
            return false;
          if (inputnumberoptionstoi (arrc_attributes [attrOptions].get().c_str(), mi_objectPoolVersion) == -1)
            return false;
          fprintf (partFile_attributes, ", %s, %s, %d, %s, %d, %s, %sUL", arrc_attributes [attrWidth].get().c_str(), arrc_attributes [attrHeight].get().c_str(), colourtoi ( arrc_attributes [attrBackground_colour].get().c_str()), getObjectReference(attrFont_attributes).c_str(), inputnumberoptionstoi (arrc_attributes [attrOptions].get().c_str(), mi_objectPoolVersion), arrc_attributes [attrVariable_reference].get().c_str(), arrc_attributes [attrValue].get().c_str());
          if (arrc_attributes [attrOffset].get().find("L") != std::string::npos ) // not found
          { // offset has already type indication -> don't add additional "L"
            fprintf (partFile_attributes, ", %s", arrc_attributes [attrOffset].get().c_str());
          }
          else
          {
            fprintf (partFile_attributes, ", %sL", arrc_attributes [attrOffset].get().c_str());
          }
          signed int retFormat = formattoi (arrc_attributes [attrFormat].get().c_str());
          signed int retHorJust = horizontaljustificationtoi (arrc_attributes [attrHorizontal_justification].get().c_str());
          if ((retFormat == -1) || (retHorJust == -1))
          {
            if (retFormat == -1)
              std::cout << "Error in formattoi() from object <" << m_nodeName << "> '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;

            if (retHorJust == -1)
              std::cout << "Error in horizontaljustificationtoi() from object <" << m_nodeName << "> '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          signed int retVertJust = verticaljustificationtoi (arrc_attributes [attrVertical_justification].get().c_str(), mi_objectPoolVersion); // we set version 4 if no version set, else we use information from vtp-file.
          if ((retFormat == -1) || (retVertJust == -1))
          {
            if (retFormat == -1)
              std::cout << "Error in formattoi() from object <" << m_nodeName << "> '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;

            if (retVertJust == -1)
              std::cout << "Error in verticaljustificationtoi() from object <" << m_nodeName << "> '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          signed int retJust = retHorJust;
          retJust |= retVertJust << 2;
          fprintf (partFile_attributes, ", %s, %s, %d, %d", formatAsFloat(arrc_attributes [attrScale].get()).c_str(), arrc_attributes [attrNumber_of_decimals].get().c_str(), (unsigned int)retFormat, (unsigned int)retJust);
          break;
        }

        case otLine:
          if (!(arrc_attributes [attrLine_attributes].isGiven() && arrc_attributes [attrWidth].isGiven() && arrc_attributes [attrHeight].isGiven()))
          {
            std::cout << "YOU NEED TO SPECIFY THE line_attributes= AND width= AND height= ATTRIBUTES FOR THE <line> OBJECT '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }

          if (linedirectiontoi (arrc_attributes [attrLine_direction].get().c_str()) == -1)
            return false;
          fprintf (partFile_attributes, ", %s, %s, %s, %d", getObjectReference(attrLine_attributes).c_str(), arrc_attributes [attrWidth].get().c_str(), arrc_attributes [attrHeight].get().c_str(), linedirectiontoi (arrc_attributes [attrLine_direction].get().c_str()));
          break;

        case otRectangle:
          if (!(arrc_attributes [attrLine_attributes].isGiven() && arrc_attributes [attrWidth].isGiven() && arrc_attributes [attrHeight].isGiven()))
          {
            std::cout << "YOU NEED TO SPECIFY THE line_attributes= AND width= AND height= ATTRIBUTES FOR THE <rectangle> OBJECT '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          arrc_attributes [attrLine_suppression].setIfNotGiven ("0");

          if (linesuppressiontoi (arrc_attributes [attrLine_suppression].get().c_str()) == -1)
            return false;
          fprintf (partFile_attributes, ", %s, %s, %s, %d, %s", getObjectReference (attrLine_attributes).c_str(), arrc_attributes [attrWidth].get().c_str(), arrc_attributes [attrHeight].get().c_str(), linesuppressiontoi (arrc_attributes [attrLine_suppression].get().c_str()), getObjectReference (attrFill_attributes).c_str());
          break;

        case otEllipse:
          if (!(arrc_attributes [attrLine_attributes].isGiven() && arrc_attributes [attrWidth].isGiven() && arrc_attributes [attrHeight].isGiven() && arrc_attributes [attrStart_angle].isGiven() && arrc_attributes [attrEnd_angle].isGiven()))
          {
            std::cout << "YOU NEED TO SPECIFY THE line_attributes= AND width= AND height= AND start_angle= AND end_angle= ATTRIBUTES FOR THE <ellipse> OBJECT from object '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          if (arrc_attributes [attrStart_angle].getIntValue() > 180 || arrc_attributes [attrEnd_angle].getIntValue() > 180)
          {
            std::cout << "start_angle= AND end_angle= FOR THE <ellipse> OBJECT '" << m_objName << "' NEED TO HAVE A VALUE LESS OR EQUAL TO 180! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          arrc_attributes [attrEllipse_type].setIfNotGiven("0");

          if (ellipsetypetoi (arrc_attributes [attrEllipse_type].get().c_str()) == -1)
            return false;
          fprintf (partFile_attributes, ", %s, %s, %s, %d, %s, %s, %s", getObjectReference(attrLine_attributes).c_str(), arrc_attributes [attrWidth].get().c_str(), arrc_attributes [attrHeight].get().c_str(), ellipsetypetoi (arrc_attributes [attrEllipse_type].get().c_str()), arrc_attributes [attrStart_angle].get().c_str(), arrc_attributes [attrEnd_angle].get().c_str(), getObjectReference (attrFill_attributes).c_str());
          break;

        case otPolygon:
          if (!(arrc_attributes [attrWidth].isGiven() && arrc_attributes [attrHeight].isGiven() && arrc_attributes [attrLine_attributes].isGiven()))
          {
            std::cout << "YOU NEED TO SPECIFY THE width= AND height= AND line_attributes= ATTRIBUTES FOR THE <polygon> OBJECT '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          arrc_attributes [attrPolygon_type].setIfNotGiven("0");

          if (polygontypetoi (arrc_attributes [attrPolygon_type].get().c_str()) == -1)
            return false;
          fprintf (partFile_attributes, ", %s, %s, %s, %s, %d", arrc_attributes [attrWidth].get().c_str(), arrc_attributes [attrHeight].get().c_str(), getObjectReference (attrLine_attributes).c_str(), getObjectReference (attrFill_attributes).c_str(),
                   polygontypetoi (arrc_attributes [attrPolygon_type].get().c_str()));
          break;

        case otMeter:
          if (!(arrc_attributes [attrWidth].isGiven() && arrc_attributes [attrNeedle_colour].isGiven() && arrc_attributes [attrBorder_colour].isGiven() && arrc_attributes [attrArc_and_tick_colour].isGiven() && arrc_attributes [attrNumber_of_ticks].isGiven() && arrc_attributes[attrStart_angle].isGiven() && arrc_attributes[attrEnd_angle].isGiven() && arrc_attributes [attrMin_value].isGiven() && arrc_attributes [attrMax_value].isGiven()))
          {
            std::cout << "YOU NEED TO SPECIFY THE width= AND needle_colour= AND border_colour= AND arc_and_tick_colour= AND number_of_ticks= AND start_angle= AND end_angle= AND min_value= AND max_value= ATTRIBUTES FOR THE <meter> OBJECT '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          if (arrc_attributes [attrStart_angle].getIntValue() > 180 || arrc_attributes [attrEnd_angle].getIntValue() > 180)
          {
            std::cout << "start_angle= AND end_angle= FOR THE <meter> OBJECT '" << m_objName << "' NEED TO HAVE A VALUE LESS OR EQUAL TO 180! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          arrc_attributes [attrValue].setIfNotGiven ("0");
          arrc_attributes [attrOptions].setIfNotGiven ("0");

          if (colourtoi ( arrc_attributes [attrNeedle_colour].get().c_str()) == -1)
            return false;
          if (colourtoi ( arrc_attributes [attrBorder_colour].get().c_str()) == -1)
            return false;
          if (colourtoi ( arrc_attributes [attrArc_and_tick_colour].get().c_str()) == -1)
            return false;
          if (meteroptionstoi (arrc_attributes [attrOptions].get().c_str()) == -1)
            return false;
          fprintf (partFile_attributes, ", %s, %d, %d, %d, %d, %s, %s, %s, %s, %s, %s, %s", arrc_attributes [attrWidth].get().c_str(), colourtoi ( arrc_attributes [attrNeedle_colour].get().c_str()), colourtoi ( arrc_attributes [attrBorder_colour].get().c_str()),
                   colourtoi ( arrc_attributes [attrArc_and_tick_colour].get().c_str()), meteroptionstoi (arrc_attributes [attrOptions].get().c_str()),
                   arrc_attributes [attrNumber_of_ticks].get().c_str(), arrc_attributes [attrStart_angle].get().c_str(), arrc_attributes [attrEnd_angle].get().c_str(), arrc_attributes [attrMin_value].get().c_str(),
                   arrc_attributes [attrMax_value].get().c_str(), arrc_attributes [attrVariable_reference].get().c_str(), arrc_attributes [attrValue].get().c_str());
          break;

        case otLinearbargraph:
          if (!(arrc_attributes [attrWidth].isGiven() && arrc_attributes [attrHeight].isGiven() && arrc_attributes [attrColour].isGiven() && arrc_attributes [attrTarget_line_colour].isGiven() && arrc_attributes [attrNumber_of_ticks].isGiven() && arrc_attributes [attrMin_value].isGiven() && arrc_attributes [attrMax_value].isGiven()))
          {
            std::cout << "YOU NEED TO SPECIFY THE width= AND height= AND colour= AND target_line_colour= AND number_of_ticks= AND min_value= AND max_value= ATTRIBUTES FOR THE <linearbargraph> OBJECT '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          arrc_attributes [attrValue].setIfNotGiven ("0");
          arrc_attributes [attrTarget_value].setIfNotGiven ("0");

          if (colourtoi ( arrc_attributes [attrColour].get().c_str()) == -1)
           return false;
          if (colourtoi ( arrc_attributes [attrTarget_line_colour].get().c_str()) == -1)
            return false;
          if (linearbargraphoptionstoi (arrc_attributes [attrOptions].get().c_str()) == -1)
            return false;
          fprintf (partFile_attributes, ", %s, %s, %d, %d, %d, %s, %s, %s, %s, %s, %s, %s", arrc_attributes [attrWidth].get().c_str(), arrc_attributes [attrHeight].get().c_str(),
                   colourtoi ( arrc_attributes [attrColour].get().c_str()), colourtoi ( arrc_attributes [attrTarget_line_colour].get().c_str()), linearbargraphoptionstoi (arrc_attributes [attrOptions].get().c_str()), arrc_attributes [attrNumber_of_ticks].get().c_str(), arrc_attributes [attrMin_value].get().c_str(), arrc_attributes [attrMax_value].get().c_str(), arrc_attributes [attrVariable_reference].get().c_str(), arrc_attributes [attrValue].get().c_str(), arrc_attributes [attrTarget_value_variable_reference].get().c_str(), arrc_attributes [attrTarget_value].get().c_str());
          break;

        case otArchedbargraph:
          if (!(arrc_attributes [attrWidth].isGiven() && arrc_attributes [attrHeight].isGiven() && arrc_attributes [attrColour].isGiven() && arrc_attributes [attrTarget_line_colour].isGiven() && arrc_attributes [attrStart_angle].isGiven() && arrc_attributes [attrEnd_angle].isGiven() && arrc_attributes[attrBar_graph_width].isGiven() && arrc_attributes [attrMin_value].isGiven() && arrc_attributes [attrMax_value].isGiven()))
          {
            std::cout << "YOU NEED TO SPECIFY THE width= AND height= AND colour= AND target_line_colour= AND start_angle= AND end_angle= AND bar_graph_width AND min_value= AND max_value= ATTRIBUTES FOR THE <archedbargraph> OBJECT '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          if (arrc_attributes [attrStart_angle].getIntValue() > 180 || arrc_attributes [attrEnd_angle].getIntValue() > 180)
          {
            std::cout << "start_angle= AND end_angle= FOR THE <archedbargraph> OBJECT '" << m_objName << "' NEED TO HAVE A VALUE LESS OR EQUAL TO 180! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          arrc_attributes [attrValue].setIfNotGiven ("0");
          arrc_attributes [attrTarget_value].setIfNotGiven ("0");
          arrc_attributes [attrOptions].setIfNotGiven ("0");

          if (colourtoi ( arrc_attributes [attrColour].get().c_str()) == -1)
            return false;
          if (colourtoi ( arrc_attributes [attrTarget_line_colour].get().c_str()) == -1)
            return false;
          if (archedbargraphoptionstoi (arrc_attributes [attrOptions].get().c_str()) == -1)
            return false;
          fprintf (partFile_attributes , ", %s, %s, %d, %d, %d, %s, %s, %s, %s, %s, %s, %s, %s, %s", arrc_attributes [attrWidth].get().c_str(), arrc_attributes [attrHeight].get().c_str(), colourtoi ( arrc_attributes [attrColour].get().c_str()), colourtoi ( arrc_attributes [attrTarget_line_colour].get().c_str()), archedbargraphoptionstoi (arrc_attributes [attrOptions].get().c_str()), arrc_attributes [attrStart_angle].get().c_str(), arrc_attributes [attrEnd_angle].get().c_str(), arrc_attributes [attrBar_graph_width].get().c_str(), arrc_attributes [attrMin_value].get().c_str(), arrc_attributes [attrMax_value].get().c_str(), arrc_attributes [attrVariable_reference].get().c_str(), arrc_attributes [attrValue].get().c_str(), arrc_attributes [attrTarget_value_variable_reference].get().c_str(), arrc_attributes [attrTarget_value].get().c_str());
          break;

        case otPicturegraphic:
          // check moved above!
          fprintf (partFile_attributes, ", %d, %d,%d, %d, %d, %d", deXwidth, deXactualWidth, deXactualHeight, deXcolourDepth,
                   objBitmapOptions, deXtransCol);
          break;

        case otNumbervariable:
          arrc_attributes [attrValue].setIfNotGiven ("0");

          fprintf (partFile_attributes, ", %sUL", arrc_attributes [attrValue].get().c_str());
          break;

        case otStringvariable:
          if (!arrc_attributes [attrValue].isGiven())
          {
            if (!(arrc_attributes [attrLength].isGiven()))
            {
              std::cout << "YOU NEED TO SPECIFY THE length= ATTRIBUTE FOR THE <stringvariable> OBJECT '" << m_objName << "' (as no value= is given)! STOPPING PARSER! bye."<<std::endl<<std::endl;
              return false;
            }
            arrc_attributes [attrValue].set("NULL");
          }
          else
          {
            signed int ret = strlenUnescaped (arrc_attributes [attrValue].get().c_str());
            if (ret == -1)
            {
              std::cout << "Error in strlenUnescaped() from object <" << m_nodeName << "> '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
              return false;
            }
            //auto-calculate length
            if (!(arrc_attributes [attrLength].isGiven()))
            {
              arrc_attributes [attrLength].set( str(format("%d") % ret) );
            }
            if (!copyWithQuoteAndLength(tempString, escapedString(arrc_attributes [attrValue].get()).c_str(), arrc_attributes [attrLength].getIntValue()))
              return false;

            arrc_attributes [attrValue].set( tempString );
          }
          fprintf (partFile_attributes, ", %s, %s", arrc_attributes [attrLength].get().c_str(), arrc_attributes [attrValue].get().c_str());
          break;

        case otFontattributes:
        {
          if (!arrc_attributes [attrFont_type].isGiven())
          {
            if ( mi_objectPoolVersion < 4 )
            {
              std::cout << "INFORMATION: WITH THAT VERSION OF VT2ISO YOU NEED TO SPECIFY THE font_type= ATTRIBUTE FOR THE <fontattributes> OBJECT '" << m_objName << "'! \n"
                << "VALID VALUES ARE latin1, latin9 or proprietary! STOPPING PARSER! bye."<<std::endl<<std::endl;
            }
            else
            {
              std::cout << "INFORMATION: WITH THAT VERSION OF VT2ISO YOU NEED TO SPECIFY THE font_type= ATTRIBUTE FOR THE <fontattributes> OBJECT '" << m_objName << "'! \n"
                << "VALID VALUES ARE latin1, latin2, latin4, latin5, latin7, latin9 or proprietary! STOPPING PARSER! bye."<<std::endl<<std::endl;
            }
            return false;
          }

          if (!(arrc_attributes [attrFont_colour].isGiven() && arrc_attributes [attrFont_size].isGiven()))
          {
            std::cout << "YOU NEED TO SPECIFY THE font_colour= AND font_size= AND font_type= ATTRIBUTE FOR THE <fontattributes> OBJECT '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          signed int ret = fonttypetoi (arrc_attributes [attrFont_type].get().c_str(), mi_objectPoolVersion);
          signed int retFontSize = fontsizetoi (arrc_attributes [attrFont_size].get().c_str());
          if ((ret == -1) || (retFontSize == -1))
          {
            if (ret == -1)
              std::cout << "Error in fonttypetoi() from object <" << m_nodeName << "> '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;

            if (retFontSize == -1)
              std::cout << "Error in fontsizetoi() from object <" << m_nodeName << "> '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }

          if (colourtoi ( arrc_attributes [attrFont_colour].get().c_str()) == -1)
            return false;
          if (fontstyletoi (arrc_attributes [attrFont_style].get().c_str()) == -1)
            return false;
          fprintf (partFile_attributes, ", %d, %d, %d, %d", colourtoi ( arrc_attributes [attrFont_colour].get().c_str()), (unsigned int)retFontSize,
                   (unsigned int)ret, fontstyletoi (arrc_attributes [attrFont_style].get().c_str()));
          break;
        }

        case otLineattributes:
          if (!(arrc_attributes [attrLine_colour].isGiven() && arrc_attributes [attrLine_width].isGiven() && arrc_attributes [attrLine_art].isGiven()))
          {
            std::cout << "YOU NEED TO SPECIFY THE line_colour= AND line_width= AND line_art= ATTRIBUTE FOR THE <lineattributes> OBJECT '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          if (colourtoi ( arrc_attributes [attrLine_colour].get().c_str()) == -1)
            return false;
          if (linearttoi (arrc_attributes [attrLine_art].get().c_str()) == -1)
            return false;
          fprintf (partFile_attributes, ", %d, %s, 0x%x", colourtoi ( arrc_attributes [attrLine_colour].get().c_str()), arrc_attributes [attrLine_width].get().c_str(), linearttoi (arrc_attributes [attrLine_art].get().c_str()));
          break;

        case otFillattributes:
          if (!(arrc_attributes [attrFill_colour].isGiven()))
          {
            std::cout << "YOU NEED TO SPECIFY THE fill_colour= ATTRIBUTE FOR THE <fillattributes> OBJECT '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          arrc_attributes [attrFill_type].setIfNotGiven ("0");

          if (colourtoi ( arrc_attributes [attrFill_colour].get().c_str()) == -1)
            return false;
          if (filltypetoi (arrc_attributes [attrFill_type].get().c_str()) == -1)
            return false;

          fprintf (partFile_attributes, ",%d, %d, %s", filltypetoi (arrc_attributes [attrFill_type].get().c_str()), colourtoi ( arrc_attributes [attrFill_colour].get().c_str()), getObjectReference (attrFill_pattern).c_str());
          break;

        case otInputattributes:
          arrc_attributes [attrValidation_type].setIfNotGiven ("0");

          if (!arrc_attributes [attrValidation_string].isGiven())
          {
            if (!(arrc_attributes [attrLength].isGiven()))
            {
              std::cout << "YOU NEED TO SPECIFY THE length= ATTRIBUTE FOR THE <inputattribute> OBJECT '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
              return false;
            }
            arrc_attributes [attrValidation_string].set ( "NULL" );
          }
          else
          {
            signed int ret = strlenUnescaped (arrc_attributes [attrValidation_string].get().c_str());
            if (ret == -1)
            {
              std::cout << "Error in strlenUnescaped() from object <" << m_nodeName << "> '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
              return false;
            }
            // auto-calculate string-length
            if (!arrc_attributes [attrLength].isGiven())
            {
              arrc_attributes [attrLength].set( str(format("%d") % ret));
            }

            if (!copyWithQuoteAndLength (tempString, escapedString(arrc_attributes [attrValidation_string].get()).c_str(), arrc_attributes [attrLength].getIntValue()))
            {
              return false;
            }
            arrc_attributes [attrValidation_string].set( tempString );
          }
          if (validationtypetoi (arrc_attributes [attrValidation_type].get().c_str()) == -1)
            return false;
          fprintf (partFile_attributes, ", %d, %s, %s", validationtypetoi (arrc_attributes [attrValidation_type].get().c_str()), arrc_attributes [attrLength].get().c_str(),
                   arrc_attributes [attrValidation_string].get().c_str());
          break;

        case otObjectpointer:
          fprintf (partFile_attributes, ", %s", getObjectReference(attrValue).c_str());
          break;

        case otMacro:
          fprintf (partFile_attributes, ", %s", arrc_attributes [attrNumber_of_bytes].get().c_str());
          break;

        case otAuxiliaryfunction:
          if (!(arrc_attributes [attrBackground_colour].isGiven() && arrc_attributes[attrFunction_type].isGiven()))
          {
            std::cout << "YOU NEED TO SPECIFY THE background_colour= and function_type= ATTRIBUTE FOR THE <auxiliaryfunction> OBJECT '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          if (colourtoi ( arrc_attributes [attrBackground_colour].get().c_str()) == -1)
            return false;
          if (auxfunctiontypetoi(arrc_attributes [attrFunction_type].get().c_str()) == -1)
            return false;
          fprintf (partFile_attributes, ", %d, %d", colourtoi ( arrc_attributes [attrBackground_colour].get().c_str()),
                   auxfunctiontypetoi(arrc_attributes [attrFunction_type].get().c_str()));
          break;

        case otAuxiliaryinput:
          if (!(arrc_attributes [attrBackground_colour].isGiven() && arrc_attributes [attrFunction_type].isGiven() && arrc_attributes[attrInput_id].isGiven()) )
          {
            std::cout << "YOU NEED TO SPECIFY THE background_colour= and function_type= and input_id= ATTRIBUTE FOR THE <auxiliaryinput> OBJECT '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          if (colourtoi ( arrc_attributes [attrBackground_colour].get().c_str()) == -1)
            return false;
          if (auxfunctiontypetoi (arrc_attributes [attrFunction_type].get().c_str()) == -1)
            return false;
          fprintf (partFile_attributes, ", %d, %d, %s", colourtoi (arrc_attributes [attrBackground_colour].get().c_str()),
                   auxfunctiontypetoi (arrc_attributes [attrFunction_type].get().c_str()),
                   arrc_attributes [attrInput_id].get().c_str());
          break;

        case otAuxiliaryFunction2:
          if (!(arrc_attributes [attrBackground_colour].isGiven() && arrc_attributes[attrFunction_type].isGiven() && arrc_attributes[attrOptions].isGiven()))
          {
            std::cout << "YOU NEED TO SPECIFY THE background_colour= and function_type= ATTRIBUTE FOR THE <auxiliaryfunction2> OBJECT '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          if (colourtoi ( arrc_attributes [attrBackground_colour].get().c_str()) == -1)
            return false;
          if (aux2functiontypetoi (arrc_attributes [attrFunction_type].get().c_str()) == -1)
            return false;
          if (auxfunction2optionstoi (arrc_attributes [attrOptions].get().c_str()) == -1)
            return false;
          fprintf (partFile_attributes, ", %d, %d", colourtoi ( arrc_attributes [attrBackground_colour].get().c_str()),
                   aux2functiontypetoi(arrc_attributes [attrFunction_type].get().c_str())
                   | auxfunction2optionstoi(arrc_attributes [attrOptions].get().c_str()) );
          break;

        case otAuxiliaryInput2:
          if (!(arrc_attributes [attrBackground_colour].isGiven() && arrc_attributes [attrFunction_type].isGiven() && arrc_attributes[attrOptions].isGiven()) )
          {
            std::cout << "YOU NEED TO SPECIFY THE background_colour= and function_type= ATTRIBUTE FOR THE <auxiliaryinput2> OBJECT '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          if (colourtoi ( arrc_attributes [attrBackground_colour].get().c_str()) == -1)
            return false;
          if (aux2functiontypetoi (arrc_attributes [attrFunction_type].get().c_str()) == -1)
            return false;
          if (auxinput2optionstoi (arrc_attributes [attrOptions].get().c_str()) == -1)
            return false;
          fprintf (partFile_attributes, ", %d, %d", colourtoi ( arrc_attributes [attrBackground_colour].get().c_str()),
                   aux2functiontypetoi(arrc_attributes [attrFunction_type].get().c_str())
                   | auxinput2optionstoi(arrc_attributes [attrOptions].get().c_str()) );
          break;

        case otAuxiliaryControlDesignatorObjectPointer:
          if (!arrc_attributes [attrPointer_type].isGiven() && arrc_attributes [attrAuxiliaryObjectID].isNull() )
          {
            std::cout << "YOU NEED TO SPECIFY THE pointer_type= and auxiliary_object_id= ATTRIBUTE FOR THE <auxiliarycontroldesignatorobjectpointer> OBJECT '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          if (auxcondesignobjptrtypetoi(arrc_attributes [attrPointer_type].get().c_str()) == -1)
            return false;
          fprintf (partFile_attributes, ", %d, %s", auxcondesignobjptrtypetoi(arrc_attributes [attrPointer_type].get().c_str()),
                   getObjectReference(attrAuxiliaryObjectID).c_str());
          break;

        case otGraphicsContext:
          if (mi_objectPoolVersion < 4)
          {
            std::cerr << "YOU ARE NOT ALLOWED TO USE GRAPHICAL CONTEXT OBJECTS IN A POOL FOR VERSION 2 and 3 VTs! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          if (!arrc_attributes [attrViewportWidth].isGiven() || !arrc_attributes [attrViewportHeight].isGiven())
          {
            std::cout << "YOU NEED TO SPECIFY THE viewport_width= and viewport_height= ATTRIBUTE FOR THE <graphicscontext> OBJECT '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          if (!arrc_attributes [attrViewportX].isGiven() || !arrc_attributes [attrViewportY].isGiven())
          {
            std::cout << "YOU NEED TO SPECIFY THE viewport_x= and viewport_y= ATTRIBUTE FOR THE <graphicscontext> OBJECT '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          if (!arrc_attributes [attrCanvasWidth].isGiven() || !arrc_attributes [attrCanvasHeight].isGiven())
          {
            std::cout << "YOU NEED TO SPECIFY THE canvas_width= and canvas_height= ATTRIBUTE FOR THE <graphicscontext> OBJECT '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          if (!arrc_attributes [attrViewportZoom].isGiven())
          {
            std::cout << "YOU NEED TO SPECIFY THE viewport_zoom= ATTRIBUTE FOR THE <graphicscontext> OBJECT '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          if (!arrc_attributes [attrForeground_colour].isGiven() || !arrc_attributes [attrBackground_colour].isGiven())
          {
            std::cout << "YOU NEED TO SPECIFY THE foreground_colour= and background_colour= ATTRIBUTE FOR THE <graphicscontext> OBJECT '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }
          if (!arrc_attributes [attrFormat].isGiven() || !arrc_attributes [attrOptions].isGiven())
          {
            std::cout << "YOU NEED TO SPECIFY THE format= and options= ATTRIBUTE FOR THE <graphicscontext> OBJECT '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
            return false;
          }

          if (colourtoi ( arrc_attributes [attrForeground_colour].get().c_str()) == -1)
            return false;
          if (colourtoi ( arrc_attributes [attrBackground_colour].get().c_str()) == -1)
            return false;
          fprintf (partFile_attributes, ", %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
                   arrc_attributes [attrViewportWidth].getIntValue(), arrc_attributes [attrViewportHeight].getIntValue(), arrc_attributes [attrViewportX].getIntValue(), arrc_attributes [attrViewportY].getIntValue(),
                   arrc_attributes [attrCanvasWidth].getIntValue(), arrc_attributes [attrCanvasHeight].getIntValue(), arrc_attributes [attrViewportZoom].getIntValue(), arrc_attributes [attrCursorX].getIntValue(), arrc_attributes [attrCursorY].getIntValue(),
                   colourtoi ( arrc_attributes [attrForeground_colour].get().c_str()),
                   colourtoi ( arrc_attributes [attrBackground_colour].get().c_str()));

          fprintf (partFile_attributes, ", %s", getObjectReference(attrFont_attributes).c_str());
          fprintf (partFile_attributes, ", %s", getObjectReference(attrLine_attributes).c_str());
          fprintf (partFile_attributes, ", %s", getObjectReference(attrFill_attributes).c_str());

          if (colourdepthtoi (arrc_attributes [attrFormat].get().c_str()) == -1)
            return false;
          if (gcoptionstoi (arrc_attributes [attrOptions].get().c_str()) == -1)
            return false;
          fprintf (partFile_attributes, ", %d, %d", colourdepthtoi (arrc_attributes [attrFormat].get().c_str()), gcoptionstoi (arrc_attributes [attrOptions].get().c_str()));

          if (!(arrc_attributes [attrTransparency_colour].isGiven()))
            fprintf (partFile_attributes, ", 0");
          else
          {
            if (colourtoi ( arrc_attributes [attrTransparency_colour].get().c_str()) == -1)
              return false;
            fprintf (partFile_attributes, ", %d", colourtoi ( arrc_attributes [attrTransparency_colour].get().c_str()));
          }
          break;
      }
      // #########################################
      // ### Print out Repeat Array REFERENCES ###
      // #########################################
      if (objHasArrayObject)
      {
        if (objChildObjects == 0)
          fprintf (partFile_attributes, ", 0, NULL");
        else
          fprintf (partFile_attributes, ", %d, iVtObject%s_aObject", objChildObjects, m_objName.c_str());
      }
      if (objHasArrayObjectXY)
      {
        if (objChildObjects == 0)
          fprintf (partFile_attributes, ", 0, NULL");
        else
          fprintf (partFile_attributes, ", %d, iVtObject%s_aObject_x_y_font_row_col", objChildObjects, m_objName.c_str());
      }
      if (objType == otPicturegraphic)
      {
        for (int actDepth=0; actDepth <= 2; actDepth++)
        {
          if ( (stdRawBitmapBytes [actDepth] == 0))
            fprintf (partFile_attributes, ", 0,NULL");
          else
            fprintf (partFile_attributes, ", %d,iVtObject%s_aRawBitmap%d", stdRawBitmapBytes [actDepth], m_objName.c_str(), actDepth);
        }
        if (fixBitmapCounter == 0)
          fprintf (partFile_attributes, ", 0,NULL");
        else
          fprintf (partFile_attributes, ", %d,iVtObject%s_aFixedBitmaps", fixBitmapCounter, m_objName.c_str());
      }
      if (objHasArrayPoints)
      {
        if(objChildPoints == 0)
          fprintf (partFile_attributes, ", 0,NULL");
        else
          fprintf (partFile_attributes, ", %d,iVtObject%s_aPoints", objChildPoints, m_objName.c_str());
      }
      if (objHasArrayEventMacro)
      {
        if (objChildMacros == 0)
          fprintf (partFile_attributes, ", 0,NULL");
        else
          // Changed this line to give the correct name to the Macro object to match the naming conventions of IsoAgLib V 1.1.0.
          // This coincides with a change made above to the name of the Macro struct. -bac 06-Jan-2005
          //fprintf (partFile_attributes, ", %d,iVtObject%s_aEvent_Macro", objChildMacros, objName);
          fprintf (partFile_attributes, ", %d,iVtObject%s_aMacro_Object", objChildMacros, m_objName.c_str());
      }

      if (objHasArrayLanguagecode)
      {
        if (objChildLanguages == 0)
          fprintf (partFile_attributes, ", 0,NULL");
        else
          fprintf (partFile_attributes, ", %d,ivtObject%s_aLanguage", objChildLanguages, m_objName.c_str());
      }
      if (objHasArrayMacroCommand)
      {
        fprintf (partFile_attributes, ", iVtObject%s_aMacro_Commands", m_objName.c_str());
      }
      if ( checkForAllowedExecution() )
        fprintf (partFile_attributes, "};\n"); //s_ROM bla blub terminator...

    } while (b_dupMode && (*dupLangNext != 0x00));
  }
  /// End of "normal" element processing


  /// Add all Child-Elements recursively, if possible
  if (objType == 0xFFFF)
  { // can't access omcTypeTable, I'm not an "object" and can't have objects as my children..
    return true;
  } // so: not possible!

  // possible: check which objects are allowed as child...
  uint64_t omcType = omcTypeTable [objType];

#if DEBUG
  fprintf(stderr, "  omcType = %lld (0x%LX)\n", omcType, omcType);
#endif

  if (objType == otContainer)
  {
    omcType = ombType; // Object May Contain what the Object Is - Simple rule. more simple than the graphic in the spec. ;)
  }
  else if ( pc_specialParsingPropTag && (objType >= maxObjectTypes) )
  {
    pc_specialParsingPropTag->setOmcType(omcType, &ombType, objType);
#if DEBUG
  fprintf(stderr, "  reset omcType to %lld (0x%LX)\n", omcType, omcType);
#endif
  }

  bool b_backupParseOnlyWorkingSet = mb_parseOnlyWorkingSet;

  if (objType == otWorkingset)
  { // do always process all children of the workingset!
    // so disable "mb_parseOnlyWorkingSet" for these children!
    mb_parseOnlyWorkingSet = false;
  }

  for (child = n->getFirstChild(); child != 0; child=child->getNextSibling())
  {
    if (child->getNodeType() == DOMNode::ELEMENT_NODE)
    {
#if DEBUG
  fprintf(stderr, "  call 'processElement' for child ...\n");
#endif
      if (!processElement (child, omcType/*, rpcc_inKey, rpcc_inButton*/))
        return false;
    }
  }

  mb_parseOnlyWorkingSet = b_backupParseOnlyWorkingSet;

  return true;
}


bool vt2iso_c::processPointElements(unsigned int& r_objChildPoints, DOMNode *r_n, bool ab_outputEnabled)
{ // Process all Child-Elements
  DOMNode *child;
  DOMNamedNodeMap *pAttributes;

  bool firstElement = true;
  std::string objChildName;
  r_objChildPoints = 0;
  for (child = r_n->getFirstChild(); child != 0; child=child->getNextSibling())
  {
    if ( (child->getNodeType() == DOMNode::ELEMENT_NODE) && (0 == strcmp (XMLString::transcode(child->getNodeName()), otCompTable [otPoint]) ))
    {
      // get 'event=' and 'name=' out of child
      if(child->hasAttributes())
      {
              // parse through all attributes
        pAttributes = patched_getAttributes(child);
        int nSize = pAttributes->getLength();
        arrc_attributes [attrPos_x].clear();
        arrc_attributes [attrPos_y].clear();
        for(int i=0;i<nSize;++i)
        {
          DOMAttr *pAttributeNode = (DOMAttr*) pAttributes->item(i);
          utf16convert (pAttributeNode->getName(), attr_name);
          utf16convert (pAttributeNode->getValue(), attr_value);

          if (attr_name.compare("pos_x") == 0)
          {
            arrc_attributes [attrPos_x].set( attr_value );
          }
          if (attr_name.compare("pos_y") == 0)
          {
            arrc_attributes [attrPos_y].set( attr_value );
          }
        }
      }

      if (ab_outputEnabled)
      {
        if (firstElement)
        {
            fprintf (partFile_attributes, "const IsoAgLib::repeat_x_y_s iVtObject%s_aPoints [] = {", m_objName.c_str());
        }
        else
        {
          fprintf (partFile_attributes, ", ");
        }
      }
      if (!(arrc_attributes [attrPos_x].isGiven()))
      {
        std::cout << "\n\npos_x ATTRIBUTE NEEDED IN <point ...>  from object <" << m_nodeName << "> '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
        return false;
      }
      if (!(arrc_attributes [attrPos_y].isGiven()))
      {
        std::cout << "\n\npos_y ATTRIBUTE NEEDED IN <point ...>  from object <" << m_nodeName << "> '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
        return false;
      }

      if (ab_outputEnabled)
        fprintf (partFile_attributes, "{%d, %d}", arrc_attributes [attrPos_x].getIntValue(), arrc_attributes [attrPos_y].getIntValue());

      r_objChildPoints++;
      firstElement = false;
    }
  }
  if (r_objChildPoints < 3)
  {
    std::cout << "\n\nYOU NEED TO SPECIFY AT LEAST 3 <point ...> elements for object <" << m_nodeName << "> '" << m_objName << "'! STOPPING PARSER! bye."<<std::endl<<std::endl;
    return false;
  }
  else
  {
     arrc_attributes [attrNumber_of_points].setIfNotGiven( str(format("%i")  % r_objChildPoints) );
  }
  if (ab_outputEnabled && firstElement == false)
    fprintf (partFile_attributes, "};\n");

  return true;
}

bool vt2iso_c::processMacroElements(unsigned int& r_objMacros, DOMNode *r_n, bool ab_outputEnabled)
{
  DOMNode *child;
  DOMNamedNodeMap *pAttributes;
  std::string objChildName;
  bool is_objChildName=false;
  bool firstElement = true;
  r_objMacros = 0;

  for (child = r_n->getFirstChild(); child != 0; child=child->getNextSibling())
  {
    if ( (child->getNodeType() == DOMNode::ELEMENT_NODE)
          && ( (0 == strcmp (XMLString::transcode(child->getNodeName()), otCompTable [otIncludemacro]))
          || (0 == strcmp (XMLString::transcode(child->getNodeName()), otCompTable [otMacro]))))
    {
            // get 'event=' and 'name=' out of child
      if(child->hasAttributes())
      {
        // parse through all attributes
        pAttributes = patched_getAttributes(child);
        int nSize = pAttributes->getLength();

        arrc_attributes [attrEvent].clear();

        is_objChildName = false;
        
        for(int i=0;i<nSize;++i)
        {
          DOMAttr *pAttributeNode = (DOMAttr*) pAttributes->item(i);
          utf16convert (pAttributeNode->getName(), attr_name);
          utf16convert (pAttributeNode->getValue(), attr_value);

          if (attr_name.compare("event") == 0)
          {
            arrc_attributes [attrEvent].set( attr_value);
          }
          if (attr_name.compare("name") == 0)
          {
            objChildName = attr_value;
            is_objChildName = true;
          }
        }
      }
      if (is_objChildName == false)
      {
              // create auto-named NAME attribute
        objChildName = str(format("Unnamed%d") % objNextUnnamedName);

        ((DOMElement *)child)->setAttribute (X("name"), X(objChildName.c_str()));
              // add pool_ident to the name for getting an ID -> if child gets processed, its name gets the pool_ident added
        objChildName = str(format("Unnamed%d") % objNextUnnamedName);
        objNextUnnamedName++;
        is_objChildName = true;
      }

      if (ab_outputEnabled)
      {
        if (firstElement)
        {
                // Changed the macro struct name in the following line to match what is in version 1.1.0 of IsoAgLib -bac 06-Jan-2005
                // fprintf (partFile_attributes, "const IsoAgLib::repeat_Macro_iVtObject_s iVtObject%s_aMacro_Object [] = {", objName);
          fprintf (partFile_attributes, "const IsoAgLib::repeat_event_iVtObjectMacro_s iVtObject%s_aMacro_Object [] = {", m_objName.c_str());
        }
        else
        {
          fprintf (partFile_attributes, ", ");
        }
      }
      if (!(arrc_attributes [attrEvent].isGiven()))
      {
        std::cout << "\n\nevent ATTRIBUTE NEEDED IN <macro ...> ! STOPPING PARSER! bye."<<std::endl<<std::endl;
        return false;
      }
            //fprintf (partFile_attributes, "{%d, &vtObject%s}", atoi (attrString [attrEvent]), objChildName);

      if (eventtoi(arrc_attributes [attrEvent].get().c_str()) == -1)
        return false;
      if (!eventtoi(arrc_attributes [attrEvent].get().c_str()))
      {
        std::cout << "\n\nevent: invalid attribute value ! STOPPING PARSER! bye.\n\n";
        return false;
      }

      if (ab_outputEnabled)
        fprintf (partFile_attributes, "{%d, &iVtObject%s}", eventtoi(arrc_attributes [attrEvent].get().c_str()), objChildName.c_str());

      r_objMacros++;
      firstElement = false;
    }
  }
  if (ab_outputEnabled && firstElement == false)
    fprintf (partFile_attributes, "};\n");
  return true;
}


bool vt2iso_c::processChildElements(unsigned int& r_objChildren, DOMNode *r_n, bool xyNeeded, bool ab_outputEnabled)
{
  DOMNode *child;
  DOMNamedNodeMap *pAttributes;

  std::string objChildName;
  bool is_objChildName=false;
  bool is_objChildID=false;
  signed long int objChildID=0;
  unsigned int objChildX=0;
  unsigned int objChildY=0;
  bool is_objChildX=false;
  bool is_objChildY=false;
  unsigned int objBlockRow=0;
  unsigned int objBlockCol=0;
  std::string objBlockFont;
  const char* rpcc_inButton=NULL;
  const char* rpcc_inKey=NULL;
  bool firstElement = true;

  r_objChildren = 0;
  for (child = r_n->getFirstChild(); child != 0; child=child->getNextSibling())
  {   // if NOT Macro insert as normal object!
    if ( (child->getNodeType() == DOMNode::ELEMENT_NODE) &&
          !((0 == strcmp (XMLString::transcode(child->getNodeName()), otCompTable [otMacro])) ||
          (0 == strcmp (XMLString::transcode(child->getNodeName()), otCompTable [otIncludemacro])) ||
          (0 == strcmp (XMLString::transcode(child->getNodeName()), otCompTable [otLanguage]))
           )
       )
    {
      autoDetectLanguage (child);
      bool b_foundLanguageAttribute=false; // default: none found in this element!
      if ( (spc_autoLanguage.length() > 0) &&
            (  (0 == strcmp (XMLString::transcode(child->getNodeName()), otCompTable [otOutputstring])) ||
            (0 == strcmp (XMLString::transcode(child->getNodeName()), otCompTable [otStringvariable]))  )
         )
      { // use default language
        b_foundLanguageAttribute = true;
        attr_value = spc_autoLanguage;
      }

      if (child->hasAttributes())
      { // see where there may be a LANGUAGE= attribute
        pAttributes = patched_getAttributes(child);
        int nSize = pAttributes->getLength();
        for (int i=0; i<nSize; ++i)
        {
          DOMAttr *pAttributeNode = (DOMAttr*) pAttributes->item(i);
          utf16convert (pAttributeNode->getName(), attr_name);
          utf16convert (pAttributeNode->getValue(), attr_value2);

          if (attr_name.compare("language") == 0)
          {
            attr_value = attr_value2;
            b_foundLanguageAttribute = true;
            break;
          }
        }
      }

      static std::string duplicateForLanguagesChild;// max for all languages
      bool b_dupModeChild=false;

      /// Duplicate Loop here also!!!!!
      if (b_foundLanguageAttribute && (attr_value.length() > 0) && (attr_value.length() != 2))
      { // "*" or multiple languages, so we need to loop!
        if (attr_value.compare("*") == 0)
        { // build all languages we have defined in the working set
          duplicateForLanguagesChild.clear();
          for (unsigned int ui=0; ui<ui_languages; ui++)
          {
            duplicateForLanguagesChild += arrs_language [ui].code;
          }
        }
        else
        {
          duplicateForLanguagesChild = attr_value;
        }
        b_dupModeChild=true;
      }

      const char* dupLangNextChild=duplicateForLanguagesChild.c_str();
      do // language duplication loop!
      {
        if (b_dupModeChild)
        { // "fake-copy" next language to "attrString [attrLanguage]"
          if (*dupLangNextChild == '+') dupLangNextChild++; // skip "+"s!
          attr_value.clear();
          attr_value.push_back (*dupLangNextChild++);
          attr_value.push_back (*dupLangNextChild++);
          if (*dupLangNextChild == '+') dupLangNextChild++; // skip "+"s!
        }
        // Check if it's an ALTERNATIVE language or NO language at all, then DO NOT add this as child!
        unsigned int childLang=0;
        bool b_addAsChild=true;

        if (b_foundLanguageAttribute && (attr_value.length() > 0))
        {
          if (ui_languages > 0)
          {
            for (unsigned int curChildLang=0; curChildLang<ui_languages; curChildLang++)
            {
              if (attr_value.compare(arrs_language[curChildLang].code) == 0)
              {
                childLang = curChildLang;
              }
            }
            if (attr_value.compare(arrs_language[0].code) != 0)
            {
              b_addAsChild = false;
            }
          }
        }
        if (b_addAsChild)
        {
          // get NAME and POS_X and POS_Y attributes out of child
          if(child->hasAttributes())
          { // parse through all attributes
            pAttributes = patched_getAttributes(child);
            int nSize = pAttributes->getLength();
            is_objChildName = false;
            is_objChildID = false;
            is_objChildX = false;
            is_objChildY = false;

            objBlockFont = "NULL";
            objBlockRow = 0;
            objBlockCol = 0;

            for (int i=0; i<nSize; ++i)
            {
              DOMAttr *pAttributeNode = (DOMAttr*) pAttributes->item(i);
              utf16convert (pAttributeNode->getName(), attr_name);
              utf16convert (pAttributeNode->getValue(),attr_value);

              // Get NAME and POS_X and POS_Y directly
              if (attr_name.compare("name") == 0)
              {
                objChildName = attr_value;
                is_objChildName = true;
                continue;
              }
              if (attr_name.compare("id") == 0)
              {
                objChildID = StringToInt(attr_value);
                is_objChildID = true;
                continue;
              }
              if (attr_name.compare("pos_x") == 0)
              {
                objChildX = StringToInt(attr_value);
                is_objChildX = true;
                continue;
              }
              if (attr_name.compare("pos_y") == 0)
              {
                objChildY = StringToInt (attr_value);
                is_objChildY = true;
                continue;
              }
              if (attr_name.compare("block_font") == 0)
              {
                objBlockFont = std::string("&iVtObject") + attr_value;
                continue;
              }
              if (attr_name.compare("block_row") == 0)
              {
                objBlockRow = StringToInt (attr_value);
                continue;
              }
              if (attr_name.compare("block_col") == 0)
              {
                objBlockCol = StringToInt (attr_value);
                continue;
              }
            }
          }

          if (0 == strcmp (XMLString::transcode(child->getNodeName()), otCompTable [otIncludeobject]))
          { // special <include_object .../> case
            // requests "name=" or id="65535" to be given!
            // no need for auto-creating a name or id!
            if (is_objChildName && (is_objChildID && (objChildID != 65535)))
            { // if NAME and ID given, assign the ID to the NAME (forward declaration)
              // give him an ID, although not necessary now...
              objChildID = getID (objChildName.c_str(), false /* assumption: not a macro here */, is_objChildID, objChildID);
              if (objChildID == -1)
              {
                std::cout << "Error in getID() from object <" << m_nodeName << "> '" << m_objName << "'! STOP PARSER! bye."<<std::endl<<std::endl;
                return false;
              }
              // continue
            }
            else if (is_objChildName && !is_objChildID)
            { // good, valid case. nothing to do - continue
              // => just include the &iVtObjectNAME statement, no id needed.
            }
            else if ((!is_objChildName || (is_objChildName && (objChildName == "NULL"))) && (is_objChildID && (objChildID == 65535)))
            { // good, valid case. nothing to do - continue
              // => just include a NULL statement, no name needed.
            }
            else
            {
              std::cout << "Invalid <include_object .../> statement!! STOP PARSER! bye."<<std::endl<<std::endl;
              return false;
            }
            // ... nothing to here at "continue" point.. just run through..
          }
          else
          { // normal object nested as child object
            // so we need to autogenerate a name or id!
            if (is_objChildName == false)
            { // create auto-named NAME attribute
              objChildName = str(format("Unnamed%d") % objNextUnnamedName);
              ((DOMElement *)child)->setAttribute (X("name"), X(objChildName.c_str()));
              // add pool_ident to the name for getting an ID -> if child gets processed, its name gets the pool_ident added
              objChildName = str(format("Unnamed%d") % objNextUnnamedName);
              objNextUnnamedName++;
              is_objChildName = true;
            }
          }

          if (ab_outputEnabled)
          {
            if (firstElement)
            {
              if (xyNeeded)
              {
                fprintf (partFile_attributes, "const IsoAgLib::repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s iVtObject%s_aObject_x_y_font_row_col [] = {", m_objName.c_str());
              }
              else
                fprintf (partFile_attributes, "const IsoAgLib::repeat_iVtObject_s iVtObject%s_aObject [] = {", m_objName.c_str());
            }
            else
            {
              fprintf (partFile_attributes, ", ");
            }
          }
          if (xyNeeded)
          {
            if (!(is_objChildX && is_objChildY))
            {
              std::cout << "\n\npos_x AND pos_y ATTRIBUTES NEEDED IN CHILD-OBJECT OF <"<< m_nodeName <<"> " << objChildName << "! STOPPING PARSER! bye."<<std::endl<<std::endl;
              return false;
            }

            if (is_objChildID && (objChildID == 65535))
            {
              std::cout << "\n\nid=\"65535\" not allowed as CHILD-OBJECT OF <"<< m_nodeName <<"> " << objChildName << "! STOPPING PARSER! bye."<<std::endl<<std::endl;
              return false;
            }

            if (ab_outputEnabled)
              fprintf (partFile_attributes, "{&iVtObject%s, %d, %d, %s ,%d, %d}", objChildName.c_str(), objChildX, objChildY, objBlockFont.c_str(), objBlockRow, objBlockCol);
          }
          else
          { // Added this if statement to account for InputList/OutputList objects who might have NULL Object IDs in their list of objects. (Which is legal per the standard!)
            // Instead of inserting a faulty object name, just insert NULL into the array. -BAC 07-Jan-2005
            if (ab_outputEnabled)
            {
              if (is_objChildID && (objChildID == 65535))
              {
                fprintf (partFile_attributes, "{NULL}");
              }
              else
              {
                fprintf (partFile_attributes, "{&iVtObject%s}", objChildName.c_str());
              }
            }
          }
          r_objChildren++;
          firstElement = false;
        }
        else
        { // !b_addAsChild ==> so manually write the "setOriginXXX" call !!
          /// Add implicit Button/Key includement
          if (ab_outputEnabled)
          {
            if (rpcc_inButton)
            {
              fprintf (partFile_functions_origin, "  iVtObject%s_%s%d.setOriginBTN (&iVtObject%s);\n", objChildName.c_str(), mstr_langPrefix.c_str(), childLang, rpcc_inButton);
            }
            if (rpcc_inKey)
            {
              fprintf (partFile_functions_origin, "  iVtObject%s_%s%d.setOriginSKM (%s);\n", objChildName.c_str(), mstr_langPrefix.c_str(), childLang, rpcc_inKey ? "true":"false"); // is now always "true"...
            }
          }
        }
      } while (b_dupModeChild && (*dupLangNextChild != 0x00));
      /// END Language Dup Loop
    }
  }
  // all child-elements processed, now:
  // special treatment for inputlist/outputlist with NULL objects
  if (((objType == otInputlist) || (objType == otOutputlist)) && r_objChildren < (uint16_t)arrc_attributes [attrNumber_of_items].getIntValue())
  { //only some items are NULL objects which were not counted in objChildObjects
    if (r_objChildren>0)
    {
      if (ab_outputEnabled)
      {
        for (uint16_t ui_leftChildObjects = r_objChildren; ui_leftChildObjects<(uint16_t) arrc_attributes [attrNumber_of_items].getIntValue(); ui_leftChildObjects++)
        {
          if (ui_leftChildObjects < arrc_attributes [attrNumber_of_items].getIntValue())
            fprintf (partFile_attributes, ", ");

          fprintf (partFile_attributes, "{NULL}");
        }
      }
      r_objChildren= ((uint16_t) arrc_attributes [attrNumber_of_items].getIntValue());
    }
    else
    { // no child-element at all in the inputlist/outputlist (all items as NULL objects)
      // fill the reference-list with {NULL}-elements --> so they could be replaced during runtime with NOT NULL objects
      if (r_objChildren == 0 && arrc_attributes [attrNumber_of_items].getIntValue() > 0)
      { // objChildObjects has to be set to number_of_items otherwise
        // it is set to 0 in the attributes of the inputlist/output
        r_objChildren = ((uint16_t) arrc_attributes [attrNumber_of_items].getIntValue());
        // create for all number_of_items a no-item placeholder
        if (ab_outputEnabled)
        {
          fprintf (partFile_attributes, "const IsoAgLib::repeat_iVtObject_s iVtObject%s_aObject [] = {", m_objName.c_str());
          for (int i_emptyChildObj=1; i_emptyChildObj <= arrc_attributes [attrNumber_of_items].getIntValue(); i_emptyChildObj++)
          {
            fprintf (partFile_attributes, "{NULL}");
            if (i_emptyChildObj < arrc_attributes [attrNumber_of_items].getIntValue())
              fprintf (partFile_attributes, ", ");
          }
          fprintf (partFile_attributes, "};\n");
        }
      }
    }
  }
  if (ab_outputEnabled && firstElement == false)
    fprintf (partFile_attributes, "};\n");

  return true;
}

vt2iso_c::vt2iso_c()
  : firstLineFileE(true)
  , partFile_split_function( NULL )
  , ui_languages(0)
  , mbitset_objIdUsed()
  , map_objNameIdTable()
  , objNextAutoID(65534)
  , objNextMacroAutoID(255)
  , kcNextAutoID(254) // for safety, 255 should also be okay though...
  , objNextUnnamedName(1)
  , opDimension(0)
  , skWidth(0)
  , skHeight(0)
  , mi_objectPoolVersion(0x02) // only version 2 vts are on the market at the moment.
  , is_opDimension(false)
  , is_opAdditionallyRequiredObjects(false)
  , is_skWidth(false)
  , is_skHeight(false)
  , b_externalize(false)
  , mb_parseOnlyWorkingSet(false)
  , mb_verbose (false)
  , mb_acceptUnknownAttributes(false)
  , mb_silentMode(false)
  , mb_pedanticMode(false)
  , ui_picBufferSize(1200*1600)
  , b_hasUnknownAttributes(false)
  , b_hasMoreThan6SoftKeys(false)
  , m_errorOccurred(false)
  , parser(NULL)
{
  picBuffer = new unsigned char[ui_picBufferSize];
}

vt2iso_c::~vt2iso_c()
{
  delete pc_specialParsing;
  delete pc_specialParsingPropTag;
}

void
vt2iso_c::skRelatedFileOutput()
{
  if (!getIsSKWidth()) setSKWidth (60);
  if (!getIsSKHeight()) setSKHeight (32);
  if (!getIsSKWidth() || !getIsSKHeight())
  {
    if(!mb_silentMode)
      std::cout << "\n\nWARNING: You have NOT specified a SoftKey-Width/Height, so vt2iso assumes your softkeys are designed on a 60x32 pixel basis.\n"
                << "ATTENTION: SoftKeys are scaled and centered to fit the SK-Dimensions of the VT it is displayed on, so take care that you know what you're doing!"<<std::endl<<std::endl;
  }
  fprintf (partFile_defines, "static const unsigned int vtObjectPoolDimension = %d;\n", getOPDimension());
  fprintf (partFile_defines, "static const unsigned int vtObjectPoolSoftKeyWidth = %d;\n", getSKWidth());
  fprintf (partFile_defines, "static const unsigned int vtObjectPoolSoftKeyHeight = %d;\n", getSKHeight());
}

void vt2iso_c::generateIncludeDefines()
{
  const char* additionally_required_objects = getOPAdditionallyRequiredObjects().c_str();
  for (int i=0; i<39; i++) // parse only standard vt objects
  {
    char* objectName=otCompTable[i];
    if (arrb_objTypes[i] || strstr(additionally_required_objects, objectName) )
    {
      fprintf (partFile_obj_selection, "#ifndef USE_VTOBJECT_%s\n", objectName);
      fprintf (partFile_obj_selection, "\t#define USE_VTOBJECT_%s\n", objectName);
      fprintf (partFile_obj_selection, "#endif\n");
    }
  }
}

const char* vt2iso_c::getAttributeValue (DOMNode* pc_node, const char* attributeName)
{
  /// get attributes of node and compare all attributes with given params or until match
  DOMNamedNodeMap *pAttributes = pc_node->getAttributes();
  int nodeSize = pAttributes->getLength();
  for (int index=0; index<nodeSize; ++index)
  {
    DOMAttr *pAttributeNode = (DOMAttr*) pAttributes->item(index);
    utf16convert (pAttributeNode->getName(), attr_name);

    if (attr_name.compare(attributeName) == 0)
    { /// correct attribute found, now compare the given vale with attribute's value
      utf16convert (pAttributeNode->getValue(),attr_value);
      return attr_value.c_str();
    }
  }
  return "";
}


bool vt2iso_c::isProjectFileMode (const std::string& astr_fileName)
{
  size_t lastDotPosition = astr_fileName.find_last_of( '.' ); // do we have a dot?
  if (lastDotPosition != string::npos)
  {
    string str_extension = astr_fileName.substr (lastDotPosition, string::npos);
    for (std::string::iterator it=str_extension.begin(); it != str_extension.end(); ++it)
    {
      *it = tolower (*it);
    }

    if (str_extension == ".vtp")
    {
      return true;
    }
  }
  return false;
}


std::vector<std::string> vt2iso_c::getSearchPath( const std::string& arcstr_searchPathArg )
{
  std::vector<std::string> vec_tmp(0);

  if( arcstr_searchPathArg.empty() ) {
    return vec_tmp;
  }

  std::string str_searchPathDelimiter( 1, msc_searchPathSeperator );
  std::string str_tmp;
  for ( std::size_t i = 0; i < arcstr_searchPathArg.size(); i++ ) {
    if (( arcstr_searchPathArg.substr( i, str_searchPathDelimiter.size() ).compare( str_searchPathDelimiter ) == 0 ) ) {
      vec_tmp.push_back( str_tmp );
      str_tmp.clear();
      i += str_searchPathDelimiter.size() - 1;
    }
    else {
      str_tmp.append( 1, arcstr_searchPathArg[i] );
      if ( i == ( arcstr_searchPathArg.size() - 1 ) )
        vec_tmp.push_back( str_tmp );
    }
  }

  // check sepeator
  for( std::vector<std::string>::iterator it = vec_tmp.begin(); it != vec_tmp.end(); ++it ) {
    std::replace ((*it).begin(), (*it).end(), scc_dirSeparatorWrong, scc_dirSeparatorCorrect);

    if ((*it)[(*it).length()-1] != scc_dirSeparatorCorrect) { // No separator given, so append separator (i.e. (back)slash)
      (*it).push_back (scc_dirSeparatorCorrect);
    }
  }


  return vec_tmp;
}

bool vt2iso_c::existsFile( const std::string& arcstr_fileWithPath ) {
  FILE* handle = fopen (arcstr_fileWithPath.c_str(), "r" );
  if( handle ) {
    fclose( handle );
    return true;
  }
  return false;
}


bool vt2iso_c::prepareFileNameAndDirectory (const std::string& astr_fileName)
{
  /// Cut extension off to be generated files...
  mstr_sourceDirAndProjectPrefix = astr_fileName;
  std::string c_expectedType[] = {".xml", ".XML", ".vtp", ".VTP"};
  for (int i=0; i<(int (sizeof (c_expectedType) / sizeof (std::string))); ++i)
  {
    const int ci_extensionLength = c_expectedType[i].size();
    if (int (mstr_sourceDirAndProjectPrefix.length()) > ci_extensionLength)
    {
      if (mstr_sourceDirAndProjectPrefix.substr( mstr_sourceDirAndProjectPrefix.length()-ci_extensionLength ) == c_expectedType[i])
      { // strip off extension
        mstr_sourceDirAndProjectPrefix.erase (mstr_sourceDirAndProjectPrefix.length()-ci_extensionLength, ci_extensionLength);
        break;
      }
    }
  }

  /// Extract Source Dir
  std::string str_separator = " "; str_separator[0] = scc_dirSeparatorCorrect;
  int lastDirPos = mstr_sourceDirAndProjectPrefix.find_last_of( str_separator );
  if (lastDirPos < 0)
  { // no separator found - dir is the current dir!
    mstr_sourceDir = "." + str_separator;
  }
  else if (lastDirPos == 0)
  { // root directory
    mstr_sourceDir = str_separator;
  }
  else
  { // separator some-where. just extract
    mstr_sourceDir = mstr_sourceDirAndProjectPrefix.substr( 0, lastDirPos ) + str_separator;
  }

  /// Extract Project (File w/o .ext) Name as default for LEGACY MODE or PROJECT-MODE w/o Project="..."
  std::string str_fileName = mstr_sourceDirAndProjectPrefix.substr( lastDirPos+1 );

  if (!processProjectFile (astr_fileName))
    return false;

  // Check for Fallback ProjectName in case none given!
  if (mstr_projectName.length() == 0)
    mstr_projectName = str_fileName;

  /// Generate ClassName from ProjectName" (remove Space and other non-C++-identifier-chars)
  mstr_className = mstr_projectName;
  for (std::string::iterator iter=mstr_className.begin(); iter != mstr_className.end(); ++iter)
  {
    if (!isalnum(*iter))
    { // replace non-alphanumeric character by underscore character
      *iter = '_';
    }
  }

  /// Override destination-directory with given outDir-parameter?
  if (mstr_outDirName.length() > 0) mstr_destinDirAndProjectPrefix = mstr_outDirName;
  else                              mstr_destinDirAndProjectPrefix = mstr_sourceDir;

  if (mstr_destinDirAndProjectPrefix.length() == 0)
    return false;

  if (mstr_destinDirAndProjectPrefix[mstr_destinDirAndProjectPrefix.length()-1] != scc_dirSeparatorCorrect)
  { // No separator given, so append separator (i.e. (back)slash)
    mstr_destinDirAndProjectPrefix.push_back (scc_dirSeparatorCorrect);
  }

  if (mstr_outFileName.length() == 0)
    mstr_outFileName = mstr_projectName;
  else if (mstr_outFileName == ":filename:")
    mstr_outFileName = str_fileName;
  // else: "mstr_outFileName" already fine.

  mstr_destinDirAndProjectPrefix.append (mstr_outFileName);

  return true;
}




bool
vt2iso_c::setCommandElement(unsigned int& commandType, DOMNode *child, DOMNode *n, unsigned int& objChildCommands, std::string &commandMessage)
{
  switch (commandType)
  {
    case ctHideShowObject:
    {
      clearAndSetElements (child, make_vector (attrObjectID, attrHideShow));

      signed long int ret = idOrName_toi(arrc_attributes [attrObjectID].get().c_str(), /*macro?*/false);
      signed int retHideShow = booltoi(arrc_attributes [attrHideShow].get().c_str());
      if ( ret == -1 )
      {
        std::cout << "Error in idOrName_toi() from object <" << m_nodeName << "> '" << m_objName << "'! STOP PARSER! bye."<<std::endl<<std::endl;
        return false;
      }
      if ( retHideShow == -1 )
      {
        std::cout << "Error in booltoi() from object <" << m_nodeName << "> '" << m_objName << "'! STOP PARSER! bye."<<std::endl<<std::endl;
        return false;
      }
      // Need check for all attributes being present for this command -bac
      commandMessage = str(format("0xA0, %d, %d, %d, 0xFF, 0xFF, 0xFF, 0xFF") %   MACRO_16bitToLE((unsigned int)ret) % (unsigned int)retHideShow);
    } break;


    case ctEnableDisableObject:
    {
      clearAndSetElements (child, make_vector (attrObjectID, attrDisable_enable));

      signed long int ret = idOrName_toi(arrc_attributes [attrObjectID].get().c_str(), /*macro?*/false);
      signed int retDisEnable = booltoi(arrc_attributes [attrDisable_enable].get().c_str());
      if ( ret == -1 )
      {
        std::cout << "Error in idOrName_toi() from object <" << m_nodeName << "> '" << m_objName << "'! STOP PARSER! bye."<<std::endl<<std::endl;
        return false;
      }
      if ( retDisEnable == -1 )
      {
        std::cout << "Error in booltoi() from object <" << m_nodeName << "> '" << m_objName << "'! STOP PARSER! bye."<<std::endl<<std::endl;
        return false;
      }
      // Need check for all attributes being present for this command -bac
      commandMessage = str(format("0xA1, %d, %d, %d, 0xFF, 0xFF, 0xFF, 0xFF") %  MACRO_16bitToLE((unsigned int)ret) % (unsigned int)retDisEnable);
    } break;


    case ctSelectInputObject:
    {
      clearAndSetElements (child, make_vector (attrObjectID, attrActivate_for_editing));

      signed long int ret = idOrName_toi(arrc_attributes [attrObjectID].get().c_str(), /*macro?*/false);
      signed int retActiveForEditing = booltoi ( arrc_attributes [attrActivate_for_editing].get().c_str());
      if (ret == -1)
      {
        std::cout << "Error in idOrName_toi() from object <" << m_nodeName << "> '" << m_objName << "'! STOP PARSER! bye."<<std::endl<<std::endl;
        return false;
      }
      if ( retActiveForEditing == -1 )
      {
        std::cout << "Error in booltoi() from object <" << m_nodeName << "> '" << m_objName << "'! STOP PARSER! bye."<<std::endl<<std::endl;
        return false;
      }
      unsigned int isActive = 0xFF;
      if (retActiveForEditing == 1)
      {
        // true -> select with Activation
        isActive = 0;
      }
      // Need check for all attributes being present for this command -bac
      commandMessage = str(format("0xA2, %d, %d, 0x%x, 0xFF, 0xFF, 0xFF, 0xFF") %  MACRO_16bitToLE((unsigned int)ret) % isActive);
    } break;


    case ctControlAudioDevice:
    {
      clearAndSetElements (child, make_vector (attrNumber_of_repetitions, attrFrequency, attrOnTime_duration, attrOffTime_duration));
      // Need check for all attributes being present for this command -bac
      commandMessage = str(format("0xA3, %d, %d, %d, %d,%d, %d, %d") % arrc_attributes [attrNumber_of_repetitions].getIntValue() % MACRO_16bitToLE ( arrc_attributes [attrFrequency].getIntValue()) %  MACRO_16bitToLE ( arrc_attributes [attrOnTime_duration].getIntValue()) % MACRO_16bitToLE ( arrc_attributes [attrOffTime_duration].getIntValue()));
    } break;


    case ctSetAudioVolume:
    {
      clearAndSetElements (child, make_vector (attrPercentage));

      // Need check for all attributes being present for this command -bac
      commandMessage = str(format("0xA4, %d, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF") % arrc_attributes [attrPercentage].getIntValue());
    } break;


    case ctChangeChildLocation:
    {
      clearAndSetElements (child, make_vector (attrParent_objectID, attrObjectID, attrX_change, attrY_change));

      // Need check for all attributes being present for this command -bac
      // add 127 to relative x,y
      signed long int retParent = idOrName_toi(arrc_attributes [attrParent_objectID].get().c_str(), /*macro?*/false);
      signed long int ret = idOrName_toi(arrc_attributes [attrObjectID].get().c_str(), /*macro?*/false);
      if ((ret == -1) || (retParent == -1))
      {
        std::cout << "Error in idOrName_toi() from object <" << m_nodeName << "> '" << m_objName << "'! STOP PARSER! bye."<<std::endl<<std::endl;
        return false;
      }
      commandMessage = str(format("0xA5, %d, %d, %d, %d, %d, %d, 0xFF") %  MACRO_16bitToLE ((unsigned int)retParent) % MACRO_16bitToLE ((unsigned int)ret) %  ( arrc_attributes [attrX_change].getIntValue() + 127) % ( arrc_attributes [attrY_change].getIntValue() + 127));
    } break;


    case ctChangeChildPosition:
    {
      clearAndSetElements (child, make_vector (attrParent_objectID, attrObjectID, attrX_pos, attrY_pos));

      signed long int retParent = idOrName_toi(arrc_attributes [attrParent_objectID].get().c_str(), /*macro?*/false);
      signed long int ret = idOrName_toi(arrc_attributes [attrObjectID].get().c_str(), /*macro?*/false);
      if ((ret == -1) || (retParent == -1))
      {
        std::cout << "Error in idOrName_toi() from object <" << m_nodeName << "> '" << m_objName << "'! STOP PARSER! bye."<<std::endl<<std::endl;
        return false;
      }
      // Need check for all attributes being present for this command -bac
      commandMessage = str(format("0xB4, %d, %d, %d, %d, %d, %d, %d, %d") % MACRO_16bitToLE((unsigned int)retParent) % MACRO_16bitToLE((unsigned int)ret) %  MACRO_16bitToLE( arrc_attributes [attrX_pos].getIntValue()) % MACRO_16bitToLE( arrc_attributes [attrY_pos].getIntValue()));
    } break;


    case ctChangeSize:
    {
      clearAndSetElements (child, make_vector (attrObjectID, attrNew_width, attrNew_height));

      signed long int ret = idOrName_toi(arrc_attributes [attrObjectID].get().c_str(), /*macro?*/false);
      if (ret == -1)
      {
        std::cout << "Error in idOrName_toi() from object <" << m_nodeName << "> '" << m_objName << "'! STOP PARSER! bye."<<std::endl<<std::endl;
        return false;
      }
      // Need check for all attributes being present for this command -bac
      commandMessage = str(format("0xA6, %d, %d, %d, %d, %d, %d, 0xFF") %  MACRO_16bitToLE((unsigned int)ret) % MACRO_16bitToLE( arrc_attributes [attrNew_width].getIntValue()) % MACRO_16bitToLE(arrc_attributes [attrNew_height].getIntValue()));
    } break;


    case ctChangeBackgroundColour:
    {
      clearAndSetElements (child, make_vector (attrObjectID, attrNew_background_colour));

      signed long int ret = idOrName_toi(arrc_attributes [attrObjectID].get().c_str(), /*macro?*/false);
      if (ret == -1)
      {
        std::cout << "Error in idOrName_toi() from object <" << m_nodeName << "> '" << m_objName << "'! STOP PARSER! bye."<<std::endl<<std::endl;
        return false;
      }
      if (colourtoi ( arrc_attributes [attrNew_background_colour].get().c_str()) == -1)
        return false;
      // Need check for all attributes being present for this command -bac
      commandMessage = str(format( "0xA7, %d, %d, %d, 0xFF, 0xFF, 0xFF, 0xFF") %  MACRO_16bitToLE((unsigned int)ret) % colourtoi ( arrc_attributes [attrNew_background_colour].get().c_str()));
    } break;


    case ctChangeNumericValue:
    {
      clearAndSetElements (child, make_vector (attrObjectID, attrNew_value));

      signed long int ret = idOrName_toi(arrc_attributes [attrObjectID].get().c_str(), /*macro?*/false);
      signed long int retNewValue = idOrName_toi(arrc_attributes [attrNew_value].get().c_str(), /*macro?*/false); // "idOrName_toi" is okay here, because we could change an objectpointer to point to some other object!
      if ((retNewValue == -1) || (ret == -1))
      {
        std::cout << "Error in idOrName_toi() from object <" << m_nodeName << "> '" << m_objName << "'! STOP PARSER! bye."<<std::endl<<std::endl;
        return false;
      }
      // Need check for all attributes being present for this command -bac
      commandMessage = str(format("0xA8, %d, %d, 0xFF, %li, %li, %li, %li") %  MACRO_16bitToLE((unsigned int)ret) % MACRO_32bitToLE(retNewValue));
    } break;


    case ctChangeStringValue:
    {
      clearAndSetElements (child, make_vector (attrObjectID, attrBytes_in_string, attrNew_value));

      // Need check for all attributes being present for this command -bac
      // Also need to replace the atoi below to some integertoi stuff so characters will register an error!
      int strLength;

      std::string tempString2;

      strLength = arrc_attributes [attrNew_value].getLength();
      for(int i = 0; i < strLength; i++)
      {
        tempString2+= str(format(", %d") % ((unsigned int)arrc_attributes [attrNew_value].getCharacterAtPos(i)));
      }
      signed long int ret = idOrName_toi(arrc_attributes [attrObjectID].get().c_str(), /*macro?*/false);
      signed long int retBytesInString = arrc_attributes [attrBytes_in_string].getIntValue();
      if ((ret == -1) || (retBytesInString == -1))
      {
        std::cout << "Error in idOrName_toi() from object <" << m_nodeName << "> '" << m_objName << "'! STOP PARSER! bye."<<std::endl<<std::endl;
        return false;
      }
      commandMessage = str(format("0xB3, %d, %d, %d, %d%s") % MACRO_16bitToLE((unsigned int)ret) % MACRO_16bitToLE((unsigned int)retBytesInString) % tempString2);
    } break;


    case ctChangeEndPoint:
    {
      clearAndSetElements (child, make_vector (attrObjectID, attrNew_width, attrNew_height, attrLine_direction));

      signed long int ret = idOrName_toi(arrc_attributes [attrObjectID].get().c_str(), /*macro?*/false);
      if (ret == -1)
      {
        std::cout << "Error in idOrName_toi() from object <" << m_nodeName << "> '" << m_objName << "'! STOP PARSER! bye."<<std::endl<<std::endl;
        return false;
      }
      // Need check for all attributes being present for this command -bac
      commandMessage = str(format("0xA9, %d, %d, %d, %d, %d, %d, %d") % MACRO_16bitToLE((unsigned int)ret) % MACRO_16bitToLE( arrc_attributes [attrNew_width].getIntValue()) %  MACRO_16bitToLE( arrc_attributes [attrNew_height].getIntValue()) %  arrc_attributes [attrLine_direction].getIntValue());
    } break;


    case ctChangeFontAttributes:
    {
      clearAndSetElements (child, make_vector (attrObjectID, attrFont_colour, attrFont_size, attrFont_type, attrFont_style));

      signed long int ret = idOrName_toi(arrc_attributes [attrObjectID].get().c_str(), /*macro?*/false);
      signed long int retFontSize = fontsizetoi(arrc_attributes [attrFont_size].get().c_str());
      if ( ret == -1 )
      {
        std::cout << "Error in idOrName_toi() from object <" << m_nodeName << "> '" << m_objName << "'! STOP PARSER! bye."<<std::endl<<std::endl;
        return false;
      }
      if ( retFontSize == -1 )
      {
        std::cout << "Error in fontsizetoi() from object <" << m_nodeName << "> '" << m_objName << "'! STOP PARSER! bye."<<std::endl<<std::endl;
        return false;
      }
      if (colourtoi ( arrc_attributes [attrFont_colour].get().c_str()) == -1)
        return false;
      if (fontstyletoi ( arrc_attributes [attrFont_style].get().c_str()) == -1)
        return false;
      // Need check for all attributes being present for this command -bac
      commandMessage = str(format("0xAA, %d, %d, %d, %d, %d, %d, 0xFF") % MACRO_16bitToLE( (unsigned int)ret) % colourtoi (arrc_attributes [attrFont_colour].get().c_str()) % (unsigned int)retFontSize % arrc_attributes [attrFont_type].getIntValue() % fontstyletoi ( arrc_attributes [attrFont_style].get().c_str()));
    } break;


    case ctChangeLineAttributes:
    {
      clearAndSetElements (child, make_vector (attrObjectID, attrLine_colour, attrLine_width, attrLine_art));

      signed long int ret = idOrName_toi(arrc_attributes [attrObjectID].get().c_str(), /*macro?*/false);
      if (ret == -1)
      {
        std::cout << "Error in idOrName_toi() from object <" << m_nodeName << "> '" << m_objName << "'! STOP PARSER! bye."<<std::endl<<std::endl;
        return false;
      }
      if (colourtoi ( arrc_attributes [attrLine_colour].get().c_str()) == -1)
        return false;
      if (linearttoi(arrc_attributes [attrLine_art].get().c_str()) == -1)
        return false;
      // Need check for all attributes being present for this command -bac
      commandMessage = str(format("0xAB, %d, %d, %d, %d, %d, %d, 0xFF") %  MACRO_16bitToLE ((unsigned int)ret) % colourtoi(arrc_attributes [attrLine_colour].get().c_str()) %  arrc_attributes [attrLine_width].getIntValue() % MACRO_16bitToLE (linearttoi(arrc_attributes [attrLine_art].get().c_str())));
    } break;


    case ctChangeFillAttributes:
    {
      clearAndSetElements (child, make_vector (attrObjectID, attrFill_type, attrFill_colour, attrFill_pattern));

      signed long int ret = idOrName_toi(arrc_attributes [attrObjectID].get().c_str(), /*macro?*/false);
      if (ret == -1)
      {
        std::cout << "Error in idOrName_toi() from object <" << m_nodeName << "> '" << m_objName << "'! STOP PARSER! bye."<<std::endl<<std::endl;
        return false;
      }
      if (colourtoi ( arrc_attributes [attrFill_colour].get().c_str()) == -1)
        return false;
      if (filltypetoi (arrc_attributes [attrFill_type].get().c_str()) == -1)
        return false;
      // Need check for all attributes being present for this command -bac
      commandMessage = str(format("0xAC, %d, %d, %d, %d, %d, %d, 0xFF") %  MACRO_16bitToLE((unsigned int)ret) % filltypetoi (arrc_attributes [attrFill_type].get().c_str()) %  colourtoi (arrc_attributes [attrFill_colour].get().c_str()) % MACRO_16bitToLE (arrc_attributes [attrFill_pattern].getIntValue()));
    } break;


    case ctChangeActiveMask:
    {
      clearAndSetElements (child, make_vector (attrWorking_setID, attrNew_active_mask));

      signed long int retWS_ID = idOrName_toi(arrc_attributes [attrWorking_setID].get().c_str(), /*macro?*/false);
      signed long int retNewMask = idOrName_toi(arrc_attributes [attrNew_active_mask].get().c_str(), /*macro?*/false);
      if ((retWS_ID == -1) || (retNewMask == -1))
      {
        std::cout << "Error in idOrName_toi() from object <" << m_nodeName << "> '" << m_objName << "'! STOP PARSER! bye."<<std::endl<<std::endl;
        return false;
      }
      // Need check for all attributes being present for this command -bac
      commandMessage = str(format("0xAD, %d, %d, %d, %d, 0xFF, 0xFF, 0xFF") %  MACRO_16bitToLE((unsigned int)retWS_ID) % MACRO_16bitToLE((unsigned int)retNewMask));
    } break;


    case ctChangeSoftKeyMask:
    {
      clearAndSetElements (child, make_vector ( attrMask_type, attrMaskID, attrNew_soft_key_mask ));

      signed long int retMaskID = idOrName_toi(arrc_attributes [attrMaskID].get().c_str(), /*macro?*/false);
      signed long int retNewSKM = idOrName_toi(arrc_attributes [attrNew_soft_key_mask].get().c_str(), /*macro?*/false);
      if ((retMaskID == -1) || (retNewSKM == -1))
      {
        std::cout << "Error in idOrName_toi() from object <" << m_nodeName << "> '" << m_objName << "'! STOP PARSER! bye."<<std::endl<<std::endl;
        return false;
      }
      // Need check for all attributes being present for this command -bac
      if ( masktypetoi(arrc_attributes [attrMask_type].get().c_str()) == -1)
        return false;
      commandMessage = str(format("0xAE, %d, %d, %d, %d, %d, 0xFF, 0xFF") % masktypetoi(arrc_attributes [attrMask_type].get().c_str()) % MACRO_16bitToLE((unsigned int)retMaskID) % MACRO_16bitToLE((unsigned int)retNewSKM));
    } break;


    case ctChangeAttribute:
    {
      clearAndSetElements(child, make_vector ( attrObjectID, attrAttributeID, attrNew_value ));

      signed long int ret = idOrName_toi(arrc_attributes [attrObjectID].get().c_str(), /*macro?*/false);
      signed long int retNewValue = idOrName_toi(arrc_attributes [attrNew_value].get().c_str(), /*macro?*/false);
      if ((ret == -1) || (retNewValue == -1))
      {
        std::cout << "Error in idOrName_toi() from object <" << m_nodeName << "> '" << m_objName << "'! STOP PARSER! bye."<<std::endl<<std::endl;
        return false;
      }
      // Need check for all attributes being present for this command -bac
      commandMessage = str(format("0xAF, %d, %d, %d, %li, %li, %li, %li") %  MACRO_16bitToLE((unsigned int)ret) % (arrc_attributes [attrAttributeID].getIntValue()) %  MACRO_32bitToLE(retNewValue));
    } break;


    case ctChangePriority:
    {
      clearAndSetElements (child, make_vector (attrObjectID, attrNew_priority));

      signed long int ret = idOrName_toi(arrc_attributes [attrObjectID].get().c_str(), /*macro?*/false);
      signed long int retPrio = prioritytoi( arrc_attributes [attrNew_priority].get().c_str());
      if (ret == -1)
      {
        std::cout << "Error in idOrName_toi() from object <" << m_nodeName << "> '" << m_objName << "'! STOP PARSER! bye."<<std::endl<<std::endl;
        return false;
      }
      if (retPrio == -1)
      {
        std::cout << "Error in prioritytoi() from object <" << m_nodeName << "> '" << m_objName << "'! STOP PARSER! bye."<<std::endl<<std::endl;
        return false;
      }
      // Need check for all attributes being present for this command -bac
      commandMessage = str(format("0xB0, %d, %d, %d, 0xFF, 0xFF, 0xFF, 0xFF") %  MACRO_16bitToLE((unsigned int)ret) % (unsigned int)retPrio);
    } break;


    case ctChangeListItem:
    {
      clearAndSetElements (child, make_vector (attrObjectID, attrList_index, attrNew_objectID));

      signed long int ret = idOrName_toi(arrc_attributes [attrObjectID].get().c_str(), /*macro?*/false);
      signed long int retNewID = idOrName_toi(arrc_attributes [attrNew_objectID].get().c_str(), /*macro?*/false);
      if ((ret == -1) || (retNewID == -1))
      {
        std::cout << "Error in idOrName_toi() from object <" << m_nodeName << "> '" << m_objName << "'! STOP PARSER! bye."<<std::endl<<std::endl;
        return false;
      }
      // Need check for all attributes being present for this command -bac
      commandMessage = str(format("0xB1, %d, %d, %d, %d, %d, 0xFF, 0xFF") %  MACRO_16bitToLE((unsigned int)ret) % StringToInt(arrc_attributes [attrList_index].get().c_str()) %  MACRO_16bitToLE((unsigned int)retNewID));
    } break;


    case ctExecuteMacro:
    {
      clearAndSetElements (child, make_vector (attrObjectID));

      signed long int ret = idOrName_toi(arrc_attributes [attrObjectID].get().c_str(), /*macro?*/false);
      if ( ret == -1 )
      {
        std::cout << "Error in idOrName_toi() from object <" << m_nodeName << "> '" << m_objName << "'! STOP PARSER! bye."<<std::endl<<std::endl;
        return false;
      }
      // Need check for all attributes being present for this command -bac
      commandMessage = str(format("0xBE, %d, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF") % (unsigned char)ret);
    } break;


    default:
      clean_exit ("WRONG COMMAND_TYPE given in MACRO.");
      exit (-1);
  }

  objChildCommands++;
  return true;
}

// ---------------------------------------------------------------------------
//
//  int main (argC, argV) --- parse XML file and output to partA, partB and partC files.
//
// ---------------------------------------------------------------------------
int main(int argC, char* argV[])
{
  c_Bitmap.setOstream(std::cout);

  // Check command line and extract arguments.
  if (argC < 2) {
    c_Bitmap.printLicenseText();
    usage();
    return 1;
  }

  AbstractDOMParser::ValSchemes valScheme = AbstractDOMParser::Val_Auto;
  bool doNamespaces       = false;
  bool doSchema           = false;
  bool schemaFullChecking = false;
  std::string dictionary = "";

  bool generatePalette = false;
  bool externalize = false;
  bool verbose = false;
  bool b_accept_unknown_attributes = false;
  bool b_silentMode = false;
  bool b_pedanticMode = false;
  bool b_disableContainmentRules = false;
  std::string str_outFileName;
  std::string str_definesPrefix = "iVtObjectID";
  std::string poolIdentStr;
  std::string localeStr;
  std::string str_outDir;
  std::string str_namespace;
  std::string str_searchPath;
  std::string str_vtPresetFile;
  std::string str_langPrefix;
  std::string str_baseClass = "IsoAgLib::iVtClientObjectPool_c"; // default value if -b commandline parameter is not specified
  std::string str_baseClassHdr; // empty so no '#include "..."' directive is written on default. Use -bh to set it and generate that directive

  int filenameInd = -1; // defaults to: no filename specified.
  for (int argInd = 1; argInd < argC; argInd++)
  {
    // Check for option or filename
    if (argV[argInd][0] != '-')
    { // no option
      if (filenameInd == -1)
      { // no filename yet, so this is the filename!
        filenameInd = argInd;
      }
      else
      { // there was already a filename given! ==> double filename!
        usage();
        std::cerr << std::endl << "vt2iso: Error: Two filenames given. Refer to following usage for proper use! ***" << std::endl;
        return 3;
      }
      // this is no option, so don't continue with the options check!
      continue;
    }

    // Watch for special case help request
    if (!strcmp(argV[argInd], "-?"))
    {
      usage();
      return 2;
    }
    else if (!strncmp(argV[argInd], "-v=", 3)
              ||  !strncmp(argV[argInd], "-V=", 3))
    {
      const char* const parm = &argV[argInd][3];

      if (!strcmp(parm, "never"))
        valScheme = AbstractDOMParser::Val_Never;
      else if (!strcmp(parm, "auto"))
        valScheme = AbstractDOMParser::Val_Auto;
      else if (!strcmp(parm, "always"))
        valScheme = AbstractDOMParser::Val_Always;
      else
      {
        std::cerr << "Unknown -v= value: " << parm << std::endl;
        return 2;
      }
    }
    else if (!strcmp(argV[argInd], "-n"))
    {
      doNamespaces = true;
    }
    else if (!strcmp(argV[argInd], "-s"))
    {
      doSchema = true;
    }
    else if (!strcmp(argV[argInd], "-f"))
    {
      schemaFullChecking = true;
    }
    else if (!strncmp(argV[argInd], "-locale=", 8))
    {
      // Get out the end of line
      localeStr.assign (&argV[argInd][8]);
    }
    else if (!strcmp(argV[argInd], "-p"))
    {
      generatePalette = true;
    }
    else if (!strcmp(argV[argInd], "-e"))
    {
      externalize = true;
    }
    else if (!strcmp(argV[argInd], "-m"))
    {
      // only set to verbose if not in silentMode!
      if (!b_silentMode)
        verbose = true;
    }
    else if (!strncmp(argV[argInd], "-i", 2))
    {
      usage();
      std::cerr << "vt2iso: Error: Option -i not supported anymore, try to rewrite to use -g." << std::endl;
      return 5;
    }
    else if (!strncmp(argV[argInd], "-o=", 3))
    {
      str_outDir.assign (&argV[argInd][3]);
    }
    else if (!strncmp(argV[argInd], "-c=", 3))
    {
      str_vtPresetFile.assign (&argV[argInd][3]);
    }
    else if (!strncmp(argV[argInd], "-a=", 3))
    {
      str_outFileName.assign (&argV[argInd][3]);
      if (str_outFileName.length() == 0)
        str_outFileName.assign (":filename:");
    }
    else if (!strcmp(argV[argInd], "-a"))
    {
      str_outFileName.assign (":filename:");
    }
    else if (!strncmp(argV[argInd], "-g=", 3))
    {
      str_namespace.assign (&argV[argInd][3]);
    }
    else if (!strcmp(argV[argInd], "-g"))
    {
      str_namespace.assign (":projectname:"); // special key for projectname-use!
    }
    else if (!strncmp(argV[argInd], "-x=", 3))
    {
      str_langPrefix = &argV[argInd][3];
    }
    else if (!strncmp(argV[argInd], "-r=", 3))
    {
      str_definesPrefix.assign (&argV[argInd][3]);
    }
    else if (!strcmp(argV[argInd], "-r"))
    {
      str_definesPrefix.assign ("");
    }
    else if (!strncmp(argV[argInd], "-d=", 3))
    {
      str_searchPath = &argV[argInd][3];
    }
    else if (!strncmp(argV[argInd], "-dict=", 6))
    {
      dictionary = &argV[argInd][6];
    }
    else if (!strcmp(argV[argInd], "-d"))
    {
      b_disableContainmentRules = true;
    }
    else if(!strcmp(argV[argInd], "-u")){
      b_accept_unknown_attributes = true;
    }
    else if(!strcmp(argV[argInd], "-l")){
      //the silent mode disactivates the verbose mode
      b_silentMode = true;
      verbose = false;
    }
    else if(!strcmp(argV[argInd], "-k")){
      b_pedanticMode = true;
    }
    else if (!strncmp(argV[argInd], "-b=", 3))
    {
      str_baseClass = &argV[argInd][3];
    }
    else if (!strncmp(argV[argInd], "-bh=", 4))
    {
      str_baseClassHdr = &argV[argInd][4];
    }
    else
    {
      usage();
      std::cerr << "vt2iso: Error: Unknown option '" << argV[argInd] << "'" << std::endl;
      return 4;
    }
  }

  if(!b_silentMode){
    c_Bitmap.printLicenseText();
  }

  //
  //  There should be only one parameter left, and that
  //  should be the file name.
  //
  if (filenameInd == -1)
  {
    usage();
    std::cerr << "vt2iso: Error: No filename given." << std::endl;
    return 1; 
  }

  // Initialize the XML4C system
  try
  {
    if (localeStr.length())
    {
      XMLPlatformUtils::Initialize(localeStr.c_str());
    }
    else
    {
      XMLPlatformUtils::Initialize("de_DE");
    }
  }

  catch (const XMLException& toCatch)
  {
    std::cerr << "Error during initialization! :\n"
        << StrX(toCatch.getMessage()) << std::endl;
    return 1;
  }

  // get file list with matching files!
  std::string str_cmdlineName( argV [filenameInd] );

  // Do INITIALIZATION STUFF
  vt2iso_c* pc_vt2iso = new vt2iso_c();

  // Instantiate the DOM parser.
  static const XMLCh gLS[] = { chLatin_L, chLatin_S, chNull };
  DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(gLS);
  DOMBuilder  *parser = ((DOMImplementationLS*)impl)->createDOMBuilder(DOMImplementationLS::MODE_SYNCHRONOUS, 0);

  parser->setFeature(XMLUni::fgDOMNamespaces, doNamespaces);
  parser->setFeature(XMLUni::fgXercesSchema, doSchema);
  parser->setFeature(XMLUni::fgXercesSchemaFullChecking, schemaFullChecking);

  if (valScheme == AbstractDOMParser::Val_Auto)        parser->setFeature(XMLUni::fgDOMValidateIfSchema, true);
  else if (valScheme == AbstractDOMParser::Val_Never)  parser->setFeature(XMLUni::fgDOMValidation, false);
  else if (valScheme == AbstractDOMParser::Val_Always) parser->setFeature(XMLUni::fgDOMValidation, true);

    // enable datatype normalization - default is off
  parser->setFeature(XMLUni::fgDOMDatatypeNormalization, true);

  std::string xsdLocation (argV[0]);
  std::string::size_type separatorPos = xsdLocation.find_last_of ("/\\");
  if (separatorPos != std::string::npos)
    xsdLocation = xsdLocation.substr (0, separatorPos+1);
  xsdLocation += "vt2iso.xsd";
  XMLCh* propertyValue = XMLString::transcode(xsdLocation.c_str());
  parser->setProperty(XMLUni::fgXercesSchemaExternalNoNameSpaceSchemaLocation, propertyValue);

  // And create our error handler and install it
  parser->setErrorHandler(pc_vt2iso);

  const bool cb_initSuccess = pc_vt2iso->init (str_cmdlineName, &dictionary, externalize, b_disableContainmentRules, parser, verbose, str_outDir, str_namespace, b_accept_unknown_attributes, b_silentMode, b_pedanticMode, str_outFileName, str_searchPath, str_langPrefix, str_definesPrefix, str_baseClass, str_baseClassHdr );

  if (cb_initSuccess)
  {

    if (!str_vtPresetFile.empty())
    {
      if (!pc_vt2iso->processVtPresetFile(str_vtPresetFile))
      {
        std::cerr << "vt2iso: Error in processing vt preset file." << std::endl;
        return 1; 
      }
    }

    if (generatePalette)
    {
      std::cout << "Generating ISO11783-6(VT)-Palette to 'iso11783vt-palette.act'..." << std::endl;
      FILE* paletteFile = &vt2iso_c::save_fopen ("iso11783vt-palette.act", "wt");
      RGB_s vtColourTableRGB [256];
      for (int i=0; i<256; i++)
      {
        vtColourTableRGB[i].rgbRed  = vtColourTable[i].bgrRed;
        vtColourTableRGB[i].rgbGreen= vtColourTable[i].bgrGreen;
        vtColourTableRGB[i].rgbBlue = vtColourTable[i].bgrBlue;
      }
      fwrite (vtColourTableRGB, sizeof(RGB_s), 256, paletteFile);
      fclose (paletteFile);
    }

    pc_vt2iso->parse();
  }

  // Delete the parser itself.  Must be done prior to calling Terminate, below.
  parser->release();
  // And call the termination method
  XMLPlatformUtils::Terminate();

  delete pc_vt2iso;
  return (cb_initSuccess) ? 0 : -1; /// everything well done - or not...
}


void vt2iso_c::parse()
{
  if (getAmountXmlFiles() == 0)
  {
    std::cout << "vt2iso: No input files. Terminating.\n" << std::endl;
    return;
  }

  if(!mb_silentMode) std::cout << "** Pass 1 **"<<std::endl;

  if (!doAllFiles (actionMarkIds)) return;

  if(!mb_silentMode) std::cout << "** Pass 2 **"<<std::endl;

  setParseModeWorkingSet (true); // only parse for the workingset-object!
  if (!doAllFiles (actionProcessElement)) return;

  if(!mb_silentMode) std::cout << "** Pass 3 **"<<std::endl;

  setParseModeWorkingSet (false); // parse all objects other than workingset!

  if (!doAllFiles (actionProcessElement)) return;

  skRelatedFileOutput();

  generateIncludeDefines();

  clean_exit ((m_errorOccurred) ? "vt2iso: XML-Parsing error occurred. Terminating.\n"
                                : mb_silentMode ? 0 : "vt2iso: All conversion done successfully.\n");
}


bool vt2iso_c::doAllFiles (action_en aen_action)
{
  int  indexXmlFile;
  uint16_t u16_xmlFileCnt = getAmountXmlFiles();
  for (indexXmlFile = 0; indexXmlFile < u16_xmlFileCnt; indexXmlFile++)
  { // loop all xmlFiles!
    XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument* doc = NULL;

    resetErrors();

    const std::string xmlFile = getXmlFile(indexXmlFile);

    if(!mb_silentMode){
       std::cout << "Processing file " << xmlFile << std::endl;
    }

    try
    { // reset document pool
      parser->resetDocumentPool();
      doc = parser->parseURI(xmlFile.c_str());
    }

    catch (const XMLException& toCatch)
    {
      std::cerr << "\nError during parsing: '" << xmlFile << "'\n"
          << "Exception message is:  \n"
          << StrX(toCatch.getMessage()) << "\n" << std::endl;
      m_errorOccurred = true;
    }
    catch (const DOMException& toCatch)
    {
      const unsigned int maxChars = 2047;
      XMLCh errText[maxChars + 1];

      std::cerr << "\nDOM Error during parsing: '" << xmlFile << "'\n"
          << "DOMException code is:  " << toCatch.code << std::endl;

      if (DOMImplementation::loadDOMExceptionMsg(toCatch.code, errText, maxChars))
        std::cerr << "Message is: " << StrX(errText) << std::endl;

      m_errorOccurred = true;
    }
    catch (...)
    {
      std::cerr << "\nUnexpected exception during parsing: '" << xmlFile << "'"<<std::endl;
      m_errorOccurred = true;
    }

    //  Extract the DOM tree, get the list of all the elements and report the
    //  length as the count of elements.
    if (getSawErrors() || m_errorOccurred)
    {
      std::cout << "\nErrors occurred, no output available\n" << std::endl;
      m_errorOccurred = true;
    }
    else
    {
      if (doc)
      {
        // ### main routine starts right here!!! ###
        switch (aen_action)
        {
          case actionProcessElement:
          {
            bool b_returnRootElement = processElement ((DOMNode*)doc->getDocumentElement(), (uint64_t(1)<<otObjectpool) /*, NULL, NULL*/); // must all be included in an objectpool tag !

            if (!getIsOPDimension())
            {
              clean_exit("\n\nYOU NEED TO SPECIFY THE dimension= TAG IN <objectpool> ! STOPPING PARSER! bye.\n\n");
              return false;
            }
            if (!b_returnRootElement)
            {
              clean_exit("ANY ERROR OCCURED DURING THE PARSING PROCESS\n\n");
              return false;
            }
          } break;

          case actionMarkIds:
            markIds ((DOMNode*)doc->getDocumentElement());
            break;

          default:
            break;
        } // switch
      }
    }
  } // loop all files
  return true;
}


//! This function will recursively scan all elements (of that xml-file).
//! and fill the bitset "mbitset_objIdUsed" with "1" at the given "uid"
//! when the "id=12345" attribute is set in the current n.
//! Will then process all children nodes recursively.
void vt2iso_c::markIds (DOMNode *n)
{
  if (!n)
  {
    printf ("markIds(): 'DOMNode == NULL'. STOPPING PARSER! bye.\n\n");
    return;
  }

  std::string local_attrName;
  std::string local_attrValue;
  DOMNode *child;
  DOMNamedNodeMap *pAttributes = n->getAttributes();

  if (n->hasAttributes())
  { // parse through all attributes
    int nSize = pAttributes->getLength();
    int id=-1;
    std::string name;
    for (int i=0;i<nSize;++i)
    {
      DOMAttr *pAttributeNode = (DOMAttr*) pAttributes->item(i);
      utf16convert (pAttributeNode->getName(), local_attrName);
      utf16convert (pAttributeNode->getValue(), local_attrValue);
      if (local_attrName.compare("id") == 0)
      {
        id = atoi (local_attrValue.c_str());
        if ((id < 0) || (id > 65535))
        {
          clean_exit ("Invalid ID given. It's out of the allowed 16bit-range! STOPPING PARSER! BYE...");
          exit (-1);
        }
        mbitset_objIdUsed.set (id, 1);
#if DEBUG
        std::cout << "Setting Uid" << id << " to true!"<<std::endl;
#endif
        continue;
      }
      if (local_attrName.compare("name") == 0)
      {
        name = local_attrValue;
        continue;
      }
    }
    // now check if a pair with name/id was found
    bool const is_found = !name.empty() && 0 <= id && id != 65535;
    if (is_found)
    { // save it in the table, so we can solve forward-references
      // (which is actually not needed for "normal" forward-references
      // but forward-references in macro-commands!)
#if DEBUG
      std::cout << "Inserting name-ID-pair (" << name << ", " << id << ")" << std::endl;
#endif
      map_objNameIdTable.insert (std::pair<std::string, uint16_t>(name, uint16_t(id)));
    }
  }

  for (child = n->getFirstChild(); child != 0; child=child->getNextSibling())
  {
    if (child->getNodeType() == DOMNode::ELEMENT_NODE)
    {
#if DEBUG
      fprintf(stderr, "  call 'markIds' for child ...\n");
#endif
      markIds (child);
    }
  }

}


void
vt2iso_c::clearAndSetElements (DOMNode *child, const std::vector <int> &avec)
{
  for (std::vector<int>::const_iterator iter = avec.begin(); iter != avec.end(); ++iter)
    arrc_attributes [*iter].clear();

  if (child->hasAttributes())
  {
    DOMNamedNodeMap *pAttributes = patched_getAttributes(child);

    int const nSize = pAttributes->getLength();
    for (int i=0; i<nSize; ++i)
    {
      DOMAttr *pAttributeNode = (DOMAttr*) pAttributes->item(i);
      utf16convert (pAttributeNode->getName(), attr_name);
      utf16convert (pAttributeNode->getValue(),attr_value);

      for (std::vector<int>::const_iterator iter = avec.begin(); iter != avec.end(); ++iter)
        setAttributeValue (*iter);
    }
  }
}


bool
vt2iso_c::processProjectFile(const std::string& pch_fileName)
{
  FILE* prjFile = &save_fopen(pch_fileName.c_str(),"r");
  fclose (prjFile);

  const std::string xmlFile = pch_fileName;

  resetErrors();

  XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument* doc = NULL;

  bool errorOccurred = false;

  try
  {   // reset document pool
    parser->resetDocumentPool();
    doc = parser->parseURI(xmlFile.c_str());
  }

  catch (const XMLException& toCatch)
  {
    std::cerr << "\nError during parsing: '" << xmlFile << "'\n"
        << "Exception message is:  \n"
        << StrX(toCatch.getMessage()) << "\n" << std::endl;
    errorOccurred = true;
  }
  catch (const DOMException& toCatch)
  {
    const unsigned int maxChars = 2047;
    XMLCh errText[maxChars + 1];

    std::cerr << "\nDOM Error during parsing: '" << xmlFile << "'\n"
        << "DOMException code is:  " << toCatch.code << std::endl;

    if (DOMImplementation::loadDOMExceptionMsg(toCatch.code, errText, maxChars))
      std::cerr << "Message is: " << StrX(errText) << std::endl;

    errorOccurred = true;
  }
  catch (...)
  {
    std::cerr << "\nUnexpected exception during parsing: '" << xmlFile << "'"<<std::endl;
    errorOccurred = true;
  }

  //  Extract the DOM tree, get the list of all the elements and report the
  //  length as the count of elements.
  if (getSawErrors() || errorOccurred)
  {
    std::cout << "\nErrors occurred, no output available\n" << std::endl;
    errorOccurred = true;
  }
  else
  {
    if (doc)
    {
      DOMNode *child;
      for (child = doc->getDocumentElement()->getFirstChild(); child != 0; child=child->getNextSibling())
      {
        std::string local_attrName;
        std::string local_attrValue;

        processNestedNodesInProjectFile(child, l_stdBitmapPath, std::string("Bitmaps"), std::string("Bitmap"), std::string("Path"));
        processNestedNodesInProjectFile(child, l_fixedBitmapPath, std::string("Bitmaps"), std::string("FixedBitmap"), std::string("Path"));

        // vec_xmlFiles needs some post processing, done after loop
        processNestedNodesInProjectFile(child, vec_xmlFiles, std::string("Files"), std::string("File"), std::string("FilePath"));

        processNestedNodesInProjectFile(child, l_dictionaryPath, std::string("Dictionaries"), std::string("Dictionary"), std::string("Path"));

        // following extractions: we are only interested in nodes with attributes and not in child nodes
        if (!child->hasAttributes())
          continue;

        DOMNamedNodeMap *pAttributes = child->getAttributes();
        int nSize = pAttributes->getLength();

        /// read project-name into "mstr_className"
        if (strcmp (XMLString::transcode(child->getNodeName()), "Meta") == 0)
        {
          for (int i=0;i<nSize;++i)
          {
            DOMAttr *pAttributeNode = (DOMAttr*) pAttributes->item(i);
            utf16convert (pAttributeNode->getName(), local_attrName);
            utf16convert (pAttributeNode->getValue(), local_attrValue);
            if ((local_attrName.compare("ProjectName") == 0) && (local_attrValue.length() > 0))
            {
              mstr_projectName = local_attrValue;
            }
          }
        }

        /// read object-pool settings
        if (strcmp (XMLString::transcode(child->getNodeName()), "ObjectPoolSettings") == 0)
        {
          for (int i=0;i<nSize;++i)
          {
            DOMAttr *pAttributeNode = (DOMAttr*) pAttributes->item(i);
            utf16convert (pAttributeNode->getName(), local_attrName);
            utf16convert (pAttributeNode->getValue(), local_attrValue);
            if (local_attrName.compare("dimension") == 0)
            {
              opDimension = StringToInt(local_attrValue);
              is_opDimension = true;
            }
            if (local_attrName.compare("softkeywidth") == 0)
            {
              skWidth = StringToInt(local_attrValue);
              is_skWidth = true;
            }
            if (local_attrName.compare("softkeyheight") == 0)
            {
              skHeight = StringToInt(local_attrValue);
              is_skHeight = true;
            }
          }
        }

        /// read object-pool settings
        if (strcmp (XMLString::transcode(child->getNodeName()), "iso11783") == 0)
        {
          for (int i=0;i<nSize;++i)
          {
            DOMAttr *pAttributeNode = (DOMAttr*) pAttributes->item(i);
            utf16convert (pAttributeNode->getName(), local_attrName);
            utf16convert (pAttributeNode->getValue(), local_attrValue);
            if (local_attrName.compare("version") == 0)
            {
              mi_objectPoolVersion = StringToInt(local_attrValue);
              if ((mi_objectPoolVersion < 2) || (mi_objectPoolVersion > 4))
                errorOccurred = true;
            }
          }
        }
      }

      // post processing for vec_xmlFiles
      for ( std::vector<Path_s>::iterator iter = vec_xmlFiles.begin(); iter != vec_xmlFiles.end(); ++iter)
      {
        if (iter->b_relativePath)
        { // add the path to the project-file, because it's relative to that directory!

          bool found = false;

          if( ! mvec_searchPath.empty() ) {
            // search in path before prepending source
            for( std::vector<std::string>::iterator it = mvec_searchPath.begin(); it != mvec_searchPath.end(); ++it ) {
              std::string searchFile( (*it) + ( (*iter).str_pathName) );
              if( existsFile( searchFile ) ) {
                iter->str_pathName.insert (0, (*it) );
                found = true;
                break;
              }
            }
          }

          // if the source dir is not in the search path list or the search path is empty: prepend as default
          if( ! found ) {
            iter->str_pathName.insert (0, mstr_sourceDir);
          }

        }
      }

    }
  }

#if 0
    // @todo ON REQUEST not yet extracted values:
    // additionally_required_objects=
    opAdditionallyRequiredObjects =
    is_opAdditionallyRequiredObjects = true;

    // auto_language=
    strcpy (spc_autoLanguage, );
#endif

  return !errorOccurred;
}


bool
vt2iso_c::processVtPresetFile(const std::string& pch_fileName)
{
  FILE* presetFile = &save_fopen(pch_fileName.c_str(),"r");
  fclose (presetFile);

  const std::string xmlFile = pch_fileName;

  resetErrors();

  XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument* doc = NULL;

  bool errorOccurred = false;

  try
  {   // reset document pool
    parser->resetDocumentPool();
    doc = parser->parseURI(xmlFile.c_str());
  }

  catch (const XMLException& toCatch)
  {
    std::cerr << "\nError during parsing: '" << xmlFile << "'\n"
        << "Exception message is:  \n"
        << StrX(toCatch.getMessage()) << "\n" << std::endl;
    errorOccurred = true;
  }
  catch (const DOMException& toCatch)
  {
    const unsigned int maxChars = 2047;
    XMLCh errText[maxChars + 1];

    std::cerr << "\nDOM Error during parsing: '" << xmlFile << "'\n"
        << "DOMException code is:  " << toCatch.code << std::endl;

    if (DOMImplementation::loadDOMExceptionMsg(toCatch.code, errText, maxChars))
      std::cerr << "Message is: " << StrX(errText) << std::endl;

    errorOccurred = true;
  }
  catch (...)
  {
    std::cerr << "\nUnexpected exception during parsing: '" << xmlFile << "'"<<std::endl;
    errorOccurred = true;
  }

  //  Extract the DOM tree, get the list of all the elements and report the
  //  length as the count of elements.
  if (getSawErrors() || errorOccurred)
  {
    std::cout << "\nErrors occurred, no output available\n" << std::endl;
    errorOccurred = true;
  }
  else
  {
    if (doc)
    {
      DOMNode* vtsettingsNode = NULL;
      DOMNode* proprietaryTableNode = NULL;
      for (vtsettingsNode = doc->getDocumentElement()->getFirstChild(); vtsettingsNode != 0; vtsettingsNode = vtsettingsNode->getNextSibling())
      {
        if (vtsettingsNode->getNodeType() == DOMNode::ELEMENT_NODE)
          break;
      }
      if (!vtsettingsNode)
        errorOccurred = true;

      if (!errorOccurred)
      {
        for (proprietaryTableNode = vtsettingsNode->getFirstChild(); proprietaryTableNode != 0; proprietaryTableNode = proprietaryTableNode->getNextSibling())
        {
          if (proprietaryTableNode->getNodeType() == DOMNode::ELEMENT_NODE)
            break;
        }
      }
      if (!proprietaryTableNode)
        errorOccurred = true;

      if (!errorOccurred)
      {
        int i_colIndex, i_colRed, i_colGreen, i_colBlue;
        i_colIndex = i_colRed = i_colGreen = i_colBlue = 0;

        for (DOMNode* child = proprietaryTableNode->getFirstChild(); child != 0; child=child->getNextSibling())
        {
          if (child->getNodeType() != DOMNode::ELEMENT_NODE)
            continue;

          bool b_colIndex = false, b_colRed = false, b_colGreen = false, b_colBlue = false;

          std::string local_attrName;
          std::string local_attrValue;

          if (!child->hasAttributes())
          {
            errorOccurred = true;
            break;
          }

          DOMNamedNodeMap *pAttributes = child->getAttributes();
          int nSize = pAttributes->getLength();

          for (int i=0;i<nSize;++i)
          {
            DOMAttr *pAttributeNode = (DOMAttr*) pAttributes->item(i);
            utf16convert (pAttributeNode->getName(), local_attrName);
            utf16convert (pAttributeNode->getValue(), local_attrValue);

            if (local_attrName.compare("index") == 0)
            {
              i_colIndex = StringToInt(local_attrValue);
              b_colIndex = true;
            }
            if (local_attrName.compare("blue") == 0)
            {
              i_colBlue = StringToInt(local_attrValue);
              b_colBlue = true;
            }
            if (local_attrName.compare("green") == 0)
            {
              i_colGreen = StringToInt(local_attrValue);
              b_colGreen = true;
            }
            if (local_attrName.compare("red") == 0)
            {
              i_colRed = StringToInt(local_attrValue);
              b_colRed = true;
            }
          }

          if (b_colIndex && b_colBlue && b_colGreen && b_colRed)
          {
            if ( (i_colIndex < 232) || (i_colIndex > 255)
              || (i_colRed < 0) || (i_colRed > 255)
              || (i_colGreen < 0) || (i_colGreen > 255)
              || (i_colBlue < 0) || (i_colBlue > 255)
               )
            { // some value out of range
              errorOccurred = true;
              break;
            }
            else
            { // values okay, write to color-table
              vtColourTable[i_colIndex].bgrRed = static_cast<uint8_t>(i_colRed);
              vtColourTable[i_colIndex].bgrGreen = static_cast<uint8_t>(i_colGreen);
              vtColourTable[i_colIndex].bgrBlue = static_cast<uint8_t>(i_colBlue);
            }
          }
          else
          {
            errorOccurred = true;
            break;
          }
        }
      }
    }
  }
  return !errorOccurred;
}


void vt2iso_c::lineWrapTextFile( const std::string &destFileName, const std::string &tmpFileName, unsigned int maxLineLen )
{
  FILE *destFile = &save_fopen( destFileName.c_str(), "wt" );
  FILE *srcFile = fopen( tmpFileName.c_str(), "r" );

  if ( destFile && srcFile )
  {
    char *strBuf = new char[maxLineLen];
    if ( !strBuf ) return; // catch out-of-memory-case. (shouldn't really happen for those few bytes)
    --maxLineLen;
    long strBufI = 0;
    long writeToI = 0;
    long bytesRead = fread( strBuf, 1, maxLineLen, srcFile );
    const char wrapChars[] = {' ', ',', '(', '{', '['}; /* list in the order to be used */
    const unsigned int NUM_WRAP_CHARS = sizeof(wrapChars);
    unsigned int i=0; // counters set/used below for different reasons
    unsigned int j=0; // counters set/used below for different reasons
    unsigned int lastWrapCharI[NUM_WRAP_CHARS]; // initialization follows right below
    unsigned int wrapCharI[NUM_WRAP_CHARS]; // initialization follows right below
    bool inStringContext = false; // add additional quote at line end and at line start if in string context
    bool lastWasBackslash = false; // for detecting \" and alike...
    
    for ( i = 0; i < NUM_WRAP_CHARS; ++i )
    {
      lastWrapCharI[i] = maxLineLen;
      wrapCharI[i] = maxLineLen;
    }

    while ( bytesRead > 0 )
    {
      for ( ; strBufI < bytesRead; ++strBufI )
      {
        char charVal = strBuf[strBufI];

        if( ('"' == charVal) && (!lastWasBackslash) )
          inStringContext = !inStringContext;
        
        lastWasBackslash = ('\\' == charVal); 
        
        /* look for places to break the line */
        i = 0;
        for ( ; i < NUM_WRAP_CHARS; ++i )
        {
          if ( charVal == wrapChars[i] )
          {
            if ( (lastWrapCharI[i] + 1) != (unsigned int)strBufI )
            {
              wrapCharI[i] = (unsigned int)strBufI;
            }

            lastWrapCharI[i] = (unsigned int)strBufI;
            break;
          }
        }

        if ( i >= NUM_WRAP_CHARS )
        {
          /* not a line break character */
          if ( charVal == '\n' )
          { /* new line index */
            writeToI = strBufI;
          }
        }
      }


      bool addLineFeed = false;
      if ( writeToI == 0 )
      { /* did not find a new line, check for a break character */
        i = 0;
        for ( ; i < NUM_WRAP_CHARS; ++i )
        {
          if ( wrapCharI[i] < maxLineLen ) /* use the first one in the list */
            break;
        }

        if ( i < NUM_WRAP_CHARS )
        {
          writeToI = wrapCharI[i];
          addLineFeed = true;
        }
        /* else, no break character (this line will be long) */
      }

      if ( writeToI == (bytesRead - 1) || (writeToI == 0) )
      { /* full buffer can be written */
        fwrite( strBuf, 1, bytesRead, destFile );
        bytesRead = fread( strBuf, 1, maxLineLen, srcFile );

        strBufI = 0;
        for ( i = 0; i < NUM_WRAP_CHARS; ++i )
        {
          lastWrapCharI[i] = maxLineLen;
          wrapCharI[i] = maxLineLen;
        }
      }
      else
      { /* partial can be written */
        j = writeToI + 1;
        fwrite( strBuf, 1, writeToI + 1, destFile );

        i = 0;
        for ( ; j < (unsigned int)bytesRead; ++i, ++j )
        { /* copy down the rest of buffer */
          strBuf[i] = strBuf[j];
        }

        /* read in a new block */
        bytesRead = fread( &strBuf[i], 1, maxLineLen - i, srcFile );
        bytesRead += i;
        strBufI = i;

        /* reset the wrap characters */
        for ( i = 0; i < NUM_WRAP_CHARS; ++i )
        {
          if ( wrapCharI[i] < maxLineLen )
          {
            if ( wrapCharI[i] > i )
            {
              lastWrapCharI[i] -= i;
              wrapCharI[i] -= i;
            }
            else
            {
              lastWrapCharI[i] = maxLineLen;
              wrapCharI[i] = maxLineLen;
            }
          }
        }
      }

      writeToI = 0;

      if ( addLineFeed )
      {
        if(inStringContext)
          fwrite( "\"\n\"", 1, 3, destFile );
        else
          fwrite( "\n", 1, 1, destFile );            
      }
    }

    delete [] strBuf;
  }
  /* else, file open error */

  if ( destFile ) fclose( destFile );
  if ( srcFile )  fclose( srcFile );

  if ( destFile && srcFile )
  { /* delete temp file */
    remove( tmpFileName.c_str() );
  }
}


void vt2iso_c::diffFileSave( const std::string &destFileName, const std::string &tempFileName )
{
  if ( existsFile( destFileName ) )
  {
    bool filesMatch = false;
    FILE* destFile = fopen( destFileName.c_str(), "rb" );
    FILE* srcFile = fopen( tempFileName.c_str(), "rb" );

    if ( destFile && srcFile )
    {
      ( void ) fseek( destFile, 0, SEEK_END );
      long destLen = ftell( destFile );

      ( void ) fseek( srcFile, 0, SEEK_END );
      long srcLen = ftell( srcFile );

      char *destBuf = new char[destLen + 2];
      char *srcBuf = new char[srcLen + 2];

      if ( destBuf && srcBuf )
      {
        ( void ) fseek( destFile, 0, SEEK_SET );
        size_t r1 = fread( destBuf, 1, destLen, destFile); ( void ) r1;
        destBuf[destLen++] = '\n';
        destBuf[destLen] = '\0';

        ( void ) fseek( srcFile, 0, SEEK_SET );
        size_t r2 = fread( srcBuf, 1, srcLen, srcFile); ( void ) r2;
        srcBuf[srcLen++] = '\n';
        srcBuf[srcLen] = '\0';

        long destStart = 0;
        long destI = 0;

        long srcStart = 0;
        long srcI = 0;

        filesMatch = true;
        while ( (destI < destLen) && (srcI < srcLen) )
        {
          /* find end of line */
          char charVal = destBuf[destI];
          while ( (charVal != '\r') && (charVal != '\n') )
          {
            charVal = destBuf[++destI];
          }
          /* terminate string */
          destBuf[destI++] = '\0';
          if ( (charVal == '\r') && (destBuf[destI] == '\n') )
            ++destI; /* carriage return line feed combo */

          /* find end of line */
          charVal = srcBuf[srcI];
          while ( (charVal != '\r') && (charVal != '\n') )
          {
            charVal = srcBuf[++srcI];
          }
          /* terminate string */
          srcBuf[srcI++] = '\0';
          if ( (charVal == '\r') && (srcBuf[srcI] == '\n') )
            ++srcI; /* carriage return line feed combo */

          if ( strcmp( &destBuf[destStart], &srcBuf[srcStart] ) == 0 )
          { /* move on to next line */
            destStart = destI;
            srcStart = srcI;
          }
          else
          { /* no match, quit */
            filesMatch = false;
            break;
          }
        }
      }

      if ( destBuf )  delete [] destBuf;
      if ( srcBuf )   delete [] srcBuf;
    }

    if ( destFile ) fclose( destFile );
    if ( srcFile )  fclose( srcFile );

    if ( filesMatch )
    {
      remove( tempFileName.c_str() );
    }
    else
    {
      remove( destFileName.c_str() );
      rename( tempFileName.c_str(), destFileName.c_str() );
    }
  }
  else
  { /* destination does not exist, just rename the temp */
    rename( tempFileName.c_str(), destFileName.c_str() );
  }
}


bool
checkForLanguageFile( const char *name, const char *langCode )
{
  // check filename length is exactly the right size
  if ( strlen( name ) != ( 2 + 1 + 3 ) ) { // code + dot + vtx
    return false;
  }

  // check first two chars for our language code
  if ( 0 != strncmp( name, langCode, 2 ) ) {
    return false;
  }

  return true;  // That is the one we want
}

bool
isThisMyLanguageFile( const char *name, language_s& a_lang )
{
  // Simple validation first
  if (!checkForLanguageFile(name, a_lang.code)) {
    return false;
  }

  // check postfix for vtl  - most our languages
  if ( 0 == strncmp( ".vtl", name + strlen( name ) - 4,  4 ) ) {
    a_lang.unicode = false;
    return true;
  }

  // check postfix for vtu - for CJKV - Multibyte languages
  if ( 0 == strncmp( ".vtu", name + strlen( name ) - 4,  4 ) ) {
    a_lang.unicode = true;
    return true;
  }

  return false;
}

std::list<std::string>
vt2iso_c::scanLanguageFilesOS( language_s& a_lang )
{
  std::list<std::string> files;

  for ( std::list<Path_s>::iterator p = l_dictionaryPath.begin(); p != l_dictionaryPath.end(); ++p ) {

    std::string str_tmpWorkDir = p->b_relativePath ? mstr_sourceDir : "";

    DIR *dir;
    struct dirent *ent;
    dir = opendir(( str_tmpWorkDir + scc_dirSeparatorCorrect + p->str_pathName ).c_str() );
    if ( dir != NULL ) {

      while (( ent = readdir( dir ) ) != NULL ) {

        if ( !isThisMyLanguageFile( ent->d_name, a_lang ) )
          continue;

        std::string langFileName = str_tmpWorkDir + p->str_pathName + scc_dirSeparatorCorrect + std::string( ent->d_name );
        files.push_front( langFileName );
        break;   // found the desired language, lets get on with it 
      }

      closedir( dir );
    }
    else
    { /* could not open directory */
      perror( "" );
      files.clear(); // this is an error - we should not use partial results from file search
      break;
    }
  }
  return files;
}


std::list<std::string>
vt2iso_c::scanLanguageFiles( language_s& a_lang )
{
  if ( l_dictionaryPath.empty() ) {
    std::cerr << "Warning: empty dictionary path!";
    return std::list<std::string>();
  }
  return scanLanguageFilesOS( a_lang );
}


void vt2iso_c::readLanguageFilesToBuffer( std::list<std::string> a_languageFiles, language_s& a_lang ) {

  std::vector<char> buffer;

  // read files
  while ( ! a_languageFiles.empty() ) {

    if ( ! mb_silentMode )
      std::cout << "    opening language file "<< a_languageFiles.front().c_str() << std::endl;

    typedef std::istream_iterator<char> istream_iterator;
    std::ifstream file( a_languageFiles.front().c_str() );

    if ( file.is_open() ) {
      file >> std::noskipws;
      std::copy( istream_iterator( file ), istream_iterator(), std::back_inserter( buffer ) );
      buffer.push_back( 0x00 );
      file.close();
    } else {
      std::cout << "    could not open " << a_languageFiles.front().c_str() << " !" << std::endl;
    }

    a_languageFiles.pop_front();
  }


  // polish and copy buffer
  if ( buffer.empty() ) {
    a_lang.valueBufferLen = 0;
    a_lang.valueBuffer = NULL;

  } else {

    // replace all newline chars with null
    const char n = 0;
    std::replace_if( buffer.begin(), buffer.end(), vt2iso_c::isEol, n );

    a_lang.valueBufferLen = buffer.size();
    a_lang.valueBuffer = new( char [a_lang.valueBufferLen] );
    std::copy( buffer.begin(), buffer.end(), a_lang.valueBuffer ) ;
  }

}


bool vt2iso_c::isEol( const char c ) {
  return (( c == '\n' ) || ( c == '\r' ) );
}


/*
This function finds the first occurrence of the wide character wc in the 
initial size wide characters of the object beginning at block. The return 
value is a pointer to the located wide character, or a null pointer if 
no match was found. 
*/
UCS2* vt2iso_c::findWideChar(const UCS2 *s, UCS2 wc, size_t n)
{
  size_t i;

  for (i = 0; i < n; i++) {
    if (*s == wc) {
      return (UCS2 *)s;
    }
    s++;
  }
  return NULL;
}

