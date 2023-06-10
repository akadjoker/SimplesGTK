#include "Bars.hpp"


//***************************************************************************************
// LevelBar
//***************************************************************************************


LevelBar::LevelBar()
{
    m_widget = gtk_level_bar_new();
}

LevelBar::LevelBar(double minValue, double maxValue)
{
    m_widget = gtk_level_bar_new_for_interval(minValue, maxValue);   


    
}

void LevelBar::SetMinValue(double minValue)
{
    
    gtk_level_bar_set_min_value (GTK_LEVEL_BAR(m_widget), minValue);

}

double LevelBar::GetMinValue()
{
    
    return gtk_level_bar_get_min_value(GTK_LEVEL_BAR(m_widget));

}

void LevelBar::SetMaxValue(double maxValue)
{
    
    gtk_level_bar_set_max_value(GTK_LEVEL_BAR(m_widget), maxValue);

}

double LevelBar::GetMaxValue()
{
    return gtk_level_bar_get_max_value(GTK_LEVEL_BAR(m_widget));
}

void LevelBar::SetValue(double value)
{
       gtk_level_bar_set_value(GTK_LEVEL_BAR(m_widget), value);
}

double LevelBar::GetValue()
{
    return gtk_level_bar_get_value(GTK_LEVEL_BAR(m_widget));
}

void LevelBar::SetInverted(bool inverted)
{
    
    gtk_level_bar_set_inverted (GTK_LEVEL_BAR(m_widget), inverted);

}

bool LevelBar::GetInverted()
{
    
    return gtk_level_bar_get_inverted(GTK_LEVEL_BAR(m_widget));

}

void LevelBar::SetContinuous(bool value)
{
    if (value)
        gtk_level_bar_set_mode(GTK_LEVEL_BAR(m_widget), GTK_LEVEL_BAR_MODE_CONTINUOUS);
    else
        gtk_level_bar_set_mode(GTK_LEVEL_BAR(m_widget), GTK_LEVEL_BAR_MODE_DISCRETE);

}





//***************************************************************************************
// ProgressBar
//***************************************************************************************


ProgressBar::ProgressBar()
{
   m_widget = gtk_progress_bar_new();
}



void ProgressBar::Pulse()
{

    gtk_progress_bar_pulse(GTK_PROGRESS_BAR(m_widget));
}



void ProgressBar::SetFraction(double fraction)
{
   
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(m_widget), fraction);
}

double ProgressBar::GetFraction()
{
   
    return gtk_progress_bar_get_fraction(GTK_PROGRESS_BAR(m_widget));
}

void ProgressBar::SetShowText(bool show)
{
 
    gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(m_widget), show);
}

bool ProgressBar::GetShowText()
{
  
    return gtk_progress_bar_get_show_text(GTK_PROGRESS_BAR(m_widget));
}

void ProgressBar::SetText(const std::string &text)
{
  
    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(m_widget), text.c_str());
}

std::string ProgressBar::GetText()
{

    return gtk_progress_bar_get_text(GTK_PROGRESS_BAR(m_widget));
}

void ProgressBar::SetPulseStep(double fraction)
{
    
    gtk_progress_bar_set_pulse_step(GTK_PROGRESS_BAR(m_widget), fraction);
}

double ProgressBar::GetPulseStep()
{

    return gtk_progress_bar_get_pulse_step(GTK_PROGRESS_BAR(m_widget));
}

void IBar::SetOrientation(Orientation orientation)
{
    assert(m_widget);
    if (orientation == Orientation::Horizontal)
        gtk_orientable_set_orientation(GTK_ORIENTABLE(m_widget), GTK_ORIENTATION_HORIZONTAL);
    else
        gtk_orientable_set_orientation(GTK_ORIENTABLE(m_widget), GTK_ORIENTATION_VERTICAL);
}

Orientation IBar::GetOrientation()
{
    assert(m_widget);
    if (gtk_orientable_get_orientation(GTK_ORIENTABLE(m_widget)) == GTK_ORIENTATION_HORIZONTAL)
        return Orientation::Horizontal;
    else
        return Orientation::Vertical;

}


//***************************************************************************************
// Spinner
//***************************************************************************************

Spinner::Spinner()
{
    m_widget = gtk_spinner_new();
}

void Spinner::Start()
{
    gtk_spinner_start(GTK_SPINNER(m_widget));
}

void Spinner::Stop()
{
    gtk_spinner_stop(GTK_SPINNER(m_widget));
}




//***************************************************************************************
// Statusbar
//***************************************************************************************

StatusBar::StatusBar()
{
    m_widget = gtk_statusbar_new();
}

int StatusBar::Push(const std::string &text, int contextId)
{
   return gtk_statusbar_push(GTK_STATUSBAR(m_widget), contextId, text.c_str());
}

void StatusBar::Pop(int contextId)
{
    gtk_statusbar_pop(GTK_STATUSBAR(m_widget), contextId);
}

void StatusBar::Remove(int contextId, int messageId)
{
    gtk_statusbar_remove(GTK_STATUSBAR(m_widget), contextId, messageId);
}

void StatusBar::RemoveAll(int context_id)
{
     
    gtk_statusbar_remove_all(GTK_STATUSBAR(m_widget), context_id);
}

