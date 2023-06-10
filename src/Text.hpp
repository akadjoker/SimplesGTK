#pragma once
#include "Widget.hpp"


enum class WrapMode
{
    None,
    Char,
    Word,
    WordChar
};


class GTK_API_EXPORT Label : public Widget
{
    public:
        Label(const std::string &text,bool mnemonic=false);
    protected:
        GtkLabel *m_label;
};


class GTK_API_EXPORT TextEdit : public Widget
{
    public:
        TextEdit();

    void SetText(const std::string &text);
    std::string GetText();

    protected:
        GtkEntry *m_entry;
} ;


class GTK_API_EXPORT TextView : public Widget
{
    public :
        TextView();

    void AddText(const std::string &text);
    void SetWrap(bool wrap);
    void ScrollToEnd();

    protected:
        GtkTextView   *m_textView;
        GtkTextBuffer *m_buffer;
        GtkTextIter end;
};