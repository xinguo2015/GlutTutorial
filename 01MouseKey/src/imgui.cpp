#include "myglut.h"
#include "imgui.h"

#ifndef CLAMP
#define CLAMP(x,min,max) ((x)<(min) ? (min) : ((x)>(max)?(max):(x)) )
#endif

namespace xglm {

	void drawRect(int x, int y, int w, int h, unsigned int color);
	void drawCrossX(int x, int y, int w, int h, unsigned int color);
	void fillRect(int x, int y, int w, int h, unsigned int color);
	void drawText(const char text[], int x, int y, unsigned int color);
	int getTicks();

	static int guiButtonSink      = 2;
	static int guiListItemHeight  = 30;
	static int guiColorStill  = 0xCCBBAA;
	static int guiColorHot    = 0xFFDDCC;
	static int guiColorCheck  = 0xFFEEDD;
	static int guiColorWhite  = 0xFFFFFF;
	static int guiColorFocus  = 0xFFEEDD;
	static int guiColorEdit   = 0xFFFFFF;
	static int guiColorPicked = 0xFFEEDD;
	static int guiColorLabel  = 0x333333;

	static GuiFont guiFont(GLUT_BITMAP_TIMES_ROMAN_24);

	int GuiFont::setFont(void *f)
	{
		if( f == GLUT_BITMAP_TIMES_ROMAN_24 ) 
			mFontHeight = 24;
		else
			return 0;
		mGlutFont = f;
		return 1;
	}

	void ImGUI::initialize()
	{
		memset(&mGuiState, 0, sizeof(mGuiState));
	}

	void ImGUI::prepare()
	{
		mGuiState.hotitem = 0;
	}

	void ImGUI::finish()
	{
		if (mGuiState.mousedown == 0)
		{
			mGuiState.activeitem = 0;
		}
		// If no widget grabbed tab, clear focus
		if (mGuiState.keypressed == IMGUI_KEY_TAB)
			mGuiState.kbditem = 0;
		// Clear the entered key
		mGuiState.keypressed = 0;  
		mGuiState.keychar = 0;
	}
		
	void ImGUI::onKeyboard(unsigned char key, int modifier, int x, int y)
	{
		mGuiState.keychar    = key;
		mGuiState.keypressed = key;
		mGuiState.keymodify  = modifier;

		mGuiState.lastkeychar    = key;
		mGuiState.lastkeypressed = key;
		mGuiState.lastkeymodify  = modifier;
	}
	
	void ImGUI::onKeyboardUp(unsigned char key, int modifier, int x, int y)
	{
		mGuiState.keymodify = modifier;
	}

	void ImGUI::onSpecial(unsigned char key, int modifier, int x, int y)
	{
		mGuiState.keypressed = key;
		mGuiState.keymodify  = modifier;
		mGuiState.lastkeypressed = key;
		mGuiState.lastkeymodify  = modifier;
	}

	void ImGUI::onSpecialUp(unsigned char key, int modifier, int x, int y)
	{
		mGuiState.keypressed = key;
		mGuiState.keymodify = modifier;
	}

	void ImGUI::onMouse(int button, int state, int x, int y)
	{
		if ( button == IMGUI_LEFT_BUTTON ) 
		{	//这里只处理鼠标**左**键
			mGuiState.mousedown = (state==IMGUI_MOUSE_DOWN);
			mGuiState.mousex = x;
			mGuiState.mousey = y;
		}
	}

	void ImGUI::onMotion(int x, int y)
	{
		mGuiState.mousex = x;
		mGuiState.mousey = y;
	}

	// Check whether current mouse position is within a rectangle
	int ImGUI::hitRect(int x, int y, int w, int h)
	{
		return (mGuiState.mousex > x && mGuiState.mousey > y &&
			mGuiState.mousex < x + w && mGuiState.mousey < y + h);
	}

