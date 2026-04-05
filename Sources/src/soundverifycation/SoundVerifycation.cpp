cpp
// SoundVerifycation.cpp : Console application to verify WAV files using FMOD Ex (Low Level API)
// Compatible with FMOD Studio Low Level API 1.07.01

#include "stdafx.h"
#include <fmod.hpp>
#include <fmod_errors.h>

// -------------------------------------------------------------------
// Configuration
// -------------------------------------------------------------------
static const char* BAD_SOUNDS_FILE = "bad_sounds.txt";
static const char* CURRENT_SOUND_FILE = "current_sound.txt";

// -------------------------------------------------------------------
// Helper: recursive directory enumeration using Win32 API
// -------------------------------------------------------------------
void EnumerateWavFiles(const std::string& directory, std::vector<std::string>& outFiles)
{
    std::string searchPath = directory + "\\*.wav";
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                std::string fullPath = directory + "\\" + findData.cFileName;
                outFiles.push_back(fullPath);
            }
        } while (FindNextFileA(hFind, &findData) != 0);
        FindClose(hFind);
    }

    // Process subdirectories
    searchPath = directory + "\\*";
    hFind = FindFirstFileA(searchPath.c_str(), &findData);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0)
                {
                    std::string subDir = directory + "\\" + findData.cFileName;
                    EnumerateWavFiles(subDir, outFiles);
                }
            }
        } while (FindNextFileA(hFind, &findData) != 0);
        FindClose(hFind);
    }
}

// -------------------------------------------------------------------
// Helper: read previously recorded bad files into a set
// -------------------------------------------------------------------
void LoadBadFilesSet(std::set<std::string>& badSet)
{
    FILE* f = fopen(BAD_SOUNDS_FILE, "rb");
    if (!f) return;

    char line[1024];
    while (fgets(line, sizeof(line), f))
    {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';
        if (len > 0 && line[len - 1] == '\r') line[len - 1] = '\0';
        if (strlen(line) > 0)
            badSet.insert(line);
    }
    fclose(f);
}

// -------------------------------------------------------------------
// Helper: save bad files set to text file (one per line)
// -------------------------------------------------------------------
void SaveBadFilesSet(const std::set<std::string>& badSet)
{
    FILE* f = fopen(BAD_SOUNDS_FILE, "w");
    if (!f) return;
    for (std::set<std::string>::const_iterator it = badSet.begin(); it != badSet.end(); ++it)
        fprintf(f, "%s\n", it->c_str());
    fclose(f);
}

// -------------------------------------------------------------------
// Helper: write current file being tested (for crash recovery)
// -------------------------------------------------------------------
void WriteCurrentSoundFile(const std::string& filename)
{
    FILE* f = fopen(CURRENT_SOUND_FILE, "w");
    if (f)
    {
        fprintf(f, "%s", filename.c_str());
        fclose(f);
    }
}

// -------------------------------------------------------------------
// Helper: clear current sound file
// -------------------------------------------------------------------
void ClearCurrentSoundFile()
{
    remove(CURRENT_SOUND_FILE);
}

// -------------------------------------------------------------------
// Helper: recover last bad file from previous crash
// -------------------------------------------------------------------
bool RecoverLastBadFile(std::string& outFilename)
{
    FILE* f = fopen(CURRENT_SOUND_FILE, "r");
    if (!f) return false;
    char buf[1024];
    if (fgets(buf, sizeof(buf), f))
    {
        size_t len = strlen(buf);
        if (len > 0 && buf[len - 1] == '\n') buf[len - 1] = '\0';
        outFilename = buf;
        fclose(f);
        return true;
    }
    fclose(f);
    return false;
}

// -------------------------------------------------------------------
// Main
// -------------------------------------------------------------------
int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Usage: SoundVerifycation.exe <directory>\n");
        printf("Scans all .wav files in <directory> recursively and tests if they can be loaded by FMOD.\n");
        printf("Bad files are listed in %s.\n", BAD_SOUNDS_FILE);
        return 1;
    }

    std::string rootDirectory = argv[1];
    printf("==================================================\n");
    printf("Scanning directory: %s\n", rootDirectory.c_str());
    printf("==================================================\n");

    // Initialize FMOD
    FMOD::System* system = NULL;
    FMOD_RESULT result = FMOD::System_Create(&system);
    if (result != FMOD_OK)
    {
        printf("FMOD error: %s\n", FMOD_ErrorString(result));
        return 1;
    }

    result = system->init(32, FMOD_INIT_NORMAL, NULL);
    if (result != FMOD_OK)
    {
        printf("FMOD init error: %s\n", FMOD_ErrorString(result));
        system->release();
        return 1;
    }

    // Load existing list of bad files
    std::set<std::string> badFiles;
    LoadBadFilesSet(badFiles);

    // Check for previous crash (unfinished file)
    std::string crashedFile;
    if (RecoverLastBadFile(crashedFile))
    {
        printf("Recovering from previous crash: %s\n", crashedFile.c_str());
        badFiles.insert(crashedFile);
        ClearCurrentSoundFile();
    }

    // Enumerate all WAV files
    std::vector<std::string> allWavs;
    EnumerateWavFiles(rootDirectory, allWavs);
    printf("Total .wav files found: %d\n", (int)allWavs.size());

    int tested = 0;
    int badCount = (int)badFiles.size();

    for (std::vector<std::string>::const_iterator it = allWavs.begin(); it != allWavs.end(); ++it)
    {
        const std::string& filename = *it;

        // Skip if already known as bad
        if (badFiles.find(filename) != badFiles.end())
        {
            printf("[SKIP] already known bad: %s\n", filename.c_str());
            continue;
        }

        // Write current file for recovery
        WriteCurrentSoundFile(filename);
        tested++;

        // Try to load the sound (use FMOD_OPENONLY to only check headers, not decode)
        FMOD::Sound* sound = NULL;
        result = system->createSound(filename.c_str(), FMOD_OPENONLY | FMOD_ACCURATETIME, NULL, &sound);

        if (result != FMOD_OK)
        {
            printf("[BAD] %s : %s\n", filename.c_str(), FMOD_ErrorString(result));
            badFiles.insert(filename);
            badCount++;
        }
        else
        {
            // Good file - release sound
            sound->release();
            // Optional: print progress
            if (tested % 100 == 0)
                printf("%d files tested, %d bad so far\n", tested, badCount);
        }

        // Clear recovery file after successful test (or after failure we keep it until next start)
        ClearCurrentSoundFile();
    }

    // Save updated bad list
    SaveBadFilesSet(badFiles);

    // Cleanup FMOD
    system->close();
    system->release();

    printf("\n==================================================\n");
    printf("Scan complete.\n");
    printf("Files tested: %d\n", tested);
    printf("Bad files total: %d\n", badCount);
    printf("Bad list saved to: %s\n", BAD_SOUNDS_FILE);
    printf("==================================================\n");

    return 0;
}