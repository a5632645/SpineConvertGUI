/*
  ==============================================================================

    FileWindows.cpp
    Created: 17 Sep 2022 6:46:28pm
    Author:  mana

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FileWindows.h"

extern std::unordered_map<float, juce::String> g_TranslatorPath;

//==============================================================================
FileWindows::FileWindows(Concerter& concerter)
    :m_concerter(concerter), m_list(new itemList(this->m_paths)),
    m_add(L"����ļ�"),
    m_antiSelect(L"��ѡ"),
    m_cancelSelect(L"ȡ��ѡ��"),
    m_convert(L"ת��"),
    m_removeSelect(L"�Ƴ�ѡ����ļ�"),
    m_selectAll(L"ȫѡ"),
    m_showConventerFind(L"��ʾ����ת����"),
    m_updateConventers(L"����Ѱ��ת����"),
    m_isShowingAvaliableConventers(false)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(m_pathList);
    m_pathList.setViewedComponent(m_list);
    m_list->SetListener(this);

    addAndMakeVisible(m_add);
    addAndMakeVisible(m_antiSelect);
    addAndMakeVisible(m_cancelSelect);
    addAndMakeVisible(m_convert);
    addAndMakeVisible(m_removeSelect);
    addAndMakeVisible(m_selectAll);
    addAndMakeVisible(m_showConventerFind);
    addAndMakeVisible(m_updateConventers);
    addChildComponent(m_avaliableConventers);

    m_avaliableConventers.setReadOnly(true);
    m_avaliableConventers.setMultiLine(true, false);
    m_avaliableConventers.setFont({ "Microsoft YaHei", 16, juce::Font::FontStyleFlags::plain });

    m_add.onClick = [this]() {
        m_list->AddItemFromChooserBox();
    };
    m_antiSelect.onClick = [this]() {
        m_list->AntiSelect();
    };
    m_cancelSelect.onClick = [this]() {
        m_list->CancelAllSelect();
    };
    m_convert.onClick = [this]() {

        for (auto& item : m_paths) {
            DBG("path: " << item->filePath << " shoudDo: " << (item->shouldDoConvente ? "yes" : "no"));

            if (item->shouldDoConvente) {
                auto result = m_concerter.StartConvent(item->filePath);

                /* ���� */
                auto * itemToDisable = m_list->GetItem(m_paths.indexOf(item));
                if (result == Concerter::Error::Succuess) {
                    itemToDisable->GetToggleButton().setToggleState(false, juce::sendNotification);
                    itemToDisable->SetComplete(true);
                    itemToDisable->SetInfoText(L"ת�����");
                }
                else if (result == Concerter::Error::ProcessLaunchFailed) {
                    itemToDisable->SetInfoText(L"ת��ʧ�ܣ��޷��򿪽���");
                }
                else if (result == Concerter::Error::UnknowSkelVersion) {
                    itemToDisable->SetInfoText(L"ת��ʧ�ܣ�δ֪skel�汾");
                }
            }
        }
    };
    m_removeSelect.onClick = [this]() {
        m_list->RemoveSelect();
    };
    m_selectAll.onClick = [this]() {
        m_list->SelectAll();
    };

    m_showConventerFind.onClick = [this]() {
        if (!m_isShowingAvaliableConventers) {
            m_avaliableConventers.setVisible(true);
            m_avaliableConventers.toFront(true);
            m_showConventerFind.setButtonText(L"�ر���ʾ");
            m_isShowingAvaliableConventers = true;
        }
        else {
            m_avaliableConventers.setVisible(false);
            m_showConventerFind.setButtonText(L"��ʾ����ת����");
            m_isShowingAvaliableConventers = false;
        }
    };

    m_updateConventers.onClick = [this]() {
        Concerter::UpdateConventers();
        SetConventerText();
    };

    /* �ݣ���ÿ����һ�����ھ�����ɨ��һ���ǰ� */
    Concerter::UpdateConventers();
    SetConventerText();
}

FileWindows::~FileWindows()
{
}

void FileWindows::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
    g.setFont(juce::Font("Microsoft YaHei", 16, juce::Font::FontStyleFlags::plain));
    g.setColour(juce::Colours::white);
    g.drawSingleLineText(L"��ɫ->δ��� ��ɫ->���", 10, getHeight() - 16);
    g.drawSingleLineText(L"����ļ�������ק�ļ����봰��", 10, getHeight() - 32);
}

void FileWindows::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    m_list->setBounds(0, 0, getWidth() - 100, itemHeight * m_list->GetNumItems());
    m_pathList.setBounds(m_list->getBounds().withHeight(getHeight()));

    juce::Rectangle<int> bound(getWidth() - 100, 0, 100, 60);
    m_add.setBounds(bound);
    bound.translate(0, 60);
    m_removeSelect.setBounds(bound);
    bound.translate(0, 60);
    m_selectAll.setBounds(bound);
    bound.translate(0, 60);
    m_cancelSelect.setBounds(bound);
    bound.translate(0, 60);
    m_antiSelect.setBounds(bound);
    bound.translate(0, 60);
    m_showConventerFind.setBounds(bound);
    bound.translate(0, 60);
    m_updateConventers.setBounds(bound);
    bound.translate(0, 100);
    m_convert.setBounds(bound);

    m_avaliableConventers.setBounds(m_pathList.getBounds());
}

bool FileWindows::isInterestedInFileDrag(const juce::StringArray& files)
{
    return std::any_of(files.begin(), files.end(), [](auto& string) -> bool
        {
            return string.contains(".skel");
        });
}

void FileWindows::filesDropped(const juce::StringArray& files, int x, int y)
{
    for (auto& item : files) {
        if (item.contains(".skel")) {
            /* this is a skel file */
            m_list->AddItem(item);
        }
    }
}

void FileWindows::ItemToggleChanged(int indexOfComponent, bool state)
{
    m_paths[indexOfComponent]->shouldDoConvente = state;
}

void FileWindows::ItemTextChanged(int indexOfComponent, juce::String text)
{
    m_paths[indexOfComponent]->filePath = text;
}

void FileWindows::ItemButtonChanged(int indexOfComponent)
{

}

void FileWindows::ItemAdded(juce::String text)
{
    m_paths.add(new itemList::Path{ text,true });
    resized();
}

void FileWindows::ItemDeleted(int indexOfComponenet)
{
    m_paths.remove(indexOfComponenet);
    resized();
}

void FileWindows::SetConventerText()
{
    juce::String s;
    for (auto& i : g_TranslatorPath) {
        s << "ver: " << i.first << " path: " << i.second << '\n';
    }
    m_avaliableConventers.setText(s);
}
