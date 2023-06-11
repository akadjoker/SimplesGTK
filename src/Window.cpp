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

gboolean on_window_configure_event(GtkWidget *widget, GdkEventConfigure *event, gpointer user_data)
{

    Window *window = (Window *)user_data;

    gint x = event->x;
    gint y = event->y;

    gint width = event->width;
    gint height = event->height;
    if (window->DoResize(width, height))
        return true;
    if (window->DoMove(x, y))
        return true;

    //  FALSE para permitir que o evento seja propagado
    return false;
}

gboolean on_idle_callback(gpointer user_data)
{
    Window *window = (Window *)user_data;
    return window->DoIdle();
}

void on_destroy(GtkWidget *widget, gpointer data)
{
    Window *window = (Window *)data;
    window->DoExit();
}

gboolean on_window_close_event(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    Window *window = (Window *)user_data;

    return window->DoClose();
}

Window::Window()
{
}

bool Window::popEvent(Event &event)
{

    if (!m_events.empty())
    {
        event = m_events.front();
        m_events.pop();
        return true;
    }

    return false;
}

bool Window::pollEvent(Event &event)
{
    return popEvent(event);
}

bool Window::DoIdle()
{
    if (OnIdle)
    {
        bool result = OnIdle();
       if (!result)
       {
            g_source_remove(idle_id);
            idle_id = 0;
       }    
       return result;
    }
    return true;
}
 
void Window::DoExit()
{
    Release();
}

bool Window::DoResize(int width, int height)
{

    if (current_state.width != width || current_state.height != height)
    {
        //  g_print("Janela redimensionada para: %d, %d\n", width, height);
        current_state.width = width;
        current_state.height = height;
        return false;
    }

    return false;
}

bool Window::DoMove(int x, int y)
{
    if (current_state.x != x || current_state.y != y)
    {
        //  g_print("Janela movida para: %d, %d\n", x, y);
        current_state.x = x;
        current_state.y = y;
        return false;
    }
    return false;
}

bool Window::DoClose()
{

    std::cout << "DoClose" << std::endl;

    if (OnClose)
        return OnClose();
    
    return false;
}

void Window::pushEvent(const Event &event)
{
    m_events.push(event);
}

void on_window_drag_data_received(GtkWidget *widget, GdkDragContext *drag_context,
                                  gint x, gint y, GtkSelectionData *data,
                                  guint info, guint time, gpointer user_data)
{
    g_print("on_drag_data_received\n");
    gchar **uris = gtk_selection_data_get_uris(data);
    if (uris != NULL)
    {
        for (int i = 0; uris[i] != NULL; i++)
        {
            gchar *path = g_filename_from_uri(uris[i], NULL, NULL);
            g_print("Arquivo solto: %s\n", path);
            g_free(path);
        }
        g_strfreev(uris);
    }

    gtk_drag_finish(drag_context, TRUE, FALSE, time);
}

void Window::Init(Application *app, const std::string &title, int width, int height)
{
    m_widget = gtk_application_window_new(app->m_app);
    // g_object_ref(m_widget);

    m_window = GTK_WINDOW(m_widget);
    m_container = GTK_CONTAINER(m_widget);
    m_width = width;
    m_height = height;

    gtk_window_set_title(m_window, title.c_str());
    gtk_window_set_default_size(m_window, width, height);
    g_signal_connect(m_widget, "destroy", G_CALLBACK(on_destroy), this);
    g_signal_connect(m_widget, "delete-event", G_CALLBACK(on_window_close_event), this);
    g_signal_connect(m_widget, "configure-event", G_CALLBACK(on_window_configure_event), this);

    //  GtkTargetEntry targets[] =
    //  {
    //         {(gchar*)"text/uri-list", 0, 0}
    //     };

    // GdkDragAction actions = (GdkDragAction)(GDK_ACTION_COPY | GDK_ACTION_MOVE | GDK_ACTION_LINK);
    // gtk_drag_dest_set(m_widget, GTK_DEST_DEFAULT_ALL, targets,  sizeof(targets) / sizeof(GtkTargetEntry),     actions);
    // drops_handler_id=g_signal_connect(m_widget, "drag-data-received", G_CALLBACK(on_window_drag_data_received), this);

    idle_id = g_idle_add(on_idle_callback, this);
    isMainWindow = true;
}

