/*
  vt2iso-defines.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "vt2iso-defines.hpp"
#include <cstring>
#include <stdio.h>

//! Number of regular object plus gap plus number of internal objects.
char otCompTable [maxObjectTypesToCompare] [stringLength+1] = {
  "workingset",
  "datamask",
  "alarmmask",
  "container",
  "softkeymask",
  "key",
  "button",
  "inputboolean",
  "inputstring",
  "inputnumber",
  "inputlist",
  "outputstring",
  "outputnumber",
  "line",
  "rectangle",
  "ellipse",
  "polygon",
  "meter",
  "linearbargraph",
  "archedbargraph",
  "picturegraphic",
  "numbervariable",
  "stringvariable",
  "fontattributes",
  "lineattributes",
  "fillattributes",
  "inputattributes",
  "objectpointer",
  "macro",
  "auxiliaryfunction",
  "auxiliaryinput",     // Object ID 30
  "auxiliaryfunction2",
  "auxiliaryinput2",
  "auxiliarycontroldesignatorobjectpointer",
  "windowmask",
  "keygroup",
  "graphicscontext",    // Object ID 36
  "outputlist",
  "extendedinputattributes",
  "colourmap",
  "objectlabelreferencelist",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "objectpool",          // Object ID 55
  "include_object",
  "include_macro",
  "point",
  "language",
  "fixedbitmap"
};

uint64_t omcTypeTable [maxObjectTypesToCompare] = {
  /* "workingset", */     ombMacro | ombOutputfield | ombOutputshape | ombOutputgraphic | ombPicturegraphic | ombGraphicsContext | ombContainer | ombObjectpointer,
  /* "datamask", */       ombMacro | ombOutputfield | ombInputfield | ombOutputgraphic | ombOutputshape | ombPicturegraphic | ombButton | ombContainer | ombObjectpointer | ombGraphicsContext | ombAuxiliaryControlDesignatorObjectPointer,
  /* "alarmmask", */      ombMacro | ombOutputfield | ombOutputgraphic | ombOutputshape | ombPicturegraphic | ombContainer | ombObjectpointer | ombGraphicsContext | ombAuxiliaryControlDesignatorObjectPointer,
  /* "container", */      0, // same as the object that included the container
  /* "softkeymask", */    ombMacro | ombKey | ombObjectpointer,
  /* "key", */            ombMacro | ombOutputfield | ombOutputshape | ombOutputgraphic | ombPicturegraphic | ombContainer | ombObjectpointer | ombGraphicsContext,
  /* "button", */         ombMacro | ombOutputfield | ombOutputshape | ombOutputgraphic | ombPicturegraphic | ombContainer | ombObjectpointer | ombGraphicsContext,
  /* "inputboolean", */   ombMacro | 0,
  /* "inputstring", */    ombMacro | 0,
  /* "inputnumber", */    ombMacro | 0,
  /* "inputlist", */      ombMacro | ombContainer | ombOutputfield | ombOutputshape | ombOutputgraphic | ombPicturegraphic | ombGraphicsContext | ombObjectpointer,
  /* "outputstring", */   ombMacro | 0,
  /* "outputnumber", */   ombMacro | 0,
  /* "line", */           ombMacro | 0,
  /* "rectangle", */      ombMacro | 0,
  /* "ellipse", */        ombMacro | 0,
  /* "polygon", */        ombMacro | ombPoint,
  /* "meter", */          ombMacro | 0,
  /* "linearbargraph", */ ombMacro | 0,
  /* "archedbargraph", */ ombMacro | 0,
  /* "picturegraphic", */ ombMacro | ombFixedBitmap,
  /* "numbervariable", */ 0, // really NONE
  /* "stringvariable", */ 0, // really NONE
  /* "fontattributes", */ ombMacro | 0,
  /* "lineattributes", */ ombMacro | 0,
  /* "fillattributes", */ ombMacro | 0,
  /* "inputattributes",*/ ombMacro | 0,
  /* "objectpointer", */  0, // really NONE
  /* "macro", */          0, // really NONE
  /* "auxfunction", */    ombOutputfield | ombOutputshape | ombPicturegraphic,
  /* "auxinput", */       ombOutputfield | ombOutputshape | ombPicturegraphic,
  /* "auxfunction2", */   ombOutputfield | ombOutputshape | ombOutputgraphic | ombPicturegraphic | ombGraphicsContext | ombContainer | ombObjectpointer,
  /* "auxinput2", */      ombOutputfield | ombOutputshape | ombOutputgraphic | ombPicturegraphic | ombGraphicsContext | ombContainer | ombObjectpointer,
  /* "auxiliarycontroldesignatorobjectpointer", */  ombAuxiliaryFunction2 | ombAuxiliaryInput2,
                          0, // check for windowmask
                          0, // ckeck for keygroup
  /* "graphicscontext" */ 0, // check for graphics context object
  /* "outputlist" */      ombMacro | ombOutputfield | ombOutputshape | ombOutputgraphic | ombInputfield | ombPicturegraphic | ombGraphicsContext | ombObjectpointer,
                          0, // check for extended input attributes
                          0, // check for colour map
                          0, // check for object label reference list
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
  /* "objectpool" */     ~0, // all
  /* "include_object" */  0, // really NONE
  /* "include_macro" */   0, // really NONE
  /* "point" */           0, // really NONE
  /* "language" */        0, // really NONE
  /* "fixedbitmap" */     0
};

