/*!
 * DSkinLite.h 
 * 
 * Copyright (c) 2008 by alex ding / uieasy.com
 * Version History:
 * 1.0.0 First Release
 * 1.0.1 
    Add function : dsAddMenuBar
 * 
 */


#ifndef _DSKINLITE_H__
#define _DSKINLITE_H__


/*! 
 * \brief   The dsLoadSkin function initialize a skin theme.
 
  * \details   

  * \param     strSkinPath
               Pointer to a null-terminated string that names the skin path.
               The path can be  integrated,or relative to the application directory. 
  
  * \return    BOOL 
  * 
  * \remarks
  * Write remarks for dsSetDrawItemVisible here.
  * 
  * \see
  * Separate items with the '|' character.

  */
DSKINDLL_API BOOL dsLoadSkin(LPCTSTR strSkinPath);


/*! 
 * \brief    The dsExitSkin function must be called when your application terminates
 
  * \details    
  * \return    BOOL 

  * 
  * \remarks
  * Write remarks for dsSetDrawItemVisible here.
  * 
  * \see
  * Separate items with the '|' character.

  */
DSKINDLL_API BOOL dsExitSkin();


/*! 
 * \brief  the dsSkinWindow function set the window's skin.
 
  * \details    
  * \param     hWnd 
               Handle to the window
  * \param     uCtrlType  
               The type of the window,define in enumeration enumWindowType  

  * \param     szXmlLabel
                Specifies the label name of window, the label name is defined in the configuration xml.
				if the parameter is NULL, the skin will use the default value to set the window's skin.
				The default value of window's skin name is follow:


  * \param     bChildSkin
               Specifies whether set the child's skin of the windows.

  * \return    BOOL 

  * 
  * \remarks
  * Write remarks for dsSetDrawItemVisible here.
  * 
  * \see
  * Separate items with the '|' character.

  */

DSKINDLL_API BOOL dsSkinWindow(HWND hWnd, UINT uCtrlType, LPCTSTR szXmlLabel = NULL, BOOL bChildSkin = TRUE );

 /*!
 * \brief    
            The dsAdjustColor function adjust the skin's hue and saturation . 

 * \details    
            The dsAdjustColor adjust the hue and saturation of image and color in skin.

 * \param     nHue  ;Specifics the hue of skin. if this parameter is  -1, the skin will resume the default hue and saturation;
 
 * \param     nSaturation  Specifics the saturation of skin
 
 * \return    BOOL
              If the function succeeds, the return value is nonzero.
              If the function fails, the return value is zero. To get extended error information, call GetLastError.
  * 
  * \remarks
  * Write remarks for dsSetDrawItemVisible here.
  * 
  * \see
  * Separate items with the '|' character.

 */
DSKINDLL_API BOOL  dsAdjustColor( int nHue, int nSaturation);


/*! 
 * \brief    
 
  * \details    
  
  * \param     bEable
  
  * \return    BOOL 
  * 
  * \remarks
  * Write remarks for dsSetDrawItemVisible here.
  * 
  * \see
  * Separate items with the '|' character.

  */
DSKINDLL_API BOOL  dsEnableMenuNoBorder( BOOL bEable);

/*! 
 * \brief  The GetLastError function retrieves the dskin's last-error code value.  
 
  * \details    

  * \return  The return value is the calling dskin's last-error code value    
  * 
  * \remarks
  * Write remarks for dsSetDrawItemVisible here.
  * 
  * \see
  * Separate items with the '|' character.
  */
DSKINDLL_API DWORD dsGetLastError(void);

/*! 
 * \brief    
 
  * \details    

  * \return     
  * 
  * \remarks
  * Write remarks for dsSetDrawItemVisible here.
  * 
  * \see
  * Separate items with the '|' character.
  */
DSKINDLL_API int   dsGetLastErrorString( LPVOID strBuf);


/*! 
 * \brief    dsTrackPopupMenu use as the win32 api TrackPopupMenu;
 
  * \details    
      dsTrackPopupMenu use as the win32 api TrackPopupMenu, the foword 5 parameters is the same as  TrackPopupMenu;

  * \param    hMenu Handle to the shortcut menu to be displayed.
  * \param    uFlags Function options,use the flag in TrackPopupMenu's uFlags parameter
  * \param    x
              Specifies the horizontal location of the shortcut menu, in screen coordinates. 
  * \param    y
			  Specifies the vertical location of the shortcut menu, in screen coordinates.  
  * \param    hWnd
              Handle to the window that owns the shortcut menu. This window receives all messages from the menu
  * \param    lpLabel
              Specifies the label name of menu, the label name is defined in the config xml.
  * \return   BOOL 
  * 
  * \remarks
  * Write remarks for dsSetDrawItemVisible here.
  * 
  * \see
  * Separate items with the '|' character.
  */
