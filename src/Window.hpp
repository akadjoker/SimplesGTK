#pragma once
#include <queue>
#include <set>
#include "Widget.hpp"
#include "Layout.hpp"
#include "Buttons.hpp"
#include "Text.hpp"
#include "Group.hpp"
#include "Menus.hpp"

/*

       FileChooser * dialog = window.OpenFileChooser("Open File","_Open");
           std::vector<std::pair<std::string, std::string>> filters = {
              {"Arquivos de Texto", "*.txt"},
              {"Planilhas CSV", "*.csv"},
              {"Documentos JSON", "*.json"}
          };
          dialog->SetFilter(filters);
          dialog->OnFileSelected = [&](const std::string &filename) -> bool
          {
              std::cout<<"File: " << filename + "\n";
              consoleText.AddText(filename + "\n");
              return true;
          };
          dialog->Show();


        FileChooser * dialog = window.SaveFileChooser("Save File","_Save");
           std::vector<std::pair<std::string, std::string>> filters = {
              {"Arquivos de Texto", "*.txt"},
              {"Planilhas CSV", "*.csv"},
              {"Documentos JSON", "*.json"}
          };
          dialog->SetFilter(filters);
          dialog->OnFileSelected = [&](const std::string &filename) -> bool
          {
              std::cout<<"File: " << filename + "\n";
              window.MessageBox("O ficheiro ja existe",DialogType::WARNING,DialogButtons::YES_NO);
              consoleText.AddText(filename + "\n");
              return true;
          };
          dialog->Show();




*/

class Window;


class GTK_API_EXPORT Application
{
    public:
        Application( const std::string &name);
        virtual ~Application();

        Window * CreateWindow(const std::string &title, int width, int height);
        Window * CreatePopUpWindow(const std::string &title, int width, int height);
        Window * CreateDialog(Window* parent, const std::string &title, int width, int height, bool isModal);

        int  Run();
        int  Run(int argc, char **argv);
        void Quit();

        std::function<bool()> OnActivate;  


    protected:
        std::vector<std::shared_ptr<Window>> m_windows;
        GtkApplication *m_app;        
        

};

class GTK_API_EXPORT Dialog
{

public:
    Dialog();
    virtual ~Dialog();

    void Show();
    void Close();

protected:
    friend class Window;
    Window *m_parent;
    GtkWidget *m_widget;
    int index{0};
};

class GTK_API_EXPORT FolderChooser : public Dialog
{
public:
    std::function<bool(const std::string &name)> OnFolderSelected;
    FolderChooser(Window *parent, int flags, const std::string &title, const std::string &open, const std::string &cancel = "_Cancel");
    ~FolderChooser();

protected:
    GtkFileChooser *dialog;
};

class GTK_API_EXPORT FileChooser : public Dialog
{
public:
    std::function<bool(const std::string &filename)> OnFileSelected;
    FileChooser(Window *parent, int flags, const std::string &title, const std::string &open, const std::string &cancel = "_Cancel");
    ~FileChooser();

    void SetFilter(std::vector<std::pair<std::string, std::string>> filters);

protected:
    GtkFileChooser *dialog;
};

class GTK_API_EXPORT Window : public Layout
{
public:
    Window();
    ~Window();

    void CreateTopLevel(const std::string &title, int width, int height);
    void CreatePopUp(const std::string &title, int width, int height);
    void CreateDialog(Window *parent, const std::string &title, int width, int height, bool isModal);

    Window *CreateDialog( const std::string &title, int width, int height, bool isModal);

    bool popEvent(Event& event);
    void pushEvent(const Event& event);
    bool pollEvent(Event& event);

    void SetPosition(int x, int y);
    void SetResizable(bool resizable);
    void SetCenter();
    void Show();
    void Run();
    
    void Close();

    void Release();

    void ProcessEvents();

    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    int GetX() const { return m_x; }
    int GetY() const { return m_y; }

    FrameLayout *CreateFrameLayout(const std::string &label, const std::string &id="FrameLayout");
    BoxLayout *CreateBoxLayout(Orientation orientation, int spacing, const std::string &id="BoxLayout");
    FixedLayout *CreateFixedLayout(const std::string &id="FixedLayout");
    ScrollLayout *CreateScrollLayout(const std::string &id="ScrollLayout");
    ViewPortLayout *CreateViewPortLayout(const std::string &id="ViewPortLayout");
    GridLayout *CreateGridLayout( const std::string &id="GridLayout");

    RadioGroup *CreateRadioGroup(const std::string &label, Orientation orientation, int spacing,const std::string &id="RadioGroup");
    CheckGroup *CreateCheckGroup(const std::string &label, Orientation orientation, int spacing,const std::string &id="CheckGroup");
    GroupBox   *CreateGroupBox(const std::string &label, Orientation orientation, int spacing,const std::string &id="GroupBox");
        


    // GridLayout* CreateGridLayout(const std::string &id, int width, int height, int flags = 0);

    FileChooser *OpenFileChooser(const std::string &title, const std::string &open, const std::string &cancel = "_Cancel");
    FileChooser *SaveFileChooser(const std::string &title, const std::string &save, const std::string &cancel = "_Cancel");

    FolderChooser *SelectFolder(const std::string &title, const std::string &open, const std::string &cancel = "_Cancel");
    FolderChooser *CreateFolder(const std::string &title, const std::string &open, const std::string &cancel = "_Cancel");

    int MessageBox(const std::string &title, const std::string &message, DialogType type, DialogButtons buttons);

public:
    std::function<bool()> OnIdle;
    std::function<bool()> OnClose;
    std::function<bool(int, int)> OnResize;
    std::function<bool(int, int)> OnMove;


protected:
    friend class FileChooser;
    friend class FolderChooser;
    friend class Dialog;
    friend class Application;
    friend class Menu;
    friend gboolean on_window_configure_event(GtkWidget *widget, GdkEventConfigure *event, gpointer user_data);

    GtkWindow *m_window;
    guint idle_id;
    bool isMainWindow{false};
    bool useLoop{false};
    std::vector<std::shared_ptr<Dialog>> m_dialogs;
    std::queue<Event> m_events;  

    int m_width{0};
    int m_height{0};
    int m_x{0};
    int m_y{0};
};
