#pragma once

#include <JuceHeader.h>
#include "FileWindows.h"

class ChineseLookAndFeel : public juce::LookAndFeel_V4
{
public:
    juce::Font getTextButtonFont(juce::TextButton&, int buttonHeight) override
    {
        return juce::Font("Microsoft YaHei", juce::jmin(16.0f, (float)buttonHeight * 0.6f), juce::Font::FontStyleFlags::plain);
    }
};
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
    std::unique_ptr<FileWindows> m_workPlace;
    std::unique_ptr<ChineseLookAndFeel> m_laf;
    Concerter m_concerter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
