#pragma once

#include "Widget.hpp"
#include "Layout.hpp"
#include "Buttons.hpp"

class GTK_API_EXPORT Group : public Layout
{
public:
    Group();
    virtual ~Group();
    virtual void Add(Widget *widget) override;
protected:

    friend class Window;
    friend class FrameLayout;
    friend class RadioGroup;

  
};


//***************************************************************************
// RadioGroup
//***************************************************************************

class GTK_API_EXPORT RadioGroup : public Group
{
    public:

        RadioGroup(const std::string &label, Orientation orientation, int spacing);
      
        RadioButton *AddItem(const std::string &label,bool expand=false, bool fill=false, int padding=0);
        void DeselectAll();
        void SetChecked(int index,bool check);
        bool IsChecked(int index);
        int GetSelectedIndex();
        RadioButton* Get(int index);


   
public:
        std::function<bool( ToggleButton* button)> OnToggled;   
protected:
        std::shared_ptr<FrameLayout>frame;
        std::shared_ptr<BoxLayout>m_box;  
        std::vector<std::shared_ptr<RadioButton>> m_radioButtons;
        std::shared_ptr<RadioButton> m_Root;
        bool m_isRoot{false};
};

//***************************************************************************
// CheckGroup
//***************************************************************************


class GTK_API_EXPORT CheckGroup : public Group
{
    public:
    CheckGroup(const std::string &label, Orientation orientation, int spacing);
 
    CheckButton *AddItem(const std::string &label,bool expand=false, bool fill=false, int padding=0);
    void SetCheckAll(bool cheked);
    void SetCheck(int index,bool check);
    bool IsChecked(int index);
    int GetIndex(const std::string &label);
    int GetIndex(const char *label);


    

public:
    std::function<bool( ToggleButton* button)> OnCheck;   

private:
        std::shared_ptr<FrameLayout>frame;
        std::shared_ptr<BoxLayout>m_box; 
        std::vector<std::shared_ptr<CheckButton>> m_checkButtons;
};

//***************************************************************************
// GroupBox
//***************************************************************************

class GTK_API_EXPORT GroupBox : public Group
{
public:
    GroupBox(const std::string &label, Orientation orientation, int spacing);

    void AddItem(Widget *widget, bool expand=false, bool fill=false, int padding=0);

    Button *AddButton(const std::string &label, bool expand=false, bool fill=false, int padding=0);
 
   
 private:
    friend class Group;
    friend class CheckGroup;
    friend class RadioGroup;
    

        std::shared_ptr<FrameLayout>frame;
        std::shared_ptr<BoxLayout>m_box; 
        std::vector<std::shared_ptr<Widget>> m_widgets;      
};