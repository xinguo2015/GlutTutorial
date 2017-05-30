#ifndef ________i_m_g_u_i_____h_____________________
#define ________i_m_g_u_i_____h_____________________

#include <stdlib.h>
#include <stdio.h>


//===========================================================================

// 
// Generate a *fake* unique ID for gui controls at compiling/run time
//
//   [L:16-1][N:16-1]
//        XOR
//   [F:32 --------1]
//

#define GenUIID(N) ( ((__LINE__<<16) | ( N & 0xFFFF))^((long)&__FILE__) )

//
// GenUIID(0) will give a unique ID at each source code line. 
// If you need one UI ID per line, just call GenUIID with 0
//
//               GenUIID(0)
//
// But, two GenUIID(0) calls at the same line will give the same ID.
// So, in a while/for loop body, GenUIID(0) will give you the same ID.
// In this case, you need call GenUIID with different N parameter: 
//
//               GenUIID(N)
//
//===========================================================================

namespace xglm {

	class GuiFont 
	{
	public:
		GuiFont(void *f) { setFont(f); }
		int setFont(void *f);
	public:
		void * mGlutFont;
		int    mFontHeight;
	};

	class ImGUIState
	{
	public:
		int mousex;     // x - mouse position 鼠标x坐标
		int mousey;     // y - mouse position 鼠标y坐标
		int mousedown;  // 1 - yes, 0 - no 是否按下鼠标

		int hotitem;    // widget below the mouse cursor 光标下的组件ID
		int activeitem; // widget under interaction 正在交互的组件ID

		int key;        // key pressed 被按下的普通键
		int keychar;    // char input 输入的字符（ASCII码）
		int modifier;   // modifier key
		int lastkey;
		int lastkeychar;
		int lastmodifier;
		
		int lastwidget; // last widget that was processed 上一个接受键盘的组件
		int kbditem;    // widget with keyboard focus 接受键盘输入的组件ID
	};

	class ImGUI
	{
	public:
		int  isVisible() const         { return mVisible; }
		void setVisible(int flag)      { mVisible = flag; }
		void setfont(void* font);
		ImGUIState getGUIState() const { return mGuiState; }
	public:
		void initialize();
		void prepare();
		void finish();	

		// glut keyboard and mouse
		void onKeyboardUp(unsigned char key, int modifier, int x, int y);
		void onKeyboard(unsigned char key, int modifier, int x, int y);
		void onSpecialUp(unsigned char key, int modifier, int x, int y);
		void onSpecial(unsigned char key, int modifier, int x, int y);
		void onMouse(int button, int state, int x, int y);
		void onMotion(int x, int y);
		int  hitRect(int x, int y, int w, int h);

		// widgets
		int button   (int id, int x, int y, int w, int h, char label[]);
		int checkbox (int id, int x, int y, int w, int h, char label[], int *value);
		int radio    (int id, int x, int y, int w, int h, char label[], int reference, int *value);
		int listbox  (int id, int x, int y, int w, int h, char*items[], int nitem, int *liststart, int *value);
		int slider   (int id, int x, int y, int w, int h, double minv, double maxv, double delta, double * value);
		int slider   (int id, int x, int y, int w, int h, float  minv, float maxv, float delta, float * value);
		int slider   (int id, int x, int y, int w, int h, int minv, int maxv, int delta, int * value);
		int editbox  (int id, int x, int y, int w, int h, char textbuf[], int maxbuf);
		int textlabel(int id, int x, int y, int w, int h, char text[]);

		void radioFrame(int x, int y, int w, int h);
	protected:
		int listitem(int id, int x, int y, int w, int h, char label[], int selected);
		int slider_base(int id, int x, int y, int w, int h, double minv, double maxv, double delta, double * value);

	protected:
		ImGUIState mGuiState;
		int        mVisible;
	};

} //namespace xglm {

#endif // ifndef ________i_m_g_u_i_____h_____________________