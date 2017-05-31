#include <memory.h>
#include "myglut.h"
#include "imgui.h"
#include "imgui_keys.h"

#ifndef CLAMP
#define CLAMP(x,min,max) ((x)<(min) ? (min) : ((x)>(max)?(max):(x)) )
#endif

namespace xglm {

	void drawRect(int x, int y, int w, int h, unsigned int color);
	void drawCrossX(int x, int y, int w, int h, unsigned int color);
	void fillRect(int x, int y, int w, int h, unsigned int color);
	void drawText(const char text[], int x, int y, unsigned int color, void * font);
	void drawText(const char text[], unsigned int color, void * font);
	int  getTicks();

	static int guiButtonSink  = 2;
	static int guiColorStill  = 0xCCBBAA;
	static int guiColorHot    = 0xFFDDCC;
	static int guiColorCheck  = 0xFFEEDD;
	static int guiColorWhite  = 0xFFFFFF;
	static int guiColorFocus  = 0xFFEEDD;
	static int guiColorEdit   = 0x550000;
	static int guiColorPicked = 0xFFEEDD;
	static int guiColorLabel  = 0x333333;

	static GuiFont guiFont(GLUT_BITMAP_TIMES_ROMAN_24);

	static int AlignY(int H, int h)
	{
		return (H-h)/2+4;
	}

	int GuiFont::setFont(void *f)
	{
		if( f == GLUT_BITMAP_TIMES_ROMAN_24 ) 
			mFontHeight = 24;
		else if( f==GLUT_STROKE_ROMAN )
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
		if (mGuiState.key == IMGUI_KEY_TAB)
			mGuiState.kbditem = 0;
		// Clear the entered key
		mGuiState.key = 0;
		mGuiState.keychar = 0;
	}
		
	void ImGUI::onKeyboard(unsigned char key, int modifier, int x, int y)
	{
		mGuiState.lastkeychar  = mGuiState.keychar  = key;
		mGuiState.lastkey      = mGuiState.key      = key;
		mGuiState.lastmodifier = mGuiState.modifier = modifier;
	}

	void ImGUI::onSpecial(unsigned char key, int modifier, int x, int y)
	{
		mGuiState.lastkey      = mGuiState.key      = MapGLUTSpecialKey(key);
		mGuiState.lastmodifier = mGuiState.modifier = modifier;
	}
	
	void ImGUI::onKeyboardUp(unsigned char key, int modifier, int x, int y)
	{
		mGuiState.modifier = modifier;
	}

	void ImGUI::onSpecialUp(unsigned char key, int modifier, int x, int y)
	{
		mGuiState.modifier = modifier;
	}

