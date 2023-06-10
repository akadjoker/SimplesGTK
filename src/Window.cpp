#include "Window.hpp"
#include "Text.hpp"

void on_file_selected(GtkFileChooser *chooser, gint response_id, gpointer user_data)
{
        
    FileChooser *fileChooser = (FileChooser *)user_data;
    if (response_id == GTK_RESPONSE_ACCEPT)
    {
        char *filename = gtk_file_chooser_get_filename(chooser);
        if (fileChooser->OnFileSelected)
            fileChooser->OnFileSelected(filename);
        
           
        g_free(filename);
    } 

    fileChooser->Close();


}

void on_folder_selected(GtkFileChooser *chooser, gint response_id, gpointer user_data)
{
        
    FolderChooser *folderChooser = (FolderChooser *)user_data;
    if (response_id == GTK_RESPONSE_ACCEPT)
    {
        char *filename = gtk_file_chooser_get_filename(chooser);
        if (folderChooser->OnFolderSelected)
            folderChooser->OnFolderSelected(filename);
        g_free(filename);
    } 

    folderChooser->Close();
}

gboolean on_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
    gdouble x = event->x;
    gdouble y = event->y;
    int button = event->button;
    Window *window = (Window *)user_data;
    if (window->OnMousePress)
        window->OnMousePress(x, y, button);
    return TRUE;
}

 gboolean on_button_release_event(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
    gdouble x = event->x;
    gdouble y = event->y;
    int button = event->button;
    Window *window = (Window *)user_data;
    if (window->OnMouseRelease)
        window->OnMouseRelease(x, y, button);
    return TRUE;
}

 gboolean on_motion_notify_event(GtkWidget *widget, GdkEventMotion *event, gpointer user_data)
{
    gdouble x = event->x;
    gdouble y = event->y;
    Window *window = (Window *)user_data;
    if (window->OnMousMove)
        window->OnMousMove(x, y);
    return TRUE;
}

 gboolean on_key_release_event(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{

    guint keyval = event->keyval;
    Window *window = (Window *)user_data;


    if (window->OnKeyPress)
    {
        if ( window->OnKeyRelease(keyval))
            return true;
    }
    
    return false;
}

 gboolean on_key_press_event(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{

    guint keyval = event->keyval;
    Window *window = (Window *)user_data;
    if (window->OnKeyPress)
    {
        if (window->OnKeyPress(keyval))
            return true;
    }
    
    return false;
}

 gboolean on_window_configure_event(GtkWidget *widget, GdkEventConfigure *event, gpointer user_data)
{
    GdkEventConfigure *configure_event = (GdkEventConfigure *)event;
    gint x = configure_event->x;
    gint y = configure_event->y;

    gint width = event->width;
    gint height = event->height;

    Event wevent;
    wevent.type        = Event::Resized;
    wevent.size.width  = width;
    wevent.size.height = height;
    


    Window *window = (Window *)user_data;
    window->pushEvent(wevent);

    window->m_width = width;
    window->m_height = height;

    window->m_x = x;
    window->m_y = y;

    if (window->OnMove)
    {
        if (window->OnMove(x, y))
            return true;
        
    }

    if (window->OnResize)
    {
         if (window->OnResize(width, height))
            return true;
    }
    //  FALSE para permitir que o evento seja propagado
    return false;
}





 gboolean on_idle_callback(gpointer user_data)
{
    Window *window = (Window *)user_data;

    if (window->OnIdle)
    {
        return window->OnIdle();
    }

    return true;
}

 void on_destroy(GtkWidget *widget, gpointer data)
{
    Window *window = (Window *)data;
  //  std::cout << "window on_destroy" << std::endl;
    window->Release();
}

 gboolean on_window_close_event(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{

 //   std::cout << "window on_close" << std::endl;
    Window *window = (Window *)user_data;
    if (window->OnClose)
    {
        return window->OnClose();
    }
    return false;
}


Window::Window()
{
}

bool Window::popEvent(Event& event)
{
    
    if (m_events.empty())
    {
        
    }


    if (!m_events.empty())
    {
        event = m_events.front();
        m_events.pop();
        return true;
    }

    return false;
}


bool Window::pollEvent(Event& event)
{
    if (popEvent(event))
    {
        return true;
    }
    else
    {
        return false;
    }
}
void Window::pushEvent(const Event& event)
{
    m_events.push(event);
}




void Window::CreateTopLevel(const std::string &title, int width, int height)
{
    m_widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    m_window = GTK_WINDOW(m_widget);
    m_container = GTK_CONTAINER(m_widget);
    m_width = width;
    m_height = height;

      gtk_widget_add_events(m_widget, GDK_BUTTON_PRESS_MASK);


    gtk_window_set_title(m_window, title.c_str());
    gtk_window_set_default_size(m_window, width, height);
    g_signal_connect(m_widget, "destroy", G_CALLBACK(on_destroy), this);
    g_signal_connect(m_widget, "delete-event", G_CALLBACK(on_window_close_event), this);
    g_signal_connect(m_widget, "configure-event", G_CALLBACK(on_window_configure_event), this);
    g_signal_connect(m_widget, "key-press-event", G_CALLBACK(on_key_press_event), this);
    g_signal_connect(m_widget, "key-release-event", G_CALLBACK(on_key_release_event), this);
    g_signal_connect(m_widget, "button-press-event", G_CALLBACK(on_button_press_event), this);
    g_signal_connect(m_widget, "button-release-event", G_CALLBACK(on_button_release_event), this);
    g_signal_connect(m_widget, "motion-notify-event", G_CALLBACK(on_motion_notify_event), this);
    


  GtkWidget* menu = gtk_menu_new();

    // Criação do item de menu
    GtkWidget* menu_item = gtk_menu_item_new_with_label("Item do Menu");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);

    // Exibição do menu
    gtk_widget_show_all(menu);


    idle_id = g_idle_add(on_idle_callback, this);
    isMainWindow = true;
}

 void Window::CreatePopUp(const std::string &title, int width, int height)
{
    m_widget = gtk_window_new(GTK_WINDOW_POPUP);
    m_window = GTK_WINDOW(m_widget);
    m_container = GTK_CONTAINER(m_widget);
    m_width = width;
    m_height = height;
    
    gtk_window_set_title(m_window, title.c_str());
    gtk_window_set_default_size(m_window, width, height);
    g_signal_connect(m_widget, "destroy", G_CALLBACK(on_destroy), this);
    g_signal_connect(m_widget, "delete-event", G_CALLBACK(on_window_close_event), this);
    g_signal_connect(m_widget, "configure-event", G_CALLBACK(on_window_configure_event), this);
    g_signal_connect(m_widget, "key-press-event", G_CALLBACK(on_key_press_event), this);
    g_signal_connect(m_widget, "key-release-event", G_CALLBACK(on_key_release_event), this);
    g_signal_connect(m_widget, "button-press-event", G_CALLBACK(on_button_press_event), this);
    g_signal_connect(m_widget, "button-release-event", G_CALLBACK(on_button_release_event), this);
    g_signal_connect(m_widget, "motion-notify-event", G_CALLBACK(on_motion_notify_event), this);

   // idle_id = g_idle_add(on_idle_callback, this);
    isMainWindow = false;
}

void Window::CreateDialog(Window *parent, const std::string &title, int width, int height, bool isModal)
{

    m_widget = gtk_dialog_new();
    m_window = GTK_WINDOW(m_widget);
    m_width = width;
    m_height = height;
    

    GtkWidget *contentArea = gtk_dialog_get_content_area(GTK_DIALOG(m_widget));
    m_container = GTK_CONTAINER(contentArea);

    gtk_window_set_title(m_window, title.c_str());
    gtk_window_set_default_size(m_window, width, height);
    gtk_window_set_modal(m_window, isModal);

 //  gtk_window_set_transient_for(m_window, this->m_window);

    g_signal_connect(m_widget, "destroy", G_CALLBACK(on_destroy), this);
    g_signal_connect(m_widget, "delete-event", G_CALLBACK(on_window_close_event), this);
    g_signal_connect(m_widget, "configure-event", G_CALLBACK(on_window_configure_event), this);
    g_signal_connect(m_widget, "key-press-event", G_CALLBACK(on_key_press_event), this);
    g_signal_connect(m_widget, "key-release-event", G_CALLBACK(on_key_release_event), this);
    g_signal_connect(m_widget, "button-press-event", G_CALLBACK(on_button_press_event), this);
    g_signal_connect(m_widget, "button-release-event", G_CALLBACK(on_button_release_event), this);
    g_signal_connect(m_widget, "motion-notify-event", G_CALLBACK(on_motion_notify_event), this);

   // idle_id = g_idle_add(on_idle_callback, this);
   


    isMainWindow = false;
}

Window* Window::CreateDialog(const std::string &title, int width, int height, bool isModal)
{
    std::shared_ptr<Window> window = std::make_shared<Window>();
    window->CreateDialog(this, title, width, height, isModal);
    m_childs.push_back(window);
    return window.get();
}

Window::~Window()
{

        m_dialogs.clear();
        //m_layouts.clear();
     //   m_childs.clear();
    //    std::cout << "**** Delete Window: ("<< m_id<<")" << std::endl;
}

void Window::SetCenter()
{
    assert(m_widget );
    gtk_window_set_position(m_window, GTK_WIN_POS_CENTER);
}

void Window::SetPosition(int x, int y)
{
    assert(m_widget );
    gtk_window_move(m_window, x, y);
    m_x = x;
    m_y = y;
}

void Window::SetResizable(bool resizable)
{

    assert(m_widget );
    gtk_window_set_resizable(m_window, resizable);
}

void Window::Show()
{
    assert(m_widget );
    gtk_widget_show_all(m_widget);
}

void Window::Run()
{
    if (isMainWindow)
    {
        useLoop = true;  
        gtk_main();
           
    }
}

void Window::Close()
{
    assert(m_widget );
   // std::cout << "Close Window: "<<m_id << std::endl;
    gtk_window_close(m_window);
    
}

void Window::Release()
{
  //  std::cout << "Release Window: "<<m_id << std::endl;
    
    if (isMainWindow)
    {
     //  std::cout << "remove event" << std::endl;
        g_source_remove(idle_id);
        gtk_widget_destroy(m_widget);
        if (useLoop)
        {
      //      std::cout << "Quit main" << std::endl;
            gtk_main_quit();
        }
    }
    
}

void Window::ProcessEvents()
{
    gtk_main_iteration();
}

FrameLayout* Window::CreateFrameLayout(const std::string &label, const std::string &id)
{
    
    std::shared_ptr<FrameLayout> layout = std::make_shared<FrameLayout>(label);
    layout.get()->SetId(id);
    layout.get()->m_mainWindow = this;
    m_childs.push_back(layout);
    return layout.get();
}

BoxLayout* Window::CreateBoxLayout(Orientation orientation, int spacing, const std::string &id)
{
    
    std::shared_ptr<BoxLayout> layout = std::make_shared<BoxLayout>(orientation, spacing);
    layout.get()->SetId(id);
    m_childs.push_back(layout);
    layout.get()->m_mainWindow = this;
    return layout.get();

}

FixedLayout* Window::CreateFixedLayout(const std::string &id)
{

    std::shared_ptr<FixedLayout> layout = std::make_shared<FixedLayout>();
    layout.get()->SetId(id);
    m_childs.push_back(layout);
    layout.get()->m_mainWindow = this;
    return layout.get();

    
}

ScrollLayout* Window::CreateScrollLayout(const std::string &id)
{
        
        std::shared_ptr<ScrollLayout> layout = std::make_shared<ScrollLayout>();
        layout.get()->SetId(id);
        m_childs.push_back(layout);
        layout.get()->m_mainWindow = this;
        return layout.get();
    
}

ViewPortLayout* Window::CreateViewPortLayout(const std::string &id)
{   
    std::shared_ptr<ViewPortLayout> layout = std::make_shared<ViewPortLayout>();
    layout.get()->SetId(id);
    m_childs.push_back(layout);
    layout.get()->m_mainWindow = this;
    return layout.get();
    
}

GridLayout* Window::CreateGridLayout(const std::string &id)
{
    
    std::shared_ptr<GridLayout> layout = std::make_shared<GridLayout>();
    layout.get()->SetId(id);
    m_childs.push_back(layout);

    layout.get()->m_mainWindow = this;
    return layout.get();


}

RadioGroup* Window::CreateRadioGroup(const std::string &label, Orientation orientation, int spacing,const std::string &id)
{
    
    std::shared_ptr<RadioGroup> group = std::make_shared<RadioGroup>(label,orientation,spacing);
    group.get()->SetId(id);
    m_childs.push_back(group);
    group.get()->m_mainWindow = this;
    return group.get();

}

CheckGroup* Window::CreateCheckGroup(const std::string &label, Orientation orientation, int spacing,const std::string &id)
{

    std::shared_ptr<CheckGroup> group = std::make_shared<CheckGroup>(label,orientation,spacing);
    group.get()->SetId(id);
    m_childs.push_back(group);
    group.get()->m_mainWindow = this;
    return group.get();


}

GroupBox* Window::CreateGroupBox(const std::string &label, Orientation orientation, int spacing,const std::string &id)
{
    
        std::shared_ptr<GroupBox> group = std::make_shared<GroupBox>(label,orientation,spacing);
        group.get()->SetId(id);
        m_childs.push_back(group);
        group.get()->m_mainWindow = this;
        return group.get();
    
    
}


FileChooser *Window::OpenFileChooser(const std::string &title,const std::string &open,const std::string &cancel)
{
    assert(m_widget != nullptr);
    std::shared_ptr<FileChooser> dialog  = std::make_shared<FileChooser>(this,GTK_FILE_CHOOSER_ACTION_OPEN, title,open,cancel);
    dialog.get()->m_parent = this;
    m_dialogs.push_back(dialog);
    dialog->index = m_dialogs.size() - 1;
    return dialog.get();
}

FileChooser *Window::SaveFileChooser(const std::string &title,const std::string &save,const std::string &cancel)
{
    assert(m_widget != nullptr);
    std::shared_ptr<FileChooser> dialog  = std::make_shared<FileChooser>(this,GTK_FILE_CHOOSER_ACTION_SAVE, title,save,cancel);
    dialog.get()->m_parent = this;
    m_dialogs.push_back(dialog);
    dialog->index = m_dialogs.size() - 1;
    return dialog.get();
}

FolderChooser* Window::SelectFolder(const std::string &title,const std::string &open,const std::string &cancel)
{
    assert(m_widget != nullptr);
    std::shared_ptr<FolderChooser> dialog  = std::make_shared<FolderChooser>(this,GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, title,open,cancel);
    dialog.get()->m_parent = this;
    m_dialogs.push_back(dialog);
    dialog->index = m_dialogs.size() - 1;
    return dialog.get();
}

FolderChooser* Window::CreateFolder(const std::string &title,const std::string &open,const std::string &cancel)
{
    assert(m_widget != nullptr);
    std::shared_ptr<FolderChooser> dialog  = std::make_shared<FolderChooser>(this,GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER, title,open,cancel);
    dialog.get()->m_parent = this;
    m_dialogs.push_back(dialog);
    dialog->index = m_dialogs.size() - 1;
    return dialog.get();
}


int Window::MessageBox(const std::string &title,const std::string &message, DialogType type, DialogButtons buttons)
{
    GtkMessageType _flags;
    GtkButtonsType _type;
    switch (buttons)
    {   
        case DialogButtons::NONE:
            _type = GTK_BUTTONS_NONE;
            break;
        case DialogButtons::OK:
            _type = GTK_BUTTONS_OK;
            break;
        case DialogButtons::CANCEL:
            _type = GTK_BUTTONS_CANCEL;
            break;
        case DialogButtons::YES_NO:
            _type = GTK_BUTTONS_YES_NO;
            break;
        case DialogButtons::OK_CANCEL:
            _type = GTK_BUTTONS_OK_CANCEL;
            break;
        default:
            _type = GTK_BUTTONS_NONE;
            break;
    };

    switch (type)
    {
        case DialogType::INFO:
            _flags = GTK_MESSAGE_INFO;
            break;
        case DialogType::WARNING:
            _flags = GTK_MESSAGE_WARNING;
            break;
        case DialogType::QUESTION:
            _flags = GTK_MESSAGE_QUESTION;
            break;
        case DialogType::ERROR:
            _flags = GTK_MESSAGE_ERROR;
            break;
        case DialogType::OTHER:
            _flags = GTK_MESSAGE_OTHER;
            break;
        default:
            _flags = GTK_MESSAGE_INFO;
            break;
    };

    GtkWidget *confirm_dialog = gtk_message_dialog_new(m_window,
                                                               GTK_DIALOG_MODAL,
                                                               _flags,
                                                               _type,
                                                               "%s",
                                                                message.c_str());

  
  
    gtk_window_set_title(GTK_WINDOW(confirm_dialog),title.c_str());

    gint result = gtk_dialog_run(GTK_DIALOG(confirm_dialog));
    gtk_widget_destroy(confirm_dialog);
    return result;
}




//***************************************************************************************
// FileChooser
//***************************************************************************************

Dialog::Dialog()
{
    
}

Dialog::~Dialog()
{
   //std::cout << "Free Dialog" << std::endl;    
}




FileChooser::FileChooser(Window* parent,int flags,const std::string &title,const std::string &open,const std::string &cancel)
{
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

    switch (flags)
    {
      case  0:
            action = GTK_FILE_CHOOSER_ACTION_OPEN;
            break;
       case 1: 
            action = GTK_FILE_CHOOSER_ACTION_SAVE;
            break;
        case 2:
            action = GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER;
            break;
        case 3:
            action = GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER;
            break;
    }

    
    m_widget = gtk_file_chooser_dialog_new(title.c_str(), parent->m_window,
                                                    action,
                                                    
                                                    cancel.c_str(), GTK_RESPONSE_CANCEL,
                                                    open.c_str(), GTK_RESPONSE_ACCEPT,
                                                    NULL);
   dialog = GTK_FILE_CHOOSER(m_widget);

   g_signal_connect(m_widget, "response", G_CALLBACK(on_file_selected), this);

}

FileChooser::~FileChooser()
{
    OnFileSelected= nullptr; 
}


void FileChooser::SetFilter(std::vector<std::pair<std::string, std::string>> filters )
{   
    
    
    for (const auto& filter : filters) 
    {
        GtkFileFilter *gtk_filter = gtk_file_filter_new();
        gtk_file_filter_set_name(gtk_filter, filter.first.c_str());
        gtk_file_filter_add_pattern(gtk_filter, filter.second.c_str());
        gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), gtk_filter);
    }
}

