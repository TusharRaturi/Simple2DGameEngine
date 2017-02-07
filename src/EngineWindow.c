#include "EngineWindow.h"

Window* new_Window(char *Name, RECT winBounds)
{
    Window *window = (Window*)malloc(sizeof(Window));
    window->windowBounds = winBounds;
    window->Name = Name;

    window->deviceContext = NULL;
    window->renderingContext = NULL;
    window->processor = (WNDPROC)MainProc;

    window->init = *init_impl;
    window->create = &create_impl;
    window->update = &update_impl;

    window->isCloseRequested = &isCloseRequested_impl;

    return window;
}

static void initGL()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, 600, 0, 600, -1.0, 1.0);

    // Select ModelView Matrix
    glMatrixMode(GL_MODELVIEW);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // No need for depth test(extra information) because we are doing 2D
    glDisable(GL_DEPTH_TEST);
}

BOOL init_impl(HINSTANCE hIn, Window *self)
{
    WNDCLASSEX MWindow;         /* Data structure for the window class */
    HICON icon;                 /* The icon for the window */
    int iFormat;
    PIXELFORMATDESCRIPTOR pfd;  /* This informs the system how we are going to use the DC. In other words, the features of OpenGl we need active */

    // Load the icon
    //icon = LoadIcon(hIn, MAKEINTRESOURCE(ICON_ID));

    // Fill the Window structure
    MWindow.hInstance = hIn;
    // Class Name of the window class
    MWindow.lpszClassName = self->Name;
    // Function called by windows
    MWindow.lpfnWndProc = self->processor;
    // Catch double clicks and own the Device Context(for opengl)
    MWindow.style = CS_DBLCLKS | CS_OWNDC;
    // Size of window structure
    MWindow.cbSize = sizeof (WNDCLASSEX);

    // Use default icon and mouse-pointer
    MWindow.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    MWindow.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
    //MWindow.hIcon = icon;
    //MWindow.hIconSm = icon;
    MWindow.hCursor = LoadCursor(NULL, IDC_ARROW);
    // Set the Resource Menu
    MWindow.lpszMenuName = NULL;//MAKEINTRESOURCE(IDR_MENU1);
    // No extra bytes after the window class
    MWindow.cbClsExtra = 0;
    // Structure or the window instance
    MWindow.cbWndExtra = 0;
    // Use Windows's default colour as the background of the window
    MWindow.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

    // Register the window class, and if it fails quit the program
    if (!RegisterClassEx(&MWindow))
    {
        printf("Could not register window class.");
        //HandleException(NULL, "Could not register window class.", "Error");
        return FALSE;
    }

    // Adjust Window To True Requested Size
    AdjustWindowRectEx(&(self->windowBounds), WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_ACCEPTFILES);

    //  Create the program after registering the class
    if(!(self->handle = CreateWindowEx (
           WS_EX_APPWINDOW | WS_EX_ACCEPTFILES,     /* Extended possibilities for variation */
           self->Name,     /* Classname */
           self->Name,           /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           self->windowBounds.right,           /* The programs width */
           self->windowBounds.bottom,          /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hIn,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           )))
    {
        return FALSE;
    }

    // Create Device Context
    if(!(self->deviceContext = GetDC(self->handle)))
    {
        return FALSE;
    }

    // Zero out the memory(initialize)
    ZeroMemory( &pfd, sizeof( pfd ) );
    pfd.nSize = sizeof( pfd );
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    // Picks the best match for what we requested
    if(!(iFormat = ChoosePixelFormat(self->deviceContext, &pfd)))
    {
        return FALSE;
    }

    if(!SetPixelFormat(self->deviceContext, iFormat, &pfd))
    {
        return FALSE;
    }

    // Create Render Context
    if(!(self->renderingContext = wglCreateContext(self->deviceContext)))
    {
        return FALSE;
    }

    // Try To Activate The Rendering Context
    if(!wglMakeCurrent(self->deviceContext, self->renderingContext))
	{
		return FALSE;
	}

    ShowWindow(self->handle, SW_SHOW);
    UpdateWindow(self->handle);
    SetForegroundWindow(self->handle);

    initGL();
    isRunning = TRUE;

    return TRUE;
}

static BOOL create_impl(HINSTANCE hi)
{
    return TRUE;
}

static void update_impl(HWND hwnd, MSG *messages)
{
    if (PeekMessage(&(*messages),NULL,0,0,PM_REMOVE))// Is There A Message Waiting?
    {
        if ((*messages).message == WM_QUIT)// Have We Received A Quit Message?
        {
            isRunning = FALSE;
        }
        else
        {
            /* Translate virtual-key messages into character messages */
            TranslateMessage(&(*messages));
            /* Send(Dispatch) message to WindowProcedure */
            DispatchMessage(&(*messages));
        }
    }
}

LRESULT CALLBACK MainProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Handle messages
    switch(message)
    {
        case WM_CLOSE:
            if(MessageBox(hwnd, "Are you sure you want to quit?", "Quit?", MB_YESNO | MB_ICONQUESTION) == IDYES)
                DestroyWindow(hwnd);
        break;

        case WM_DESTROY:
            PostQuitMessage(0);       // send a WM_QUIT to the message queue
        break;

        default:                      // for messages that we don't deal with
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

BOOL isCloseRequested_impl()
{
    return !isRunning;
}