DSKINDLL_API BOOL  dsTrackPopupMenu( HMENU hMenu, UINT  uFlags, int   x, int y, HWND  hWnd);



/*! 
 * \brief      
     Set the display item in xml to be visible or hide
  

  * \param     hwnd 
     the control handle

  * \param     nMinIndex 
     the min index of item.the value according to the parameter "index" in xml; 
 
 * \param     nMaxIndex 
     the max index of item.the value according to the parameter "index" in xml; 

  * \param     bVisible  
    Specifies whether the items to be visible,  TURE for visible, FALSE for hide.

  * \param     bRepaint  
     Specifies whether the window is to be repainted. If this parameter is TRUE,
     the window receives a message. If the parameter is FALSE, no repainting of any kind occurs.

  * \returns    BOOL 

  * 
  * dsSetDrawItemVisible set the display item in xml to be visible or hide.
  *  once set the relatives item to hide,they will not be drawn when control paints.
  * 
  * \remarks
  * Write remarks for dsSetDrawItemVisible here.
  * 
  * \see
  * Separate items with the '|' character.
  */


DSKINDLL_API BOOL  dsSetDrawItemVisible( HWND hwnd, int nMinIndex, int nMaxIndex, BOOL bVisible , BOOL bRepaint = TRUE);



/*!
 * \brief
 * Add menu bar to window;
 * 
 * \param hwnd
 * The handle of window
 *
 * \param hwnd*
 * return the handle of toolbar created to load menu
 * 
 * \param hMenu
 * The hMenu of menu to be insterted.
 * 
 * \param szToolBarLabel
 * The label of Toolbarctrl.
 * 
 * \param szMenuLabel
 * The Label of menu
 * 
 * \returns
 * BOOL for succ, FALSE for fail.
 * 
 * \throws <exception class>
 * Description of criteria for throwing this exception.
 * 
 * This function adds an menu bar to window, 
 * 
 * \remarks
 * Write remarks for dsAddMenuBar here.
 * 
 * \see
 * Separate items with the '|' character.
 */
DSKINDLL_API BOOL  dsAddMenuBar( HWND hwnd, HWND* pToolBar, HMENU hMenu, LPCTSTR szToolBarLabel, LPCTSTR szMenuLabel);

/*!
 * \brief
 * dsGetIcon return the handle HICON according the parameter strIconLabel.
 * 
 * \param strIconLabel
 * strIcon is the "id" of icon image, defined in xml.For example:
 * <image id="standard.icon" value="All_logo16-xp.ico" />
 * 
 * \returns
 * return the handle of icon. It can be used in DrawIcon or DrawIconEx
 * 
 * 
 * \remarks
 * When the skin changes, the HICON is invalid, you should re-call the function to get new HICON
 * 
 * \see
 * dsGetColor | dsGetFont | dsGetBitmap.
 */
DSKINDLL_API HICON    dsGetIcon( LPCTSTR strIconLabel);


/*!
 * \brief
 * Get the color value according to the parameter.
 * 
 * \param strColorLabel
 * The id of color defined in xml. for example:
 * "<color id="system.border" value="RGB(84,165,213)"/>"
 * you can get the color value by passing the id "system.border";
 *
 * \returns
 * return the color value.
 *
 * \remarks
 * You should deletes these resources by calling DestroyIcon  when you are finished using the HICON .
 * 
 * \see
 * dsGetIcon | dsGetFont | dsGetBitmap.
 */
DSKINDLL_API COLORREF dsGetColor( LPCTSTR strColorLabel);

/*!
 * \brief
 * return the handle of bitmap. 
 * 
 * \param strIconLabel
 * the id of image.
 * 
 * \returns
 * return the HBITMAP.
 * 
 * \remark
 *You should deletes these resources by calling DeleteObject when you are finished using the HBITMAP
 * 
 * 
 * \see
 * dsGetIcon | dsGetFont | dsGetColor.
 */
DSKINDLL_API HBITMAP  dsGetBitmap( LPCTSTR strBitmapLabel);



/*!
 * \brief
 * return the handle of font.
 * 
 * \param strFont
 * The id of font defined in xml.
 * 
 * \returns
 * return HFONT of font.
 * 
 * 
 * 
 * \remarks
 * You should deletes these resources by calling DeleteObject  when you are finished using the HFONT
 * 
 * \see
 * dsGetIcon | dsGetBitmap | dsGetColor.
 */
DSKINDLL_API HFONT dsGetFont( LPCTSTR strFont);


#endif