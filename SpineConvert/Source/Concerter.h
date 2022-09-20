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
        STARTUPINFO startUpInfo;
        PROCESS_INFORMATION processInfo;
    };

    Concerter() = default;
    ~Concerter() = default;

    static void UpdateConventers();

    Error StartConvent(juce::String path);
    float GetSkelVersion(juce::FileInputStream& streamIn);
private:
};
