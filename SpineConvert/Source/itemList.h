#pragma once
#include <JuceHeader.h>

extern std::unordered_map<float, juce::String> g_TranslatorPath;

class itemInList : public juce::Component,
				   public juce::ToggleButton::Listener,
				   public juce::TextEditor::Listener
{
public:
	//==============================================================================
	itemInList(juce::StringRef text)
		: m_listener(nullptr), m_button(L"...")
	{
		addAndMakeVisible(m_toggle);
		addAndMakeVisible(m_text);
		addAndMakeVisible(m_button);
		addAndMakeVisible(m_info);

		m_toggle.addListener(this);
		m_text.addListener(this);
		m_button.addListener(this);

		m_text.setFont({ "Microsoft YaHei", 16, juce::Font::FontStyleFlags::plain });
		m_text.setText(text, false);
		m_text.setReadOnly(true);
		
		m_info.setFont({ "Microsoft YaHei", 16, juce::Font::FontStyleFlags::plain });

		SetComplete(false);
		SetInfoText(L"Î´¿ªÊ¼");
	};

	void resized() override
	{
		constexpr auto buttonWith = 50;
		m_toggle.setBounds(0, 0, getHeight(), getHeight());
		m_text.setBounds(getHeight(), 0, getWidth() - getHeight() - buttonWith, getHeight());
		m_button.setBounds(getWidth() - buttonWith, 0, buttonWith, getHeight());
		m_info.setBounds(getHeight(), 10, getWidth() - getHeight() - buttonWith, getHeight());
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

	void textEditorReturnKeyPressed(juce::TextEditor& editor)
	{
		if (m_listener != nullptr) {
			m_listener->TextEditorChanged(this);
		}
	}

	//==============================================================================
	juce::ToggleButton& GetToggleButton()
	{
		return m_toggle;
	}

	juce::TextEditor& GetTextEditor()
	{
		return m_text;
	}

	bool GetState()
	{
		return m_toggle.getToggleState();
	}

	juce::String GetText()
	{
		return m_text.getText();
	}

	void SetInfoText(juce::String text)
	{
		m_info.setText(text, juce::dontSendNotification);
	}
	//==============================================================================
	void SetComplete(bool complete)
	{
		m_text.setColour(juce::TextEditor::ColourIds::backgroundColourId,
			complete ? juce::Colours::darkgreen : juce::Colours::red
		);
		m_text.repaint();
	}

	//==============================================================================
	class Listener
	{
	public:
		~Listener() {}
		virtual void ToggleButtonChanged(itemInList* item) {}
		virtual void TextEditorChanged(itemInList* item) {}
		virtual void SelectButtonClicked(itemInList* item) {}
	};
	void SetListener(Listener* ls) { m_listener = ls; }

	//==============================================================================
private:
	Listener* m_listener;
	juce::ToggleButton m_toggle;
	juce::TextEditor   m_text;
	juce::TextButton   m_button;
	juce::Label        m_info;
};

//==============================================================================





constexpr auto itemHeight = 36;
class itemList : public juce::Component,
				 public itemInList::Listener
{
public:
	struct Path
	{
		juce::String filePath;
		bool shouldDoConvente;
	};

	itemList(const juce::OwnedArray<Path>& strings);

	void paint(juce::Graphics& g) override;
	void resized() override;

	//==============================================================================
	void ToggleButtonChanged(itemInList* item);
	void TextEditorChanged  (itemInList* item);
	void SelectButtonClicked(itemInList* item);

	//==============================================================================
	void AddItem(juce::StringRef text);
	void AddItemFromChooserBox();
	void RemoveItem(int index);
	void RemoveItem(const Path* p);

	itemInList* GetItem(int index);

	int GetNumItems() const { return m_items.size(); }

	//==============================================================================
	void CancelAllSelect();
	void SelectAll();
	void AntiSelect();
	void RemoveSelect();

	//==============================================================================
	class Listener
	{
	public:
		~Listener(){}
		virtual void ItemToggleChanged(int indexOfComponent, bool state){}
		virtual void ItemTextChanged  (int indexOfComponent, juce::String text){}
		virtual void ItemButtonChanged(int indexOfComponent){}
		virtual void ItemAdded        (juce::String text){}
		virtual void ItemDeleted	  (int indexOfComponenet){}
	};
	void SetListener(Listener* ls) { m_listener = ls; }

	//==============================================================================
	

private:
	const juce::OwnedArray<Path>& m_lib;
	std::unique_ptr<juce::FileChooser> m_chooser;
	Listener* m_listener;
	juce::OwnedArray<itemInList> m_items;
};