	// Simple button IMGUI widget
	int ImGUI::button(int id, int x, int y, int w, int h, char label[])
	{
		int alignDY = (h-guiFont.mFontHeight)/2;
		// Check whether the button should be hot
		if (hitRect(x, y, w, h))
		{
			mGuiState.hotitem = id;
			if (mGuiState.activeitem == 0 && mGuiState.mousedown)
				mGuiState.activeitem = id;
		}
		// Draw button 
		if (mGuiState.hotitem == id)    {
			// button is hot
			if ( ! mGuiState.mousedown ) {
				// Button is merely 'hot', 绘制颜色为guiColorHot
				fillRect(x, y, w, h, guiColorHot);
				// 添加按钮的标签
				drawText(label, x, y+alignDY,guiColorLabel);
			} else {
				// mouse is down，将按钮的位置进行稍许偏移，增加动感
				fillRect(x+guiButtonSink, y+guiButtonSink, w, h, guiColorHot);
				// 添加按钮的标签
				drawText(label, x+guiButtonSink, y+guiButtonSink+alignDY, guiColorLabel);
			}
		} else {
			// button is not hot
			fillRect(x, y, w, h, guiColorStill);
			drawText(label, x, y+alignDY, guiColorLabel);
		}

		// If button is hot and active, but mouse button is not down, 
		// the user must have clicked the button.
		if (mGuiState.hotitem == id && 
			mGuiState.activeitem == id &&
			mGuiState.mousedown == 0 )
		{
			return 1;
		}

		// Otherwise, no clicky (click and release).
		return 0;
	}

	int ImGUI::checkbox (int id, int x, int y, int w, int h, char label[], int *value)
	{
		// Check whether the button should be hot
		if (hitRect(x, y, w, h))
		{
			mGuiState.hotitem = id;
			if (mGuiState.activeitem == 0 && mGuiState.mousedown)
				mGuiState.activeitem = id;
		}
		// Draw radio button
		fillRect(x,y, w, h, mGuiState.hotitem == id ? guiColorHot : guiColorStill);
		drawRect(x,y, w, h, guiColorCheck);
		drawText(label, x+w+4, y-10, guiColorLabel);
		if ( *value )
		{
			// is checked, then draw a cross
			drawCrossX(x, y, w, h, guiColorCheck);
		}

		// If button is hot and active, but mouse button is not down, 
		// the user must have clicked the button.
		if (mGuiState.hotitem == id && 
			mGuiState.activeitem == id &&
			mGuiState.mousedown == 0 )
		{
			*value = !(*value);
			return 1;
		}

		return 0;
	}


	int ImGUI::radio(int id, int x, int y, int w, int h, char label[], int reference, int *value)
	{
		// Check whether the button should be hot
		if (hitRect(x, y, w, h))
		{
			mGuiState.hotitem = id;
			if (mGuiState.activeitem == 0 && mGuiState.mousedown)
				mGuiState.activeitem = id;
		}
		// Draw radio button
		fillRect(x,y, w, h, mGuiState.hotitem == id ? guiColorHot : guiColorStill);
		drawText(label, x+w+4, y-10, guiColorLabel);
		//drawRect(x,y, w, h, guiColorCheck);
		if ( reference == *value )
		{
			fillRect(x+w/4,y+h/4, w/2, h/2, guiColorCheck);
		}

		// If button is hot and active, but mouse button is not down, 
		// the user must have clicked the button.
		if (mGuiState.hotitem == id && 
			mGuiState.activeitem == id &&
			mGuiState.mousedown == 0 )
		{
			if( *value != reference )
			{
				*value = reference;
				return 1;
			}
		}

		return 0;
	}