	void ImGUI::onMouse(int button, int state, int x, int y)
	{
		if ( button == IMGUI_LEFT_BUTTON ) 
		{	//这里只处理鼠标**左**键
			mGuiState.mousedown = (state==IMGUI_MOUSE_DOWN);
			mGuiState.mousex = x;
			mGuiState.mousey = y;
		}
		else if ( button==3 )
			mGuiState.wheel = state ? 1 : 0;
		else if ( button==4 )
			mGuiState.wheel = state ? -1 : 0;
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
		int alignX = 0;
		int alignY = AlignY(h, guiFont.mFontHeight);
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
				drawText(label, x+alignX, y+alignY,guiColorLabel, guiFont.mGlutFont);
			} else {
				// mouse is down，将按钮的位置进行稍许偏移，增加动感
				fillRect(x+guiButtonSink, y-guiButtonSink, w, h, guiColorHot);
				// 添加按钮的标签
				drawText(label, x+alignX+guiButtonSink, y-guiButtonSink+alignY, guiColorLabel, guiFont.mGlutFont);
			}
		} else {
			// button is not hot
			fillRect(x, y, w, h, guiColorStill);
			drawText(label, x+alignX, y+alignY, guiColorLabel, guiFont.mGlutFont);
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
		int bs = w<h ? w : h;
		int textAlignY = AlignY(h, guiFont.mFontHeight);
		// Check whether the button should be hot
		if (hitRect(x, y, w, h))
		{
			mGuiState.hotitem = id;
			if (mGuiState.activeitem == 0 && mGuiState.mousedown)
				mGuiState.activeitem = id;
		}
		// Draw radio button
		fillRect(x, y, bs, bs, mGuiState.hotitem == id ? guiColorHot : guiColorStill);
		drawText(label, x+bs+4, y+textAlignY, guiColorLabel, guiFont.mGlutFont);
		if ( *value ) // is checked, then draw a cross
			drawCrossX(x+1, y+1, bs-2, bs-2, guiColorCheck);

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


	void ImGUI::radioFrame(int x, int y, int w, int h)
	{
		drawRect(x,y,w,h,guiColorHot);
	}

	int ImGUI::radio(int id, int x, int y, int w, int h, char label[], int reference, int *value)
	{
		int bs = w<h ? w : h;
		int textAlignY = AlignY(h, guiFont.mFontHeight);
		// Check whether the button should be hot
		if (hitRect(x, y, w, h))
		{
			mGuiState.hotitem = id;
			if (mGuiState.activeitem == 0 && mGuiState.mousedown)
				mGuiState.activeitem = id;
		}
		// Draw radio button
		fillRect(x,y, bs, bs, mGuiState.hotitem == id ? guiColorHot : guiColorStill);
		drawText(label, x+bs+4, y+textAlignY, guiColorLabel, guiFont.mGlutFont);
		if ( reference == *value )
		{
			fillRect(x+bs/4,y+bs/4, bs/2, bs/2, guiColorCheck);
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
			mGuiState.kbditem = id;
			mGuiState.hotitem = id;
			if (mGuiState.activeitem == 0 && mGuiState.mousedown)
				mGuiState.activeitem = id;
		}

		// If no widget has keyboard focus, take it
		if (mGuiState.kbditem == 0)
			mGuiState.kbditem = id;

		// render the bar
		if (mGuiState.kbditem == id) {
			fillRect(x, y, w, h, guiColorFocus );
			fillRect(x+hintsize, y+hintsize, w-hintsize*2, h-hintsize*2, guiColorStill);
		}
		else {
			fillRect(x, y, w, h, guiColorStill );
		}
		
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
			switch (mGuiState.key)
			{
			case IMGUI_KEY_TAB:
			case IMGUI_RETURN:
				// If tab is pressed, lose keyboard focus.
				// Next widget will grab the focus.
				mGuiState.kbditem = 0;
				// If shift was also pressed, we want to move focus
				// to the previous widget instead.
				if (mGuiState.modifier & IMGUI_SHIFT)
					mGuiState.kbditem = mGuiState.lastwidget;
				// Also clear the key so that next widget
				// won't process it
				mGuiState.key = 0;
				break;
			case IMGUI_KEY_DOWN:
			case IMGUI_KEY_LEFT:
				// Slide slider up (if not at zero)
				if (*value > min) {
					(*value) = CLAMP(*value - delta, min, max);
					return 1;
				}
				break;
			case IMGUI_KEY_UP:
			case IMGUI_KEY_RIGHT:
				// Slide slider down (if not at max)
				if (*value < max) {
					(*value) = CLAMP(*value + delta, min, max);
					return 1;
				}
				break;
			default:
				if( mGuiState.wheel>0 && *value<max) {
					(*value) = CLAMP(*value + delta, min, max);
					mGuiState.wheel = 0;
					return 1;
				}
				else if( mGuiState.wheel<0 && *value>min) {
					(*value) = CLAMP(*value - delta, min, max);
					mGuiState.wheel = 0;
					return 1;
				}
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

	int ImGUI::slider(int id, int x, int y, int w, int h, float min, float max, float delta, float * value)
	{
		double v = *value;
		if( slider(id, x, y, w, h, (double)min, (double)max, (double)delta, & v) ) {
			*value = (float)v;
			return 1;
		}
		return 0;
	}	
	
	int ImGUI::slider(int id, int x, int y, int w, int h, int min, int max, int delta, int * value)
	{
		double v = *value;
		if( slider(id, x, y, w, h, (double)min, (double)max, (double)delta, & v) ) {
			*value = (int)(v+0.1);
			return 1;
		}
		return 0;
	}

	int ImGUI::editbox(int id, int x, int y, int w, int h, char textbuf[], int maxbuf)
	{
		int alignX = 5, alignY = AlignY(h, guiFont.mFontHeight);
		int len = strlen(textbuf);
		int finishEditting = 0;
		// Check whether the button should be hot
		if (hitRect(x, y, w, h))
		{
			mGuiState.kbditem = id;
			mGuiState.hotitem = id;
			if (mGuiState.activeitem == 0 && mGuiState.mousedown)
				mGuiState.activeitem = id;
		}
		// If no widget has keyboard focus, take it
		if (mGuiState.kbditem == 0)
			mGuiState.kbditem = id;
		// If we have keyboard focus, show it
		if (mGuiState.kbditem == id)
			fillRect(x-1, y-1, w+2, h+2, guiColorFocus);

		// Render the text box 
		if ( mGuiState.hotitem == id || mGuiState.activeitem == id ) {
			// 'hot' or 'active'
			fillRect(x, y, w, h, guiColorHot);
		} else {
			fillRect(x, y, w, h, guiColorStill   );
		}

		// show text
		drawText(textbuf, x+alignX, y+alignY, guiColorEdit, guiFont.mGlutFont);
		// Render cursor if we have keyboard focus
		if ( mGuiState.kbditem == id && ( getTicks() >> 18) & 1)
			drawText("_", guiColorEdit, guiFont.mGlutFont);

		// If we have keyboard focus, we'll need to process the keys
		if (mGuiState.kbditem == id)
		{
			switch (mGuiState.key)
			{
			case IMGUI_RETURN:
				finishEditting = 1;
			case IMGUI_KEY_TAB:
				// If tab is pressed, lose keyboard focus.
				// Next widget will grab the focus.
				mGuiState.kbditem = 0;
				// If shift was also pressed, we want to move focus
				// to the previous widget instead.
				if (mGuiState.modifier & IMGUI_SHIFT)
					mGuiState.kbditem = mGuiState.lastwidget;
				// Also clear the key so that next widget
				// won't process it
				mGuiState.key = 0;
				break;
			case IMGUI_BACKSPACE:
				if( len > 0 ) {
					textbuf[--len] = 0;
					//textChanged = 1;
				}
				mGuiState.key = 0;
				break;
			}
			if (mGuiState.keychar >= 32 && mGuiState.keychar < 127 && len < maxbuf ) {
				textbuf[len] = mGuiState.keychar;
				textbuf[++len] = 0;
				//textChanged = 1;
			}
		}

		mGuiState.lastwidget = id;

		// If button is hot and active, but mouse button is not
		// down, the user must have clicked the button.
		if (   mGuiState.mousedown == 0 
			&& mGuiState.hotitem == id 
			&& mGuiState.activeitem == id )
			mGuiState.kbditem = id;
		return finishEditting;
	}

	int ImGUI::textlabel(int id, int x, int y, int w, int h, char text[])
	{
		drawText(text, x, y+AlignY(h, guiFont.mFontHeight), guiColorLabel, guiFont.mGlutFont);
		return 0;
	}

	int ImGUI::listbox (int id, int x, int y, int w, int h, char*items[], int nitem, int *firstitem, int *selection)
	{
		int needslider = 0;
		int nShow, k, witems = w;
		int newSelection = *selection;
		int listItemHeight = guiFont.mFontHeight+5;

		nShow = (h-4) / listItemHeight;
		nShow = CLAMP( nShow, 1, nitem); 
		if( nShow<nitem ) {
			needslider = 1;
			witems -= 22;
		}
		
		if( needslider && hitRect(x,y,w,h) )
			mGuiState.kbditem = 0; // force slider take keyboard

		fillRect(x, y, w, h,guiColorStill);
		int slidervalue = nitem-*firstitem;
		if( needslider && slider(id+GenUIID(0), x+witems, y+2, 
			w-witems-2, h-4, nShow-1, nitem, 1, &slidervalue) ) {
			*firstitem = nitem-(int)(slidervalue+0.1);
		}

		drawRect(x,  y,  w,  h,  0x77777777);

		for( k = 0; k<nShow; k++ ) {
			int iid = needslider ? k + *firstitem : k;
			if( iid<nitem && listitem(id+GenUIID(k), x+2, y+h-3-(k+1)*listItemHeight, 
				witems-4, listItemHeight, items[iid], iid==*selection) )
				newSelection = iid;
		}

		if( needslider )
			// a separator to fill the gap
			fillRect(x+witems-2, y, 2, h, 0x77777777);

		if( *selection != newSelection ) {
			*selection = newSelection	;
			return 1;
		}
		return 0;
	}
	
	int ImGUI::listitem(int id, int x, int y, int w, int h, char label[], int selected)
	{
		int alignY = AlignY(h, guiFont.mFontHeight);
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
		drawText(label, x+5, y+alignY, guiColorLabel, guiFont.mGlutFont);

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
}; //namespace xglm {
