#include "Widget.hpp"





gboolean on_button_press_event(GtkWidget *c_widget, GdkEventButton *event, gpointer user_data)
{
    gdouble x = event->x;
    gdouble y = event->y;
    int button = event->button;
    Widget *widget = static_cast<Widget*>(user_data);
    if(widget)
        return widget->DoMousePress(x, y, button);
    return false;
}

 gboolean on_button_release_event(GtkWidget *c_widget, GdkEventButton *event, gpointer user_data)
{
    gdouble x = event->x;
    gdouble y = event->y;
    int button = event->button;
    Widget *widget = static_cast<Widget*>(user_data);
    if (widget)
        return widget->DoMouseRelease(x, y, button);
    return false;
}

 gboolean on_motion_notify_event(GtkWidget *c_widget, GdkEventMotion *event, gpointer user_data)
{
    gdouble x = event->x;
    gdouble y = event->y;
    Widget *widget = static_cast<Widget*>(user_data);
    if (widget)
        return widget->DoMouseMove(x, y);
    return false;
}

 gboolean on_key_release_event(GtkWidget *c_widget, GdkEventKey *event, gpointer user_data)
{

    guint keyval = event->keyval;
    Widget *widget = static_cast<Widget*>(user_data);
    if (widget)
        return widget->DoKeyPress(keyval);
    return false;
}

 gboolean on_key_press_event(GtkWidget *c_widget, GdkEventKey *event, gpointer user_data)
{

    guint keyval = event->keyval;

    Widget *widget = static_cast<Widget*>(user_data);
    if (widget)
        return widget->DoKeyPress(keyval);
   return false;

}

void Widget::AddMouseEvents()
{
    assert(m_widget);
    g_signal_connect(m_widget, "button-press-event", G_CALLBACK(on_button_press_event), this);
    g_signal_connect(m_widget, "button-release-event", G_CALLBACK(on_button_release_event), this);
    g_signal_connect(m_widget, "motion-notify-event", G_CALLBACK(on_motion_notify_event), this);

}

void Widget::AddKeyEvents()
{
    assert(m_widget);
    g_signal_connect(m_widget, "key-press-event", G_CALLBACK(on_key_press_event), this);
    g_signal_connect(m_widget, "key-release-event", G_CALLBACK(on_key_release_event), this);
    

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


bool Widget::DoMousePress(int x, int y, int button)
{
    if (OnMousePress)
        return OnMousePress(x, y, button);
    return false;
}

bool Widget::DoMouseRelease(int x, int y, int button)
{
    if (OnMouseRelease)
       return OnMouseRelease(x, y, button);
    return false;
}

bool Widget::DoMouseMove(int x, int y)
{
    if (OnMousMove)
         return OnMousMove(x, y);
    return false;
}

bool Widget::DoKeyPress(int key)
{
   // std::cout << "keyval: " << key << std::endl;

    if (OnKeyPress)
        return OnKeyPress(key);
    return false;
}

bool Widget::DoKeyRelease(int key)
{
    //    std::cout << "keyval: " << key << std::endl;

    if (OnKeyRelease)
        return OnKeyRelease(key);
    return false;
}

