#include <App.h>

int WinMain()
{
    {
        fw::App app;

        if (!app.Start())
        {
            system("pause");
            return 1;
        }

        app.Run();
    }

    return 0;
}