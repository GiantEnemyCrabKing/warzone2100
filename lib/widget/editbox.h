/*
	This file is part of Warzone 2100.
	Copyright (C) 1999-2004  Eidos Interactive
	Copyright (C) 2005-2020  Warzone 2100 Project

	Warzone 2100 is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	Warzone 2100 is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Warzone 2100; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/
/** @file
 *  Definitions for the edit box functions.
 */

#ifndef __INCLUDED_LIB_WIDGET_EDITBOX_H__
#define __INCLUDED_LIB_WIDGET_EDITBOX_H__

#include "widget.h"
#include "widgbase.h"
#include "lib/framework/wzstring.h"
#include <functional>

/* Edit Box states */
#define WEDBS_FIXED		0x0001		// No editing is going on
#define WEDBS_INSERT	0x0002		// Insertion editing
#define WEDBS_OVER		0x0003		// Overwrite editing
#define WEDBS_MASK		0x000f		//
#define WEDBS_HILITE	0x0010		//
#define WEDBS_DISABLE   0x0020		// disable button from selection

struct EditBoxDisplayCache {
	WzText wzDisplayedText;
	WzText modeText;
};

class W_EDITBOX : public WIDGET
{

public:
	W_EDITBOX(W_EDBINIT const *init);
	W_EDITBOX();
	~W_EDITBOX();

	void clicked(W_CONTEXT *psContext, WIDGET_KEY key = WKEY_PRIMARY) override;
	void simulateClick(W_CONTEXT *psContext, bool silenceClickAudio = false, WIDGET_KEY key = WKEY_PRIMARY);
	void highlight(W_CONTEXT *psContext) override;
	void highlightLost() override;
	void focusLost() override;
	void run(W_CONTEXT *psContext) override;
	void display(int xOffset, int yOffset) override;
	void geometryChanged() override;

	void stopEditing();
	bool isEditing();

	void setState(unsigned state) override;
	WzString getString() const override;
	void setString(WzString string) override;
	void setPlaceholder(WzString value);
	void setMaxStringSize(int size);

	void setBoxColours(PIELIGHT first, PIELIGHT second, PIELIGHT background);

	typedef std::function<void (W_EDITBOX&)> OnReturnHandler;
	void setOnReturnHandler(const OnReturnHandler& func);
	void setOnEscapeHandler(const OnReturnHandler& func);
	void setOnEditingStoppedHandler(const OnReturnHandler& func);

	typedef std::function<bool (W_EDITBOX&)> OnTabHandler; // Returns true if the editbox should be done processing the current input
	void setOnTabHandler(const OnTabHandler& func);

	UDWORD		state;						// The current edit box state
	WzString	aText;						// The text in the edit box
	WzString	placeholderText;
	iV_fonts	FontID;
	int			blinkOffset;				// Cursor should be visible at time blinkOffset.
	int			maxStringSize;				// max characters string will accept
	int			insPos;						// The insertion point in the buffer
	int			printStart;					// Where in the string appears at the far left of the box
	int			printChars;					// The number of characters appearing in the box
	int			printWidth;					// The pixel width of the characters in the box
	WIDGET_DISPLAY	pBoxDisplay;			// Optional callback to display the edit box background.
	SWORD HilightAudioID;					// Audio ID for form clicked sound
	SWORD ClickedAudioID;					// Audio ID for form hilighted sound
	SWORD ErrorAudioID;						// Audio ID for error sound
	WIDGET_AUDIOCALLBACK AudioCallback;		// Pointer to audio callback function

private:
	void initialise();  // Moves the cursor to the end.
	void delCharRight();
	void delCharLeft();
	bool insertChar(WzUniCodepoint ch);
	bool overwriteChar(WzUniCodepoint ch);
	void fitStringStart();  // Calculate how much of the start of a string can fit into the edit box
	void fitStringEnd();
	void setCursorPosPixels(int xPos);

	PIELIGHT boxColourFirst, boxColourSecond, boxColourBackground;
	EditBoxDisplayCache displayCache;
	bool suppressAudioCallback = false;
	OnReturnHandler	onRetHandler = nullptr;
	OnTabHandler onTabHandler = nullptr;
	OnReturnHandler onEscHandler = nullptr;
	OnReturnHandler onEditingStoppedHandler = nullptr;
};

#endif // __INCLUDED_LIB_WIDGET_EDITBOX_H__
