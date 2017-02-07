#include<windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

typedef struct EngineWindow
{
    HWND handle;                /* This is the handle for our window */
    HDC deviceContext;          /* Private GDI Device Context of the window */
    HGLRC renderingContext;     /* Permanent Rendering Context  of the window */
    WNDPROC processor;
    RECT windowBounds;          /* Current Window's Rectangle */
    char *Name;

    /* Initialize Window */
    BOOL (*init)(HINSTANCE, struct EngineWindow*);// = NULL;

    /* Creates the window */
    BOOL (*create)(HINSTANCE);// = NULL;

    /* Will update the window */
    void (*update)(HWND, MSG*);// = NULL;

    BOOL (*isCloseRequested)();// = NULL;
}Window;

Window* new_Window(char*, RECT);
LRESULT CALLBACK MainProc(HWND, UINT, WPARAM, LPARAM);
static void initGL();

static BOOL isRunning = FALSE;

/* Initialize Window Implementation */
BOOL init_impl(HINSTANCE, Window*);

/* Create window Implementation */
static BOOL create_impl(HINSTANCE);

/* Update the window Implementation */
static void update_impl(HWND, MSG*);

static BOOL isCloseRequested_impl();
