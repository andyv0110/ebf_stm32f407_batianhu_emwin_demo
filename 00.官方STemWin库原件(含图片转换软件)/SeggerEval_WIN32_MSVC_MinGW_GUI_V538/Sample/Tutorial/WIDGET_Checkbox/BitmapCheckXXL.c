/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2016  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.38 - Graphical user interface for embedded applications **
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only  be used  in accordance  with  a license  and should  not be  re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : BitmapBar.c
Purpose     : Sample bitmap for checkbox 16x16
----------------------------------------------------------------------
*/

#include "GUI.h"
#include "Main.h"

/* Colors */
static const GUI_COLOR _aColorDisabled[] = {0x808080, 0x101010};
static const GUI_COLOR _aColorEnabled[]  = {GUI_WHITE, GUI_BLACK};

/* Palettes */
static const GUI_LOGPALETTE _PalCheckDisabled = {
  2,	/* number of entries */
  1, 	/* transparency flag */
  _aColorDisabled
};

static const GUI_LOGPALETTE _PalCheckEnabled = {
  2,	/* number of entries */
  1, 	/* transparency flag */
  _aColorEnabled
};

/* Pixel data */
static const unsigned char _acXXLChecked[] = {
  ________, ________, ________,
  ________, ________, ________,
  ________, ________, ________,
  ________, ________, ________,
  ________, ________, __XX____,
  ________, ________, _XXX____,
  ________, ________, XXXX____,
  ________, _______X, XXXX____,
  ________, ______XX, XXX_____,
  ________, _____XXX, XX______,
  ________, ____XXXX, X_______,
  __XX____, ___XXXXX, ________,
  __XXX___, __XXXXX_, ________,
  __XXXX__, _XXXXX__, ________,
  __XXXXX_, XXXXX___, ________,
  ___XXXXX, XXXX____, ________,
  ____XXXX, XXX_____, ________,
  _____XXX, XX______, ________,
  ______XX, X_______, ________,
  ________, ________, ________,
  ________, ________, ________,
};

/* Bitmaps */
const GUI_BITMAP _abmXXL[2] = {
  { 22, 22, 3, 1, _acXXLChecked, &_PalCheckDisabled },
  { 22, 22, 3, 1, _acXXLChecked, &_PalCheckEnabled },
};

/*************************** End of file ****************************/
