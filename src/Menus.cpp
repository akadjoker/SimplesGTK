#include "Menus.hpp"


void on_menu_activate(GtkWidget *widget, gpointer data)
{
    MenuItem *item = (MenuItem *)data;
    if (item->GetIgnoreEvents())
        return;
    item->DoActivate();

}

void on_menu_toggled(GtkWidget *widget, gpointer data)
{
    CheckMenuItem *item = (CheckMenuItem *)data;
    if (item->GetIgnoreEvents())
        return;
    bool active = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));
    item->DoToggled(active);

}

//********************************************************************************
//  Menu Shell
//********************************************************************************


MenuShell::MenuShell() 
{
    
}

MenuShell::~MenuShell()
{
  //  std::cout << "MenuShell::~MenuShell()" << std::endl;
}

void MenuShell::Append(MenuItem *item)
{
      gtk_menu_shell_append(m_menuShell, item->m_widget);
}
void MenuShell::Append(IMenuItem *item)
{
      gtk_menu_shell_append(m_menuShell, item->m_widget);
}


//********************************************************************************
//  Menu Separator
//********************************************************************************

MenuSeparator::MenuSeparator()
{
    m_widget = gtk_separator_menu_item_new();
}


//********************************************************************************
//  Menu Item
//********************************************************************************

MenuItem::MenuItem()
{
    m_widget = gtk_menu_item_new();
    m_menuItem =  GTK_MENU_ITEM(m_widget);

}

MenuItem::MenuItem(const std::string &label,bool mnemonic,int tag)
{
    if (mnemonic)
        m_widget = gtk_menu_item_new_with_mnemonic(label.c_str());
    else
        m_widget = gtk_menu_item_new_with_label(label.c_str());
    m_menuItem =  GTK_MENU_ITEM(m_widget);
    g_signal_connect(m_widget, "activate", G_CALLBACK(on_menu_activate), this);
    SetTag(tag);  
}

void MenuItem::SetSubMenu(Menu *menu)
{
    gtk_menu_item_set_submenu(m_menuItem, menu->m_widget);
}

int MenuItem::GetIndex()
{
    return m_index;
}

void MenuItem::SetIgnoreEvents(bool value)
{
    m_ignoreEvent = value;
}

bool MenuItem::GetIgnoreEvents()
{
    return m_ignoreEvent;
}


void MenuItem::DoActivate()
{
    std::cout << "MenuItem::DoActivate()"<< m_id << " " << m_index << std::endl;
}

MenuItem::~MenuItem()
{
   std::cout << "MenuItem::~MenuItem() "<< m_id  << std::endl;
}

//***************************************************************************************
//  Check Menu Item
//***************************************************************************************
CheckMenuItem::CheckMenuItem(bool checked)
{
    m_widget = gtk_check_menu_item_new();
    m_checkItem =  GTK_CHECK_MENU_ITEM(m_widget);
    SetChecked(checked);
}

CheckMenuItem::CheckMenuItem(bool checked,const std::string &label,bool mnemonic,int tag)
{
    if (mnemonic)
        m_widget = gtk_check_menu_item_new_with_mnemonic(label.c_str());
    else
        m_widget = gtk_check_menu_item_new_with_label(label.c_str());
    m_checkItem =  GTK_CHECK_MENU_ITEM(m_widget);
    SetChecked(checked);
    g_signal_connect(m_widget, "activate", G_CALLBACK(on_menu_activate), this);
    g_signal_connect(m_widget, "toggled", G_CALLBACK(on_menu_toggled), this);
    SetTag(tag);  
}

CheckMenuItem::~CheckMenuItem()
{
   // std::cout << "CheckMenuItem::~CheckMenuItem()" << std::endl;
}



void CheckMenuItem::SetChecked(bool value)
{
    gtk_check_menu_item_set_active(m_checkItem,value);   
}

void CheckMenuItem::DoToggled(bool value)
{
    
}

void CheckMenuItem::SetToggle()
{
    gtk_check_menu_item_toggled(m_checkItem);
}

void CheckMenuItem::SetDrawAsRadio(bool value)
{
    gtk_check_menu_item_set_draw_as_radio(m_checkItem,value);
}

bool CheckMenuItem::GetChecked()
{
    return gtk_check_menu_item_get_active(m_checkItem);
}

//********************************************************************************
//  Radio Menu Item
//********************************************************************************