char otClassnameTable [maxObjectTypes] [stringLength+1] = {
  "WorkingSet",
  "DataMask",
  "AlarmMask",
  "Container",
  "SoftKeyMask",
  "Key",
  "Button",
  "InputBoolean",
  "InputString",
  "InputNumber",
  "InputList",
  "OutputString",
  "OutputNumber",
  "Line",
  "Rectangle",
  "Ellipse",
  "Polygon",
  "Meter",
  "LinearBarGraph",
  "ArchedBarGraph",
  "PictureGraphic",
  "NumberVariable",
  "StringVariable",
  "FontAttributes",
  "LineAttributes",
  "FillAttributes",
  "InputAttributes",
  "ObjectPointer",
  "Macro",
  "AuxiliaryFunction",
  "AuxiliaryInput",
  "AuxiliaryFunction2",
  "AuxiliaryInput2",
  "AuxiliaryControlDesignatorObjectPointer",
  "WindowMask",
  "KeyGroup",
  "GraphicsContext",
  "OutputList",
  "ExtendedInputAttributes",
  "ColourMap",
  "ObjectLabelReferenceList"
};

char attrNameTable [maxAttributeNames] [stringLength+1] = {
  "background_colour",
  "selectable",
  "active_mask",
  "soft_key_mask",
  "priority",
  "acoustic_signal",
  "width",
  "height",
  "hidden",
  "key_code",
  "border_colour",
  "foreground_colour",
  "variable_reference",
  "value",
  "font_attributes",
  "input_attributes",
  "options",
  "horizontal_justification",
  "length",
  "min_value",
  "max_value",
  "offset",
  "scale",
  "number_of_decimals",
  "format",
  "line_attributes",
  "line_suppression",
  "fill_attributes",
  "ellipse_type",
  "start_angle",
  "end_angle",
  "polygon_type",
  "needle_colour",
  "arc_and_tick_colour",
  "number_of_ticks",
  "colour",
  "target_line_colour",
  "target_value_variable_reference",
  "target_value",
  "bar_graph_width",
  "actual_width",
  "actual_height",
  "transparency_colour",
  "font_colour",
  "font_size",
  "font_type",
  "font_style",
  "line_colour",
  "line_width",
  "line_art",
  "fill_type",
  "fill_colour",
  "fill_pattern",
  "validation_type",
  "validation_string",
  "pos_x",
  "pos_y",
  "event",
  "file",
  "line_direction",
  "enabled",
  "file1",
  "file4",
  "file8",
  "block_font",
  "block_row",
  "block_col",
// new attributes from the new objects (and macros?!)
  "number_of_items",
  "number_of_points",
  "rle",
  "number_of_bytes",
//
  "function_type",
  "input_id",
  "pointer_type",
//
//
  "object_id",
  "hide_show",
  "enable_disable",
  "number_of_repetitions",
  "frequency",
  "on_time_duration",
  "off_time_duration",
  "percentage",
  "parent_object_id",
  "x_pos_change",
  "y_pos_change",
  "x_pos",
  "y_pos",
  "new_width",
  "new_height",
  "new_background_colour",
  "new_value",
  "working_set_object_id",
  "new_active_mask_object_id",
  "mask_type",
  "mask_object_id",
  "new_soft_key_mask_object_id",
  "attribute_id",
  "new_priority",
  "list_index",
  "new_object_id",
  "bytes_in_string",
  "code",
  "language",
  "in_key",
  "in_button",
//
//
  "viewport_width",
  "viewport_height",
  "viewport_x",
  "viewport_y",
  "canvas_width",
  "canvas_height",
  "viewport_zoom",
  "cursor_x",
  "cursor_y",
//
//
//
//
//
//
//
//
//
//
//
  "inputobject_options",
// new attributes (VT revision 3)
  "vertical_justification",
  "activate_for_editing",
  "lock_unlock",
  "lock_timeout",
  "live_editing",
  "abs_path", //proprietary
  "abs_path1", //proprietary
  "abs_path4", //proprietary
  "abs_path8", //proprietary
  "auto_set_length", //proprietary
  "auxiliary_object_id" // for AuxConDesignOP
};

