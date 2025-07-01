//
// Basic instrumentation profiler by Cherno
// Video link: https://www.youtube.com/watch?v=xlAH4dbMVnU&list=PLlrATfBNZ98dudnM48yfGUldqGD0S4FFb&index=81

// Usage: include this header file somewhere in your code (eg. precompiled header), and then use like:
//
// Instrumentor::Get().BeginSession("Session Name");        // Begin session 
// {
//     InstrumentationTimer timer("Profiled Scope Name");   // Place code like this in scopes you'd like to include in profiling
//     // Code
// }
// Instrumentor::Get().EndSession();                        // End Session
//
// You will probably want to macro-fy this, to switch on/off easily and use things like __FUNCSIG__ for the profile name.
//
#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>

#include <iostream>
#include <ranges>
#include <stacktrace>
#include <unordered_map>

// TODO(danybeam) formatting
// TODO(danybeam) Add constructors for arrays and other stuff I saw in MTracker

// Known issues:
// - The profiling macro needs to be the first thing in the scope to make sure it gets freed last.
//  - IDK if there's any way around that

class InstrumentationMemory;

struct ProfileLock
{
    ProfileLock()
    {
        selfPointer = this;
        std::cout << "Locking profiler " << selfPointer << std::endl;
        saveProfiling++;
    };

    ~ProfileLock()
    {
        std::cout << "Unlocking profiler " << selfPointer << std::endl;
        saveProfiling--;
    };

    static bool getForceLock()
    {
        return forceLock;
    }

    static void requestForceLock()
    {
        forceLock = true;
    }

    static void requestForceUnlock()
    {
        forceLock = false;
    }

    static uint8_t getSaveProfiling()
    {
        return saveProfiling;
    }

private:
    void* selfPointer; // without this the destructor gets called at weird times
    static uint8_t saveProfiling;
    static bool forceLock;
};

struct ProfileResult_Time
{
    std::string Name;
    long long Start, End;
    uint32_t ThreadID;
};

struct ProfileResult_Memory
{
    void* location;
    size_t size;
    std::stacktrace StackTrace;
    long long Start, End = -1;
};

struct InstrumentationSession
{
    std::string Name;
};

class Instrumentor
{
private:
    InstrumentationSession* m_CurrentSession;
    InstrumentationMemory* m_CurrentMemoryCheck = nullptr;
    std::ofstream m_OutputStream;
    int m_ProfileCount_time;
    int m_ProfileCount_mem;

public:
    Instrumentor()
        : m_CurrentSession(nullptr), m_ProfileCount_time(0), m_ProfileCount_mem(0)
    {
    }

    void BeginSession(const std::string& name, const std::string& filepath = "results.json")
    {
        if (m_CurrentSession)
        {
            throw
                "There is already a profiling session running. Make sure you're not calling START_SESSION(name) more than once";
        }

        m_OutputStream.open(filepath);
        WriteHeader();
        m_CurrentSession = new InstrumentationSession{name};
    }

    void EndSession()
    {
        if (!m_CurrentSession)
        {
            throw "Closing a session was requested even though there's no sessions running.";
        }

        WriteFooter();
        m_OutputStream.close();
        delete m_CurrentSession;
        m_CurrentSession = nullptr;
        m_ProfileCount_time = 0;
    }

    void WriteProfile(const ProfileResult_Time& result)
    {
        if (m_ProfileCount_time++ > 0)
            m_OutputStream << ",";

        std::string name = result.Name;
        std::replace(name.begin(), name.end(), '"', '\'');

        m_OutputStream << "{";
        m_OutputStream << "\"cat\":\"function\",";
        m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
        m_OutputStream << "\"name\":\"" << name << "\",";
        m_OutputStream << "\"ph\":\"X\",";
        m_OutputStream << "\"pid\":0,";
        m_OutputStream << "\"tid\":" << result.ThreadID << ",";
        m_OutputStream << "\"ts\":" << result.Start;
        m_OutputStream << "}";

        m_OutputStream.flush();
    }

    void WriteProfile(const ProfileResult_Memory& result)
    {
        if (m_CurrentMemoryCheck == nullptr)
        {
            throw "A memory instrumentor tried to write before registering";
        }
        if (m_ProfileCount_mem++ > 0)
            m_OutputStream << ",";

        uint32_t threadID = static_cast<uint32_t>(std::hash<std::thread::id>{}(std::this_thread::get_id()));

        m_OutputStream << "{";
        m_OutputStream << "\"cat\":\"" << ((result.End >= 0) ? "Deallocated mem" : "Memory leaked") << "\",";
        m_OutputStream << "\"dur(ms)\":" << ((result.End >= 0) ? (result.End - result.Start) : -1) << ',';
        m_OutputStream << "\"name\":\"" << result.location << "\",";
        m_OutputStream << "\"tid\":" << threadID << ",";
        m_OutputStream << "\"tStart\":" << result.Start << ",";
        m_OutputStream << "\"tEnd\":" << result.End << ",";
        m_OutputStream << "\"size\":" << result.size << ",";
        m_OutputStream << "\"callStack\":[";
        for (int i = 0; i < result.StackTrace.size(); i++)
        {
            std::string stackTraceString = std::to_string(result.StackTrace[i]);
            std::replace(stackTraceString.begin(), stackTraceString.end(), '\\', '/');

            m_OutputStream << "\"";
            m_OutputStream << stackTraceString;
            m_OutputStream << "\"";
            if (i < result.StackTrace.size() - 1)
            {
                m_OutputStream << ",";
            }
        }
        m_OutputStream << "]";
        m_OutputStream << "}";

        m_OutputStream.flush();
    }

