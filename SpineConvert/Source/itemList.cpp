#include "itemList.h"

//==============================================================================
itemList::itemList(const juce::OwnedArray<Path>& strings)
	:m_lib(strings),
	m_chooser(new juce::FileChooser(L"选择skel文件",juce::File(),"*.skel")), m_listener(nullptr)
{
	/*for (auto& item : strings) {
		m_items.add(new itemInList(item));
	}
	for (auto* item : m_items) {
		addAndMakeVisible(*item);
		item->SetListener(this);
	}*/
	/*for (auto* item : strings) {
		AddItem(item->filePath);
	}*/
}

void itemList::paint(juce::Graphics& g)
{
	g.setColour(juce::Colours::grey);
	g.drawRect(getLocalBounds(), 2);
}

void itemList::resized()
{
	juce::Rectangle<int> bound(getWidth(), itemHeight);
	for (auto* item : m_items) {
		item->setBounds(bound);
		bound.translate(0, itemHeight);
	}
}

//==============================================================================
void itemList::ToggleButtonChanged(itemInList* item)
{
	if (m_listener != nullptr) {		
		m_listener->ItemToggleChanged(m_items.indexOf(item), item->GetState());
	}
}

void itemList::TextEditorChanged(itemInList* item)
{
	if (m_listener != nullptr) {
		m_listener->ItemTextChanged(m_items.indexOf(item), item->GetText());
	}
}

void itemList::SelectButtonClicked(itemInList* item)
{
	m_chooser->launchAsync(juce::FileBrowserComponent::openMode, [this,item](const juce::FileChooser& c) mutable {
		if (c.getResult() != juce::File()) {
			/* file choosed */
			auto path = c.getResult().getFullPathName();
			for (auto* item : m_lib) {
				if (item->filePath == path) {
					return;
				}
			}

			item->GetTextEditor().setText(path);
			item->GetToggleButton().setToggleState(true, juce::sendNotification);
			item->SetComplete(false);
			item->SetInfoText(L"未开始");

			if (m_listener != nullptr) {
				m_listener->ItemTextChanged(m_items.indexOf(item), path);
			}
		}
		});
}

//==============================================================================
void itemList::AddItem(juce::StringRef text)
{
	if (m_listener) {
		for (auto* item : m_lib) {
			if (item->filePath == text) {
				return;
			}
		}
	}

	itemInList* temp = new itemInList(text);
	addAndMakeVisible(*temp);
	temp->SetListener(this);
	temp->GetToggleButton().setToggleState(true, juce::dontSendNotification);
	m_items.add(temp);

	if (m_listener != nullptr) {
		m_listener->ItemAdded(text);
	}
}

void itemList::AddItemFromChooserBox()
{
	using flags = juce::FileBrowserComponent::FileChooserFlags;

	auto flag = flags::openMode | flags::canSelectMultipleItems;

	m_chooser->launchAsync(flag, [this](const juce::FileChooser& c) {
		auto paths = c.getResults();
		for (auto& path : paths) {
			AddItem(path.getFullPathName());
		}
	});
}

void itemList::RemoveItem(int index)
{
	removeChildComponent(index);
	m_items.remove(index);

	if (m_listener != nullptr) {
		m_listener->ItemDeleted(index);
	}
}

void itemList::RemoveItem(const Path* p)
{
	RemoveItem(m_lib.indexOf(p));
}

itemInList* itemList::GetItem(int index)
{
	return m_items[index];
}

//==============================================================================
void itemList::CancelAllSelect()
{
	for (auto* item : m_items) {
		auto& toggle = item->GetToggleButton();
		toggle.setToggleState(false, juce::sendNotification);
	}
}

void itemList::SelectAll()
{
	for (auto* item : m_items) {
		auto& toggle = item->GetToggleButton();
		toggle.setToggleState(true, juce::sendNotification);
	}
}

void itemList::AntiSelect()
{
	for (auto* item : m_items) {
		auto& toggle = item->GetToggleButton();
		toggle.setToggleState(!toggle.getToggleState(), juce::sendNotification);
	}
}

void itemList::RemoveSelect()
{
	for (auto* item : m_items) {
		if (item->GetState()) {
			auto index = m_items.indexOf(item);
			RemoveItem(index);
		}
	}
}

//==============================================================================
