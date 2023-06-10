#include "Group.hpp"



static void on_radio_button_toggled(GtkToggleButton *toggleButton, gpointer user_data) 
{
    ToggleButton *button = (ToggleButton*)user_data;
    RadioGroup *layout = (RadioGroup*)button->GetLayout();
     if (gtk_toggle_button_get_active(toggleButton)) 
     {
          if (layout->OnToggled)
              layout->OnToggled(button);
     }
}

static void on_checkbox_toggled(GtkToggleButton *toggleButton, gpointer user_data) 
{
    ToggleButton *button = (ToggleButton*)user_data;
    CheckGroup *layout  = (CheckGroup*)button->GetLayout();
    if (layout->OnCheck)
         layout->OnCheck(button);
   
}

Group::Group()
{
    
}

Group::~Group()
{
    
}

void Group::Add(Widget *widget)
{
    gtk_container_add(m_container, widget->m_widget);
}


//***********************************************************************************
// RadioGroup
//***********************************************************************************

RadioGroup::RadioGroup(const std::string &label,Orientation orientation, int spacing)
{

    
    frame = std::make_shared<FrameLayout>(label);
    m_box = std::make_shared<BoxLayout>(orientation, spacing);
    m_widget = frame->m_widget;
    frame.get()->Add(m_box.get());
  
}



RadioButton *RadioGroup::AddItem(const std::string &label,bool expand, bool fill, int padding)
{
    std::shared_ptr<RadioButton> item;
  
    if (!m_isRoot)
    {
         item = std::make_shared<RadioButton>(label);
         m_Root = item;
         m_isRoot=true;
    } else
    {
        item = std::make_shared<RadioButton>(m_Root.get(),label);
    }

    item.get()->m_layout =this;
    item.get()->SetIndex(m_radioButtons.size()+1);

   g_signal_connect(item.get()->m_widget, "toggled", G_CALLBACK(on_radio_button_toggled), item.get());
   m_box.get()->packStart(item.get()->m_widget, expand, fill, padding);
   m_radioButtons.push_back(item);
    return item.get();
}

void RadioGroup::DeselectAll()
{
    for (auto &item : m_radioButtons)
    {
        item.get()->SetChecked(false);
    }
}

void RadioGroup::SetChecked(int index,bool check)
{

    for (auto &item : m_radioButtons)
    {
        if (item.get()->GetIndex() == index)
        {
            item.get()->SetChecked(check);
        } else
        {
            item.get()->SetChecked(!check);
        }
    }
}

bool RadioGroup::IsChecked(int index)
{
     if (index >=0 && index<(int)m_radioButtons.size())
         return m_radioButtons[index]->IsChecked();
    return false;
}

int RadioGroup::GetSelectedIndex()
{

    for (auto &item : m_radioButtons)
    {
        if (item.get()->IsChecked())
        {
            return item.get()->GetIndex();
        }
    }  
  
    return -1;

}

RadioButton* RadioGroup::Get(int index)
{
    if (index >=0 && index<(int)m_radioButtons.size())
        return m_radioButtons[index].get();
    return NULL;
}

//********************************************************************************
// CheckGroup
//********************************************************************************


CheckGroup::CheckGroup(const std::string &label,Orientation orientation, int spacing)
{
    
    frame = std::make_shared<FrameLayout>(label);
    m_box = std::make_shared<BoxLayout>(orientation, spacing);
    m_widget = frame->m_widget;
    frame.get()->Add(m_box.get());
  
}



CheckButton *CheckGroup::AddItem(const std::string &label,bool expand, bool fill, int padding)
{

    std::shared_ptr<CheckButton> item = std::make_shared<CheckButton>(label);
    item.get()->SetIndex(m_checkButtons.size()+1);
    item.get()->m_layout =this;
    g_signal_connect(item.get()->m_widget, "toggled", G_CALLBACK(on_checkbox_toggled), item.get());
    m_box.get()->packStart(item.get()->m_widget, expand, fill, padding);
    m_checkButtons.push_back(item);
    return item.get();
}

void CheckGroup::SetCheckAll(bool active)
{

    for (auto &item : m_checkButtons)
    {
        item.get()->SetChecked(active);
    }
    
}
int CheckGroup::GetIndex(const char *label)
{

    for (auto &item : m_checkButtons)
    {
        const gchar* l = gtk_button_get_label(GTK_BUTTON(item.get()->m_widget));
        if (strcmp(l,label)==0)
        {
            return item.get()->GetIndex();
        }
    }

    
    return 0;
}
int CheckGroup::GetIndex(const std::string &label)
{
    
    for (auto &item : m_checkButtons)
    {
        const gchar* l = gtk_button_get_label(GTK_BUTTON(item.get()->m_widget));
        if (strcmp(l,label.c_str())==0)
        {
            return item.get()->GetIndex();
        }
    }
    return 0;

}


void CheckGroup::SetCheck(int index,bool check)
{
    for (auto &item : m_checkButtons)
    {
        if (item.get()->GetIndex() == index)
        {
            item.get()->SetChecked(check);
            break;
        }
    }
}

bool CheckGroup::IsChecked(int index)
{
     if (index >=0 && index<(int)m_checkButtons.size())
         return m_checkButtons[index]->IsChecked();
    return false;
}



//********************************************************************************
// GroupBox
//********************************************************************************


GroupBox::GroupBox(const std::string &label, Orientation orientation, int spacing)
{
    frame = std::make_shared<FrameLayout>(label);
    m_box = std::make_shared<BoxLayout>(orientation, spacing);
    m_widget = frame->m_widget;
    frame.get()->Add(m_box.get());
}

void GroupBox::AddItem(Widget *widget, bool expand, bool fill, int padding)
{
    widget->m_layout =this;
    m_box.get()->PackStart(widget, expand, fill, padding);
    
}

Button* GroupBox::AddButton(const std::string &label, bool expand, bool fill, int padding)
{
    std::shared_ptr<Button> button = std::make_shared<Button>(label);
    button->m_layout =this;
    m_box.get()->PackStart(button.get(), expand, fill, padding);  
    m_widgets.push_back(button);
    return button.get();
}