// Table of possible Macro Commands
char ctCommandTable [maxCommandsToCompare] [stringLength+1] = {
  "command_hide_show_object",
  "command_enable_disable_object",
  "command_select_input_object",
  "command_control_audio_device",
  "command_set_audio_volume",
  "command_change_child_location",
  "command_change_size",
  "command_change_background_colour",
  "command_change_numeric_value",
  "command_change_end_point",
  "command_change_font_attributes",
  "command_change_line_attributes",
  "command_change_fill_attributes",
  "command_change_active_mask",
  "command_change_soft_key_mask",
  "command_change_attribute",
  "command_change_priority",
  "command_change_list_item",
  " ",
  "command_change_string_value",
  "command_change_child_position",
  "command_set_object_label",       // @todo ON REQUEST: Revision 4
  "command_change_polygon_point",   // @todo ON REQUEST: Revision 4
  "command_change_polygon_scale",   // @todo ON REQUEST: Revision 4
  "command_graphics_context",       // @todo ON REQUEST: Revision 4
  " ",
  " ",
  " ",
  " ",
  "command_lock_unlock_mask",       // @todo ON REQUEST: Revision 4
  "command_execute_macro"
};

char colourTable [16] [stringLength+1] = {
  "black",
  "white",
  "green",
  "teal",
  "maroon",
  "purple",
  "olive",
  "silver",
  "grey",
  "blue",
  "lime",
  "cyan",
  "red",
  "magenta",
  "yellow",
  "navy"
};

char masktypeTable [3] [stringLength+1] = {
  "undefined",
  "data",
  "alarm"
};

char colourDepthTable [3] = {'1', '4', '8'};

char fontsizeTable [maxFontsizeTable] [stringLength+1] = {
  "6x8",
  "8x8",
  "8x12",
  "12x16",
  "16x16",
  "16x24",
  "24x32",
  "32x32",
  "32x48",
  "48x64",
  "64x64",
  "64x96",
  "96x128",
  "128x128",
  "128x192"
};

char fontstyleTable [maxFontstyleTable] [stringLength+1] = {
  "bold",
  "crossed",
  "underlined",
  "italic",
  "inverted",
  "flashinginverted",
  "flashinghidden"
};

char fonttypeTable [maxFonttypeTable] [stringLength+1] = {
  "latin1", // 0
  "latin9", // 1
  "latin2", // 2
  "reserved", // 3
  "latin4", // 4
  "latin5", // 5
  "reserved", // 6
  "latin7", // 7
  "proprietary" // 0xFF
};

char truthTable [maxTruthTable] [stringLength+1] = {
  "yes",
  "true",
  "on",
  "show",
  "enable",
  "1",
  "unlocked"
};

char falseTable [maxFalseTable] [stringLength+1] = {
  "no",
  "false",
  "off",
  "hide",
  "disable",
  "0",
  "locked"
};

char formatTable [maxFormatTable] [stringLength+1] = {
  "fixed",
  "exponential"
};

char horizontalJustificationTable [maxHorizontalJustificationTable] [stringLength+1] = {
  "left",
  "middle",
  "right"
};

char verticalJustificationTable [maxVerticalJustificationTable] [stringLength+1] = {
  "top",
  "middle",
  "bottom"
};

char stringOptionsTable [maxStringOptionsTable] [stringLength+1] = {
  "transparent",
  "autowrap",
  "wraponhyphen"
};

char inputNumberOptionsTable [maxInputNumberOptionsTable] [stringLength+1] = {
  "transparent",
  "leadingzeros",
  "blankzero",
  "truncate"
};

char outputNumberOptionsTable [maxOutputNumberOptionsTable] [stringLength+1] = {
  "transparent",
  "leadingzeros",
  "blankzero",
  "truncate"
};

char lineSuppressionTable [maxLineSuppressionTable] [stringLength+1] = {
  "top",
  "right",
  "bottom",
  "left"
};

char ellipseTypeTable [maxEllipseTypeTable] [stringLength+1] = {
  "closed",
  "open",
  "closedsegment",
  "closedsection"
};

char polygonTypeTable [maxPolygonTypeTable] [stringLength+1] = {
  "convex",
  "nonconvex",
  "complex",
  "open"
};


char validationTypeTable [maxValidationTypeTable] [stringLength+1] = {
  "validcharacters",
  "invalidcharacters"
};


char fillTypeTable [maxFillTypeTable] [stringLength+1] = {
  "nofill",
  "linecolour",
  "fillcolour",
  "pattern"
};

