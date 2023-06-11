#pragma once

#include "Widget.hpp"
#include "Buttons.hpp"
#include "Bars.hpp"
#include "Lists.hpp"
#include "Text.hpp"

class GTK_API_EXPORT ComboBox : public Widget
{
public:
    ComboBox();

protected:
};

class GTK_API_EXPORT ComboBoxText : public ComboBox
{

public:
    ComboBoxText(bool hasEntry = false);
    virtual ~ComboBoxText() = default;
    void Append(const std::string &text);
    void Prepend(const std::string &text);
    void Insert(int pos, const std::string &text);
    void AppendText(const std::string &text);
    void PrependText(const std::string &text);
    void InsertText(int pos, const std::string &text);
    void Remove(int pos);
    void RemoveAll();
    std::string GetActiveText();
};

// https://devdocs.io/gtk~3.20/gtklistbox

/*

void 	gtk_list_box_prepend ()
void 	gtk_list_box_insert ()
void 	gtk_list_box_select_row ()
void 	gtk_list_box_unselect_row ()
void 	gtk_list_box_select_all ()
void 	gtk_list_box_unselect_all ()
*/
class GTK_API_EXPORT ListBox : public Widget
{
public:
    ListBox();
    virtual ~ListBox();

    void Append(Widget *widget);
    void Prepend(Widget *widget);
    void Insert(Widget *widget, int pos = -1);
    void Remove(Widget *widget);

    Button       *AddButton(const std::string &text, const std::string &ID = "Button");
    CheckButton  *AddCheckButton(const std::string &text, const std::string &ID = "CheckButton");
    RadioButton  *AddRadioButton(const std::string &text, const std::string &ID = "RadioButton");
    ToggleButton *AddToggleButton(const std::string &text, const std::string &ID = "ToggleButton");
    Label        *AddLabel(const std::string &text, const std::string &ID = "Label");

    ProgressBar *AddProgressBar(const std::string &ID = "ProgressBar");
    LevelBar    *AddLevelBar(const std::string &ID = "LevelBar");
    LevelBar    *AddLevelBar(double min_value, double max_value,const std::string &ID = "LevelBar");


    

protected:
    std::vector<std::shared_ptr<Widget>> m_childs;
};