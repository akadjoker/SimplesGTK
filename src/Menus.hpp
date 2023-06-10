#pragma once

#include "Widget.hpp"
// https://zetcode.com/gui/gtk2/menusandtoolbars/

// todo: implementar os eventos para menuitem e tool item

class MenuShell;
class MenuBar;
class Menu;
class SubMenu;
class ToolBar;

class IMenuItem : public Widget
{
public:
    IMenuItem() = default;
    virtual ~IMenuItem() = default;

protected:
    friend class MenuShell;
    friend class MenuBar;
    friend class Menu;
    friend class SubMenu;

    int m_index{0};
    Menu *m_mParent{nullptr};
};

class MenuSeparator : public IMenuItem
{
public:
    MenuSeparator();
};

class MenuItem : public IMenuItem
{
public:
    MenuItem();
    MenuItem(const std::string &label, bool mnemonic = false, int tag = 0);
    virtual ~MenuItem();

    void SetSubMenu(Menu *menu);
    int GetIndex();

    void SetIgnoreEvents(bool value);
    bool GetIgnoreEvents();

    virtual void DoActivate();

protected:
    friend class SubMenu;
    GtkMenuItem *m_menuItem{NULL};
    bool m_ignoreEvent{false};
};

class CheckMenuItem : public MenuItem
{
public:
    CheckMenuItem() = default; // pk do RadioMenuItem
    CheckMenuItem(bool checked);
    CheckMenuItem(bool checked, const std::string &label, bool mnemonic = false, int tag = 0);
    virtual ~CheckMenuItem();

    virtual void DoToggled(bool value);

    void SetToggle();

    void SetDrawAsRadio(bool value);

    void SetChecked(bool value);
    bool GetChecked();

protected:
    friend class SubMenu;
    GtkCheckMenuItem *m_checkItem{NULL};
};

class RadioMenuItem : public CheckMenuItem
{
public:
    RadioMenuItem();
    RadioMenuItem(const std::string &label, bool mnemonic = false, int tag = 0);
    RadioMenuItem(RadioMenuItem *parent, const std::string &label, bool mnemonic, int tag);

    RadioMenuItem *AddRadioItem(const std::string &label, bool mnemonic = false, int tag = 0);

    virtual ~RadioMenuItem();

protected:
    friend class SubMenu;
    GtkRadioMenuItem *m_radioItem{NULL};
};

class MenuShell : public Widget
{
public:
    MenuShell();
    virtual ~MenuShell();

    void Append(IMenuItem *item);
    void Append(MenuItem *item);

protected:
    friend class MenuBar;
    friend class Menu;
    friend class MenuItem;
    friend class CheckMenuItem;
    friend class RadioMenuItem;
    GtkMenuShell *m_menuShell{nullptr};
};

class Menu : public MenuShell
{

public:
    Menu();
    virtual ~Menu();

    virtual void OnAdd() override;

    void DoActivate(MenuItem *item);
    void DoCheck(MenuItem *item, bool value);
    std::function<void(MenuItem *)> OnActivate{nullptr};
    std::function<void(MenuItem *,bool )> OnCheck{nullptr};



    void Popup();
    void Popup(Widget *widget, Gravity widgetAnchor, Gravity menuAnchor);
    void Popup(Widget *widget, int x, int y, Gravity widgetAnchor, Gravity menuAnchor);

    MenuItem *CreateItem(const std::string &label, const std::string &ID = "MenuItem", bool mnemonic = false, int tag = 0);
    CheckMenuItem *CreateCheckItem(bool checked, const std::string &label, const std::string &ID = "CheckMenuItem", bool mnemonic = false, int tag = 0);
    RadioMenuItem *CreateRadioItem(const std::string &label, const std::string &ID = "RadioMenuItem", bool mnemonic = false, int tag = 0);

protected:
    friend class MenuBar;
    friend class MenuItem;
    friend class CheckMenuItem;
    friend class RadioMenuItem;
    friend class MenuShell;
    GtkMenu *m_menu;
    std::vector<std::shared_ptr<IMenuItem>> m_items;
};

class SubMenu : public Menu
{
public:
    SubMenu(const std::string &id, const std::string &itemId, const std::string &itemLabel, bool mnemonic = false, int tag = 0);

    virtual ~SubMenu();

