#pragma once

#include "Widget.hpp"
//https://zetcode.com/gui/gtk2/menusandtoolbars/
class MenuShell;
class MenuBar;
class Menu;
class SubMenu;

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
    Menu *m_parent{nullptr};
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
    MenuItem(const std::string &label, bool mnemonic = false,int tag=0);
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
    CheckMenuItem(bool checked, const std::string &label, bool mnemonic = false,int tag=0);
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
    RadioMenuItem(const std::string &label, bool mnemonic = false,int tag=0);
    RadioMenuItem(RadioMenuItem* parent,const std::string &label, bool mnemonic ,int tag);  

    RadioMenuItem * AddRadioItem(const std::string &label, bool mnemonic = false,int tag=0);

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
    SubMenu(const std::string &id, const std::string &itemId, const std::string &itemLabel, bool mnemonic = false,int tag=0);

    virtual ~SubMenu();

    MenuItem *AddItem(const std::string &label, const std::string &ID = "MenuItem", bool mnemonic = false,int tag=0);
    CheckMenuItem *AddCheckItem(bool checked,const std::string &label, const std::string &ID = "CheckMenuItem", bool mnemonic = false,int tag=0);
    RadioMenuItem *AddRadioItem(const std::string &label, const std::string &ID = "RadioMenuItem", bool mnemonic = false,int tag=0);

    SubMenu *AddSubMenu(const std::string &id, const std::string &itemId, const std::string &itemLabel, bool mnemonic = false,int tag=0);
    MenuSeparator *AddSeparator(const std::string &ID="MenuSeparator");

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
    MenuItem *CreateItem(const std::string &label, const std::string &ID = "MenuItem", bool mnemonic = false,int tag=0);
    CheckMenuItem *CreateCheckItem(bool checked,const std::string &label, const std::string &ID = "CheckMenuItem", bool mnemonic = false,int tag=0);
    RadioMenuItem *CreateRadioItem(const std::string &label, const std::string &ID = "RadioMenuItem", bool mnemonic = false,int tag=0);

    SubMenu *CreateSubMenu(const std::string &id, const std::string &itemId, const std::string &itemLabel, bool mnemonic = false,int tag=0);

protected:
    friend class MenuItem;
    friend class MenuShell;
    friend class Menu;

    GtkMenuBar *m_menuBar;

    std::vector<std::shared_ptr<Menu>> m_menus;
    std::vector<std::shared_ptr<MenuItem>> m_items;
};