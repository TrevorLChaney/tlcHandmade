#ifndef WIN32_HANDMADE_H
#define WIN32_HANDMADE_H

struct win32_offscreen_buffer {
    BITMAPINFO Info;
    void *Memory;
    int Width;
    int Height;
    int Pitch;
    int BytesPerPixel;
};

struct win32_window_dimensions {
    int Width;
    int Height;
};

struct win32_sound_output {
    int SamplesPerSecond;
    uint32_t RunningSampleIndex;
    int BytesPerSample;
    DWORD SecondaryBufferSize;
    DWORD SafetyBytes;
    float tSine;
};

struct win32_debug_time_marker {
    DWORD OutputPlayCursor;
    DWORD OutputWriteCursor;
    DWORD OutputLocation;
    DWORD OutputByteCount;

    DWORD ExpectedFlipPlayCursor;
    DWORD FlipPlayCursor;
    DWORD FlipWriteCursor;
};

struct win32_game_code {
    HMODULE GameCodeDLL;
    FILETIME DLLLastWriteTime;
    game_update_and_render *UpdateAndRender;
    game_get_sound_samples *GetSoundSamples;

    bool32 IsValid;
};

#define WIN32_STATE_FILENAME_COUNT MAX_PATH

struct win32_replay_buffer {
    HANDLE FileHandle;
    HANDLE MemoryMap;
    char Filename[WIN32_STATE_FILENAME_COUNT];
    void *MemoryBlock;
};

struct win32_state {
    uint64_t TotalSize;
    void *GameMemoryBlock;
    win32_replay_buffer ReplayBuffers[4];

    HANDLE RecordingHandle;
    int InputRecordingIndex;

    HANDLE PlaybackHandle;
    int InputPlayingIndex;

    char EXEFilename[WIN32_STATE_FILENAME_COUNT];
    char *OnePastLastEXEFilenameSlash;
};
#endif
