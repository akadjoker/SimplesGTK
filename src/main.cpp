#include <iostream>
#include "Layout.hpp"
#include "Menus.hpp"
#include "Widget.hpp"
#include "Widgets.hpp"

static float progress = 0.0;

void window_idle(Window *win)
{

  

    Event event;
    while (win->pollEvent(event))
    {
        //  std::cout << "Event type: " << event.type << std::endl;
        switch (event.type)
        {
        case Event::ButtonClick:
            std::cout << "Button Clicked" << event.buttonClick.button->GetId() << std::endl;
  
            break;
        case Event::ButtonCheck:
            std::cout << "Button Checked" << std::endl;
            break;
        default:
            break;
        }
    }
}

void create_main(Application *app)
{

    Window *window = app->CreateWindow("Teste", 880, 400);
    window->SetId("mainWindow");
    window->SetCenter();
    window->SetResizable(true);
    window->SetBorder(1);

    window->OnClose = [&]()
    {
        return false;
    };

    window->AddMouseEvents();
    window->AddKeyEvents();

    window->OnKeyPress = [&](int key)
    {
        if (key == KEY_Escape)
        {
            window->Close();
        }
        return true;
    };

    FixedLayout *fixed = window->CreateFixedLayout("layout");



    window->Add(fixed);

    window->Show();

    window->OnIdle = [&]()
    {
        window_idle(window);
        return true;
    };

    window->Run();
}

int main(int argc, char **argv)
{
    Application *app = new Application("com.djokersoft.teste");
    create_main(app);
    delete app;
    std::cout << "By By !\n";
    return 0;
}