    void WriteHeader()
    {
        m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
        m_OutputStream.flush();
    }

    void WriteFooter()
    {
        m_OutputStream << "]}";
        m_OutputStream.flush();
    }

    static Instrumentor& Get()
    {
        static Instrumentor instance;
        return instance;
    }

    static void RegisterInstrumentation(InstrumentationMemory* instrumentation)
    {
        if (Instrumentor::Get().m_CurrentMemoryCheck)
        {
            throw "An instrumentation was already registered";
        }

        Instrumentor::Get().m_CurrentMemoryCheck = instrumentation;
    }

    static InstrumentationMemory* GetCurrentMemoryInstrumentation()
    {
        return Instrumentor::Get().m_CurrentMemoryCheck;
    }
};

class InstrumentationMemory
{
public:
    InstrumentationMemory(const char* name)
        : m_Name(name), m_Stopped(false)
    {
        Instrumentor::Get().RegisterInstrumentation(this);
    }

    ~InstrumentationMemory()
    {
        if (!m_Stopped)
            Stop();
    }

    void Stop()
    {
        ProfileLock lock;
        for (auto& ProfileResult : this->m_results | std::views::values)
        {
            Instrumentor::Get().WriteProfile(ProfileResult);
        }

        std::cout << "Profiling stopped\n";
        m_Stopped = true;
    }

    void register_push(void* address, size_t size)
    {
        if (m_Stopped) return;

        m_results[address] = {
            .location = address,
            .size = size,
            .StackTrace = std::stacktrace::current(),
            .Start = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).
                     time_since_epoch().
                     count()
        };
    }

    void register_pop(void* address)
    {
        if (m_Stopped) return;

        auto findResult = m_results.find(address);
        if (findResult != m_results.end())
        {
            findResult->second.End = std::chrono::time_point_cast<std::chrono::microseconds>(
                                         std::chrono::high_resolution_clock::now()).
                                     time_since_epoch().
                                     count();
        }
    }

private:
    const char* m_Name;
    std::unordered_map<void*, ProfileResult_Memory> m_results;
    bool m_Stopped;
};

class InstrumentationTimer
{
public:
    InstrumentationTimer(const char* name)
        : m_Name(name), m_Stopped(false)
    {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~InstrumentationTimer()
    {
        if (!m_Stopped)
            Stop();
    }

    void Stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().
            count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().
            count();

        uint32_t threadID = static_cast<uint32_t>(std::hash<std::thread::id>{}(std::this_thread::get_id()));
        Instrumentor::Get().WriteProfile({m_Name, start, end, threadID});

        m_Stopped = true;
    }

private:
    const char* m_Name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
    bool m_Stopped;
};


inline void* operator new(size_t size)
{
    void* ptr = malloc(size);
    if (ProfileLock::getSaveProfiling() == 0 && !ProfileLock::getForceLock())
    {
        ProfileLock lock;
        if (auto MemoryInstrumentation = Instrumentor::GetCurrentMemoryInstrumentation())
            // Because there's no guarantee there will already be an instrumentator active
            MemoryInstrumentation->register_push(ptr, size);
        std::cout << "Allocated memory: " << size << " at " << ptr << std::endl;
    }
    return ptr;
}

inline void operator delete(void* memory)
{
    if (ProfileLock::getSaveProfiling() == 0 && !ProfileLock::getForceLock())
    {
        ProfileLock lock;
        if (auto MemoryInstrumentation = Instrumentor::GetCurrentMemoryInstrumentation())
            MemoryInstrumentation->register_pop(memory);
        std::cout << "deleted " << memory << std::endl;
    }

    free(memory);
}

// The "if" preprocessor command and the macro commands are a mix of Cherno and danybeam (me)
// Regardless of the copyright notice on modified versions of the code in the code the section bellow should be considered under the MIT license.
#if PROFILE
#define PROFILE_SCOPE_MEMORY(name) ProfileLock::requestForceLock();InstrumentationMemory memoryProfiler##__LINE__(name);ProfileLock::requestForceUnlock();
#define PROFILE_SCOPE_TIME(name) InstrumentationTimer timer##__LINE__(name)
#define PROFILE_FUNCTION_TIME() PROFILE_SCOPE(__FUNCSIG__)
#define START_SESSION(name)  Instrumentor::Get().BeginSession(name)
#define END_SESSION()  Instrumentor::Get().EndSession()
#else
#define PROFILE_SCOPE_MEMORY(name)
#define PROFILE_SCOPE_TIME(name)
#define PROFILE_FUNCTION_TIME()
#define START_SESSION(name)
#define END_SESSION()
#endif
