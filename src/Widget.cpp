#include "Widget.hpp"


int on_button_press_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    Widget *w = (Widget *)data;
    if (w->OnMousePressed)
        return w->OnMousePressed(event->button.x, event->button.y, event->button.button);
    
    return false;
}

void Widget::AddMouseEvents()
{
    //g_signal_connect(m_widget, "button-press-event", G_CALLBACK(on_button_press_event), this);
    //gtk_widget_add_events(m_widget, GDK_BUTTON_PRESS_MASK);
   // g_signal_connect_swapped(G_OBJECT(m_widget), "button-press-event", G_CALLBACK(on_button_press_event), this);  
    g_signal_connect(m_widget, "button-press-event", G_CALLBACK(on_button_press_event), this);

}

void Widget::SetVisible(bool visible)
{
    assert(m_widget);
    gtk_widget_set_visible(m_widget, visible);
}

void Widget::SetActive(bool active)
{
    assert(m_widget);
    gtk_widget_set_sensitive(m_widget, active);
}

bool Widget::IsVisible() const
{
    assert(m_widget);
    return gtk_widget_get_visible(m_widget);
}

bool Widget::IsActive() const
{
    assert(m_widget);
    return gtk_widget_get_sensitive(m_widget);
}

void Widget::SetSize(int width, int height)
{
    assert(m_widget);
    gtk_widget_set_size_request(m_widget, width, height);
}

void Widget::SetId(const std::string &id)
{
    m_id = id;
}

const std::string& Widget::GetId() const 
{
    return m_id;
}



void Widget::SetChildVisible(bool visible)
{
    assert(m_widget);
    gtk_widget_set_child_visible(m_widget, visible);
}

Widget::~Widget()
{
   // std::cout << "Remove Widget: " << m_id << std::endl;
}


Widget* Widget::GetLayout()
{
    return m_layout;
}

void Widget::OnAdd()
{
    //std::cout << "OnAdd"<< m_id << std::endl;   
}

void Widget::SetTag(int tag)
{
    m_tag = tag;
}

int Widget::GetTag() const
{
    return m_tag;
}


