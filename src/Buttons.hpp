#pragma once

#include "Widget.hpp"




class GTK_API_EXPORT IButton : public Widget
{
    public:

    IButton()=default;
    virtual ~IButton();

   

    int  GetIndex(){return m_index;}

    
protected:
    
    
    

    friend class RadioGroup;
    friend class CheckGroup;
    int m_index{0};    
    void SetIndex(int index){m_index=index;}

    
};

class GTK_API_EXPORT Button  : public IButton
{
    public:
        Button();
        Button(const std::string &label,bool mnemonic=false);

        void SetLabel(const std::string &label);
        std::string GetLabel();

        virtual void DoClick();
        virtual void DoCheck(bool value);
       
  protected:
        

};

class GTK_API_EXPORT ToggleButton  : public Button
{
    public:
        ToggleButton();
        ToggleButton(const std::string &label,bool mnemonic=false);
        void SetChecked(bool active);
        bool IsChecked();
         void SetMode(bool draw_indicator);
        bool GetMode();
        
protected:
     GtkToggleButton * m_toggleButton{NULL};  
    
          

    
};

class GTK_API_EXPORT RadioButton  : public ToggleButton
{
    public:
        RadioButton();
        RadioButton(const std::string &label,bool mnemonic=false);
        RadioButton(RadioButton* parent,const std::string &label,bool mnemonic=false);

protected:
        GtkRadioButton * m_radio{NULL};  
        

};


class GTK_API_EXPORT CheckButton  : public ToggleButton
{
    public:
        CheckButton();
        CheckButton(const std::string &label,bool mnemonic=false);
    protected:
        GtkCheckButton * m_check{NULL};        
};






