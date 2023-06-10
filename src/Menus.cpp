#include "Menus.hpp"
#include "Window.hpp"

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

void on_menu_item_clicked(GtkMenuItem *menu_item, gpointer user_data)
{
  g_print("Item de menu selecionado\n");
}

// tool bat item
void on_tool_bar_button_clicked(GtkToolButton *tool_button, gpointer user_data)
{
    ToolItem *item = (ToolItem *)user_data; 
    item->DoActivate();
}
void on_tool_bar_button_toggled(GtkWidget *tool_button, gpointer user_data)
{
  gboolean active = gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(tool_button));
  ToolButtonToggle *item = (ToolButtonToggle *)user_data;
  item->DoToggled(active);
}

//********************************************************************************
//  Menu Shell
//********************************************************************************

MenuShell::MenuShell() {}

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

MenuSeparator::MenuSeparator() { m_widget = gtk_separator_menu_item_new(); }

//********************************************************************************
//  Menu Item
//********************************************************************************

MenuItem::MenuItem()
{
  m_widget = gtk_menu_item_new();
  m_menuItem = GTK_MENU_ITEM(m_widget);
}

MenuItem::MenuItem(const std::string &label, bool mnemonic, int tag)
{
  if (mnemonic)
    m_widget = gtk_menu_item_new_with_mnemonic(label.c_str());
  else
    m_widget = gtk_menu_item_new_with_label(label.c_str());
  m_menuItem = GTK_MENU_ITEM(m_widget);
  g_signal_connect(m_widget, "activate", G_CALLBACK(on_menu_activate), this);
  SetTag(tag);
}

void MenuItem::SetSubMenu(Menu *menu)
{
  gtk_menu_item_set_submenu(m_menuItem, menu->m_widget);
}

int MenuItem::GetIndex() { return m_index; }

void MenuItem::SetIgnoreEvents(bool value) { m_ignoreEvent = value; }

bool MenuItem::GetIgnoreEvents() { return m_ignoreEvent; }

void MenuItem::DoActivate()
{
  
  m_mParent->DoActivate(this);
  if (OnClick)
  {
      bool state = OnClick();
      SetActive(state);
  }




}

MenuItem::~MenuItem()
{
 // std::cout << "MenuItem::~MenuItem() " << m_id << std::endl;
}

//***************************************************************************************
//  Check Menu Item
//***************************************************************************************
CheckMenuItem::CheckMenuItem(bool checked)
{
  m_widget = gtk_check_menu_item_new();
  m_checkItem = GTK_CHECK_MENU_ITEM(m_widget);
  SetChecked(checked);
}

CheckMenuItem::CheckMenuItem(bool checked, const std::string &label,
                             bool mnemonic, int tag)
{
  if (mnemonic)
    m_widget = gtk_check_menu_item_new_with_mnemonic(label.c_str());
  else
    m_widget = gtk_check_menu_item_new_with_label(label.c_str());
  m_checkItem = GTK_CHECK_MENU_ITEM(m_widget);
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
  gtk_check_menu_item_set_active(m_checkItem, value);
}

void CheckMenuItem::DoToggled(bool value) 
{
  
  m_mParent->DoCheck(this, value);

  if (OnCheck)
      OnCheck(value);

}

void CheckMenuItem::SetToggle() { gtk_check_menu_item_toggled(m_checkItem); }

void CheckMenuItem::SetDrawAsRadio(bool value)
{
  gtk_check_menu_item_set_draw_as_radio(m_checkItem, value);
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
  m_radioItem = GTK_RADIO_MENU_ITEM(m_widget);
  m_checkItem = GTK_CHECK_MENU_ITEM(m_widget);
  g_signal_connect(m_widget, "activate", G_CALLBACK(on_menu_activate), this);
}

RadioMenuItem::RadioMenuItem(const std::string &label, bool mnemonic, int tag)
{
  if (mnemonic)
    m_widget = gtk_radio_menu_item_new_with_mnemonic(NULL, label.c_str());
  else
    m_widget = gtk_radio_menu_item_new_with_label(NULL, label.c_str());
  m_radioItem = GTK_RADIO_MENU_ITEM(m_widget);
  m_checkItem = GTK_CHECK_MENU_ITEM(m_widget);
  g_signal_connect(m_widget, "activate", G_CALLBACK(on_menu_activate), this);
  SetTag(tag);
}