void Window::InitTopLevel(const std::string &title, int width, int height)
{
    m_widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    m_window = GTK_WINDOW(m_widget);
    m_container = GTK_CONTAINER(m_widget);
    m_width = width;
    m_height = height;

    gtk_window_set_title(m_window, title.c_str());
    gtk_window_set_default_size(m_window, width, height);
    g_signal_connect(m_widget, "destroy", G_CALLBACK(on_destroy), this);
    g_signal_connect(m_widget, "delete-event", G_CALLBACK(on_window_close_event), this);
    g_signal_connect(m_widget, "configure-event", G_CALLBACK(on_window_configure_event), this);

    //  GtkTargetEntry targets[] =
    //  {
    //         {(gchar*)"text/uri-list", 0, 0}
    //     };

    // GdkDragAction actions = (GdkDragAction)(GDK_ACTION_COPY | GDK_ACTION_MOVE | GDK_ACTION_LINK);
    // gtk_drag_dest_set(m_widget, GTK_DEST_DEFAULT_ALL, targets,  sizeof(targets) / sizeof(GtkTargetEntry),     actions);
    // drops_handler_id=g_signal_connect(m_widget, "drag-data-received", G_CALLBACK(on_window_drag_data_received), this);

    idle_id = g_idle_add(on_idle_callback, this);
    isMainWindow = true;
}

void Window::InitPopUp(const std::string &title, int width, int height)
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
    SetId("popUp");

    // idle_id = g_idle_add(on_idle_callback, this);
    isMainWindow = false;
}

void Window::InitDialog(Window *parent, const std::string &title, int width, int height, bool isModal)
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
    SetId("Dialog");
    //  gtk_window_set_transient_for(m_window, this->m_window);

    g_signal_connect(m_widget, "destroy", G_CALLBACK(on_destroy), this);
    g_signal_connect(m_widget, "delete-event", G_CALLBACK(on_window_close_event), this);
    g_signal_connect(m_widget, "configure-event", G_CALLBACK(on_window_configure_event), this);

    // idle_id = g_idle_add(on_idle_callback, this);

    isMainWindow = false;
}

Window *Window::CreateDialog(const std::string &title, int width, int height, bool isModal)
{
    std::shared_ptr<Window> window = std::make_shared<Window>();
    window->InitDialog(this, title, width, height, isModal);
    m_childs.push_back(window);
    return window.get();
}

Window *Window::CreatePopUp(const std::string &title, int width, int height)
{
    std::shared_ptr<Window> window = std::make_shared<Window>();
    window->InitPopUp(title, width, height);

    m_childs.push_back(window);
    return window.get();
}

Window::~Window()
{

    m_dialogs.clear();
    // m_layouts.clear();
     m_childs.clear();
  //  std::cout << "**** Delete Window: (" << m_id << ")" << std::endl;
}

void Window::SetCenter()
{
    assert(m_widget);
    gtk_window_set_position(m_window, GTK_WIN_POS_CENTER);
}

void Window::SetPosition(int x, int y)
{
    assert(m_widget);
    gtk_window_move(m_window, x, y);
    m_x = x;
    m_y = y;
}

void Window::SetResizable(bool resizable)
{

    assert(m_widget);
    gtk_window_set_resizable(m_window, resizable);
}

void Window::Show()
{
    assert(m_widget);
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
    assert(m_widget);
    // std::cout << "Close Window: "<<m_id << std::endl;
    gtk_window_close(m_window);
}

