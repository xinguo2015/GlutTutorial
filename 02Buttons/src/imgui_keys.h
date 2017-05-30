#ifndef ________i_m_g_u_i__keys___h_____________________
#define ________i_m_g_u_i__keys___h_____________________

#define IMGUI_GLUT

#define IMGUI_ESC             27
#define IMGUI_BACKSPACE       8
#define IMGUI_RETURN          '\r'
#define IMGUI_KEY_TAB         '\t'

#if defined(IMGUI_GLUT)
// special keys in glut
#define MapGLUTSpecialKey(k)    ((k)<<8)
#define IMGUI_KEY_F1            MapGLUTSpecialKey(GLUT_KEY_F1  )
#define IMGUI_KEY_F2            MapGLUTSpecialKey(GLUT_KEY_F2  )
#define IMGUI_KEY_F3            MapGLUTSpecialKey(GLUT_KEY_F3  )
#define IMGUI_KEY_F4            MapGLUTSpecialKey(GLUT_KEY_F4  )
#define IMGUI_KEY_F5            MapGLUTSpecialKey(GLUT_KEY_F5  )
#define IMGUI_KEY_F6            MapGLUTSpecialKey(GLUT_KEY_F6  )
#define IMGUI_KEY_F7            MapGLUTSpecialKey(GLUT_KEY_F7  )
#define IMGUI_KEY_F8            MapGLUTSpecialKey(GLUT_KEY_F8  )
#define IMGUI_KEY_F9            MapGLUTSpecialKey(GLUT_KEY_F9  )
#define IMGUI_KEY_F10           MapGLUTSpecialKey(GLUT_KEY_F10 )
#define IMGUI_KEY_F11           MapGLUTSpecialKey(GLUT_KEY_F11 )
#define IMGUI_KEY_F12           MapGLUTSpecialKey(GLUT_KEY_F12 )
// leys
#define IMGUI_KEY_LEFT          MapGLUTSpecialKey(GLUT_KEY_LEFT     )
#define IMGUI_KEY_UP            MapGLUTSpecialKey(GLUT_KEY_UP       )
#define IMGUI_KEY_RIGHT         MapGLUTSpecialKey(GLUT_KEY_RIGHT    )
#define IMGUI_KEY_DOWN          MapGLUTSpecialKey(GLUT_KEY_DOWN     )
#define IMGUI_KEY_PAGE_UP       MapGLUTSpecialKey(GLUT_KEY_PAGE_UP  )
#define IMGUI_KEY_PAGE_DOWN     MapGLUTSpecialKey(GLUT_KEY_PAGE_DOWN)

#define IMGUI_KEY_HOME          MapGLUTSpecialKey(GLUT_KEY_HOME     )
#define IMGUI_KEY_END           MapGLUTSpecialKey(GLUT_KEY_END      )
#define IMGUI_KEY_INSERT        MapGLUTSpecialKey(GLUT_KEY_INSERT   )
// key modifiers
#define IMGUI_SHIFT             GLUT_ACTIVE_SHIFT 
#define IMGUI_CTRL              GLUT_ACTIVE_CTRL  
#define IMGUI_ALT               GLUT_ACTIVE_ALT   
// mouse 
#define IMGUI_LEFT_BUTTON       GLUT_LEFT_BUTTON  
#define IMGUI_MIDDLE_BUTTON     GLUT_MIDDLE_BUTTON
#define IMGUI_RIGHT_BUTTON      GLUT_RIGHT_BUTTON 
#define IMGUI_MOUSE_DOWN        GLUT_DOWN         
#define IMGUI_MOUSE_UP          GLUT_UP

#else

#error "IMGUI_GLUT not defined"

#endif

#endif // #ifndef ________i_m_g_u_i__keys___h_____________________