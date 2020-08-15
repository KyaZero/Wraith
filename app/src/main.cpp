#include <App.h>

int WinMain()
{
    {
        fw::App app;

        if (!app.Start())
        {
            return 1;
        }

        app.Run();
    }

    return 0;
}