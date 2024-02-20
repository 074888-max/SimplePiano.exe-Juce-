/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   PianoC4_wav;
    const int            PianoC4_wavSize = 2693552;

    extern const char*   Ring02_wav;
    const int            Ring02_wavSize = 314468;

    extern const char*   Ring06_wav;
    const int            Ring06_wavSize = 305876;

    extern const char*   PianoPic_png;
    const int            PianoPic_pngSize = 376054;

    extern const char*   MidiPiano_1_png;
    const int            MidiPiano_1_pngSize = 1669812;

    extern const char*   PicSynth_png;
    const int            PicSynth_pngSize = 438960;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 6;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