char pictureGraphicOptionsTable [maxPictureGraphicOptionsTable] [stringLength+1] = {
  "transparent",
  "flashing",
};

char pictureGraphicRleTable [maxPictureGraphicRleTable] [stringLength+1] = {
  "1",
  "4",
  "8",
  "auto"
};

char linearBarGraphOptionsTable [maxLinearBarGraphOptionsTable] [stringLength+1] = {
  "border",
  "targetline",
  "ticks",
  "nofill",
  "horizontal",
  "growpositive"
};

char meterOptionsTable[maxMeterOptionsTable] [stringLength+1] ={
  "arc",
  "border",
  "ticks",
  "clockwise"
};

char archedBarGraphOptionsTable [maxArchedBarGraphOptionsTable] [stringLength+1] = {
  "border",
  "targetline",
  "notused",
  "nofill",
  "clockwise",
};

char priorityAcousticSignalTable [maxPriorityAcousticSignalTable] [stringLength+1] = {
  "high",
  "medium",
  "low",
  "none"
};

//event Table for Macros
char eventTable [maxEventTable] [stringLength+1] = {
  "on_activate",
  "on_deactivate",
  "on_show",
  "on_hide",
  "on_enable",
  "on_disable",
  "on_change_active_mask",
  "on_change_soft_key_mask",
  "on_change_attribute",
  "on_change_background_colour",
  "on_change_font_attributes",
  "on_change_line_attributes",
  "on_change_fill_attributes",
  "on_change_child_location",
  "on_change_size",
  "on_change_value",
  "on_change_priority",
  "on_change_end_point",
  "on_input_field_selection",
  "on_input_field_deselection",
  "on_ESC",
  "on_entry_of_value",
  "on_entry_of_new_value",
  "on_key_press",
  "on_key_release",
  "on_change_child_position"
};

char auxFunctionTypeTable [maxAuxFunctionTypes] [stringLength+1] = {
  "latchingboolean",
  "analog",
  "nonlatchingboolean"
};

char auxFunction2TypeTable [maxAuxFunction2Types] [stringLength+1] = {
  "latchingboolean",
  "analog",
  "nonlatchingboolean",
  "analogreturnto50",
  "analogreturnto0",
  "bothlatchingdualboolean",
  "bothnonlatchingdualboolean",
  "latchingdualbooleanup",
  "latchingdualbooleandown",
  "combinedanalogreturnto50duallatchingboolean",
  "combinedanalogduallatchingboolean",
  "nonlatchingquadboolean",
  "analogquad",
  "analogreturnto50quad",
  "encoder"
};

char auxFunction2OptionsTable [maxAuxFunction2Options] [stringLength+1] = {
  "criticalcontrol",
  "assignmentlock",
  "singleassignment"
};

char auxInput2OptionsTable [maxAuxInput2Options] [stringLength+1] = {
  "criticalcontrol",
  "reserved",
  "singleassignment"
};

char auxConDesignObjPtrTypeTable [maxAuxConDesignObjPtrTypes] [stringLength+1] = {
  "object",
  "objectassigned",
  "workingset",
  "workingsetassigned"
};

char GCOptionsTable [maxGCOptions] [stringLength+1] = {
  "transparent",
  "uselinefontfillcolourfordraw"
};

char buttonOptionsTable [maxButtonOptions] [stringLength+1] = {
  "latchable",
  "latched",
  "suppressborder",
  "transparentbackground",
  "disabled",
  "noborder"
};

char inputobjectOptionsTable [maxInputObjectOptionsTable] [stringLength+1] = {
  "enabled",
  "liveediting"
};

char keyGroupOptionsTable [maxKeyGroupOptionsTable] [stringLength+1] = {
  "available",
  "transparent"
};

int utf16_strlen(const char* source)
{
  int len=0;
  const uint16_t* wideSource = (const uint16_t*) source;
  while (*wideSource++ != 0) len++;
  return len;
}

void utf16convert (const char* source, std::string &destin)
{
  int const len = utf16_strlen(source);
  destin.clear();
  destin.reserve(len);
  int index=0;
  while (index < len)
  {
    //destin << source [index*2];
    destin.push_back(source [index*2]);
    index++;
  }
}


unsigned int objectIsType (char* lookup_name)
{
  for (int i=0; i<maxObjectTypesToCompare; i++)
  {
    if (0 == strncmp (lookup_name, otCompTable [i], stringLength))
    {
      return i;
    }
  }
  return 0xFFFF;
}

unsigned int commandIsType (char* lookup_name)
{
  for (int i=0; i<maxCommandsToCompare; i++) {
    if (0 == strncmp (lookup_name, ctCommandTable [i], stringLength)) {
      return i;
    }
  }
  return 0xFFFF;
}