void Dialog::Show()
{
     gtk_widget_show_all(m_widget);
}

void Dialog::Close()
{
        gtk_widget_destroy(m_widget);
        m_parent->m_dialogs.erase(m_parent->m_dialogs.begin() + index);
}

FolderChooser::FolderChooser(Window* parent,int flags,const std::string &title ,const std::string &open, const std::string &cancel)
{
     GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

    switch (flags)
    {
      case  0:
            action = GTK_FILE_CHOOSER_ACTION_OPEN;
            break;
       case 1: 
            action = GTK_FILE_CHOOSER_ACTION_SAVE;
            break;
        case 2:
            action = GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER;
            break;
        case 3:
            action = GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER;
            break;
    }

    
    m_widget = gtk_file_chooser_dialog_new(title.c_str(), parent->m_window,
                                                    action,
                                                    
                                                    cancel.c_str(), GTK_RESPONSE_CANCEL,
                                                    open.c_str(), GTK_RESPONSE_ACCEPT,
                                                    NULL);
   dialog = GTK_FILE_CHOOSER(m_widget);

   g_signal_connect(m_widget, "response", G_CALLBACK(on_folder_selected), this);
}

FolderChooser::~FolderChooser()
{
    
}

static void app_activate(GtkApplication* app, gpointer user_data) 
{
    Application* app_ = (Application*)user_data;
 //   std::cout << "Activate" << std::endl;
    if (app_->OnActivate != nullptr)
        app_->OnActivate();
    
}