	// Simple scroll bar IMGUI widget
	int ImGUI::slider(int id, int x, int y, int w, int h, double min, double max, double delta, double * value)
	{
		int cursize = 16; // cursor size
		int border = 2;   // distance against the cursor
		int hintsize = 2; // thickness of the focus hint
		int vertical = w < h;// sliding direction
		double posratio = CLAMP((*value - min)/(max-min), 0, 1);
		int curpos;

		w = CLAMP( w, cursize+border*2, w);
		h = CLAMP( h, cursize+border*2, h);
		if( vertical ) h = h<cursize*4 ? cursize*4 : h;
		else           w = w<cursize*4 ? cursize*4 : w;

		curpos = (int)( posratio * ((vertical?h:w) - border*2 - cursize) ) + border;

		// Check for hotness
		if (hitRect(x, y, w, h)) {
			mGuiState.hotitem = id;
			if (mGuiState.activeitem == 0 && mGuiState.mousedown)
				mGuiState.activeitem = id;
		}

		// If no widget has keyboard focus, take it
		if (mGuiState.kbditem == 0)
			mGuiState.kbditem = id;

		// If we have keyboard focus, show it
		if (mGuiState.kbditem == id)
			drawRect(x-hintsize, y-hintsize, w+hintsize*2, h+hintsize*2, guiColorFocus);

		// render the bar
		fillRect(x, y, w, h, guiColorStill   );
		// render the cursor
		if (mGuiState.activeitem == id || mGuiState.hotitem == id) {
			fillRect( vertical ? x+(w-cursize)/2 : x+curpos,
				vertical ? y + curpos : y+(h-cursize)/2, cursize, cursize, guiColorWhite);
		} else {
			fillRect( vertical ? x+(w-cursize)/2 : x+curpos,
				vertical ? y + curpos : y+(h-cursize)/2, cursize, cursize, guiColorHot);
		}
		// If we have keyboard focus, we'll need to process the keys
		if (mGuiState.kbditem == id)
		{
			switch (mGuiState.keypressed)
			{
			case IMGUI_KEY_TAB:
				// If tab is pressed, lose keyboard focus.
				// Next widget will grab the focus.
				mGuiState.kbditem = 0;
				// If shift was also pressed, we want to move focus
				// to the previous widget instead.
				if (mGuiState.keymodify & IMGUI_SHIFT)
					mGuiState.kbditem = mGuiState.lastwidget;
				// Also clear the key so that next widget
				// won't process it
				mGuiState.keypressed = 0;
				break;
			case IMGUI_KEY_UP:
			case IMGUI_KEY_LEFT:
				// Slide slider up (if not at zero)
				if (*value > min) {
					(*value) = CLAMP(*value - delta, min, max);
					return 1;
				}
				break;
			case IMGUI_KEY_DOWN:
			case IMGUI_KEY_RIGHT:
				// Slide slider down (if not at max)
				if (*value < max) {
					(*value) = CLAMP(*value + delta, min, max);
					return 1;
				}
				break;
			}
		}

		mGuiState.lastwidget = id;

		// Update widget value
		if (mGuiState.activeitem == id) {
			double newvalue = vertical ? 
				(mGuiState.mousey - (y + border + cursize/2))/(double)(h-border*2-cursize) :
			(mGuiState.mousex - (x + border + cursize/2))/(double)(w-border*2-cursize) ;
			newvalue = min + CLAMP(newvalue,0,1)*(max-min);
			mGuiState.kbditem = id; // let it accept keyboard
			if (*value != newvalue ) {
				*value = newvalue;
				return 1;
			}
		}

		return 0;
	}

