#ifndef ________i_m_g_u_i__keys___h_____________________
#define ________i_m_g_u_i__keys___h_____________________

#define IMGUI_GLUT

#if defined(IMGUI_GLUT)

// leys
#define IMGUI_KEY_LEFT          GLUT_KEY_LEFT     
#define IMGUI_KEY_UP            GLUT_KEY_UP       
#define IMGUI_KEY_RIGHT         GLUT_KEY_RIGHT    
#define IMGUI_KEY_DOWN          GLUT_KEY_DOWN     
#define IMGUI_KEY_PAGE_UP       GLUT_KEY_PAGE_UP  
#define IMGUI_KEY_PAGE_DOWN     GLUT_KEY_PAGE_DOWN
#define IMGUI_KEY_HOME          GLUT_KEY_HOME     
#define IMGUI_KEY_END           GLUT_KEY_END      
#define IMGUI_KEY_INSERT        GLUT_KEY_INSERT   
// mouse 
#define IMGUI_LEFT_BUTTON       GLUT_LEFT_BUTTON  
#define IMGUI_MIDDLE_BUTTON     GLUT_MIDDLE_BUTTON
#define IMGUI_RIGHT_BUTTON      GLUT_RIGHT_BUTTON 
#define IMGUI_MOUSE_DOWN        GLUT_DOWN         
#define IMGUI_MOUSE_UP          GLUT_UP

#else

#error "IMGUI_GLUT not defined"

#endif
#define IMGUI_ESC             27
#define IMGUI_BACKSPACE       8
#define IMGUI_KEY_TAB         '\t'
#define IMGUI_SHIFT           GLUT_ACTIVE_SHIFT 
#define IMGUI_CTRL            GLUT_ACTIVE_CTRL  
#define IMGUI_ALT             GLUT_ACTIVE_ALT   

#endif // #ifndef ________i_m_g_u_i__keys___h_____________________