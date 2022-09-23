#pragma once

#include <JuceHeader.h>

class storger
{
public:
	//==============================================================================
	struct Path
	{
		juce::String filePath;
		bool shouldDoConvente;
		int version;

		static int floatStringToInt(const juce::String& in) {
			auto indexOfPoint = in.indexOfChar(L'.');
			auto st1 = in.substring(0, indexOfPoint);
			auto nd2 = in.substring(indexOfPoint + 1, in.length());
			return st1.getIntValue() * 10 + nd2.getIntValue();
		}
	};

	//==============================================================================
	bool isExist(const juce::StringRef toFind)
	{
		return IndexOf(toFind) != GetNumSaved();
	}

	int IndexOf(const juce::StringRef toFind)
	{
		int i = 0;
		for (auto* item : m_pathsSaved) {
			if (item->filePath == toFind) {
				break;
			}
			i++;
		}

		return i;
	}

	int GetNumSaved() const
	{
		return m_pathsSaved.size();
	}

	//==============================================================================
	void AddPath(Path* newPath)
	{
		m_pathsSaved.add(newPath);
	}

	void RemovePath(const juce::StringRef toDelete)
	{
		auto index = IndexOf(toDelete);
		if (index != GetNumSaved()) {
			m_pathsSaved.remove(index);
		}
	}

	//==============================================================================
	Path* Get(const juce::StringRef toFind)
	{
		auto index = IndexOf(toFind);
		if (index != GetNumSaved()) {
			return m_pathsSaved[index];
		}
		return nullptr;
	}

private:
	juce::OwnedArray<Path> m_pathsSaved;
};