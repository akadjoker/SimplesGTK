#include "Widget.hpp"


void on_drag_data_received(GtkWidget *widget, GdkDragContext *drag_context,
                           gint x, gint y, GtkSelectionData *data,
                           guint info, guint time, gpointer user_data)
{
    //g_print("on_drag_data_received\n");
    Widget *window = (Widget *)user_data;
    gchar **uris = gtk_selection_data_get_uris(data);
    if (uris != NULL)
    {
        for (int i = 0; uris[i] != NULL; i++)
        {
            gchar *path = g_filename_from_uri(uris[i], NULL, NULL);
            window->DoDrop(x, y,path);
      //      g_print("Arquivo solto: %s\n", path);
            g_free(path);
        }
        g_strfreev(uris);
    }

    gtk_drag_finish(drag_context, TRUE, FALSE, time);
}

gboolean on_widget_events(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    Widget *window = (Widget *)user_data;
    switch (event->type)
    {
        case GDK_EXPOSE:
            {
                std::cout << "GDK_EXPOSE" << std::endl;
                break;
            }
        case GDK_MOTION_NOTIFY:
            return window->DoMouseMove(event->motion.x, event->motion.y);
        case GDK_BUTTON_PRESS:
            return window->DoMouseDown(event->button.button, event->button.x, event->button.y);
        case GDK_BUTTON_RELEASE:
            return window->DoMouseUp(event->button.button, event->button.x, event->button.y);
        case GDK_KEY_PRESS:
            return window->DoKeyDown(event->key.keyval, event->key.state);
        case GDK_KEY_RELEASE:
            return window->DoKeyUp(event->key.keyval, event->key.state);
        case GDK_ENTER_NOTIFY:
                return window->DoEnter();
        case GDK_LEAVE_NOTIFY:
                return window->DoLeave();
        case GDK_FOCUS_CHANGE:
        {
                if (event->focus_change.in==1)
                    return window->DoFocus();
                else
                    return window->DoLostFocus();
        }
        case GDK_DRAG_ENTER:
        {
            std::cout << "GDK_DRAG_ENTER" << std::endl;
            break;
        }
        case GDK_DRAG_LEAVE:
        {
            std::cout << "GDK_DRAG_LEAVE" << std::endl;
            break;
        }
        case GDK_DRAG_MOTION:
        {
            std::cout << "GDK_DRAG_MOTION" << std::endl;
            break;
        }
        case GDK_DRAG_STATUS:
        {
            std::cout << "GDK_DRAG_STATUS" << std::endl;
            break;
        }
        case GDK_DROP_START:
        {
            std::cout << "GDK_DROP_START" << std::endl;
            break;  
        }
        case GDK_DROP_FINISHED:
        {
            std::cout << "GDK_DROP_FINISHED" << std::endl;
            break;  
        }

        default:
            break;
    }

        

        return false;
}


// void Widget::AddMouseEvents()
// {
//     assert(m_widget);
//     // g_signal_connect(m_widget, "button-press-event", G_CALLBACK(on_button_press_event), this);
//     // g_signal_connect(m_widget, "button-release-event", G_CALLBACK(on_button_release_event), this);
//     // g_signal_connect(m_widget, "motion-notify-event", G_CALLBACK(on_motion_notify_event), this);

// }

// void Widget::AddKeyEvents()
// {
//     assert(m_widget);
//     // g_signal_connect(m_widget, "key-press-event", G_CALLBACK(on_key_press_event), this);
//     // g_signal_connect(m_widget, "key-release-event", G_CALLBACK(on_key_release_event), this);
    

// }

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

void Widget::SetDraggable(bool draggable,int button)
{
        GtkTargetEntry targets[] = 
    {
        {(gchar*)"STRING", 0, 0}
    };

  GdkModifierType flags;
  switch (button)
    {
      case  0:
        {
        flags = GDK_BUTTON1_MASK;
        break;
        }
        case 1:
        {
        flags = GDK_BUTTON2_MASK;   
        break;
        }
        case  2:
        {
        flags = GDK_BUTTON3_MASK;
        break;
        }
        default:
        {       
        flags = GDK_BUTTON1_MASK;
        break;
        }
    }




    GdkDragAction actions = (GdkDragAction)(GDK_ACTION_MOVE);
    if (draggable)
        gtk_drag_source_set(m_widget, flags, targets, sizeof(targets) / sizeof(GtkTargetEntry),  actions);
    else
        gtk_drag_source_set(m_widget, flags, NULL, 0, (GdkDragAction)0);
      
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




bool Widget::DoMouseMove(int x, int y)
{
    if (OnMousMove)
         return OnMousMove(x, y);
    return false;
}


bool Widget::DoKeyDown(int key, int state)
{
   // std::cout << "DoKeyDown  "<<key << " " << state << std::endl;

    if (OnKeyPress)
        return OnKeyPress(key,state);
    return false;    
}

bool Widget::DoKeyUp(int key, int state)
{
    if (OnKeyRelease)
        return OnKeyRelease(key,state);
   // std::cout << "DoKeUp  "<<key << " " << state << std::endl;
    return false;    
}

bool Widget::DoMouseDown(int button, int x, int y)
{
    return false;    
}

bool Widget::DoMouseUp(int button, int x, int y)
{
    return false;    
}


bool Widget::DoFocus()
{
  //  std::cout << "DoFocus:  "<< m_id << std::endl;
    return false;   
}

bool Widget::DoLostFocus()
{
  // std::cout << "DoLostFocus:  "<< m_id << std::endl;
    return false;
}


bool Widget::DoEnter()
{
   // std::cout << "DoEnter:  "<< m_id << std::endl;
    return false;
}




bool Widget::DoLeave()
{
 //   std::cout << "DoLeave:  "<< m_id << std::endl;
    return false;
}

bool Widget::DoDrop(int x, int y, const std::string &data)
{
    if (OnDrop)
        return OnDrop(x,y,data);
    return false;
}

void Widget::SetProcessEvents(bool process)
{
        if (process && handler_id==0)
        {
            handler_id = g_signal_connect(m_widget, "event", G_CALLBACK(on_widget_events), this);    
        } else
        {
            if (handler_id != 0)
            {
                g_signal_handler_disconnect(m_widget, handler_id);
                handler_id = 0;
            }
        }
}

void Widget::SetAcceptDrops(bool accept)
{
     
            GtkTargetEntry targets[] = {
                {(char*)"text/uri-list", 0, 0}
            };

        GdkDragAction actions = (GdkDragAction)(GDK_ACTION_COPY | GDK_ACTION_MOVE | GDK_ACTION_LINK);

        if (accept && drops_handler_id==0)
        {
            gtk_drag_dest_set(m_widget, GTK_DEST_DEFAULT_ALL, targets,  sizeof(targets) / sizeof(GtkTargetEntry),     actions);
            drops_handler_id=g_signal_connect(m_widget, "drag-data-received", G_CALLBACK(on_drag_data_received), this);
        } else
        {
            if (drops_handler_id != 0)
            {
                g_signal_handler_disconnect(m_widget, drops_handler_id);
                gtk_drag_dest_unset(m_widget);
                drops_handler_id = 0;
            }
        }

    //    gtk_drag_dest_set(m_widget, GTK_DEST_DEFAULT_ALL, targets,  sizeof(targets) / sizeof(GtkTargetEntry),     actions);
    //    drops_handler_id=g_signal_connect(m_widget, "drag-data-received", G_CALLBACK(on_drag_data_received), this);
}
