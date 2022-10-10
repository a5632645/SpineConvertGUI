/*
  ==============================================================================

    Concerter.h
    Created: 17 Sep 2022 6:47:18pm
    Author:  mana

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <Windows.h>
#include "storger.h"

class Concerter
{
public:
    enum Error
    {
        Succuess,
        UnknowSkelVersion,
        ProcessLaunchFailed
    };

    struct Process
    {
        bool state;
        STARTUPINFOW startUpInfo;
        PROCESS_INFORMATION processInfo;
    };

    Concerter();
    ~Concerter();

    static void UpdateConventers();

    Error StartConvent(storger::Path& p);
    static int GetSkelVersion(juce::StringRef filePath);
    inline static bool IsExistConverter(int version);

    void SetOutputFolder();
    juce::String& GetOutputPath() { return m_outputPath; }

    std::function<void(const juce::String& newPath)> onOutFolderChanged;
private:
    std::unique_ptr<juce::FileChooser> m_chooser;
    juce::String m_outputPath;
};
