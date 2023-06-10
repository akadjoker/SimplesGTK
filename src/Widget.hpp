#pragma once

#include <string>
#include <cassert>
#include <vector>
#include <functional>
#include <memory>
#include <iostream>
#include <gtk/gtk.h>

#if defined(_WIN32)
     #define GTK_SYSTEM_WINDOWS
#elif defined(__unix__)
    #define GTK_SYSTEM_LINUX
#endif

#if !defined(GTK_STATIC)

    #if defined(GTK_SYSTEM_WINDOWS)
        #define GTK_API_EXPORT __declspec(dllexport)
        #define GTK_API_IMPORT __declspec(dllimport)
        #ifdef _MSC_VER

            #pragma warning(disable: 4251)

        #endif

    #else // Linux, FreeBSD, Mac OS X

        #if __GNUC__ >= 4

            // GCC 4 has special keywords for showing/hidding symbols,
            // the same keyword is used for both importing and exporting
            #define GTK_API_EXPORT __attribute__ ((__visibility__ ("default")))
            #define GTK_API_IMPORT __attribute__ ((__visibility__ ("default")))

        #else

            // GCC < 4 has no mechanism to explicitely hide symbols, everything's exported
            #define GTK_API_EXPORT
            #define GTK_API_IMPORT
        #endif
    #endif
#else
   // Static build doesn't need import/export macros
    #define GTK_API_EXPORT
    #define GTK_API_IMPORT
#endif

enum class Orientation
{
    Vertical,
    Horizontal
};

enum class Justification
{
    Left,
    Right,
    Center,
    Fill
};

enum class PositionType
{
    Left,
    Right,
    Top,
    Bottom
};

enum class ReliefStyle
{
    Normal,
    Half,
    None
};

enum class Align
{
    Start,
    End,
    Center,
    Baseline,
    Fill
};

enum class ShadowType
{
    None,
    In,
    Out,
    EtchedIn,
    EtchedOut
};

enum class SortType
{
    Ascending,
    Descending
};

enum class DialogButtons
{
    NONE = 0,
    OK = 1,
    CANCEL = 2,
    YES_NO = 3,
    OK_CANCEL = 4
};

enum class DialogType
{
    INFO = 0,
    WARNING = 1,
    QUESTION = 2,
    ERROR = 3,
    OTHER = 4
};

class Widget;
class Button;
class Window;


class Event
{
public:

    struct SizeEvent
    {
        unsigned int width;  
        unsigned int height; 
    };

    struct MouseMoveEvent
    {
        int x; 
        int y; 
    };
       struct MouseButtonEvent
    {
        int           button; 
        int           x;      
        int           y;      
    };

    struct ButtonClickEvent
    {
        Button *button;
    };

    struct ButtonCheckEvent
    {
        Button *button;
        bool    checked;
    };

    
    enum EventType
    {
        Closed,                 
        Resized,                
        LostFocus,              
        GainedFocus,            
        KeyPressed,           
        KeyReleased,          
        MouseButtonPressed,   
        MouseButtonReleased,  
        MouseMoved,      
        ButtonClick,  
        ButtonCheck,   
        Count                 
    };
    EventType type; 

    union
    {
        SizeEvent             size;    
        MouseMoveEvent        mouseMove;       
        MouseButtonEvent      mouseButton;    
        ButtonClickEvent      buttonClick; 
        ButtonCheckEvent      buttonCheck;
    };
};



class GTK_API_EXPORT Widget
{
public:
    Widget() = default;
    virtual ~Widget();

    
    Widget    *GetLayout() ;
   



    void SetTag(int tag) ;
    int  GetTag() const ;
    void SetChildVisible(bool visible);
    void SetVisible(bool visible);
    void SetActive(bool active);

    bool IsVisible() const;
    bool IsActive() const;

    void SetSize(int width, int height);

    void SetId(const std::string &id) ;
    const std::string &GetId() const ;

    void AddMouseEvents();


    std::function<bool()> OnClick;  
    std::function<bool(bool)> OnCheck;   

    std::function<bool(const std::string &text)> OnTextReturn;
    std::function<bool(const std::string &text)> OnTextChange;



protected:
    friend class Window;
    friend class FrameLayout;
    friend class RadioGroup;
    friend class CheckGroup;
    friend class FixedLayout;
    friend class BoxLayout;
    friend class GroupBox;
    friend class Group;
    friend class Layout;
    friend class GridLayout;
    GtkWidget   *m_widget{NULL};
    Widget      *m_layout{nullptr};
    Window      *m_window{nullptr};
    int m_tag{0};
    std::string m_id;
};