RadioMenuItem::RadioMenuItem(RadioMenuItem *parent, const std::string &label,
                             bool mnemonic, int tag)
{
  if (mnemonic)
    m_widget = gtk_radio_menu_item_new_with_mnemonic_from_widget(parent->m_radioItem, label.c_str());
  else
    m_widget = gtk_radio_menu_item_new_with_label_from_widget(parent->m_radioItem, label.c_str());
  m_radioItem = GTK_RADIO_MENU_ITEM(m_widget);
  m_checkItem = GTK_CHECK_MENU_ITEM(m_widget);
  g_signal_connect(m_widget, "activate", G_CALLBACK(on_menu_activate), this);
  SetTag(tag);
}

RadioMenuItem *RadioMenuItem::AddRadioItem(const std::string &label,
                                           bool mnemonic, int tag)
{
  std::shared_ptr<RadioMenuItem> menu_item =      std::make_shared<RadioMenuItem>(this, label, mnemonic, tag);
  m_mParent->Append(menu_item.get());
  menu_item->m_index = m_mParent->m_items.size() - 1;
  m_mParent->m_items.push_back(menu_item);
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

Menu::Menu() : MenuShell()
{
  m_widget = gtk_menu_new();
  m_menuShell = GTK_MENU_SHELL(m_widget);
  m_menu = GTK_MENU(m_widget);
}

void Menu::OnAdd()
{
  gtk_widget_show_all(m_widget);
  isShow = true;
}

void Menu::DoActivate(MenuItem *item)
{
  if (OnActivate)
      OnActivate(item);
}

void Menu::DoCheck(MenuItem *item, bool state)
{
  
  if (OnCheck)
      OnCheck(item,state);

}

void Menu::Popup()
{
  if (!isShow)
     OnAdd();
  gtk_menu_popup_at_pointer(m_menu, NULL);
}

void Menu::Popup(Widget *widget, int x, int y, Gravity widgetAnchor,
                 Gravity menuAnchor)
{

  if (!isShow)
    OnAdd();

  // GdkRectangle rect = {20, 20, 1, 1};
  // GdkGravity rect_anchor = GDK_GRAVITY_NORTH_WEST;
  // GdkGravity menu_anchor = GDK_GRAVITY_SOUTH_WEST;

  // gtk_menu_popup_at_rect(m_menu, gtk_widget_get_window(widget->m_widget),
  // &rect, rect_anchor, menu_anchor,NULL);

  GdkGravity widget_anchor = (GdkGravity)widgetAnchor;
  GdkGravity menu_anchor = (GdkGravity)menuAnchor;

  GdkRectangle rect;
  rect.x = x;
  rect.y = y;
  rect.width = 1;
  rect.height = 1;

  gtk_menu_popup_at_rect(m_menu, gtk_widget_get_window(widget->m_widget), &rect,
                         widget_anchor, menu_anchor, NULL);
}

void Menu::Popup(Widget *widget, Gravity widgetAnchor, Gravity menuAnchor)
{
  if (!isShow)
    OnAdd();

  GdkGravity widget_anchor = (GdkGravity)widgetAnchor;
  GdkGravity menu_anchor = (GdkGravity)menuAnchor;

  gtk_menu_popup_at_widget(m_menu, widget->m_widget, widget_anchor, menu_anchor,
                           NULL);
}

Menu::~Menu()
{
  m_items.clear();
  // std::cout << "Menu::~Menu()" << std::endl;
}

MenuItem *Menu::CreateItem(const std::string &label, const std::string &ID,
                           bool mnemonic, int tag)
{
  std::shared_ptr<MenuItem> menu =
      std::make_shared<MenuItem>(label, mnemonic, tag);
  menu.get()->SetId(ID);
  m_items.push_back(menu);
  return menu.get();
}

CheckMenuItem *Menu::CreateCheckItem(bool checked, const std::string &label,
                                     const std::string &ID, bool mnemonic,
                                     int tag)
{

  std::shared_ptr<CheckMenuItem> menu =
      std::make_shared<CheckMenuItem>(checked, label, mnemonic, tag);
  menu.get()->SetId(ID);
  m_items.push_back(menu);
  return menu.get();
}

RadioMenuItem *Menu::CreateRadioItem(const std::string &label,
                                     const std::string &ID, bool mnemonic,
                                     int tag)
{

  std::shared_ptr<RadioMenuItem> menu =
      std::make_shared<RadioMenuItem>(label, mnemonic, tag);
  menu.get()->SetId(ID);
  m_items.push_back(menu);
  return menu.get();
}

//********************************************************************************
//  Sub Menu
//********************************************************************************

SubMenu::SubMenu(const std::string &id, const std::string &itemId,
                 const std::string &itemLabel, bool mnemonic, int tag)
    : Menu()
{
  m_main_item = std::make_shared<MenuItem>(itemLabel, mnemonic);
  SetId(id);
  SetTag(tag);
  m_main_item.get()->SetId(itemId);
  m_main_item.get()->SetTag(tag);

  m_main_item.get()->SetSubMenu(this);
  m_main_item.get()->m_ignoreEvent = true;
  m_main_item.get()->m_mParent = this;
  m_main_item.get()->m_index = 0;
  m_items.push_back(m_main_item);
}

SubMenu::~SubMenu()
{
  // std::cout << "SubMenu::~SubMenu()" << std::endl;

  m_menus.clear();
}

SubMenu *SubMenu::AddSubMenu(const std::string &id, const std::string &itemId,
                             const std::string &itemLabel, bool mnemonic,
                             int tag)
{
  std::shared_ptr<SubMenu> menu =
      std::make_shared<SubMenu>(id, itemId, itemLabel, mnemonic, tag);
  m_menus.push_back(menu);
  Append(menu.get()->m_main_item.get());
  return menu.get();
}

MenuItem *SubMenu::AddItem(const std::string &label, const std::string &ID,
                           bool mnemonic, int tag)
{
  std::shared_ptr<MenuItem> menu_item =
      std::make_shared<MenuItem>(label, mnemonic, tag);
  menu_item.get()->SetId(ID);
  menu_item.get()->m_mParent = this;
  menu_item.get()->m_index = m_items.size() - 1;
  m_items.push_back(menu_item);
  Append(menu_item.get());
  return menu_item.get();
}

CheckMenuItem *SubMenu::AddCheckItem(bool checked, const std::string &label,
                                     const std::string &ID, bool mnemonic,
                                     int tag)
{

  std::shared_ptr<CheckMenuItem> menu_item =
      std::make_shared<CheckMenuItem>(checked, label, mnemonic, tag);
  menu_item.get()->SetId(ID);
  menu_item.get()->m_mParent = this;
  menu_item.get()->m_index = m_items.size() - 1;
  m_items.push_back(menu_item);
  Append(menu_item.get());
  return menu_item.get();
}

RadioMenuItem *SubMenu::AddRadioItem(const std::string &label,
                                     const std::string &ID, bool mnemonic,
                                     int tag)
{

  std::shared_ptr<RadioMenuItem> menu_item =
      std::make_shared<RadioMenuItem>(label, mnemonic, tag);
  menu_item.get()->SetId(ID);
  menu_item.get()->m_mParent = this;
  menu_item.get()->m_index = m_items.size() - 1;
  m_items.push_back(menu_item);
  Append(menu_item.get());
  return menu_item.get();
}

MenuSeparator *SubMenu::AddSeparator(const std::string &ID)
{
  std::shared_ptr<MenuSeparator> menu_item = std::make_shared<MenuSeparator>();
  menu_item.get()->SetId(ID);
  menu_item.get()->m_mParent = this;
  menu_item.get()->m_index = m_items.size() - 1;
  m_items.push_back(menu_item);
  Append(menu_item.get());
  return menu_item.get();
}

bool SubMenu::Contains(const std::string &id)
{

  for (auto item : m_items)
  {
    if (item.get()->GetId() == id)
      return true;
  }
  return false;
}

MenuItem *SubMenu::GetItemById(const std::string &id)
{

  for (auto item : m_items)
  {
    if (item.get()->GetId() == id)
    {
      MenuItem *mItem = dynamic_cast<MenuItem *>(item.get());
      return mItem;
    }
  }
  return nullptr;
}

MenuItem *SubMenu::GetItemByIndex(int index)
{

  if (index >= 0 && index < (int)m_items.size())
  {
    MenuItem *mItem = dynamic_cast<MenuItem *>(m_items[index].get());
    return mItem;
  }
  else
    return nullptr;
}

//********************************************************************************
//  Menu Bar
//********************************************************************************

MenuBar::MenuBar() : MenuShell()
{
  m_widget = gtk_menu_bar_new();
  m_menuShell = GTK_MENU_SHELL(m_widget);
}

MenuBar::~MenuBar()
{
 // std::cout << "MenuBar::~MenuBar()" << std::endl;
  m_menus.clear();
}

Menu *MenuBar::CreateMenu(const std::string &ID)
{
  std::shared_ptr<Menu> menu = std::make_shared<Menu>();
  menu.get()->SetId(ID);
  m_menus.push_back(menu);
  return menu.get();
}

SubMenu *MenuBar::CreateSubMenu(const std::string &id,
                                const std::string &itemId,
                                const std::string &itemLabel, bool mnemonic,
                                int tag)
{
  std::shared_ptr<SubMenu> menu =
      std::make_shared<SubMenu>(id, itemId, itemLabel, mnemonic, tag);
  m_menus.push_back(menu);
  Append(menu.get()->m_main_item.get());
  return menu.get();
}

//******************************************************************
// Menu Tool Bar
//******************************************************************

ToolItem::ToolItem()
{
  m_Item = gtk_tool_item_new();
  // m_Item        = GTK_MENU_TOOL_ITEM(m_widget);
}

ToolItem::~ToolItem() {}

int ToolItem::GetIndex() { return m_index; }

void ToolItem::DoActivate()
{

  m_toolBar->DoActivate(this);

    if (OnClick)
        OnClick();

}

//******************************************************************
// Menu Tool Button Toogle
//******************************************************************
ToolButtonToggle::ToolButtonToggle(const std::string &label, bool active)
{
  m_Item = gtk_toggle_tool_button_new();

  gtk_tool_button_set_label(GTK_TOOL_BUTTON(m_Item), label.c_str());
  g_signal_connect(m_Item, "clicked", G_CALLBACK(on_tool_bar_button_clicked), this);
  g_signal_connect(m_Item, "toggled", G_CALLBACK(on_tool_bar_button_toggled), this);

  gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(m_Item), active);
}

