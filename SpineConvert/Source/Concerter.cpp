/*
  ==============================================================================

    Concerter.cpp
    Created: 17 Sep 2022 6:47:18pm
    Author:  mana

  ==============================================================================
*/

#include "Concerter.h"

constexpr auto Bin35Path = L"D:\\files\\download\\compressed\\SpineConverter-main\\Bin35ToBin38\\bin\\Debug\\net5.0\\Bin35ToBin38.exe";
constexpr auto Bin36Path = L"D:\\files\\download\\compressed\\SpineConverter-main\\Bin36ToBin38\\bin\\Debug\\net5.0\\Bin36ToBin38.exe";

std::unordered_map<float, juce::String> g_TranslatorPath;

void Concerter::UpdateConventers()
{
    /* get conventer avaliable */
    g_TranslatorPath.clear();

    auto path = juce::File::getCurrentWorkingDirectory().getChildFile("conventer");
    juce::Array<juce::File> versions = path.findChildFiles(juce::File::TypesOfFileToFind::findDirectories, false);

    for (auto& c : versions) {
        float versionNum = c.getFileName().getFloatValue();
        auto conventers = c.findChildFiles(juce::File::TypesOfFileToFind::findFiles, false, "*.exe");

        if (conventers.size()) {
            auto& versionPath = conventers.getFirst().getFullPathName();
            g_TranslatorPath[versionNum] = versionPath;
        }
    }
}

Concerter::Error Concerter::StartConvent(juce::String path)
{
    juce::File f(path);
    auto input = f.createInputStream();
    auto version = GetSkelVersion(*input);

    if (g_TranslatorPath.find(version) == g_TranslatorPath.end()) {
        return UnknowSkelVersion;
    }

    /* before : xxx.exe*.skel  */
    juce::String pathWithSpaceBegin{ " " }; pathWithSpaceBegin << path;
    /* after  : xxx.exe *.skel */

    Process p;
    memset(&p, 0, sizeof(Process));

    /*p.state = CreateProcessW(Bin35Path, pathWithSpaceBegin.toUTF16().getAddress(), nullptr, nullptr, false, CREATE_NEW_CONSOLE, nullptr, nullptr, &p.startUpInfo, &p.processInfo);
    p.state = CreateProcessW(Bin36Path, pathWithSpaceBegin.toUTF16().getAddress(), nullptr, nullptr, false, CREATE_NEW_CONSOLE, nullptr, nullptr, &p.startUpInfo, &p.processInfo);
    */
    p.state = CreateProcessW(g_TranslatorPath[version].toUTF16().getAddress(), pathWithSpaceBegin.toUTF16().getAddress(), nullptr, nullptr, false, CREATE_NEW_CONSOLE, nullptr, nullptr, &p.startUpInfo, &p.processInfo);

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

float Concerter::GetSkelVersion(juce::FileInputStream& streamIn)
{
    streamIn.setPosition(0x1d);
    char fisrtNum = streamIn.readByte();
    streamIn.readByte();
    char secondNum = streamIn.readByte();

    DBG("version is:" << fisrtNum << '.' << secondNum);

    float version = fisrtNum - '0' + (secondNum - '0') * 0.1f;
    return version;
}
