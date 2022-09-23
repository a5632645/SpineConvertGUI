#include "itemList.h"
#include "Concerter.h"

//==============================================================================
itemList::itemList(storger& toBind)
	:m_lib(toBind),
	m_chooser(new juce::FileChooser(L"Ñ¡ÔñskelÎÄ¼þ",juce::File(),"*.skel"))
{
	
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

	setBounds(getBounds().withHeight(itemHeight * GetNumItems()));
}

//==============================================================================
void itemList::ToggleButtonChanged(itemInList* item)
{
	auto i = m_lib.Get(item->GetPath());
	if (i != nullptr) {
		i->shouldDoConvente = item->IsSelected();
	}
}

void itemList::SelectButtonClicked(itemInList* item)
{
	m_chooser->launchAsync(juce::FileBrowserComponent::openMode, [this, item](const juce::FileChooser& c) mutable {
		if (c.getResult() != juce::File()) {
			/* file choosed */
			auto path = c.getResult().getFullPathName();
			auto i = m_lib.Get(item->GetPath());
			if (i != nullptr) {
				i->filePath = path;
				item->SetPath(path);
				item->SetStatus(itemInList::ConvertStauts::WaitConvert);
			}
		}
		});
}

//==============================================================================
void itemList::AddItem(juce::StringRef path)
{
	if (m_lib.isExist(path)) {
		return;
	}

	auto* newPath = new storger::Path{ juce::String(path),true,Concerter::GetSkelVersion(path) };
	m_lib.AddPath(newPath);

	itemInList* temp = new itemInList(newPath->filePath, newPath->version);
	addAndMakeVisible(*temp);
	temp->SetListener(this);
	temp->SetStatus(itemInList::ConvertStauts::WaitConvert);
	m_items.add(temp);

	resized();
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

//==============================================================================
void itemList::CancelAllSelect()
{
	for (auto* item : m_items) {
		item->SetSelected(false);
	}
}

void itemList::SelectAll()
{
	for (auto* item : m_items) {
		item->SetSelected(true);
	}
}

void itemList::AntiSelect()
{
	for (auto* item : m_items) {
		item->SetSelected(!item->IsSelected());
	}
}

void itemList::RemoveSelect()
{
	for (auto* item : m_items) {
		if (item->IsSelected()) {
			
			m_lib.RemovePath(item->GetPath());
			m_items.removeObject(item, true);

		}
	}

	resized();
}

//==============================================================================