ToolButtonToggle::~ToolButtonToggle() {}

void ToolButtonToggle::SetActive(bool value)
{
  gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(m_Item), value);
}

bool ToolButtonToggle::GetActive()
{
  return gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(m_Item));
}

void ToolButtonToggle::DoToggled(bool value)
{

  m_toolBar->DoCheck(this, value);

  if (OnCheck)
      OnCheck(value);

}

//******************************************************************
// Menu Tool Radio Button
//******************************************************************

ToolRadioButton::ToolRadioButton(const std::string &label)
{
  m_Item = gtk_radio_tool_button_new(NULL);

  gtk_tool_button_set_label(GTK_TOOL_BUTTON(m_Item), label.c_str());
  g_signal_connect(m_Item, "clicked", G_CALLBACK(on_tool_bar_button_clicked), this);
  g_signal_connect(m_Item, "toggled", G_CALLBACK(on_tool_bar_button_toggled), this);

  // g_signal_connect(m_Item, "clicked", G_CALLBACK(on_tool_button_clicked), this);
  // gtk_toggle_tool_button_set_active(m_toolButton,active);
}

ToolRadioButton::ToolRadioButton(ToolRadioButton *parent, const std::string &label)
{
  // m_Item        = gtk_radio_tool_button_new_from_widget(parent->m_toolButton);
  m_Item = gtk_radio_tool_button_new(gtk_radio_tool_button_get_group(GTK_RADIO_TOOL_BUTTON(parent->m_Item)));
  gtk_tool_button_set_label(GTK_TOOL_BUTTON(m_Item), label.c_str());
  g_signal_connect(m_Item, "clicked", G_CALLBACK(on_tool_bar_button_clicked), this);
  g_signal_connect(m_Item, "toggled", G_CALLBACK(on_tool_bar_button_toggled), this);

  // g_signal_connect(m_Item, "clicked", G_CALLBACK(on_tool_button_clicked), this);
  //  gtk_toggle_tool_button_set_active(m_toolButton,active);
}