RadioMenuItem::RadioMenuItem()
{
    m_widget = gtk_radio_menu_item_new(NULL);
    m_radioItem =  GTK_RADIO_MENU_ITEM(m_widget);
    m_checkItem =  GTK_CHECK_MENU_ITEM(m_widget);
    g_signal_connect(m_widget, "activate", G_CALLBACK(on_menu_activate), this);
}



RadioMenuItem::RadioMenuItem(const std::string &label,bool mnemonic,int tag)
{
    if (mnemonic)
        m_widget = gtk_radio_menu_item_new_with_mnemonic(NULL,label.c_str());
    else
        m_widget = gtk_radio_menu_item_new_with_label(NULL,label.c_str());
    m_radioItem =  GTK_RADIO_MENU_ITEM(m_widget);
    m_checkItem =  GTK_CHECK_MENU_ITEM(m_widget);
    g_signal_connect(m_widget, "activate", G_CALLBACK(on_menu_activate), this);
    SetTag(tag);  
}



RadioMenuItem::RadioMenuItem(RadioMenuItem* parent,const std::string &label, bool mnemonic ,int tag)
{
  if (mnemonic)
        m_widget = gtk_radio_menu_item_new_with_mnemonic_from_widget(parent->m_radioItem,label.c_str());
    else
        m_widget = gtk_radio_menu_item_new_with_label_from_widget(parent->m_radioItem,label.c_str());
    m_radioItem =  GTK_RADIO_MENU_ITEM(m_widget);
    m_checkItem =  GTK_CHECK_MENU_ITEM(m_widget);
    g_signal_connect(m_widget, "activate", G_CALLBACK(on_menu_activate), this);
    SetTag(tag);      
}


RadioMenuItem *RadioMenuItem::AddRadioItem(const std::string &label, bool mnemonic ,int tag)
{
    std::shared_ptr<RadioMenuItem> menu_item = std::make_shared<RadioMenuItem>(this,label,mnemonic,tag);
    m_parent->Append(menu_item.get());
    menu_item->m_index = m_parent->m_items.size()-1;
    m_parent->m_items.push_back(menu_item);
    return menu_item.get();
}
    




RadioMenuItem::~RadioMenuItem()
{
  //  m_items.clear();
   // std::cout << "RadioMenuItem::~RadioMenuItem()" << std::endl;
}




//********************************************************************************
//  Menu
//********************************************************************************

Menu::Menu():MenuShell()
{
    m_widget    =  gtk_menu_new();
    m_menuShell =  GTK_MENU_SHELL(m_widget);
}





Menu::~Menu()
{
    m_items.clear();
   // std::cout << "Menu::~Menu()" << std::endl;
}


//********************************************************************************
//  Sub Menu
//********************************************************************************


SubMenu::SubMenu(const std::string &id, const std::string &itemId,const  std::string &itemLabel, bool mnemonic,int tag):Menu()
{
    m_main_item = std::make_shared<MenuItem>(itemLabel,mnemonic);
    SetId(id);
    SetTag(tag);
    m_main_item.get()->SetId(itemId);
    m_main_item.get()->SetTag(tag);

    m_main_item.get()->SetSubMenu(this);
    m_main_item.get()->m_ignoreEvent=true;
    m_main_item.get()->m_parent=this;
    m_main_item.get()->m_index = 0;
    m_items.push_back(m_main_item);
}

SubMenu::~SubMenu()
{
   // std::cout << "SubMenu::~SubMenu()" << std::endl;
    
    m_menus.clear();
}

SubMenu* SubMenu::AddSubMenu(const std::string &id, const std::string &itemId,const  std::string &itemLabel, bool mnemonic,int tag)
{
    std::shared_ptr<SubMenu> menu = std::make_shared<SubMenu>(id,itemId,itemLabel,mnemonic,tag);
    m_menus.push_back(menu);
    Append(menu.get()->m_main_item.get());
    return menu.get();
}


MenuItem* SubMenu::AddItem(const std::string &label, const std::string &ID, bool mnemonic,int tag)
{
    std::shared_ptr<MenuItem> menu_item = std::make_shared<MenuItem>(label,mnemonic,tag);
    menu_item.get()->SetId(ID);
    menu_item.get()->m_parent=this;
    menu_item.get()->m_index = m_items.size() - 1;
    m_items.push_back(menu_item);
    Append(menu_item.get());
    return menu_item.get();
}

