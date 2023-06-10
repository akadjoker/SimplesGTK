#pragma once

#include "Widget.hpp"
#include "Buttons.hpp"
#include "Text.hpp"
#include "Bars.hpp"
#include "Menus.hpp"


class GTK_API_EXPORT Layout : public Widget
{
public:
    Layout() = default;
    virtual ~Layout();

    virtual void Add(Widget *widget);
    virtual void Add(Layout *layout);

  

    void SetBoder(int border);

    bool Contains(const std::string &id) const;

    void SetBorder(int border);


    Layout *GetLayoutByID(const std::string &id);
    Button *GetButtonByID(const std::string &id);
    Widget *GetWidgetByID(const std::string &id);
    TextEdit *GetTextEditByID(const std::string &id);

    Button *CreateButton(const std::string &text, const std::string &ID = "Button");
    CheckButton *CreateCheckButton(const std::string &text, const std::string &ID = "CheckButton");
    RadioButton *CreateRadioButton(const std::string &text, const std::string &ID = "RadioButton");
    ToggleButton *CreateToggleButton(const std::string &text, const std::string &ID = "ToggleButton");
    Label *CreateLabel(const std::string &text, const std::string &ID = "Label");
    TextEdit *CreateTextEdit(const std::string &text, const std::string &ID = "TextEdit");
    TextView *CreateTextView(const std::string &text, const std::string &ID = "TextView");

    MenuBar *CreateMenuBar(const std::string &ID = "MenuBar");
    Menu    *CreateMenu( const std::string &ID="Menu");
    ToolBar *CreateToolBar(ToolBarStyle style,const std::string &ID = "ToolBar");

    ProgressBar *CreateProgressBar(const std::string &ID = "ProgressBar");
    LevelBar *CreateLevelBar(const std::string &ID = "LevelBar");
    LevelBar *CreateLevelBar(double min_value, double max_value,const std::string &ID = "LevelBar");


    

protected:
    friend class FrameLayout;
    friend class RadioGroup;

    std::vector<std::shared_ptr<Widget>> m_childs;

    void addWidget(GtkWidget *widget);

    GtkContainer *m_container;
};

class GTK_API_EXPORT FrameLayout : public Layout
{
public:
    FrameLayout(const std::string &title);

    // void Add(Widget *widget) override;
    // void Add(Layout *layout) override;

private:
    GtkFrame *m_frame;
};

class GTK_API_EXPORT BoxLayout : public Layout
{
public:
    BoxLayout(Orientation orientation, int spacing);

    void PackStart(Widget *widget, bool expand = false, bool fill = false, int padding = 0);
    void PackEnd(Widget *widget, bool expand = false, bool fill = false, int padding = 0);
  
private:
  void packStart(GtkWidget *widget, bool expand = false, bool fill = false, int padding = 0);
  void packEnd(GtkWidget *widget, bool expand = false, bool fill = false, int padding = 0);
  
    friend class Group;
    friend class CheckGroup;
    friend class RadioGroup;
    

    GtkBox *m_box;
};

class GTK_API_EXPORT FixedLayout : public Layout
{
public:
    FixedLayout();

    void Add(Widget *widget, int x, int y);
    void Add(Widget *widget, int x, int y, int width, int height);
    

protected:
    GtkFixed *m_fixed;
    friend class Window;
};

class GTK_API_EXPORT GridLayout : public Layout
{
public:
    GridLayout();
    void Add(Widget *widget, int left, int top, int width, int height);
    void SetRowSpacing(int spacing);
    void SetColumnSpacing(int spacing);
    void SetRowHomogeneous(bool homogeneous);
    void SetColumnHomogeneous(bool homogeneous);
    /*
      gtk_grid_set_row_homogeneous(m_grid, true);
     gtk_grid_set_column_homogeneous(m_grid, true);
    */
protected:
    GtkGrid *m_grid;
    friend class Window;    
};

class GTK_API_EXPORT ScrollLayout : public Layout
{
public:
    ScrollLayout();

protected:
    GtkScrolledWindow *m_scroll;
    friend class Window;
};

class GTK_API_EXPORT ViewPortLayout : public Layout
{
public:
    ViewPortLayout();

protected:
    GtkViewport *m_viewport;
    friend class Window;
};