#pragma once

#include <string>
#include <cassert>
#include <vector>
#include <functional>
#include <memory>
#include <vector>
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

enum class ToolBarStyle
{
    Icons,
    Text,
    Both,
    BothHoriz
};

enum class Gravity
{
    TopLeft=1,
    Top,
    TopRigh,
    Left,
    Center,
    Right,
    BottomLeft,
    Bottom,
    BottomRight,
    Static
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
   
    virtual void OnAdd();//event handler if this widget is on Container so, call show all on this widget
    virtual void AddMouseEvents();
    virtual void AddKeyEvents();



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

    bool DoMousePress(int x, int y, int button);
    bool DoMouseRelease(int x, int y, int button);
    bool DoMouseMove(int x, int y);

    bool DoKeyPress(int key);
    bool DoKeyRelease(int key);



    std::function<bool()> OnClick{nullptr};
    std::function<bool(bool)> OnCheck{nullptr};
 

    std::function<bool(const std::string &text)> OnTextReturn{nullptr};
    std::function<bool(const std::string &text)> OnTextChange{nullptr};

    std::function<bool(int)> OnKeyPress{nullptr};
    std::function<bool(int)> OnKeyRelease{nullptr};

    std::function<bool(int, int, int)> OnMousePress{nullptr};
    std::function<bool(int, int, int)> OnMouseRelease{nullptr};
    std::function<bool(int, int)> OnMousMove{nullptr};

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
    friend class Menu;
    friend class MenuItem;
    friend class MenuBar;
    GtkWidget   *m_widget{NULL};
    Widget      *m_layout{nullptr};
    Window      *m_mainWindow{nullptr};
    bool        isShow{false};
    int m_tag{0};
    std::string m_id;
};

#define KEY_VoidSymbol 0xffffff
#define KEY_BackSpace 0xff08
#define KEY_Tab 0xff09
#define KEY_Linefeed 0xff0a
#define KEY_Clear 0xff0b
#define KEY_Return 0xff0d
#define KEY_Pause 0xff13
#define KEY_Scroll_Lock 0xff14
#define KEY_Sys_Req 0xff15
#define KEY_Escape 0xff1b
#define KEY_Delete 0xffff
#define KEY_Multi_key 0xff20
#define KEY_Codeinput 0xff37
#define KEY_SingleCandidate 0xff3c
#define KEY_MultipleCandidate 0xff3d
#define KEY_PreviousCandidate 0xff3e
#define KEY_Kanji 0xff21
#define KEY_Muhenkan 0xff22
#define KEY_Henkan_Mode 0xff23
#define KEY_Henkan 0xff23
#define KEY_Romaji 0xff24
#define KEY_Hiragana 0xff25
#define KEY_Katakana 0xff26
#define KEY_Hiragana_Katakana 0xff27
#define KEY_Zenkaku 0xff28
#define KEY_Hankaku 0xff29
#define KEY_Zenkaku_Hankaku 0xff2a
#define KEY_Touroku 0xff2b
#define KEY_Massyo 0xff2c
#define KEY_Kana_Lock 0xff2d
#define KEY_Kana_Shift 0xff2e
#define KEY_Eisu_Shift 0xff2f
#define KEY_Eisu_toggle 0xff30
#define KEY_Kanji_Bangou 0xff37
#define KEY_Zen_Koho 0xff3d
#define KEY_Mae_Koho 0xff3e
#define KEY_Home 0xff50
#define KEY_Left 0xff51
#define KEY_Up 0xff52
#define KEY_Right 0xff53
#define KEY_Down 0xff54
#define KEY_Prior 0xff55
#define KEY_Page_Up 0xff55
#define KEY_Next 0xff56
#define KEY_Page_Down 0xff56
#define KEY_End 0xff57
#define KEY_Begin 0xff58
#define KEY_Select 0xff60
#define KEY_Print 0xff61
#define KEY_Execute 0xff62
#define KEY_Insert 0xff63
#define KEY_Undo 0xff65
#define KEY_Redo 0xff66
#define KEY_Menu 0xff67
#define KEY_Find 0xff68
#define KEY_Cancel 0xff69
#define KEY_Help 0xff6a
#define KEY_Break 0xff6b
#define KEY_Mode_switch 0xff7e
#define KEY_script_switch 0xff7e
#define KEY_Num_Lock 0xff7f
#define KEY_KP_Space 0xff80
#define KEY_KP_Tab 0xff89
#define KEY_KP_Enter 0xff8d
#define KEY_KP_F1 0xff91
#define KEY_KP_F2 0xff92
#define KEY_KP_F3 0xff93
#define KEY_KP_F4 0xff94
#define KEY_KP_Home 0xff95
#define KEY_KP_Left 0xff96
#define KEY_KP_Up 0xff97
#define KEY_KP_Right 0xff98
#define KEY_KP_Down 0xff99
#define KEY_KP_Prior 0xff9a
#define KEY_KP_Page_Up 0xff9a
#define KEY_KP_Next 0xff9b
#define KEY_KP_Page_Down 0xff9b
#define KEY_KP_End 0xff9c
#define KEY_KP_Begin 0xff9d
#define KEY_KP_Insert 0xff9e
#define KEY_KP_Delete 0xff9f
#define KEY_KP_Equal 0xffbd
#define KEY_KP_Multiply 0xffaa
#define KEY_KP_Add 0xffab
#define KEY_KP_Separator 0xffac
#define KEY_KP_Subtract 0xffad
#define KEY_KP_Decimal 0xffae
#define KEY_KP_Divide 0xffaf
#define KEY_KP_0 0xffb0
#define KEY_KP_1 0xffb1
#define KEY_KP_2 0xffb2
#define KEY_KP_3 0xffb3
#define KEY_KP_4 0xffb4
#define KEY_KP_5 0xffb5
#define KEY_KP_6 0xffb6
#define KEY_KP_7 0xffb7
#define KEY_KP_8 0xffb8
#define KEY_KP_9 0xffb9
#define KEY_F1 0xffbe
#define KEY_F2 0xffbf
#define KEY_F3 0xffc0
#define KEY_F4 0xffc1
#define KEY_F5 0xffc2
#define KEY_F6 0xffc3
#define KEY_F7 0xffc4
#define KEY_F8 0xffc5
#define KEY_F9 0xffc6
#define KEY_F10 0xffc7
#define KEY_F11 0xffc8
#define KEY_L1 0xffc8
#define KEY_F12 0xffc9
#define KEY_L2 0xffc9
#define KEY_F13 0xffca
#define KEY_L3 0xffca
#define KEY_F14 0xffcb
#define KEY_L4 0xffcb
#define KEY_F15 0xffcc
#define KEY_L5 0xffcc
#define KEY_F16 0xffcd
#define KEY_L6 0xffcd
#define KEY_F17 0xffce
#define KEY_L7 0xffce
#define KEY_F18 0xffcf
#define KEY_L8 0xffcf
#define KEY_F19 0xffd0
#define KEY_L9 0xffd0
#define KEY_F20 0xffd1
#define KEY_L10 0xffd1
#define KEY_F21 0xffd2
#define KEY_R1 0xffd2
#define KEY_F22 0xffd3
#define KEY_R2 0xffd3
#define KEY_F23 0xffd4
#define KEY_R3 0xffd4
#define KEY_F24 0xffd5
#define KEY_R4 0xffd5
#define KEY_F25 0xffd6
#define KEY_R5 0xffd6
#define KEY_F26 0xffd7
#define KEY_R6 0xffd7
#define KEY_F27 0xffd8
#define KEY_R7 0xffd8
#define KEY_F28 0xffd9
#define KEY_R8 0xffd9
#define KEY_F29 0xffda
#define KEY_R9 0xffda
#define KEY_F30 0xffdb
#define KEY_R10 0xffdb
#define KEY_F31 0xffdc
#define KEY_R11 0xffdc
#define KEY_F32 0xffdd
#define KEY_R12 0xffdd
#define KEY_F33 0xffde
#define KEY_R13 0xffde
#define KEY_F34 0xffdf
#define KEY_R14 0xffdf
#define KEY_F35 0xffe0
#define KEY_R15 0xffe0
#define KEY_Shift_L 0xffe1
#define KEY_Shift_R 0xffe2
#define KEY_Control_L 0xffe3
#define KEY_Control_R 0xffe4
#define KEY_Caps_Lock 0xffe5
#define KEY_Shift_Lock 0xffe6

