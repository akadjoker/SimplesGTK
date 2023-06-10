#pragma once

#include "Widget.hpp"

class Spinner : public Widget
{

public:
    Spinner();
    void Start();
    void Stop();
};

class StatusBar : public Widget
{
public:
    StatusBar();
    int Push(const std::string &text,int id);
    void Pop(int id);
    void Remove(int context_id, int message_id);
    void RemoveAll(int context_id);
};

class IBar : public Widget
{
public:
    IBar() = default;
    virtual ~IBar() = default;

    void SetOrientation(Orientation orientation);
    Orientation GetOrientation();
};

class ProgressBar : public IBar
{
public:
    ProgressBar();
    void SetFraction(double fraction);
    double GetFraction();
    void Pulse();
    void SetText(const std::string &text);
    std::string GetText();
    void SetShowText(bool show);
    bool GetShowText();
    void SetPulseStep(double fraction);
    double GetPulseStep();

protected:
};

class LevelBar : public IBar
{
public:
    LevelBar();
    LevelBar(double minValue, double maxValue);

    void SetMinValue(double minValue);
    double GetMinValue();
    void SetMaxValue(double maxValue);
    double GetMaxValue();
    void SetValue(double value);
    double GetValue();
    void SetInverted(bool inverted);
    bool GetInverted();
    void SetContinuous(bool value);
};
