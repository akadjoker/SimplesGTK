#include "Buttons.hpp"
#include "Window.hpp"


void on_button_clicked(GtkWidget *widget, gpointer data)
{
    Button *wd = (Button*)data;
    wd->DoClick();


}

 void on_button_toggled(GtkToggleButton *button, gpointer user_data) 
{
    ToggleButton *wd = (ToggleButton*)user_data;
    gboolean isChecked = gtk_toggle_button_get_active(button);
    wd->DoCheck((bool)isChecked);
     
   
}
void on_drag_data_get(GtkWidget *widget, GdkDragContext *context,
                      GtkSelectionData *data, guint info, guint time, gpointer user_data)
{
   // gtk_selection_data_set(data, gtk_selection_data_get_target(data), 8, (guchar *)"", 0);
}


IButton::~IButton()
{
   // std::cout << "Delete IButton" << std::endl;
}

void Button::DoClick()
{
    assert(m_widget);
    assert(m_mainWindow);

    // Event event;
    // event.type        = Event::ButtonClickEvent;
    // event.buttonClick.button = this;
    // m_mainWindow->pushEvent(event);

    if (OnClick)
        OnClick();
}

void Button::DoCheck(bool value)
{
    assert(m_widget);
    assert(m_mainWindow);

    // Event event;
    // event.type        = Event::ButtonCheckEvent;
    // event.buttonCheck.button = this;
    // event.buttonCheck.checked = value;
    // m_mainWindow->pushEvent(event);

    if (OnCheck)
        OnCheck(value);
    
}



Button::Button()
{
   m_widget = gtk_button_new ();
   
   g_signal_connect(m_widget, "clicked", G_CALLBACK(on_button_clicked), this);
   OnClick = nullptr;
}





Button::Button(const std::string &label,bool mnemonic)
{
    OnClick = nullptr;
    if (mnemonic)
        m_widget = gtk_button_new_with_mnemonic (label.c_str());
    else
        m_widget = gtk_button_new_with_label (label.c_str());
    
    
    g_signal_connect(m_widget, "clicked", G_CALLBACK(on_button_clicked), this);
    

  

 //   g_signal_connect(m_widget, "drag-data-get", G_CALLBACK(on_drag_data_get), NULL);

}

void Button::SetLabel(const std::string &label)
{
    
    gtk_button_set_label(GTK_BUTTON(m_widget),label.c_str());

}

std::string Button::GetLabel()
{
    const gchar * lb =gtk_button_get_label(GTK_BUTTON(m_widget));
    std::string label = lb;
    return label;
}


RadioButton::RadioButton()
{
    m_widget  = gtk_radio_button_new(NULL);
    m_toggleButton = GTK_TOGGLE_BUTTON(m_widget);
    
    m_radio = GTK_RADIO_BUTTON(m_widget);
     g_signal_connect(m_widget, "toggled", G_CALLBACK(on_button_toggled), this);
}

RadioButton::RadioButton(const std::string &label,bool mnemonic)
{
    m_widget = gtk_radio_button_new_with_label(NULL,label.c_str());
    m_toggleButton = GTK_TOGGLE_BUTTON(m_widget);
    
    m_radio = GTK_RADIO_BUTTON(m_widget);
     g_signal_connect(m_widget, "toggled", G_CALLBACK(on_button_toggled), this);
    
}

RadioButton::RadioButton(RadioButton* parent,const std::string &label,bool mnemonic)
{

    if (mnemonic)
        m_widget = gtk_radio_button_new_with_mnemonic_from_widget(parent->m_radio,label.c_str());
    else
        m_widget = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(parent->m_widget),label.c_str());
    m_toggleButton = GTK_TOGGLE_BUTTON(m_widget);
    m_radio = GTK_RADIO_BUTTON(m_widget);
     g_signal_connect(m_widget, "toggled", G_CALLBACK(on_button_toggled), this);
}


ToggleButton::ToggleButton()
{
    m_widget =  gtk_toggle_button_new();
    m_toggleButton = GTK_TOGGLE_BUTTON(m_widget);
    
    
    
}

ToggleButton::ToggleButton(const std::string &label,bool mnemonic)
{
    
    if (mnemonic)
        m_widget =  gtk_toggle_button_new_with_mnemonic(label.c_str());
    else
        m_widget =  gtk_toggle_button_new_with_label(label.c_str());
    m_toggleButton = GTK_TOGGLE_BUTTON(m_widget);
     g_signal_connect(m_widget, "toggled", G_CALLBACK(on_button_toggled), this);

}

void ToggleButton::SetChecked(bool active)
{
    assert(m_toggleButton);
    gtk_toggle_button_set_active(m_toggleButton,active);
}

bool ToggleButton::IsChecked()
{
    assert(m_toggleButton);
    return gtk_toggle_button_get_active(m_toggleButton);
}

void ToggleButton::SetMode(bool draw_indicator)
{
    assert(m_toggleButton);
    gtk_toggle_button_set_mode(m_toggleButton,draw_indicator);
    
}

bool ToggleButton::GetMode()
{
    assert(m_toggleButton);
    return gtk_toggle_button_get_mode(m_toggleButton);
}









CheckButton::CheckButton()
{
    m_widget =  gtk_check_button_new();
    m_toggleButton = GTK_TOGGLE_BUTTON(m_widget);
    g_signal_connect(m_widget, "toggled", G_CALLBACK(on_button_toggled), this);
}

CheckButton::CheckButton(const std::string &label,bool mnemonic)
{
    if (mnemonic)
        m_widget =  gtk_check_button_new_with_mnemonic(label.c_str());
    else
        m_widget =  gtk_check_button_new_with_label(label.c_str());
    m_toggleButton = GTK_TOGGLE_BUTTON(m_widget);
    g_signal_connect(m_widget, "toggled", G_CALLBACK(on_button_toggled), this);
}

