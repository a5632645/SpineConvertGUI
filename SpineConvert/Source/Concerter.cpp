/*
  ==============================================================================

    Concerter.cpp
    Created: 17 Sep 2022 6:47:18pm
    Author:  mana

  ==============================================================================
*/

#include "Concerter.h"

std::unordered_map<int, juce::String> g_TranslatorPath;

Concerter::Concerter()
    :m_chooser(new juce::FileChooser(L"输出文件夹"))
{
}

Concerter::~Concerter()
{
    m_chooser = nullptr;
}

void Concerter::UpdateConventers()
{
    /* get conventer avaliable */
    g_TranslatorPath.clear();

    auto path = juce::File::getCurrentWorkingDirectory().getChildFile("conventer");
    juce::Array<juce::File> versions = path.findChildFiles(juce::File::TypesOfFileToFind::findDirectories, false);

    for (auto& c : versions) {
        int versionNum = storger::Path::floatStringToInt(c.getFileName());
        auto conventers = c.findChildFiles(juce::File::TypesOfFileToFind::findFiles, false, "*.exe");

        if (conventers.size()) {
            auto& versionPath = conventers.getFirst().getFullPathName();
            g_TranslatorPath[versionNum] = versionPath;
        }
    }
}

Concerter::Error Concerter::StartConvent(storger::Path& toConvert)
{
    if (!IsExistConverter(toConvert.version)) {
        return UnknowSkelVersion;
    }

    /* before : xxx.exe*.skel  */
    juce::String pathWithSpaceBegin{ " " }; pathWithSpaceBegin << toConvert.filePath;
    /* after  : xxx.exe *.skel */

    if (m_outputPath.isNotEmpty()) {

        /* ?你输出命令不是应该是个文件名吗? */
        juce::File f(toConvert.filePath);

        juce::String outFileName{ "\\" };
        outFileName << f.getFileNameWithoutExtension() << ".38" << ".skel";

        pathWithSpaceBegin << ' ' << m_outputPath << outFileName;
    }

    Process p;
    memset(&p, 0, sizeof(Process));

    p.state = CreateProcessW(
        g_TranslatorPath[toConvert.version].toUTF16().getAddress(), 
        pathWithSpaceBegin.toUTF16().getAddress(),
        nullptr, 
        nullptr, 
        false, 
        CREATE_NEW_CONSOLE, 
        nullptr, 
        nullptr, 
        &p.startUpInfo, 
        &p.processInfo
    );

    if (p.state) {
        WaitForSingleObject(p.processInfo.hProcess, INFINITE);

        CloseHandle(p.processInfo.hProcess);
        CloseHandle(p.processInfo.hThread);
        return Succuess;
    }
    else {
        return ProcessLaunchFailed;
    }
}

int Concerter::GetSkelVersion(juce::StringRef filePath)
{
    juce::File f(filePath);
    auto streamIn = f.createInputStream();

    streamIn->setPosition(0x1d);
    char fisrtNum = streamIn->readByte();
    streamIn->readByte();
    char secondNum = streamIn->readByte();

    DBG("version is:" << fisrtNum << '.' << secondNum);

    return (fisrtNum - '0') * 10 + (secondNum - '0');
}

bool Concerter::IsExistConverter(int version)
{
    return g_TranslatorPath.find(version) != g_TranslatorPath.end();
}

void Concerter::SetOutputFolder()
{
    using flags = juce::FileBrowserComponent::FileChooserFlags;
    auto flag = flags::canSelectDirectories | flags::openMode;

    m_chooser->launchAsync(flag, [this](const juce::FileChooser& c) mutable {

        auto folder = c.getResult().getFullPathName();

        if (folder.isNotEmpty()) {

            m_outputPath = folder;

            if (onOutFolderChanged) {
                onOutFolderChanged(m_outputPath);
            }

        }
        }

    );
}
