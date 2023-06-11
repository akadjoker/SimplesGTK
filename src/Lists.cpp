#include "Lists.hpp"




//************************************************************************* 
//ComboBox
//*************************************************************************

void on_combo_box_changed(GtkComboBox *combo_box, gpointer user_data)
{
    
}

gchar* on_combo_box_format_entry_text(GtkComboBox *combo_box, const gchar *path, gpointer user_data)
{
    // Retorna uma nova string alocada dinamicamente que será usada como o texto formatado. :O
    return NULL;
}

void on_combo_box_popdown(GtkComboBox *combo_box, gpointer user_data)
{
   
}

void on_combo_box_popup(GtkComboBox *combo_box, gpointer user_data)
{
    
}


ComboBox::ComboBox()
{
    
}


/*
g_signal_connect(combo_box, "format-entry-text", G_CALLBACK(on_format_entry_text), NULL);
g_signal_connect(combo_box, "move-active",     G_CALLBACK(on_format_entry_text), NULL);
g_signal_connect(combo_box, "changed", G_CALLBACK(on_combo_box_changed), NULL);
g_signal_connect(combo_box, "popdown", G_CALLBACK(on_combo_box_popdown), NULL);
g_signal_connect(combo_box, "popup", G_CALLBACK(on_combo_box_popup), NULL);
   
        */

//************************************************************************* 
//ComboBoxText
//*************************************************************************

void ComboBoxText::Append(const std::string &text)
{
    assert(m_widget);
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(m_widget), nullptr, text.c_str());
}

void ComboBoxText::Prepend(const std::string &text)
{
    assert(m_widget);
    gtk_combo_box_text_prepend(GTK_COMBO_BOX_TEXT(m_widget), nullptr, text.c_str());
}

void ComboBoxText::Insert(int pos, const std::string &text)
{
    assert(m_widget);
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(m_widget), pos, nullptr, text.c_str());
}

void ComboBoxText::AppendText(const std::string &text)
{
    assert(m_widget);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(m_widget), text.c_str());
}

void ComboBoxText::PrependText(const std::string &text)
{
    assert(m_widget);
    gtk_combo_box_text_prepend_text(GTK_COMBO_BOX_TEXT(m_widget), text.c_str());
}

void ComboBoxText::InsertText(int pos, const std::string &text)
{
    assert(m_widget);
    gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(m_widget), pos, text.c_str());
}

void ComboBoxText::Remove(int pos)
{
    assert(m_widget);
    gtk_combo_box_text_remove(GTK_COMBO_BOX_TEXT(m_widget), pos);
}


void ComboBoxText::RemoveAll()
{
    assert(m_widget);
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(m_widget));
}

std::string ComboBoxText::GetActiveText()
{
    assert(m_widget);
    return gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(m_widget));
}


ComboBoxText::ComboBoxText(bool hasEntry)
{
    if (hasEntry)
        m_widget = gtk_combo_box_text_new_with_entry();
    else
        m_widget = gtk_combo_box_text_new();
}


void on_list_box_row_selected(GtkListBox *list_box, GtkListBoxRow *row, gpointer user_data)
{
    
}
void on_list_box_row_activated(GtkListBox *list_box, GtkListBoxRow *row, gpointer user_data)
{
  
}
void on_list_box_select_all(GtkListBox *list_box, gpointer user_data)
{
    
}

void on_list_box_unselect_all(GtkListBox *list_box, gpointer user_data)
{
    
}


//************************************************************************* 
//ListBox
//*************************************************************************

ListBox::ListBox()
{
    m_widget = gtk_list_box_new();
}

ListBox::~ListBox()
{
    m_childs.clear();
}

void ListBox::Append(Widget *widget)
{
    assert(m_widget);
    gtk_list_box_insert(GTK_LIST_BOX(m_widget), widget->m_widget, -1);
    gtk_widget_show_all(m_widget);
    
}

void ListBox::Prepend(Widget *widget)
{
    
    assert(m_widget);
    gtk_list_box_prepend(GTK_LIST_BOX(m_widget), widget->m_widget);
    gtk_widget_show_all(m_widget);
}


void ListBox::Insert(Widget *widget, int pos)
{
    assert(m_widget);
    gtk_list_box_insert(GTK_LIST_BOX(m_widget), widget->m_widget, pos);
    gtk_widget_show_all(m_widget);
}

Button* ListBox::AddButton(const std::string &text, const std::string &ID)
{
    std::shared_ptr<Button> button = std::make_shared<Button>(text);
    button.get()->SetId(ID);
    m_childs.push_back(button);
    button->m_mainWindow = m_mainWindow;
    Append(button.get());
    return button.get();
}

CheckButton* ListBox::AddCheckButton(const std::string &text, const std::string &ID)
{
    
    std::shared_ptr<CheckButton> checkButton = std::make_shared<CheckButton>(text);
    checkButton.get()->SetId(ID);
    m_childs.push_back(checkButton);
    checkButton->m_mainWindow = m_mainWindow;
    Append(checkButton.get());
    return checkButton.get();

}

RadioButton* ListBox::AddRadioButton(const std::string &text, const std::string &ID)
{
    std::shared_ptr<RadioButton> radioButton = std::make_shared<RadioButton>(text);
    radioButton.get()->SetId(ID);
    m_childs.push_back(radioButton);
    radioButton->m_mainWindow = m_mainWindow;
    Append(radioButton.get());
    return radioButton.get();
}

ToggleButton* ListBox::AddToggleButton(const std::string &text, const std::string &ID)
{
    
    std::shared_ptr<ToggleButton> toggleButton = std::make_shared<ToggleButton>(text);
    toggleButton.get()->SetId(ID);
    m_childs.push_back(toggleButton);
    toggleButton->m_mainWindow = m_mainWindow;
    Append(toggleButton.get());
    return toggleButton.get();

}

Label* ListBox::AddLabel(const std::string &text, const std::string &ID)
{
    std::shared_ptr<Label> label = std::make_shared<Label>(text);
    label.get()->SetId(ID);
    m_childs.push_back(label);
    label->m_mainWindow = m_mainWindow;
    Append(label.get());
    return label.get();
    
}

ProgressBar* ListBox::AddProgressBar(const std::string &ID)
{
        
        std::shared_ptr<ProgressBar> progressBar = std::make_shared<ProgressBar>();
        progressBar.get()->SetId(ID);
        m_childs.push_back(progressBar);
        progressBar->m_mainWindow = m_mainWindow;
        Append(progressBar.get());
        return progressBar.get();
    
}

LevelBar* ListBox::AddLevelBar(const std::string &ID)
{
        
        std::shared_ptr<LevelBar> levelBar = std::make_shared<LevelBar>();
        levelBar.get()->SetId(ID);
        m_childs.push_back(levelBar);
        levelBar->m_mainWindow = m_mainWindow;
        Append(levelBar.get());
        return levelBar.get();
        
    
}

LevelBar* ListBox::AddLevelBar(double min_value, double max_value,const std::string &ID)
{
            
            std::shared_ptr<LevelBar> levelBar = std::make_shared<LevelBar>(min_value, max_value);
            levelBar.get()->SetId(ID);
            m_childs.push_back(levelBar);
            levelBar->m_mainWindow = m_mainWindow;
            Append(levelBar.get());
            return levelBar.get();
            
        
    
}



void ListBox::Remove(Widget *widget)
{
    assert(m_widget);
    gtk_container_remove(GTK_CONTAINER(m_widget), widget->m_widget);
}