static void on_shutdown(GtkWidget *widget, gpointer user_data)
{
    Application* app_ = (Application*)user_data;
  //  std::cout << "Shutdown" << std::endl;
    app_->Quit();
}

Application::Application(const std::string &name)
{
    gtk_init(0, NULL);
    m_app = gtk_application_new(name.c_str(), G_APPLICATION_FLAGS_NONE);
    g_signal_connect(m_app, "activate", G_CALLBACK(app_activate), this);
    g_signal_connect(m_app, "shutdown", G_CALLBACK(on_shutdown), this);

}

int  Application::Run()
{
    int status =g_application_run(G_APPLICATION(m_app), 0, NULL);
    return status;
}

int Application::Run(int argc, char **argv)
{
    int status =g_application_run(G_APPLICATION(m_app), argc, argv);
    return status;
}

void Application::Quit()
{
    g_application_quit(G_APPLICATION(m_app));   
}

Application::~Application()
{
    m_windows.clear();
    g_object_unref(m_app);
  //  std::cout << "Delete Aplication" << std::endl;
}

Window *Application::CreateWindow(const std::string &title, int width, int height)
{

    std::shared_ptr<Window> window = std::make_shared<Window>();
    window->CreateTopLevel(title, width, height);
    m_windows.push_back(window);
    //gtk_application_add_window(GTK_APPLICATION(m_app), window->m_window);
    return window.get();
}


Window *Application::CreatePopUpWindow(const std::string &title, int width, int height)
{
    std::shared_ptr<Window> window = std::make_shared<Window>();
    window->CreatePopUp(title, width, height);
    m_windows.push_back(window);
   // gtk_application_add_window(GTK_APPLICATION(m_app), window->m_window);
    return window.get();
}


Window *Application::CreateDialog(Window* parent, const std::string &title, int width, int height, bool isModal)
{
    std::shared_ptr<Window> window = std::make_shared<Window>();
    window->CreateDialog(parent, title, width, height, isModal);
    m_windows.push_back(window);
    gtk_application_add_window(GTK_APPLICATION(m_app), window->m_window);
    return window.get();
}
