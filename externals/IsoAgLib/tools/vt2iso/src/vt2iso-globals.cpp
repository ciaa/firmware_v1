/*
  vt2iso-globals.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "vt2iso-globals.hpp"
#include <sstream>
#include <list>
#include <stdlib.h>


using namespace std;


bool itogeneraloption(uint8_t ui8_options, std::string& c_outputText, uint8_t ui8_numEntries, char* pchar_table )
{
  uint16_t pos;
  bool b_addPlus = false;
  c_outputText.clear();

  for (pos=0 ; pos < ui8_numEntries; pos++)
  {
    if (ui8_options & (1<<pos))
    {
      if (b_addPlus) c_outputText.append( "+" );
      b_addPlus = true;

      // multidimensional array access!
      c_outputText.append( &pchar_table[pos*(stringLength+1)] );
    }
  }
  if (!b_addPlus) c_outputText.append( "none" );

  return true;
}

void parseOptionsIntoList(const string& arc_option, list<string>& arl_tokens)
{
  arl_tokens.clear();
  std::string str_argument(arc_option);
  while (!str_argument.empty())
  {
    size_t found = str_argument.find_first_of('+');
    if (str_argument.substr(0, found).compare("none") != 0)
      // skip "none" options
      arl_tokens.push_back(str_argument.substr(0, found));

    if (string::npos == found)
      break; // last item

    str_argument = str_argument.substr(found + 1); // skip '+';
  }
}

int colourtoi (const char* text_colour)
{
  int l;
  for (l=0; l<maxColourTable; l++) {
    if (strncmp (text_colour, colourTable [l], stringLength) == 0) {
      return l;
    }
  }

  char* p_checkChar = NULL;
  int i_colIndex = strtol(text_colour, &p_checkChar, 10);
  if (*p_checkChar == '\0')
  {
    if ((i_colIndex >=0) && (i_colIndex < 256)) {
      if (i_colIndex != 0 || text_colour[0] == '0') {
        return i_colIndex;
      }
    }
  }
  std::cout << "INVALID Colour '" << text_colour << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
  return -1;
}

bool itocolour(unsigned int ui_index, string& c_outputText)
{
  if(ui_index < maxColourTable )
  {
   c_outputText = colourTable[ui_index];

  }
  else
  {
    char c_tmp_buf[stringLength];
    sprintf(c_tmp_buf,"%d",ui_index);
    c_outputText = c_tmp_buf;
  }
  return true;
}

int masktypetoi (const char* masktype)
{
  int l;
  for (l=0; l<3; l++) {
    if (strncmp (masktype, masktypeTable [l], stringLength) == 0) {
      return l;
    }
  }

  char* p_checkChar = NULL;
  int i_maskType = strtol(masktype, &p_checkChar, 10);
  if (*p_checkChar == '\0')
  {
    if ((i_maskType >=0) && (i_maskType < 3)) {
      if (i_maskType != 0 || masktype[0] == '0') {
        return i_maskType;
      }
    }
  }

  std::cout << "INVALID MASK TYPE '" << masktype << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
  return -1;
}

bool itomasktype(uint8_t ui8_index, std::string& c_outputText)
{
  if(ui8_index < 3 )
  {
    c_outputText = masktypeTable[ui8_index];
    return true;
  }
  return false;
}

int colourdepthtoi (const char* text_colourdepth)
{
  int l;
  for (l=0; l<3; l++) {
    if (text_colourdepth [0] == colourDepthTable [l]) {
      return l;
    }
  }

  char* p_checkChar = NULL;
  int i_colDepth = strtol(text_colourdepth, &p_checkChar, 10);
  if (*p_checkChar == '\0')
  {
    if ((i_colDepth >=0) && (i_colDepth < 3)) {
      if (i_colDepth != 0 || text_colourdepth[0] == '0') {
        return i_colDepth;
      }
    }
  }

  std::cout << "INVALID Colour DEPTH'" << text_colourdepth << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
  return -1;
}

bool itocolourdepth(uint8_t ui8_options, std::string& c_outputText)
{
  c_outputText.clear();
  switch (ui8_options) {
    case 0: c_outputText="1bit"; break;
    case 1: c_outputText="4bit"; break;
    case 2: c_outputText="8bit"; break;
    default: return false;
  }
  return true;
}


signed int fonttypetoi (const char* text_fonttype, unsigned int aui_revision)
{
  int l;
  for (l=0; l<maxFonttypeTable; l++) {
    if (strncmp (text_fonttype, fonttypeTable [l], stringLength) == 0) {
      if (l == maxFonttypeTable-1) return 0xFF; // map "proprietary" to 0xFF.
      if ( ( aui_revision < 4) && l > 1)
      {
        std::cout << "INVALID FONT TYPE '" << text_fonttype << "' ENCOUNTERED FOR PRIOR VERSION 4! STOPPING PARSER! bye.\n\n";
        return -1;
      }
      return l;
    }
  }

  char* p_checkChar = NULL;
  int i_fontType = strtol(text_fonttype, &p_checkChar, 10);
  if (*p_checkChar == '\0')
  {
    if (aui_revision < 4)
    {
      switch (i_fontType)
      {
        case 0:
        case 1:
        case 255:
          return i_fontType;
        default:
          std::cout << "INVALID FONT TYPE '" << text_fonttype << "' FOR PRIOR VERSION 4 ENCOUNTERED! STOPPING PARSER! bye.\n\n";
          return -1;
      }
    }
    else
    {
      if ((i_fontType < 0) || (i_fontType > 255))
      {
        std::cout << "INVALID FONT TYPE '" << text_fonttype << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
        return -1;
      }
    }
    return i_fontType;
  }
  std::cout << "INVALID FONT TYPE '" << text_fonttype << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
  return -1;
}


bool itofonttype(unsigned int ui_index, string& c_outputText)
{

  if(ui_index < 8 )
  {
    char c_tmpBuf[stringLength];
    sprintf(c_tmpBuf,"%d",ui_index);
    c_outputText = c_tmpBuf;
  }
  else if( ui_index == 255)
  {
    c_outputText = "proprietary";
  }
  else
  {
    if(ui_index < maxFonttypeTable )
    {
      c_outputText = fonttypeTable[ui_index];
    }
    else return false;
  }
  return true;
}


signed int booltoi (const char *text_bool)
{
  int l;
  for (l=0; l<maxTruthTable; l++)
  {
    if (strncmp (text_bool, truthTable [l], stringLength) == 0)
    {
      return 1;
    }
  }
  for (l=0; l<maxFalseTable; l++)
  {
    if (strncmp (text_bool, falseTable [l], stringLength) == 0)
    {
      return 0;
    }
  }
  std::cout << "INVALID TRUTH VALUE '" << text_bool << " ENCOUNTERED! STOPPING PARSER! bye.\n\n";
  return -1;
}

signed int fontsizetoi (const char *text_fontsize)
{
  int l;
  for (l=0; l<maxFontsizeTable; l++) {
    if (strncmp (text_fontsize, fontsizeTable [l], stringLength) == 0) {
      return l;
    }
  }
  std::cout << "INVALID FONT SIZE '" << text_fontsize << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
  return -1;
}

bool itofontsize(unsigned int ui_index, string& c_outputText)
{
  if(ui_index < maxFontsizeTable )
  {
    c_outputText = fontsizeTable[ui_index];
    return true;
  }
  return false;
}


signed int formattoi (const char *text_format)
{
  int l;
  for (l=0; l<maxFormatTable; l++) {
    if (strncmp (text_format, formatTable [l], stringLength) == 0) {
      return l;
    }
  }
  std::cout << "INVALID FORMAT '" << text_format << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
  return -1;
}

bool itoformat(unsigned int ui_index, string& c_outputText)
{
  if(ui_index < maxFormatTable )
  {
    c_outputText = formatTable[ui_index];
    return true;
  }
  return false;
}

signed int horizontaljustificationtoi (const char *text_horiz)
{
  for (int l=0; l<maxHorizontalJustificationTable; l++)
  {
    if (strncmp (text_horiz, horizontalJustificationTable [l], stringLength) == 0) {
      return l;
    }
  }
  std::cout << "INVALID HORIZONTALJUSTIFICATION '" << text_horiz << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
  return -1;
}

signed int
verticaljustificationtoi (const char *text_vert, unsigned int aui_revision)
{
  if (aui_revision < 4)
    return 0x00; // version 2+3 does not support vertical justification -> set to 0x00.

  int l;
  for (l=0; l<maxVerticalJustificationTable; l++)
  {
    if (strncmp (text_vert, verticalJustificationTable [l], stringLength) == 0)
    {
      return l;
    }
  }
  std::cout << "INVALID VERTICALJUSTIFICATION '" << text_vert << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
  return -1;
}

bool itohorizontaljustification(unsigned int ui_index, string& c_outputText)
{
  if(ui_index < maxHorizontalJustificationTable )
  {
    c_outputText = horizontalJustificationTable[ui_index];
    return true;
  }
  return false;
}

bool itoverticaljustification(unsigned int ui_index, string& c_outputText)
{
  if(ui_index < maxVerticalJustificationTable )
  {
    c_outputText = verticalJustificationTable[ui_index];
    return true;
  }
  return false;
}

signed int stringoptionstoi (const char *text_options, unsigned int aui_revision)
{
  int l, retval=0;

  std::list<std::string> l_tokens;
  parseOptionsIntoList(text_options, l_tokens);

  for (std::list<std::string>::const_iterator iter = l_tokens.begin(); iter != l_tokens.end(); ++iter)
  {
    for (l=0; l<maxStringOptionsTable; l++)
    {
      if (iter->compare(stringOptionsTable [l]) == 0)
      {
        retval += (uint64_t(1)<<l);
        break;
      }
    }
    if (l == maxStringOptionsTable) // error
    {
      std::cout << "INVALID STRING OPTION '" << text_options << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
      return -1;
    }
  }

  return (aui_revision < 4)
      ? (retval & 0x03) // version 2+3
      : retval;         // version 4 upwards
}

bool itostringoptions(uint8_t ui8_options, std::string& c_outputText)
{
  return itogeneraloption(ui8_options, c_outputText, maxStringOptionsTable, &stringOptionsTable[0][0]);
}

signed int inputnumberoptionstoi (const char *text_options, unsigned int aui_revision)
{
  int l, retval=0;

  std::list<std::string> l_tokens;
  parseOptionsIntoList(text_options, l_tokens);

  for (std::list<std::string>::const_iterator iter = l_tokens.begin(); iter != l_tokens.end(); ++iter)
  {
    for (l=0; l<maxInputNumberOptionsTable; l++)
    {
      if (iter->compare(inputNumberOptionsTable [l]) == 0)
      {
        retval += (uint64_t(1)<<l);
        break;
      }
    }
    if (l == maxInputNumberOptionsTable) // error
    {
      std::cout << "INVALID INPUT NUMBER OPTION '" << text_options << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
      return -1;
    }
  }
  return (aui_revision < 4)
      ? (retval & 0x07) // version 2+3
      : retval;         // version 4 upwards
}

bool itoinputnumberoptions(uint8_t ui8_options, std::string& c_outputText)
{
  return itogeneraloption(ui8_options, c_outputText, maxInputNumberOptionsTable, &inputNumberOptionsTable[0][0]);
}


signed int numberoptionstoi (const char *text_options, unsigned int aui_revision)
{
  int l, retval=0;

  std::list<std::string> l_tokens;
  parseOptionsIntoList(text_options, l_tokens);

  for (std::list<std::string>::const_iterator iter = l_tokens.begin(); iter != l_tokens.end(); ++iter)
  {
    for (l=0; l<maxOutputNumberOptionsTable; l++)
    {
      if (iter->compare(outputNumberOptionsTable [l]) == 0)
      {
        retval += (uint64_t(1)<<l);
        break;
      }
    }
    if (l == maxOutputNumberOptionsTable) // error
    {
      std::cout << "INVALID OUTPUT NUMBER OPTION '" << text_options << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
      return -1;
    }
  }
  return (aui_revision < 4)
      ? (retval & 0x07) // version 2+3
      : retval;         // version 4 upwards
}

bool itonumberoptions(uint8_t ui8_options, std::string& c_outputText)
{
  return itogeneraloption(ui8_options, c_outputText, maxOutputNumberOptionsTable, &outputNumberOptionsTable[0][0]);
}

signed int picturegraphicoptionstoi (const char *text_options)
{
  int l, retval=0;

  std::list<std::string> l_tokens;
  parseOptionsIntoList(text_options, l_tokens);

  for (std::list<std::string>::const_iterator iter = l_tokens.begin(); iter != l_tokens.end(); ++iter)
  {
    for (l=0; l<maxPictureGraphicOptionsTable; l++)
    {
      if (iter->compare(pictureGraphicOptionsTable [l]) == 0)
      {
        retval += (uint64_t(1)<<l);
        break;
      }
    }
    if (l == maxPictureGraphicOptionsTable) // error
    {
      std::cout << "INVALID PICTUREGRAPHIC OPTION '" << text_options << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
      return -1;
    }
  }

  return retval;
}

bool itopicturegraphicoptions(uint8_t ui8_options, std::string& c_outputText)
{
  return itogeneraloption(ui8_options, c_outputText, maxPictureGraphicOptionsTable, &pictureGraphicOptionsTable[0][0]);
}


signed int picturegraphicrletoi (const char *text_options)
{
  int l, retval=0;

  std::list<std::string> l_tokens;
  parseOptionsIntoList(text_options, l_tokens);

  for (std::list<std::string>::const_iterator iter = l_tokens.begin(); iter != l_tokens.end(); ++iter)
  {
    for (l=0; l<maxPictureGraphicRleTable; l++)
    {
      if (iter->compare(pictureGraphicRleTable [l]) == 0)
      {
        retval += (uint64_t(1)<<l);
        break;
      }
    }
    if (l == maxPictureGraphicRleTable) // error
    {
      std::cout << "INVALID PICTUREGRAPHICRLE OPTION '" << text_options << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
      return -1;
    }
  }

  return retval;
}

bool itopicturegraphicrle(uint8_t ui8_options, std::string& c_outputText)
{
  return itogeneraloption(ui8_options, c_outputText, maxPictureGraphicRleTable, &pictureGraphicRleTable[0][0]);
}


signed int meteroptionstoi (const char *text_options)
{
  int l, retval=0;

  std::list<std::string> l_tokens;
  parseOptionsIntoList(text_options, l_tokens);

  for (std::list<std::string>::const_iterator iter = l_tokens.begin(); iter != l_tokens.end(); ++iter)
  {
    for (l=0; l<maxMeterOptionsTable; l++)
    {
      if (iter->compare(meterOptionsTable [l]) == 0)
      {
        retval += (uint64_t(1)<<l);
        break;
      }
    }
    if (l == maxMeterOptionsTable) // error
    {
      std::cout << "INVALID METER OPTION '" << text_options << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
      return -1;
    }
  }

  return retval;
}

bool itometeroptions(uint8_t ui8_options, string& c_outputText)
{
  return itogeneraloption(ui8_options, c_outputText, maxMeterOptionsTable, &meterOptionsTable[0][0]);
}


signed int linearbargraphoptionstoi (const char *text_options)
{
  int l, retval=0;

  std::list<std::string> l_tokens;
  parseOptionsIntoList(text_options, l_tokens);

  for (std::list<std::string>::const_iterator iter = l_tokens.begin(); iter != l_tokens.end(); ++iter)
  {
    for (l=0; l<maxLinearBarGraphOptionsTable; l++)
    {
      if (iter->compare(linearBarGraphOptionsTable [l]) == 0)
      {
        retval += (uint64_t(1)<<l);
        break;
      }
    }
    if (l ==maxLinearBarGraphOptionsTable) // error
    {
      std::cout << "INVALID LINEARBARGRAPH OPTION '" << text_options << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
      return -1;
    }
  }

  return retval;
}

bool itolinearbargraphoptions(uint8_t ui8_options, std::string& c_outputText)
{
  return itogeneraloption(ui8_options, c_outputText, maxLinearBarGraphOptionsTable, &linearBarGraphOptionsTable[0][0]);
}

signed int archedbargraphoptionstoi (const char *text_options)
{
  int l, retval=0;

  std::list<std::string> l_tokens;
  parseOptionsIntoList(text_options, l_tokens);

  for (std::list<std::string>::const_iterator iter = l_tokens.begin(); iter != l_tokens.end(); ++iter)
  {
    for (l=0; l<maxArchedBarGraphOptionsTable; l++)
    {
      if (iter->compare(archedBarGraphOptionsTable [l]) == 0)
      {
        retval += (uint64_t(1)<<l);
        break;
      }
    }
    if (l == maxArchedBarGraphOptionsTable) // error
    {
      std::cout << "INVALID ARCHEDBARGRAPH OPTION '" << text_options << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
      return -1;
    }
  }

  return retval;
}

bool itoarchedbargraphoptions(uint8_t ui8_options, std::string& c_outputText)
{
  return itogeneraloption(ui8_options, c_outputText, maxArchedBarGraphOptionsTable, &archedBarGraphOptionsTable[0][0]);
}

signed int prioritytoi (const char *text_priority)
{
  int l;
  for (l=0; l<maxPriorityAcousticSignalTable-1; l++)
  {
    if (strncmp (text_priority, priorityAcousticSignalTable [l], stringLength) == 0)
    {
      return l;
    }
  }
  std::cout << "INVALID PRIORITY '" << text_priority << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
  return -1;
}

bool itopriority(unsigned int ui_index, string& c_outputText)
{
  if(ui_index < maxPriorityAcousticSignalTable )
  {
    c_outputText = priorityAcousticSignalTable[ui_index];
    return true;
  }
  return false;
}


signed int acousticsignaltoi (const char *text_acousticsignal)
{
  int l;
  for (l=0; l<maxPriorityAcousticSignalTable; l++) {
    if (strncmp (text_acousticsignal, priorityAcousticSignalTable [l], stringLength) == 0) {
      return l;
    }
  }
  std::cout << "INVALID ACOUSTIC SIGNAL '" << text_acousticsignal << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
  return -1;
}

bool itoacousticsignal(unsigned int ui_index,string& c_outputText)
{
  if(ui_index < maxPriorityAcousticSignalTable )
  {
    c_outputText = priorityAcousticSignalTable[ui_index];
    return true;
  }
  return false;
}

signed int fontstyletoi (const char *text_fontstyle)
{
  int l, retval=0;

  std::list<std::string> l_tokens;
  parseOptionsIntoList(text_fontstyle, l_tokens);

  // first check if all values are correct:
  for (std::list<std::string>::const_iterator iter = l_tokens.begin(); iter != l_tokens.end(); ++iter)
  {
    for (l=0; l<maxFontstyleTable; l++)
    {
      if (iter->compare(fontstyleTable [l]) == 0)
      {
        retval += (uint64_t(1)<<l);
        break;
      }
    }
    if (l == maxFontstyleTable) // error
    {
      std::cout << "INVALID FONTSTYLE OPTION '" << text_fontstyle << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
      return -1;
    }
  }

  return retval;
}

bool itofontstyle(uint8_t ui8_options, std::string& c_outputText)
{ // the additional check in the above function is not necessary
  return itogeneraloption(ui8_options, c_outputText, maxFontstyleTable, &fontstyleTable[0][0]);
}

signed int linedirectiontoi (const char *text_linedirection)
{
  if (strcmp (text_linedirection, "bottomlefttotopright") == 0)
    return 1;
  if (strcmp (text_linedirection, "toplefttobottomright") == 0)
    return 0;

  if (strlen(text_linedirection))
  {
    std::cout << "INVALID LINEDIRECTION '" << text_linedirection << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
    return -1;
  }

  return 0;
}

bool itolineardirection(unsigned int ui_index, string& c_outputText)
{
  char c_tmpBuf[stringLength];
  if(ui_index == 0)
  {
    sprintf(c_tmpBuf,"%s","toplefttobottomright");
    c_outputText = c_tmpBuf;
  }
  else if(ui_index == 1)
  {
    sprintf(c_tmpBuf,"%s","bottomlefttotopright");
    c_outputText = c_tmpBuf;
  }
  else return false;

  return true;
}


signed int linearttoi (const char *text_lineart)
{
  int retval=0;
  char thischar;
  while ((thischar = *text_lineart) != 0x00) {
    retval <<= 1;
    if (thischar == '1')
      retval |= 0x0001;
    else if (thischar == '0')
      ; // nothing to do
    else
    {
      std::cout << "INVALID LINEART '" << text_lineart << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
      return -1;
    }

    text_lineart++;
  }
  return retval;
}

bool itolineart(int i_lineart, std::string& c_outputText)
{
  c_outputText.clear();
  for (int8_t i8_cnt = 15; i8_cnt >= 0; i8_cnt--)
  {
    if (i_lineart & (1<<i8_cnt))
      c_outputText.append("1");
    else
      c_outputText.append("0");
  }
  return true;
}

signed int linesuppressiontoi (const char *text_linesuppression)
{
  int l, retval=0;

  std::list<std::string> l_tokens;
  parseOptionsIntoList(text_linesuppression, l_tokens);

  for (std::list<std::string>::const_iterator iter = l_tokens.begin(); iter != l_tokens.end(); ++iter)
  {
    for (l=0; l<maxLineSuppressionTable; l++)
    {
      if (iter->compare(lineSuppressionTable [l]) == 0)
      {
        retval += (uint64_t(1)<<l);
        break;
      }
    }
    if (l == maxLineSuppressionTable) // error
    {
      std::cout << "INVALID LINESUPPRESSION '" << text_linesuppression << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
      return -1;
    }
  }

  return retval;
}

bool itolinesuppression(uint8_t ui8_options, std::string& c_outputText)
{
  return itogeneraloption(ui8_options, c_outputText, maxLineSuppressionTable, &lineSuppressionTable[0][0]);
}

signed int
ellipsetypetoi (const char *text_ellipsetype)
{
  int l;
  for (l=0; l<maxEllipseTypeTable; l++)
  {
    if (strcmp(text_ellipsetype, ellipseTypeTable [l]) == 0)
    {
      return l;
    }
  }
  std::cout << "INVALID ELLIPSETYPE '" << text_ellipsetype << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
  return -1;
}

bool itoellipsetype(unsigned int ui_index, string& c_outputText)
{
  if(ui_index < maxEllipseTypeTable )
  {
    c_outputText = ellipseTypeTable[ui_index];
    return true;
  }
  return false;
}

signed int polygontypetoi (const char *text_polygontype)
{
  int l;
  for (l=0; l<maxPolygonTypeTable; l++)
  {
    if (strcmp (text_polygontype, polygonTypeTable [l]) == 0)
    {
      return l;
    }
  }
  std::cout << "INVALID POLYGONTYPE '" << text_polygontype << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
  return -1;
}

bool itopolygontype(unsigned int ui_index, string& c_outputText)
{
  if(ui_index < maxPolygonTypeTable )
  {
    c_outputText = polygonTypeTable[ui_index];
    return true;
  }
  return false;
}


signed int validationtypetoi (const char *text_validationtype)
{
  if (strcmp (text_validationtype, "valid_characters") == 0)
    return 0;
  if (strcmp (text_validationtype, "invalid_characters") == 0)
    return 1;

  if (strlen(text_validationtype))
  {
    std::cout << "INVALID VALIDATIONTYPE '" << text_validationtype << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
    return -1;
  }
  return 0;
}

bool itovalidationtype(unsigned int ui_index, string& c_outputText)
{
  char c_tmpBuf[stringLength];

  if(ui_index == 0)
  {
    sprintf(c_tmpBuf,"%s","valid_characters");
    c_outputText = c_tmpBuf;
  }
  else if(ui_index == 1)
  {
    sprintf(c_tmpBuf,"%s","invalid_characters");
    c_outputText = c_tmpBuf;
  }
  else return false;

 return true;
}

signed int filltypetoi (const char *text_filltype)
{
  int l;
  for (l=0; l<maxFillTypeTable; l++)
  {
    if (strcmp (text_filltype, fillTypeTable [l]) == 0)
    {
      return l;
    }
  }
  std::cout << "INVALID FILLTYPE '" << text_filltype << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
  return -1;
}

bool itofilltype(unsigned int ui_index, string& c_outputText)
{
  if(ui_index < maxFillTypeTable )
  {
    c_outputText = fillTypeTable[ui_index];
    return true;
  }
  return false;
}

signed int eventtoi (const char *text_eventName)
{
  int l;
  for (l=0; l<maxEventTable; l++) {
    if (strcmp (text_eventName, eventTable [l]) == 0) {
      return (l + 1);
    }
  }

  char* p_checkChar = NULL;
  int i_eventIndex = strtol(text_eventName, &p_checkChar, 10);
  if (*p_checkChar == '\0')
  {
    if ( ((i_eventIndex >0) && (i_eventIndex <= 26))
          || ((i_eventIndex >= 240) && (i_eventIndex <= 254))
        )
      return i_eventIndex;
  }

  std::cout << "INVALID EVENT '" << text_eventName << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
  return -1; // should not happen
}

bool itoevent(unsigned int ui_index, string& c_outputText)
{
  if(ui_index < maxEventTable && (ui_index > 0) )
  { // event index starts from 1!
    c_outputText = eventTable[ui_index-1];
    return true;
  }
  if ((ui_index >= 240) && (ui_index <= 254))
  {
    std::ostringstream oss (std::stringstream::out);
    oss << ui_index;
    c_outputText = oss.str();
    return true;
  }
  return false;
}

signed int auxfunctiontypetoi(const char *text_auxFunctionType)
{
  int l;
  for (l=0; l<maxAuxFunctionTypes; l++)
  {
    if (strcmp (text_auxFunctionType, auxFunctionTypeTable [l]) == 0)
    {
      return l;
    }
  }
  std::cout << "INVALID AUXFUNCTIONTYPE '" << text_auxFunctionType << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
  return -1;
}

bool itoauxfunctiontype(unsigned int ui_index, string& c_outputText)
{
  if(ui_index < maxAuxFunctionTypes )
  {
    c_outputText = auxFunctionTypeTable[ui_index];
    return true;
  }
  return false;
}


signed int aux2functiontypetoi(const char *text_auxFunction2Type)
{
  int l;
  for (l=0; l<maxAuxFunction2Types; ++l)
  {
    if (strcmp (text_auxFunction2Type, auxFunction2TypeTable [l]) == 0)
    {
      return l;
    }
  }

  std::cout << "INVALID AUXFUNCTION2TYPE '" << text_auxFunction2Type << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
  return -1;
}

bool itoaux2functiontype(unsigned int ui_index, string& c_outputText)
{
  ui_index &= 0x1F;

  if ( ui_index < maxAuxFunction2Types )
  {
    c_outputText = auxFunction2TypeTable[ui_index];

    return true;
  }

  return false;
}


signed int auxfunction2optionstoi(const char *text_auxFunction2Type)
{
  int l, retval=0;

  std::list<std::string> l_tokens;
  parseOptionsIntoList(text_auxFunction2Type, l_tokens);

  for (std::list<std::string>::const_iterator iter = l_tokens.begin(); iter != l_tokens.end(); ++iter)
  {
    for (l=0; l<maxAuxFunction2Options; l++)
    {
      if (iter->compare(auxFunction2OptionsTable [l]) == 0)
      {
        retval += (uint64_t(1)<<l);
        break;
      }
    }
    if (l == maxAuxFunction2Options) // error
    {
      std::cout << "INVALID AUXFUNCTION2OPTION '" << text_auxFunction2Type << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
      return -1;
    }
  }

  return retval << 5;
}

signed int auxinput2optionstoi(const char *text_auxInput2Options)
{
  int l, retval=0;

  std::list<std::string> l_tokens;
  parseOptionsIntoList(text_auxInput2Options, l_tokens);

  for (std::list<std::string>::const_iterator iter = l_tokens.begin(); iter != l_tokens.end(); ++iter)
  {
    for (l=0; l<maxAuxInput2Options; l++)
    {
      if (iter->compare(auxInput2OptionsTable [l]) == 0)
      {
        retval += (uint64_t(1)<<l);
        break;
      }
    }
    if (l == maxAuxInput2Options) // error
    {
      std::cout << "INVALID AUXINPUT2OPTION '" << text_auxInput2Options << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
      return -1;
    }
  }

  return retval << 5;
}

bool itoauxfunction2options(unsigned int ui_index, string& c_outputText)
{
  uint8_t ui8_options = ui_index >> 5;
  
  return itogeneraloption(ui8_options, c_outputText, maxAuxFunction2Options, &auxFunction2OptionsTable[0][0]);
}

bool itoauxinput2options(unsigned int ui_index, string& c_outputText)
{
  uint8_t ui8_options = ui_index >> 5;
  
  return itogeneraloption(ui8_options, c_outputText, maxAuxInput2Options, &auxInput2OptionsTable[0][0]);
}

signed int auxcondesignobjptrtypetoi(const char *text_auxConDesignObjPtrType)
{
  int l;
  for (l=0; l<maxAuxConDesignObjPtrTypes; ++l)
  {
    if (strcmp (text_auxConDesignObjPtrType, auxConDesignObjPtrTypeTable [l]) == 0)
    {
      return l;
    }
  }
  std::cout << "INVALID AUXCONDESIGNOBJPTRTYPE '" << text_auxConDesignObjPtrType << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
  return -1;
}

bool itoauxcondesignobjptrtype(unsigned int ui_index, std::string& c_outputText)
{
  if(ui_index < maxAuxConDesignObjPtrTypes )
  {
    c_outputText = auxConDesignObjPtrTypeTable[ui_index];
    return true;
  }
  return false;
}



signed int keygroupoptionstoi (const char *text_options)
{
  int l, retval=0;

  std::list<std::string> l_tokens;
  parseOptionsIntoList(text_options, l_tokens);

  for (std::list<std::string>::const_iterator iter = l_tokens.begin(); iter != l_tokens.end(); ++iter)
  {
    for (l=0; l<maxKeyGroupOptionsTable; l++)
    {
      if (iter->compare(keyGroupOptionsTable [l]) == 0)
      {
        retval += (uint64_t(1)<<l);
        break;
      }
    }
    if (l == maxKeyGroupOptionsTable) // error
    {
      std::cout << "INVALID KEY GROUP OPTION '" << text_options << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
      return -1;
    }
  }

  return retval;
}

bool itokeygroupoptions(uint8_t ui8_options, string& c_outputText)
{
  return itogeneraloption(ui8_options, c_outputText, maxKeyGroupOptionsTable, &keyGroupOptionsTable[0][0]);
}



signed int gcoptionstoi (const char *text_options)
{
  int l, retval=0;

  std::list<std::string> l_tokens;
  parseOptionsIntoList(text_options, l_tokens);

  for (std::list<std::string>::const_iterator iter = l_tokens.begin(); iter != l_tokens.end(); ++iter)
  {
    for (l=0; l<maxGCOptions; l++)
    {
      if (iter->compare(GCOptionsTable [l]) == 0)
      {
        retval += (uint64_t(1)<<l);
        break;
      }
    }
    if (l == maxGCOptions) // error
    {
      std::cout << "INVALID GRAPHICCONTEXT OPTION '" << text_options << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
      return -1;
    }
  }

  return retval;
}

bool itogcoptions(uint8_t ui8_options, string& c_outputText)
{
  return itogeneraloption(ui8_options, c_outputText, maxGCOptions, &GCOptionsTable[0][0]);
}

signed int inputobjectoptiontoi (const char *text_inputobjectoptions, unsigned int aui_revision)
{
  int l, retval=0;

  std::list<std::string> l_tokens;
  parseOptionsIntoList(text_inputobjectoptions, l_tokens);

  for (std::list<std::string>::const_iterator iter = l_tokens.begin(); iter != l_tokens.end(); ++iter)
  {
    for (l=0; l<maxInputObjectOptionsTable; l++)
    {
      if (iter->compare(inputobjectOptionsTable [l]) == 0)
      {
        retval |= (uint64_t(1)<<l);
        break;
      }
    }
    if (l == maxInputObjectOptionsTable) // error
    {
      std::cout << "INVALID INPUT OBJECT OPTION '" << text_inputobjectoptions << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
      return -1;
    }
  }

  return (aui_revision < 4)
      ? (retval & 0x01) // version 2+3
      : retval;         // version 4 upwards
}

bool itoinputobjectoptions(uint8_t ui8_options, string& c_outputText)
{
  return itogeneraloption(ui8_options, c_outputText, maxInputObjectOptionsTable, &inputobjectOptionsTable[0][0]);
}

signed int buttonoptiontoi (const char *text_buttonoptions, unsigned int aui_revision)
{
  int l, retval=0;

  std::list<std::string> l_tokens;
  parseOptionsIntoList(text_buttonoptions, l_tokens);

  for (std::list<std::string>::const_iterator iter = l_tokens.begin(); iter != l_tokens.end(); ++iter)
  {
    for (l=0; l<maxButtonOptions; l++)
    {
      if (iter->compare(buttonOptionsTable [l]) == 0)
      {
        retval += (uint64_t(1)<<l);
        break;
      }
    }
    if (l == maxButtonOptions) // error
    {
      std::cout << "INVALID BUTTON OPTION '" << text_buttonoptions << "' ENCOUNTERED! STOPPING PARSER! bye.\n\n";
      return -1;
    }
  }

  // versions 2 and 3 only allow latchable, rest has to be set to 1
  return ( aui_revision < 4 )
      ? (retval & 0x03) // version 2+3
      : retval;         // version 4 upwards
}

bool itobuttonoptions(uint8_t ui8_options, string& c_outputText)
{
  return itogeneraloption(ui8_options, c_outputText, maxButtonOptions, &buttonOptionsTable[0][0]);
}

bool itomacrocommand(uint8_t ui8_command, string& c_outputText)
{
  if(ui8_command < maxCommandsToCompare )
  {
    c_outputText = ctCommandTable[ui8_command];
    return true;
  }
  return false;
}