	int ImGUI::textbox(int id, int x, int y, int w, int h, char textbuf[], int maxbuf)
	{
		int len = strlen(textbuf);
		int cursorpos = 0;
		int textChanged = 0;
		// Check whether the button should be hot
		if (hitRect(x, y, w, h))
		{
			mGuiState.hotitem = id;
			if (mGuiState.activeitem == 0 && mGuiState.mousedown)
				mGuiState.activeitem = id;
		}
		// If no widget has keyboard focus, take it
		if (mGuiState.kbditem == 0)
			mGuiState.kbditem = id;
		// If we have keyboard focus, show it
		if (mGuiState.kbditem == id)
			drawRect(x-2, y-2, w+4, h+4, 0xffddee);

		// Render the text box 
		if ( mGuiState.hotitem == id || mGuiState.activeitem == id ) {
			// 'hot' or 'active'
			fillRect(x, y, w, h, guiColorHot);
		} else {
			fillRect(x, y, w, h, guiColorStill   );
		}

		// show text
		drawText(textbuf, x+4, y-6, guiColorEdit);
		// Render cursor if we have keyboard focus
		if ( mGuiState.kbditem == id && ( getTicks() >> 8) & 1)
			drawText("_", cursorpos, y-6, guiColorEdit);

		// If we have keyboard focus, we'll need to process the keys
		if (mGuiState.kbditem == id)
		{
			switch (mGuiState.keypressed)
			{
			case IMGUI_KEY_TAB:
				// If tab is pressed, lose keyboard focus.
				// Next widget will grab the focus.
				mGuiState.kbditem = 0;
				// If shift was also pressed, we want to move focus
				// to the previous widget instead.
				if (mGuiState.keypressed==IMGUI_KEY_TAB && mGuiState.keymodify & IMGUI_SHIFT)
					mGuiState.kbditem = mGuiState.lastwidget;
				// Also clear the key so that next widget
				// won't process it
				mGuiState.keypressed = 0;
				break;
			case IMGUI_BACKSPACE:
				if( len > 0 ) {
					textbuf[--len] = 0;
					textChanged = 1;
				}
				mGuiState.keypressed = 0;
				break;
			}
			if (mGuiState.keychar >= 32 && mGuiState.keychar < 127 && len < maxbuf ) {
				textbuf[len] = mGuiState.keychar;
				textbuf[++len] = 0;
				textChanged = 1;
			}
		}

		mGuiState.lastwidget = id;

		// If button is hot and active, but mouse button is not
		// down, the user must have clicked the button.
		if (   mGuiState.mousedown == 0 
			&& mGuiState.hotitem == id 
			&& mGuiState.activeitem == id )
			mGuiState.kbditem = id;
		return textChanged;
	}

	int ImGUI::textlabel(int id, int x, int y, char text[])
	{
		drawText(text, x, y, guiColorLabel);
		return 0;
	}


	int ImGUI::listitem(int id, int x, int y, int w, int h, char label[], int selected)
	{
		if (hitRect(x, y, w, h))
		{
			mGuiState.hotitem = id;
			if (mGuiState.activeitem == 0 && mGuiState.mousedown)
				mGuiState.activeitem = id;
		}

		if (mGuiState.hotitem == id	&& mGuiState.mousedown )
			selected = 1;

		if (mGuiState.hotitem == id)
			fillRect(x, y, w, h, guiColorHot);
		else if ( selected )
			fillRect(x, y, w, h, guiColorPicked);
		else
			fillRect(x, y, w, h, guiColorStill);
		drawText(label, x+5, y-10, guiColorLabel);

		// If button is hot and active, but mouse button is not down, 
		// the user must have clicked the button.
		if (mGuiState.hotitem == id && 
			mGuiState.activeitem == id &&
			mGuiState.mousedown == 0 )
		{
			return 1;
		}

		// Otherwise, no clicky.
		return 0;
	}

	int ImGUI::listbox (int id, int x, int y, int w, int h, char*items[], int nitem, int *firstitem, int *selection)
	{
		int needslider = 0;
		double slidervalue = *firstitem;
		int nShow, k, wext;
		int newSelection = *selection;


		nShow = CLAMP( (h-4) / guiListItemHeight, 1, nitem); 
		if( nShow<nitem ) needslider = 1;

		fillRect(x,y,w,h,guiColorStill);
		if( needslider && slider(id+GenUIID(0), x+w-2, y+2, 20, h-4, (double)0, (double)(nitem-nShow+1), 1.0, &slidervalue) ) {
			*firstitem = (int)(slidervalue+0.1);
		}

		wext = nShow<nitem ? w + 20 : w;
		drawRect(x,  y,  wext,  h,  0x77777777);
		drawRect(x+1,y+1,wext-2,h-2,0x77777777);

		for( k = 0; k<nShow; k++ ) {
			int iid = k + *firstitem;
			if( iid<nitem && listitem(id+GenUIID(k), x+2, y+2+k*guiListItemHeight, w-4, guiListItemHeight, items[iid], iid==*selection) )
				newSelection = iid;
		}

		if( needslider )
			drawRect(x+w-4, y, 2, h, 0x77777777);

		if( *selection != newSelection ) {
			*selection = newSelection	;
			return 1;
		}
		return 0;
	}

}; //namespace xglm {