/*
  ==============================================================================

    FileWindows.h
    Created: 17 Sep 2022 6:46:28pm
    Author:  mana

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <algorithm>
#include "Concerter.h"
#include "itemList.h"
#include "storger.h"

//==============================================================================
/*
*/
class FileWindows  : public juce::Component,
                     public juce::FileDragAndDropTarget
{
public:
    FileWindows();
    ~FileWindows() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    //==============================================================================
    bool isInterestedInFileDrag(const juce::StringArray& files);
    void filesDropped(const juce::StringArray& files, int x, int y);

    //==============================================================================
    void SetConventerText();
    void SetOutputPath(const juce::String& folder);

    //==============================================================================
    void EnableButtons(bool enable);

    //==============================================================================
    Concerter m_concerter;
    itemList* m_list;
    storger m_savedPaths;
    juce::Viewport m_pathList;

    //==============================================================================
    juce::TextButton m_selectAll;
    juce::TextButton m_antiSelect;
    juce::TextButton m_cancelSelect;
    juce::TextButton m_add;
    juce::TextButton m_removeSelect;
    juce::TextButton m_convert;

    juce::TextButton m_showConventerFind;
    juce::TextButton m_updateConventers;
    juce::TextEditor m_avaliableConventers;

    juce::Label      m_outputPath;
    juce::TextButton m_setOutputPath;

    bool m_isShowingAvaliableConventers;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FileWindows)
};