    MenuItem *AddItem(const std::string &label, const std::string &ID = "MenuItem", bool mnemonic = false, int tag = 0);
    CheckMenuItem *AddCheckItem(bool checked, const std::string &label, const std::string &ID = "CheckMenuItem", bool mnemonic = false, int tag = 0);
    RadioMenuItem *AddRadioItem(const std::string &label, const std::string &ID = "RadioMenuItem", bool mnemonic = false, int tag = 0);

    SubMenu *AddSubMenu(const std::string &id, const std::string &itemId, const std::string &itemLabel, bool mnemonic = false, int tag = 0);
    MenuSeparator *AddSeparator(const std::string &ID = "MenuSeparator");

    bool Contains(const std::string &id);
    MenuItem *GetItemById(const std::string &id);
    MenuItem *GetItemByIndex(int index);

protected:
    friend class MenuBar;
    friend class MenuItem;
    friend class MenuShell;

    std::vector<std::shared_ptr<Menu>> m_menus;
    std::shared_ptr<MenuItem> m_main_item;
};

class MenuBar : public MenuShell
{
public:
    MenuBar();
    virtual ~MenuBar();

    Menu *CreateMenu(const std::string &ID = "Menu");

    SubMenu *CreateSubMenu(const std::string &id, const std::string &itemId, const std::string &itemLabel, bool mnemonic = false, int tag = 0);

protected:
    friend class MenuItem;
    friend class MenuShell;
    friend class Menu;

    GtkMenuBar *m_menuBar;

    std::vector<std::shared_ptr<Menu>> m_menus;
    std::vector<std::shared_ptr<MenuItem>> m_items;
};

//***************************************************************************************
// ToolBar
//***************************************************************************************

class ToolItem : public Widget
{
public:
    ToolItem();
    virtual ~ToolItem();
    int GetIndex();

    virtual void DoActivate();

protected:
    friend class ToolBar;
    friend class ToolButton;
    friend class ToolButtonToggle;
    friend class ToolSeparator;

    GtkToolItem *m_Item;
    ToolBar *m_toolBar{nullptr};
    int m_index{0};
};

class ToolSeparator : public ToolItem
{
public:
    ToolSeparator();
    ~ToolSeparator() = default;

protected:
    friend class ToolBar;
};

class ToolButtonToggle : public ToolItem
{
public:
    ToolButtonToggle(const std::string &label, bool active);

    void SetActive(bool value);
    bool GetActive();

    virtual void DoToggled(bool value);

    virtual ~ToolButtonToggle();

protected:
    friend class ToolBar;
    ToolButtonToggle() = default;
};

class ToolRadioButton : public ToolButtonToggle
{
public:
    ToolRadioButton(const std::string &label);
    ToolRadioButton(ToolRadioButton *parent, const std::string &label);

    ToolRadioButton *AddRadioButtom(const std::string &label, int pos = -1);

    virtual ~ToolRadioButton();

protected:
    ToolRadioButton() = default;
    friend class ToolBar;
};

class ToolButton : public ToolItem
{
public:
    ToolButton(const std::string &label);
    ToolButton(const std::string &stockIcon, const std::string &label);
    virtual ~ToolButton();

protected:
    friend class ToolBar;
};

class ToolBar : public Widget
{
public:
    ToolBar(ToolBarStyle style);
    virtual ~ToolBar();

    void Insert(ToolItem *item, int pos = -1);

     
    void DoActivate(ToolItem *item);
    void DoCheck(ToolItem *item, bool value);
    std::function<void(ToolItem *)> OnActivate{nullptr};
    std::function<void(ToolItem *,bool)> OnCheck{nullptr};

    ToolButton *AddButton(const std::string &label, const std::string &ID = "ToolButton", int pos = -1);
    ToolButton *AddButton(const std::string &stockIcon, const std::string &label, const std::string &ID = "ToolButton", int pos = -1);
    ToolButtonToggle *AddButtonToggle(const std::string &label, bool active, const std::string &ID = "ToolButtonToggle", int pos = -1);
    ToolRadioButton *AddRadioButton(const std::string &label, const std::string &ID = "ToolRadioButton", int pos = -1);
    ToolSeparator *AddSeparator(int pos = -1);

protected:
    friend class ToolItem;
    friend class ToolButton;
    friend class ToolButtonToggle;
    friend class ToolRadioButton;
    friend class ToolSeparator;

    GtkToolbar *m_toolbar;
    std::vector<std::shared_ptr<ToolItem>> m_items;
};