ToolRadioButton::~ToolRadioButton() {}

ToolRadioButton *ToolRadioButton::AddRadioButtom(const std::string &label, int pos)
{
  std::shared_ptr<ToolRadioButton> menu = std::make_shared<ToolRadioButton>(this, label);
  menu.get()->m_index = m_toolBar->m_items.size() - 1;
  menu.get()->m_toolBar = m_toolBar;
  m_toolBar->m_items.push_back(menu);
  m_toolBar->Insert(menu.get(), pos);
  return menu.get();
}

//******************************************************************
// Menu Tool Button
//******************************************************************

ToolButton::ToolButton(const std::string &label)
{
  m_Item = gtk_tool_button_new(NULL, label.c_str());
  g_signal_connect(m_Item, "clicked", G_CALLBACK(on_tool_bar_button_clicked), this);
  g_signal_connect(m_Item, "toggled", G_CALLBACK(on_tool_bar_button_toggled), this);

}

ToolButton::ToolButton(const std::string &stockIcon, const std::string &label)
{
  GtkWidget *icon = gtk_image_new_from_icon_name(stockIcon.c_str(), GTK_ICON_SIZE_SMALL_TOOLBAR);
  m_Item = gtk_tool_button_new(icon, label.c_str());
  // m_toolButton = GTK_TOOL_BUTTON(m_Item);
  g_signal_connect(m_Item, "clicked", G_CALLBACK(on_tool_bar_button_clicked), this);
  g_signal_connect(m_Item, "toggled", G_CALLBACK(on_tool_bar_button_toggled), this);

}

