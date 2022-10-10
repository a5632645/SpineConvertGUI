#include "MainComponent.h"
#include "itemList.h"

//==============================================================================
MainComponent::MainComponent()
    :m_laf(new ChineseLookAndFeel), m_workPlace(new FileWindows)
{
    juce::Desktop::getInstance().setDefaultLookAndFeel(m_laf.get());

    setSize (800, 600);
    addAndMakeVisible(*m_workPlace);
}

MainComponent::~MainComponent()
{
    juce::Desktop::getInstance().setDefaultLookAndFeel(nullptr);

    m_laf = nullptr;
    m_workPlace = nullptr;
}

//==============================================================================
void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    m_workPlace->setBounds(getLocalBounds());
}