#define KEY_space 0x020
#define KEY_exclam 0x021
#define KEY_quotedbl 0x022
#define KEY_numbersign 0x023
#define KEY_dollar 0x024
#define KEY_percent 0x025
#define KEY_ampersand 0x026
#define KEY_apostrophe 0x027
#define KEY_quoteright 0x027
#define KEY_parenleft 0x028
#define KEY_parenright 0x029
#define KEY_asterisk 0x02a
#define KEY_plus 0x02b
#define KEY_comma 0x02c
#define KEY_minus 0x02d
#define KEY_period 0x02e
#define KEY_slash 0x02f
#define KEY_0 0x030
#define KEY_1 0x031
#define KEY_2 0x032
#define KEY_3 0x033
#define KEY_4 0x034
#define KEY_5 0x035
#define KEY_6 0x036
#define KEY_7 0x037
#define KEY_8 0x038
#define KEY_9 0x039
#define KEY_colon 0x03a
#define KEY_semicolon 0x03b
#define KEY_less 0x03c
#define KEY_equal 0x03d
#define KEY_greater 0x03e
#define KEY_question 0x03f
#define KEY_at 0x040
#define KEY_A 0x041
#define KEY_B 0x042
#define KEY_C 0x043
#define KEY_D 0x044
#define KEY_E 0x045
#define KEY_F 0x046
#define KEY_G 0x047
#define KEY_H 0x048
#define KEY_I 0x049
#define KEY_J 0x04a
#define KEY_K 0x04b
#define KEY_L 0x04c
#define KEY_M 0x04d
#define KEY_N 0x04e
#define KEY_O 0x04f
#define KEY_P 0x050
#define KEY_Q 0x051
#define KEY_R 0x052
#define KEY_S 0x053
#define KEY_T 0x054
#define KEY_U 0x055
#define KEY_V 0x056
#define KEY_W 0x057
#define KEY_X 0x058
#define KEY_Y 0x059
#define KEY_Z 0x05a
#define KEY_bracketleft 0x05b
#define KEY_backslash 0x05c
#define KEY_bracketright 0x05d
#define KEY_asciicircum 0x05e
#define KEY_underscore 0x05f
#define KEY_grave 0x060
#define KEY_quoteleft 0x060
#define KEY_a 0x061
#define KEY_b 0x062
#define KEY_c 0x063
#define KEY_d 0x064
#define KEY_e 0x065
#define KEY_f 0x066
#define KEY_g 0x067
#define KEY_h 0x068
#define KEY_i 0x069
#define KEY_j 0x06a
#define KEY_k 0x06b
#define KEY_l 0x06c
#define KEY_m 0x06d
#define KEY_n 0x06e
#define KEY_o 0x06f
#define KEY_p 0x070
#define KEY_q 0x071
#define KEY_r 0x072
#define KEY_s 0x073
#define KEY_t 0x074
#define KEY_u 0x075
#define KEY_v 0x076
#define KEY_w 0x077
#define KEY_x 0x078
#define KEY_y 0x079
#define KEY_z 0x07a