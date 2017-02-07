#include "Core.h"

void init(HINSTANCE hThisInstance, char *Name, RECT WinSize)
{
    // Initialize Engine and create window
    window = new_Window(Name, WinSize);
    window->init(hThisInstance, window);
}

static void input()
{
    //handle input
}

static void update()
{
    //update game
}

static void render(MSG *msg)
{
    // Set the clear color
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the framebuffer

    glLoadIdentity();

    glColor3f(0.1,0.7,0.3);
    rect(0, 0, 50, 50);

    //render game
    SwapBuffers(window->deviceContext);
    window->update(window->handle, msg);
}

static void rect(float x, float y, float width, float height)
{
    glPushMatrix();

    glTranslatef(x, y,0);

    glBegin(GL_QUADS);

    glVertex2f(0, 0);
    glVertex2f(0, height);
    glVertex2f(width, height);
    glVertex2f(width, 0);

    glEnd();

    glPopMatrix();
}

static void gameLoop(MSG *msg)
{
    while(!window->isCloseRequested())
    {
        input();
        update();
        render(msg);
    }
    cleanUp();
}

static void cleanUp()
{
    // Clean everything up and destroy things
}

int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
    MSG messages;
    RECT WinSize;

    WinSize.left = 0l;
    WinSize.top = 0l;
    WinSize.right = 600;
    WinSize.bottom = 600;

    init(hThisInstance, "Engine", WinSize);

    gameLoop(&messages);

    cleanUp();

    return messages.wParam;
}
