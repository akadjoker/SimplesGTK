#include <iostream>
#include "Layout.hpp"
#include "Menus.hpp"
#include "Widget.hpp"
#include "Widgets.hpp"

Window *window;
Notebook *notebook;
ListBox *listbox;

bool on_idle()
{
  
      // Event event;
    // while (win->pollEvent(event))
    // {
    //     //  std::cout << "Event type: " << event.type << std::endl;
    //     switch (event.type)
    //     {
    //     case Event::ButtonClick:
    //         std::cout << "Button Clicked" << event.buttonClick.button->GetId() << std::endl;
  
    //         break;
    //     case Event::ButtonCheck:
    //         std::cout << "Button Checked" << std::endl;
    //         break;
    //     default:
    //         break;
    //     }
    // }
    return true;
}

bool on_key_press( int key, int state)
{
    if (key == KEY_Escape)
    {
        
        window->Close();
    }
    return false;
}



void create_main(Application *app)
{

    window = app->CreateWindow("Teste", 880, 400);
   
    window->SetId("mainWindow");
    window->SetCenter();
    window->SetResizable(true);
    window->SetBorder(1);
    window->SetProcessEvents(true);
    window->SetAcceptDrops(true);

     window->OnKeyPress = on_key_press;
     window->OnIdle     = on_idle;
    window->OnClose = [&]() -> bool
    {
        std::cout << "Window Closed" << std::endl;
        
        return false;
    };
    notebook = window->CreateNotebook();
    
    {
      FixedLayout *fixed = window->CreateFixedLayout();
      notebook->AppendPage(fixed, "Dektops");
      listbox =   fixed->CreateListBox("Code");
      listbox->SetAcceptDrops(true);
      listbox->OnDrop = [=](int x, int y, const std::string &data) -> bool
      {
          std::cout << "Drop: " << data << std::endl;
          listbox->AddLabel("data");
          return true;
      };

      fixed->Add(listbox,10,10,200,200);
    }

 {
    FixedLayout *fixed = window->CreateFixedLayout();
    notebook->AppendPage(fixed, "Android");
    }

 {
    FixedLayout *fixed = window->CreateFixedLayout();
    notebook->AppendPage(fixed, "Web");
    }


    

    window->Add(notebook);



        

    window->Show();

  // window->Run();
}

int main(int argc, char **argv)
{
    Application app("com.djokersoft.teste");
    app.OnActivate = [&]() -> bool
    {
        create_main(&app);
        std::cout << "Application Activated" << std::endl;
        return true;
    };
   //create_main(&app);
   app.Run(argc,argv);

 
       
    
    std::cout << "By By !\n";
    return 0;
}
