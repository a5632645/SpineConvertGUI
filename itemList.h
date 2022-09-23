#pragma once
#include <JuceHeader.h>
#include "storger.h"

extern std::unordered_map<int, juce::String> g_TranslatorPath;

class itemInList : public juce::Component,
				   public juce::ToggleButton::Listener
{
public:
	//==============================================================================
	enum class ConvertStauts
	{
		Complete,
		CannotFindConventer,
		UnknowVersion,
		FailedLaunchProcess,
		WaitConvert
	};

	//==============================================================================
	itemInList(juce::StringRef path, int versionIn)
		: m_listener(nullptr), m_button(L"...")
	{
		

		addAndMakeVisible(m_toggle);
		addAndMakeVisible(m_status);
		addAndMakeVisible(m_button);
		addAndMakeVisible(m_path);
		addAndMakeVisible(m_version);

		m_toggle.addListener(this);
		m_button.addListener(this);

		m_path.setFont({ "Microsoft YaHei", 16, juce::Font::FontStyleFlags::plain });
		m_path.setText(path, juce::dontSendNotification);
		
		m_status.setFont({ "Microsoft YaHei", 16, juce::Font::FontStyleFlags::plain });
		
		m_version.setFont({ "Microsoft YaHei", 16, juce::Font::FontStyleFlags::plain });

		juce::String v{ "ver: " }; v << versionIn;
		m_version.setText(v, juce::dontSendNotification);

		SetStatus(itemInList::ConvertStauts::WaitConvert);
	};

	void resized() override
	{
		// 口[ version ][ path ][ status ][###]
		constexpr auto buttonWidth = 50;
		constexpr auto versionWidth = 60;
		constexpr auto statusWidth = 120;
		m_toggle.setBounds(0, 0, getHeight(), getHeight());
		m_version.setBounds(m_toggle.getBounds().getRight(), 0, versionWidth, getHeight());
		m_button.setBounds(getWidth() - buttonWidth, 0, buttonWidth, getHeight());
		m_status.setBounds(m_button.getBounds().getX() - statusWidth, 0, statusWidth, getHeight());
		m_path.setBounds(m_version.getBounds().getRight(), 0, m_status.getBounds().getX() - m_version.getBounds().getRight(), getHeight());
	}

	//==============================================================================
	void buttonClicked(juce::Button* buttonThatClicked)
	{
		if (m_listener != nullptr) {
			if (buttonThatClicked == &m_toggle) {
				m_listener->ToggleButtonChanged(this);
			}
			else if (buttonThatClicked == &m_button) {
				m_listener->SelectButtonClicked(this);
			}
		}

	}

	//==============================================================================
	bool IsSelected() const
	{
		return m_toggle.getToggleState();
	}

	void SetSelected(bool newStatus)
	{
		m_toggle.setToggleState(newStatus, juce::sendNotification);
	}

	//==============================================================================
	juce::String GetPath() const
	{
		return m_path.getText();
	}

	void SetPath(const juce::StringRef newPath)
	{
		m_path.setText(newPath, juce::sendNotification);
	}

	//==============================================================================
	void SetVersion(const float newVersion)
	{
		m_version.setText(juce::String{ newVersion }, juce::dontSendNotification);
	}
	
	void SetStatus(ConvertStauts newStatus)
	{
		switch (newStatus)
		{
		case itemInList::ConvertStauts::Complete:

			m_backGroundColor = juce::Colours::darkgreen;
			m_status.setText(L"转换完成", juce::dontSendNotification);
			SetSelected(false);

			break;
		case itemInList::ConvertStauts::CannotFindConventer:

			m_backGroundColor = juce::Colours::orange;
			m_status.setText(L"找不到对应版本的转换器", juce::dontSendNotification);

			break;
		case itemInList::ConvertStauts::UnknowVersion:

			m_backGroundColor = juce::Colours::yellow;
			m_status.setText(L"未知版本", juce::dontSendNotification);

			break;
		case itemInList::ConvertStauts::FailedLaunchProcess:

			m_backGroundColor = juce::Colours::red;
			m_status.setText(L"未成功调用转换器", juce::dontSendNotification);

			break;
		case itemInList::ConvertStauts::WaitConvert:

			m_backGroundColor = juce::Colours::black;
			m_status.setText(L"等待转换", juce::dontSendNotification);
			SetSelected(true);

			break;
		}

		repaint();
	}

	void paint(juce::Graphics& g) override
	{
		g.fillAll(m_backGroundColor);
		g.setColour(juce::Colours::grey);

		g.drawRect(m_toggle.getBounds(), 2.f);
		g.drawRect(m_path.getBounds(), 2.f);
		g.drawRect(m_version.getBounds(), 2.f);
		g.drawRect(m_status.getBounds(), 2.f);
	}

	//==============================================================================
	class Listener
	{
	public:
		~Listener() {}
		virtual void ToggleButtonChanged(itemInList* item) {}
		virtual void SelectButtonClicked(itemInList* item) {}
	};
	void SetListener(Listener* ls) { m_listener = ls; }

	//==============================================================================
private:
	Listener* m_listener;
	juce::ToggleButton m_toggle;
	juce::Label		   m_path;
	juce::Label		   m_version;
	juce::Label        m_status;
	juce::TextButton   m_button;

	juce::Colour       m_backGroundColor;
};

//==============================================================================



#include "storger.h"

constexpr auto itemHeight = 36;
class itemList : public juce::Component,
				 public itemInList::Listener
{
public:

	itemList(storger& toBind);

	void paint(juce::Graphics& g) override;
	void resized() override;

	//==============================================================================
	void ToggleButtonChanged(itemInList* item);
	void SelectButtonClicked(itemInList* item);

	//==============================================================================
	void AddItem(juce::StringRef text);
	void AddItemFromChooserBox();

	int GetNumItems() const { return m_items.size(); }
	auto& GetItems() const { return m_items; }
	//==============================================================================
	void CancelAllSelect();
	void SelectAll();
	void AntiSelect();
	void RemoveSelect();

	//==============================================================================

private:
	storger& m_lib;
	std::unique_ptr<juce::FileChooser> m_chooser;
	juce::OwnedArray<itemInList> m_items;
};
