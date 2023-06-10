#include <iostream>
#include "Widgets.hpp"

static float progress = 0.0;

void window_idle(Window *win)
{

    if (win->Contains("layout"))
    {
        FixedLayout *grid =  dynamic_cast<FixedLayout*>(win->GetLayoutByID("layout"));

        if (grid->Contains("progressBar"))
        {
         ProgressBar *bar =  dynamic_cast<ProgressBar*>(grid->GetWidgetByID("progressBar"));
          bar->SetFraction(progress);

                // float progress = 0.0;
                // while (progress <= 1.0) 
                // {
                //     bar->SetFraction(progress);
                //     progress += 0.1;
                //     std::cout << "Progress: " << progress << std::endl;
                //     win->ProcessEvents();  // Atualiza a interface gráfica
                //     g_usleep(50000);      // Aguarda 500ms
                // }
             //make   bar->SetFraction(1.0);
        }
        } else
        {
            std::cout << "Não encontrou o layout" << std::endl;
        }

    // Exibição do progresso completo
    



    Event event;
    while(win->pollEvent(event))
    {
      //  std::cout << "Event type: " << event.type << std::endl;
        switch (event.type)
        {
            case Event::ButtonClick:
                std::cout << "Button Clicked" <<event.buttonClick.button->GetId() << std::endl;
                progress += 0.1;
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



    FixedLayout *fixed = window->CreateFixedLayout("layout");
    fixed->AddMouseEvents();




    MenuBar *menuBar = window->CreateMenuBar("MenuBar");



     SubMenu *fileMenu    = menuBar->CreateSubMenu("FileMenu", "idFile", "File");
     SubMenu *compileMenu = menuBar->CreateSubMenu("CompileMenu", "idCompile", "Compile");
     SubMenu *optionsMenu = menuBar->CreateSubMenu("OptionsMenu", "idOptions", "Options");

   
     MenuItem *quitMi = fileMenu->AddItem("Quit");
     MenuItem *openMi = fileMenu->AddItem("Open");



    MenuItem *stopMi    = compileMenu->AddItem("Build");
    CheckMenuItem *runMi     = compileMenu->AddCheckItem(true,"Run");
    


    runMi->SetDrawAsRadio(true);
    compileMenu->AddSeparator();

    RadioMenuItem *debugMi   = compileMenu->AddRadioItem("Debug");
    debugMi->AddRadioItem("Release","idRelease");
    debugMi->AddRadioItem("Profile","idProfile");
    compileMenu->AddSeparator();
    


    SubMenu * menuCompiler = compileMenu->AddSubMenu("idCompile", "Compile", "Compile");
  
    menuCompiler->AddItem("Linux","idLinux");
    menuCompiler->AddItem("Android","idAndroid");
    menuCompiler->AddItem("Web","idWeb");

    SubMenu * menuOptions = menuCompiler->AddSubMenu("idOptions", "Options", "Options");
    menuOptions->AddItem("Debug","idDebug");
    menuOptions->AddItem("Release","idRelease");
    menuOptions->AddItem("Profile","idProfile");


  



    fixed->Add(menuBar,0,0,800,30);

    
    Menu * popUp = window->CreateMenu( "idPopUp");
          MenuItem  *item= popUp->CreateItem("Item 1", "idItem1");
          popUp->Append(item);
                     item= popUp->CreateItem("Item 2", "idItem2");
          popUp->Append(item);

     


    Button *b = fixed->CreateButton("button1", "Button 1");
     fixed->Add(b,100,40,100,30);
     b->OnClick  = [&]() -> bool
     {
          popUp->Popup(fixed,500,500,Gravity::Center,Gravity::Center);
         std::cout << "Button 1 Clicked" << std::endl;
         return true;
     };
     fixed->OnMousePressed = [&](int x, int y, int button) -> bool
     {
        popUp->Popup(fixed,x,y,Gravity::Top,Gravity::TopLeft);
         std::cout << "Mouse Pressed: " << x << "," << y << std::endl;
         return true;
     };

    ProgressBar *p = fixed->CreateProgressBar("progressBar");
    p->SetOrientation(Orientation::Vertical);
    fixed->Add(p,200,100,10,100);

    LevelBar *l = fixed->CreateLevelBar(0.0,100.0,"levelBar");
    l->SetValue(70.7);
    l->SetOrientation(Orientation::Vertical);
    fixed->Add(l,20,120,10,100);


    

    window->Add(fixed);

    // FixedLayout *fixed = window->CreateFixedLayout("layout");

    // Button *button = fixed->CreateButton("button1", "Button 1");
    // fixed->Add(button,10,10,100,30);
   

    // window->Add(fixed);
  
  /*
   GtkWidget* button1 = gtk_button_new_with_label("Botão 1");
    gtk_grid_attach(GTK_GRID(grid), button1, 0, 0, 1, 1); // (widget, left, top, width, height)

    GtkWidget* button2 = gtk_button_new_with_label("Botão 2");
    gtk_grid_attach(GTK_GRID(grid), button2, 1, 0, 1, 1);

    GtkWidget* button3 = gtk_button_new_with_label("Botão 3");
    gtk_grid_attach(GTK_GRID(grid), button3, 0, 1, 2, 1);

  */
  
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