CheckMenuItem* SubMenu::AddCheckItem(bool checked,const std::string &label, const std::string &ID, bool mnemonic,int tag)
{
    
    std::shared_ptr<CheckMenuItem> menu_item = std::make_shared<CheckMenuItem>(checked,label,mnemonic,tag);
    menu_item.get()->SetId(ID);
    menu_item.get()->m_parent=this;
    menu_item.get()->m_index = m_items.size() - 1;
    m_items.push_back(menu_item);
    Append(menu_item.get());
    return menu_item.get();

}

RadioMenuItem* SubMenu::AddRadioItem(const std::string &label, const std::string &ID, bool mnemonic,int tag)
{
    
    std::shared_ptr<RadioMenuItem> menu_item = std::make_shared<RadioMenuItem>(label,mnemonic,tag);
    menu_item.get()->SetId(ID);
    menu_item.get()->m_parent=this;
    menu_item.get()->m_index = m_items.size() - 1;
    m_items.push_back(menu_item);
    Append(menu_item.get());
    return menu_item.get();


}

MenuSeparator* SubMenu::AddSeparator(const std::string &ID)
{
    std::shared_ptr<MenuSeparator> menu_item = std::make_shared<MenuSeparator>();
    menu_item.get()->SetId(ID);
    menu_item.get()->m_parent=this;
    menu_item.get()->m_index = m_items.size() - 1;
    m_items.push_back(menu_item);
    Append(menu_item.get());
    return menu_item.get();
}


bool SubMenu::Contains(const std::string &id)
{
    
    for(auto item : m_items)
    {
        if (item.get()->GetId() == id)
            return true;
    }
    return false;
}

MenuItem* SubMenu::GetItemById(const std::string &id)
{
    
    for(auto item : m_items)
    {
        if (item.get()->GetId() == id)
        {
          MenuItem * mItem = dynamic_cast<MenuItem*>(item.get());
          return mItem;
        }
    }
    return nullptr;

}

MenuItem* SubMenu::GetItemByIndex(int index)
{

    if (index>=0 && index < (int)m_items.size())
    {
          MenuItem * mItem = dynamic_cast<MenuItem*>(m_items[index].get());
          return mItem;
    }
    else
        return nullptr;
}




//********************************************************************************
//  Menu Bar
//********************************************************************************

MenuBar::MenuBar():MenuShell()
{
     m_widget = gtk_menu_bar_new();
     m_menuShell =  GTK_MENU_SHELL(m_widget);
}



MenuBar::~MenuBar()
{
    std::cout << "MenuBar::~MenuBar()" << std::endl;
    m_menus.clear();
}

Menu* MenuBar::CreateMenu( const std::string &ID)
{
     std::shared_ptr<Menu> menu = std::make_shared<Menu>();
     menu.get()->SetId(ID);
     m_menus.push_back(menu);
     return menu.get();
}

MenuItem* MenuBar::CreateItem(const std::string &label,const std::string &ID, bool mnemonic,int tag)
{
    std::shared_ptr<MenuItem> menu = std::make_shared<MenuItem>(label,mnemonic,tag);
    menu.get()->SetId(ID);
    m_items.push_back(menu);
    return menu.get();
}

CheckMenuItem* MenuBar::CreateCheckItem(bool checked,const std::string &label, const std::string &ID, bool mnemonic,int tag)
{
    
    std::shared_ptr<CheckMenuItem> menu = std::make_shared<CheckMenuItem>(checked,label,mnemonic,tag);
    menu.get()->SetId(ID);
    m_items.push_back(menu);
    return menu.get();

}

RadioMenuItem* MenuBar::CreateRadioItem(const std::string &label, const std::string &ID, bool mnemonic,int tag)
{
    
    std::shared_ptr<RadioMenuItem> menu = std::make_shared<RadioMenuItem>(label,mnemonic,tag);
    menu.get()->SetId(ID);
    m_items.push_back(menu);
    return menu.get();

}

SubMenu* MenuBar::CreateSubMenu(const std::string &id,const std::string &itemId, const std::string &itemLabel, bool mnemonic,int tag)
{
    std::shared_ptr<SubMenu> menu = std::make_shared<SubMenu>(id,itemId,itemLabel,mnemonic,tag);
    m_menus.push_back(menu);
    Append(menu.get()->m_main_item.get());
    return menu.get();
}