ToolButton::~ToolButton() {}

//******************************************************************
// Menu Tool Separator
//******************************************************************

ToolSeparator::ToolSeparator()
{
  m_Item = gtk_separator_tool_item_new();
}

//******************************************************************
// Tool Bar
//****************************************************************
ToolBar::ToolBar(ToolBarStyle style)
{

  m_widget = gtk_toolbar_new();
  m_toolbar = GTK_TOOLBAR(m_widget);
  GtkToolbarStyle c_style = (GtkToolbarStyle)style;
  gtk_toolbar_set_style(m_toolbar, c_style);
}

ToolBar::~ToolBar() { m_items.clear(); }

void ToolBar::Insert(ToolItem *item, int pos)
{

  gtk_toolbar_insert(m_toolbar, item->m_Item, pos);
}

void ToolBar::DoActivate(ToolItem *item)
{

  if (OnActivate)
      OnActivate(item);


}

void ToolBar::DoCheck(ToolItem *item,bool value)
{
  
    if (OnCheck)
        OnCheck(item,value);
  
}

ToolButton *ToolBar::AddButton(const std::string &label, const std::string &ID, int pos)
{
  std::shared_ptr<ToolButton> menu = std::make_shared<ToolButton>(label);
  menu.get()->SetId(ID);
  menu.get()->m_index = m_items.size() - 1;
  m_items.push_back(menu);
  menu.get()->m_toolBar = this;

  Insert(menu.get(), pos);
  return menu.get();
}

ToolButton *ToolBar::AddButton(const std::string &stockIcon, const std::string &label, const std::string &ID, int pos)
{
  std::shared_ptr<ToolButton> menu = std::make_shared<ToolButton>(stockIcon, label);
  menu.get()->SetId(ID);
  menu.get()->m_index = m_items.size() - 1;
  m_items.push_back(menu);
  menu.get()->m_toolBar = this;
  Insert(menu.get(), pos);
  return menu.get();
}

ToolButtonToggle *ToolBar::AddButtonToggle(const std::string &label, bool active, const std::string &ID, int pos)
{
  std::shared_ptr<ToolButtonToggle> menu = std::make_shared<ToolButtonToggle>(label, active);
  menu.get()->SetId(ID);
  menu.get()->m_index = m_items.size() - 1;
  m_items.push_back(menu);
  menu.get()->m_toolBar = this;
  Insert(menu.get(), pos);
  return menu.get();
}
ToolRadioButton *ToolBar::AddRadioButton(const std::string &label, const std::string &ID, int pos)
{

  std::shared_ptr<ToolRadioButton> menu = std::make_shared<ToolRadioButton>(label);
  menu.get()->SetId(ID);
  menu.get()->m_index = m_items.size() - 1;
  menu.get()->m_toolBar = this;
  m_items.push_back(menu);
  Insert(menu.get(), pos);
  return menu.get();
}

ToolSeparator *ToolBar::AddSeparator(int pos)
{
  std::shared_ptr<ToolSeparator> menu = std::make_shared<ToolSeparator>();
  m_items.push_back(menu);
  menu.get()->m_toolBar = this;
  Insert(menu.get(), pos);
  return menu.get();
}