void Window::Release()
{
    std::cout << "Release Window: " << m_id << std::endl;
    //  g_object_unref  (m_widget);
    if (isMainWindow)
    {
        //  std::cout << "remove event" << std::endl;
        if (idle_id != 0)
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

FrameLayout *Window::CreateFrameLayout(const std::string &label, const std::string &id)
{

    std::shared_ptr<FrameLayout> layout = std::make_shared<FrameLayout>(label);
    layout.get()->SetId(id);
    layout.get()->m_mainWindow = this;
    m_childs.push_back(layout);
    return layout.get();
}

BoxLayout *Window::CreateBoxLayout(Orientation orientation, int spacing, const std::string &id)
{

    std::shared_ptr<BoxLayout> layout = std::make_shared<BoxLayout>(orientation, spacing);
    layout.get()->SetId(id);
    m_childs.push_back(layout);
    layout.get()->m_mainWindow = this;
    return layout.get();
}

FixedLayout *Window::CreateFixedLayout(const std::string &id)
{

    std::shared_ptr<FixedLayout> layout = std::make_shared<FixedLayout>();
    layout.get()->SetId(id);
    m_childs.push_back(layout);
    layout.get()->m_mainWindow = this;
    return layout.get();
}

ScrollLayout *Window::CreateScrollLayout(const std::string &id)
{

    std::shared_ptr<ScrollLayout> layout = std::make_shared<ScrollLayout>();
    layout.get()->SetId(id);
    m_childs.push_back(layout);
    layout.get()->m_mainWindow = this;
    return layout.get();
}

ViewPortLayout *Window::CreateViewPortLayout(const std::string &id)
{
    std::shared_ptr<ViewPortLayout> layout = std::make_shared<ViewPortLayout>();
    layout.get()->SetId(id);
    m_childs.push_back(layout);
    layout.get()->m_mainWindow = this;
    return layout.get();
}

GridLayout *Window::CreateGridLayout(const std::string &id)
{

    std::shared_ptr<GridLayout> layout = std::make_shared<GridLayout>();
    layout.get()->SetId(id);
    m_childs.push_back(layout);

    layout.get()->m_mainWindow = this;
    return layout.get();
}

RadioGroup *Window::CreateRadioGroup(const std::string &label, Orientation orientation, int spacing, const std::string &id)
{

    std::shared_ptr<RadioGroup> group = std::make_shared<RadioGroup>(label, orientation, spacing);
    group.get()->SetId(id);
    m_childs.push_back(group);
    group.get()->m_mainWindow = this;
    return group.get();
}

CheckGroup *Window::CreateCheckGroup(const std::string &label, Orientation orientation, int spacing, const std::string &id)
{

    std::shared_ptr<CheckGroup> group = std::make_shared<CheckGroup>(label, orientation, spacing);
    group.get()->SetId(id);
    m_childs.push_back(group);
    group.get()->m_mainWindow = this;
    return group.get();
}

GroupBox *Window::CreateGroupBox(const std::string &label, Orientation orientation, int spacing, const std::string &id)
{

    std::shared_ptr<GroupBox> group = std::make_shared<GroupBox>(label, orientation, spacing);
    group.get()->SetId(id);
    m_childs.push_back(group);
    group.get()->m_mainWindow = this;
    return group.get();
}

Notebook* Window::CreateNotebook(const std::string &ID)
{
    
    std::shared_ptr<Notebook> notebook = std::make_shared<Notebook>();
    notebook.get()->SetId(ID);
    m_childs.push_back(notebook);
    notebook->m_mainWindow = m_mainWindow;
    return notebook.get();
}

FileChooser *Window::OpenFileChooser(const std::string &title, const std::string &open, const std::string &cancel)
{
    assert(m_widget != nullptr);
    std::shared_ptr<FileChooser> dialog = std::make_shared<FileChooser>(this, GTK_FILE_CHOOSER_ACTION_OPEN, title, open, cancel);
    dialog.get()->m_parent = this;
    m_dialogs.push_back(dialog);
    dialog->index = m_dialogs.size() - 1;
    return dialog.get();
}

FileChooser *Window::SaveFileChooser(const std::string &title, const std::string &save, const std::string &cancel)
{
    assert(m_widget != nullptr);
    std::shared_ptr<FileChooser> dialog = std::make_shared<FileChooser>(this, GTK_FILE_CHOOSER_ACTION_SAVE, title, save, cancel);
    dialog.get()->m_parent = this;
    m_dialogs.push_back(dialog);
    dialog->index = m_dialogs.size() - 1;
    return dialog.get();
}

FolderChooser *Window::SelectFolder(const std::string &title, const std::string &open, const std::string &cancel)
{
    assert(m_widget != nullptr);
    std::shared_ptr<FolderChooser> dialog = std::make_shared<FolderChooser>(this, GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, title, open, cancel);
    dialog.get()->m_parent = this;
    m_dialogs.push_back(dialog);
    dialog->index = m_dialogs.size() - 1;
    return dialog.get();
}

FolderChooser *Window::CreateFolder(const std::string &title, const std::string &open, const std::string &cancel)
{
    assert(m_widget != nullptr);
    std::shared_ptr<FolderChooser> dialog = std::make_shared<FolderChooser>(this, GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER, title, open, cancel);
    dialog.get()->m_parent = this;
    m_dialogs.push_back(dialog);
    dialog->index = m_dialogs.size() - 1;
    return dialog.get();
}

int Window::MessageBox(const std::string &title, const std::string &message, DialogType type, DialogButtons buttons)
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

    gtk_window_set_title(GTK_WINDOW(confirm_dialog), title.c_str());

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
    // std::cout << "Free Dialog" << std::endl;
}

FileChooser::FileChooser(Window *parent, int flags, const std::string &title, const std::string &open, const std::string &cancel)
{
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

    switch (flags)
    {
    case 0:
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
    OnFileSelected = nullptr;
}

void FileChooser::SetFilter(std::vector<std::pair<std::string, std::string>> filters)
{

    for (const auto &filter : filters)
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
    
    //m_parent->m_dialogs.erase(m_parent->m_dialogs.begin() + index);
}

FolderChooser::FolderChooser(Window *parent, int flags, const std::string &title, const std::string &open, const std::string &cancel)
{
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

    switch (flags)
    {
    case 0:
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

static void app_activate(GtkApplication *app, gpointer user_data)
{
    Application *app_ = (Application *)user_data;
  //  std::cout << "Activate" << std::endl;
    if (app_->OnActivate)
        app_->OnActivate();
}

static void on_shutdown(GtkWidget *widget, gpointer user_data)
{
    Application *app_ = (Application *)user_data;
    //  std::cout << "Shutdown" << std::endl;
    app_->Quit();
}

Application::Application(const std::string &name)
{
    //  gtk_init(0, NULL);
    m_app = gtk_application_new(name.c_str(), G_APPLICATION_FLAGS_NONE);
    g_signal_connect(m_app, "activate", G_CALLBACK(app_activate), this);
    g_signal_connect(m_app, "shutdown", G_CALLBACK(on_shutdown), this);
}

int Application::Run()
{
    int status = g_application_run(G_APPLICATION(m_app), 0, NULL);
    g_object_unref(m_app);
    return status;
}

int Application::Run(int argc, char **argv)
{
    int status = g_application_run(G_APPLICATION(m_app), argc, argv);
    g_object_unref(m_app);
    return status;
}

void Application::Quit()
{
    g_application_quit(G_APPLICATION(m_app));
}

Application::~Application()
{
    m_windows.clear();
    std::cout << "Delete Aplication" << std::endl;
}

Window *Application::CreateWindow(const std::string &title, int width, int height)
{
    std::shared_ptr<Window> window = std::make_shared<Window>();
    m_windows.push_back(window);
    window->Init(this, title, width, height);

    // gtk_application_add_window(GTK_APPLICATION(m_app), window->m_window);
    return window.get();
}

Window *Application::CreateTopLevel(const std::string &title, int width, int height)
{
    std::shared_ptr<Window> window = std::make_shared<Window>();
    m_windows.push_back(window);
    window->InitTopLevel(title, width, height);
    gtk_application_add_window(GTK_APPLICATION(m_app), window->m_window);
    return window.get();
}

/*
CommandExecutor executor;
    try 
    {
        // int status = executor.execute("vlc");
        // std::string gccOutput = executor.getResult();
        // std::string gccError = executor.getError();
        // std::cout << "GCC status: " << status << std::endl;
        // std::cout << "GCC output: " << gccOutput << std::endl;
        // std::cout << "GCC error: " << gccError << std::endl;

     } catch(const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
*/
int CommandExecutor::execute(const std::string &command)
{
    gchar *output;
    gchar *err;
    gint status;
    GError *error = NULL;

    if (!g_spawn_command_line_sync(command.c_str(), &output, &err, &status, &error))
    {
        g_error_free(error);
        throw std::runtime_error("Spawn failed: " + std::string(error->message));
    }

    m_result = std::string(output);
    m_status = status;
    m_error = std::string(err);
    g_free(output);
    g_free(err);

    return status;
}

Process::~Process()
{
    if (out != NULL)
    {
        g_free(out);
    }

    if (err != NULL)
    {
        g_free(err);
    }

    g_spawn_close_pid(pid);
}

 void child_watch_cb(GPid pid,
                           gint status,
                           gpointer user_data)
{
 //   g_message("Child %" G_PID_FORMAT " exited %s", pid,
   //           g_spawn_check_wait_status(status, NULL) ? "normally" : "abnormally");

    // Free any resources associated with the child here, such as I/O channels
    // on its stdout and stderr FDs. If you have no code to put in the
    // child_watch_cb() callback, you can remove it and the g_child_watch_add()
    // call, but you must also remove the G_SPAWN_DO_NOT_REAP_CHILD flag,
    // otherwise the child process will stay around as a zombie until this
    // process exits.

   // g_spawn_close_pid(pid);
}

gboolean on_std_output(GIOChannel *channel, GIOCondition condition, gpointer data)
{
    gchar *string;
    gsize size;

    if (condition & G_IO_HUP)
    {
        return FALSE;
    }


    g_io_channel_read_line(channel, &string, &size, NULL, NULL);

    std::cout << string << std::endl;

    g_free(string);

    // mantem o watcher ativo
    return TRUE;
}

gboolean on_stderr_output(GIOChannel *channel, GIOCondition condition, gpointer data)
{
    gchar *string;
    gsize size;

    if (condition & G_IO_HUP)
    {
         return FALSE;
    }

    g_io_channel_read_line(channel, &string, &size, NULL, NULL);

    std::cout << string << std::endl;

    g_free(string);

    return TRUE;
}

Process::Process() : out(NULL), err(NULL)
{
 
    // GIOChannel *out_ch = g_io_channel_unix_new(err_fd);
    // g_io_channel_set_encoding(out_ch, NULL, NULL);
    // g_io_channel_set_flags(out_ch, G_IO_FLAG_NONBLOCK, NULL);
    // gchar *str_return;
    // gsize size;
    // GIOStatus status = g_io_channel_read_line(out_ch, &str_return, &size, NULL, NULL);
    // while (status != G_IO_STATUS_EOF)
    // {
    //    if (size>0)
    //         g_print("%s\n", str_return);
    //     g_free(str_return);
    //     status = g_io_channel_read_line(out_ch, &str_return, &size, NULL, NULL);;
    // }
    // g_io_channel_shutdown(out_ch, TRUE, NULL);


}

bool Process::Execute(const std::string &command) 
{



gchar **argv = NULL;
gint argc = 0;
GError *error = NULL;

if (!g_shell_parse_argv(command.c_str(), &argc, &argv, &error)) 
{
    g_printerr("Failed to parse command line: %s\n", error->message);
    g_error_free(error);
    return false;
}

// for (gint i = 0; i < argc; i++) 
// {
//     g_print("Argument %d: %s\n", i, argv[i]);
// }



    // gchar *argv[] = {"/bin/sh", "-c", const_cast<char *>(command.c_str()), NULL};

  //  gchar *argv[] = {"vlc",  NULL};
    // const gchar * const argv[] = { "gcc", "-v", NULL };


    GSpawnFlags flags = (GSpawnFlags)(G_SPAWN_SEARCH_PATH | G_SPAWN_DO_NOT_REAP_CHILD);

    gboolean result = g_spawn_async_with_pipes(NULL, argv, NULL,
                                               flags, NULL, NULL, &pid, NULL, &out_fd, &err_fd, &error);

    g_strfreev(argv);  
   
    if (!result)
    {
        g_error("Spawn failed: %s\n", error->message);
        g_error_free(error);
        return false;
    }
    g_child_watch_add(pid, child_watch_cb, this);

    GIOCondition cond =(GIOCondition) (G_IO_IN | G_IO_HUP | G_IO_ERR | G_IO_NVAL);

    GIOChannel *channel_out = g_io_channel_unix_new(out_fd);
    g_io_channel_set_close_on_unref (channel_out, TRUE);
    g_io_channel_set_flags(channel_out, G_IO_FLAG_NONBLOCK, NULL);
    g_io_add_watch(channel_out, cond, on_std_output, this);
    g_io_channel_unref (channel_out);


    GIOChannel *channel_err = g_io_channel_unix_new(err_fd);
    g_io_channel_set_close_on_unref (channel_err, TRUE);
    g_io_channel_set_flags(channel_err, G_IO_FLAG_NONBLOCK, NULL);
    g_io_add_watch(channel_err, cond, on_stderr_output, this);
    g_io_channel_unref (channel_err);



    return true;
}

bool Process::Run(const std::string &command) 
{

gchar **argv = NULL;
gint argc = 0;
GError *error = NULL;

if (!g_shell_parse_argv(command.c_str(), &argc, &argv, &error)) 
{
    g_printerr("Failed to parse command line: %s\n", error->message);
    g_error_free(error);
    return false;
}

// for (gint i = 0; i < argc; i++) 
// {
//     g_print("Argument %d: %s\n", i, argv[i]);
// }


    // gchar *argv[] = {"/bin/sh", "-c", const_cast<char *>(command.c_str()), NULL};

    //gchar *argv[] = {"vlc",  NULL};
    // const gchar * const argv[] = { "gcc", "-v", NULL };



    GSpawnFlags flags = (GSpawnFlags)(G_SPAWN_SEARCH_PATH | G_SPAWN_DO_NOT_REAP_CHILD);

    gboolean result = g_spawn_async_with_pipes(NULL, argv, NULL,
                                               flags, NULL, NULL, &pid, NULL, &out_fd, &err_fd, &error);
    g_strfreev(argv);  
    if (!result)
    {
        g_error("Spawn failed: %s\n", error->message);
        g_error_free(error);
        return false;
    }
    g_child_watch_add(pid, child_watch_cb, this);




    return true;
}


 std::string Process::readOutput() 
{
    GIOChannel *out_ch = g_io_channel_unix_new(out_fd);
    g_io_channel_set_encoding(out_ch, NULL, NULL);
    gchar *str_return;
    gsize size;
    GIOStatus status = g_io_channel_read_line(out_ch, &str_return, &size, NULL, NULL);
    std::string output = "";

    while (status != G_IO_STATUS_EOF)
    {
        if (size >0)
            output += str_return;
        g_free(str_return);
        status = g_io_channel_read_line(out_ch, &str_return, &size, NULL, NULL);
    }
    g_io_channel_shutdown(out_ch, TRUE, NULL);
    return output;
}

// std::string Process::readOutput()
// {
//     std::string result="";
//     GIOChannel *channel = g_io_channel_unix_new(out_fd);
//     g_io_channel_set_encoding(channel, NULL, NULL);
//     g_io_channel_read_to_end(channel, &out, NULL, NULL);
//     g_io_channel_shutdown(channel, TRUE, NULL);
//     g_io_channel_unref(channel);

//     return result;
// }

std::string Process::readError()
{
    GIOChannel *out_ch = g_io_channel_unix_new(err_fd);
    g_io_channel_set_encoding(out_ch, NULL, NULL);
    gchar *str_return;
    gsize size;
    GIOStatus status = g_io_channel_read_line(out_ch, &str_return, &size, NULL, NULL);
    std::string output = "";

    while (status != G_IO_STATUS_EOF)
    {
        if (size >0)
            output += str_return;
        g_free(str_return);
        status = g_io_channel_read_line(out_ch, &str_return, &size, NULL, NULL);
    }

    g_io_channel_shutdown(out_ch, TRUE, NULL);

    return output;
}
