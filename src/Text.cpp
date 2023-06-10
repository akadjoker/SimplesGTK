#include "Text.hpp"

TextView::TextView()
{
    m_widget = gtk_text_view_new();
    m_textView = GTK_TEXT_VIEW(m_widget);
    m_buffer = gtk_text_view_get_buffer(m_textView);
    gtk_text_buffer_get_end_iter(m_buffer, &end);
    gtk_text_view_set_wrap_mode(m_textView, GTK_WRAP_WORD);
}

void TextView::AddText(const std::string &text)
{
    gtk_text_buffer_get_end_iter(m_buffer, &end);
    gtk_text_buffer_insert(m_buffer, &end, text.c_str(), -1);
}

void TextView::ScrollToEnd()
{
    GtkTextMark *end_mark = gtk_text_buffer_create_mark(m_buffer, NULL, &end, FALSE);
    gtk_text_view_scroll_to_mark(m_textView, end_mark, 0.0, TRUE, 0.5, 0.5);
    gtk_text_buffer_delete_mark(m_buffer, end_mark);
}

Label::Label(const std::string &text, bool mnemonic)
{

    if (mnemonic)
        m_widget = gtk_label_new_with_mnemonic(text.c_str());
    else
        m_widget = gtk_label_new(text.c_str());

    m_label = GTK_LABEL(m_widget);
}

//***************************************************************************
//  TextEdit
//***************************************************************************

void on_entry_activate(GtkEntry *entry, gpointer user_data)
{
    TextEdit *textEdit = (TextEdit *)user_data;
    const gchar *text = gtk_entry_get_text(entry);
    if (textEdit->OnTextReturn)
        textEdit->OnTextReturn(text);
    
}
void on_entry_changed(GtkEntry *entry, gpointer user_data)
{
    TextEdit *textEdit = (TextEdit *)user_data;
    const gchar *text = gtk_entry_get_text(entry);
    if (textEdit->OnTextChange)
        textEdit->OnTextChange(text);

    
}

TextEdit::TextEdit()
{
    m_widget = gtk_entry_new();
    m_entry = GTK_ENTRY(m_widget);
    g_signal_connect(m_widget, "activate", G_CALLBACK(on_entry_activate), this);
    g_signal_connect(m_widget, "changed", G_CALLBACK(on_entry_changed), this);
    // gtk_editable_set_editable(GTK_EDITABLE(m_widget), TRUE);
    // gtk_entry_set_editable(GTK_ENTRY(entry), TRUE); // Torna o GtkEntry editável
    
   
  
}

void TextEdit::SetText(const std::string &text)
{
    gtk_entry_set_text(m_entry, text.c_str());
}

std::string TextEdit::GetText()
{
    const gchar *text = gtk_entry_get_text(m_entry);
    return std::string(text);
}
