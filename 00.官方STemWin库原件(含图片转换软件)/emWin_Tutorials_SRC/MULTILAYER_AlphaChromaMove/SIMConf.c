/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2009  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File        : SIMConf.c
Purpose     : Windows Simulator configuration
---------------------------END-OF-HEADER------------------------------
*/

#ifdef WIN32

#include "LCD_SIM.h"

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       SIM_X_Config
*
* Purpose:
*   Called during the initialization process in order to configure
*   the simulator.
*/
void SIM_X_Config() {
  SIM_GUI_SetCompositeSize(320, 240);  // Set size of composite window
  SIM_GUI_SetTransColor(0xff0000);     // Define the transparent color
}

#else

void SIMConf_C(void); // Avoid empty object files
void SIMConf_C(void) {}

#endif

/*************************** End of file ****************************/
