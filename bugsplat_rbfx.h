#pragma once
#include "Urho3D/Core/Object.h"
#include "BugSplat.h"
#include <functional>
#include "Urho3D/IO/Log.h"

#define URHOBUGSPLAT_EXPORT_API __declspec(dllexport)


#ifdef URHOBUGSPLAT_STATIC
#  define URHOBUGSPLAT_API
#  define URHOBUGSPLAT_NO_EXPORT
#else
#  ifndef URHOBUGSPLAT_API
#    ifdef URHOBUGSPLAT_EXPORTS
/* We are building this library */
#      define URHOBUGSPLAT_API URHOBUGSPLAT_EXPORT_API
#    else
/* We are using this library */
#      define URHOBUGSPLAT_API __declspec(dllimport)
#    endif
#  endif

#  ifndef URHOBUGSPLAT_NO_EXPORT
#    define URHOBUGSPLAT_NO_EXPORT 
#  endif
#endif


namespace Urho3D {


	URHOBUGSPLAT_API bool BugSplatExceptionCallback(UINT nCode, LPVOID lpVal1, LPVOID lpVal2);
	MiniDmpSender* mpSender;

	URHOBUGSPLAT_API class BugReportingSystem : public Object {
		URHO3D_OBJECT(BugReportingSystem, Object);
	public:
		BugReportingSystem(Context* context) : Object(context) {}
		virtual ~BugReportingSystem() {}
		void Initialize() {

			//if (IsDebuggerPresent())
			//{
			//	wprintf(L"Run this application without the debugger attached to enable BugSplat exception handling.\n");
			//	/*DebugBreak();
			//	exit(0);*/
			//}
			//else
			//{
				// BugSplat initialization.  Post crash reports to the "Fred" database for application "myConsoleCrasher" version "1.0"
				mpSender = new MiniDmpSender(L"MechanismGame", L"windowsnative", L"0.0.2", NULL, MDSF_USEGUARDMEMORY | MDSF_LOGFILE | MDSF_PREVENTHIJACKING);

				// The following calls add support for collecting crashes for abort(), vectored exceptions, out of memory,
				// pure virtual function calls, and for invalid parameters for OS functions.
				// These calls should be used for each module that links with a separate copy of the CRT.
				SetGlobalCRTExceptionBehavior();
				SetPerThreadCRTExceptionBehavior();  // This call needed in each thread of your app

				// A guard buffer of 20mb is needed to catch OutOfMemory crashes
				mpSender->setGuardByteBufferSize(20 * 1024 * 1024);

				// Set optional default values for user, email, and user description of the crash.
				mpSender->setDefaultUserName(L"Trevor");
				mpSender->setDefaultUserEmail(L"cashtrevor@msn.com");
				mpSender->setDefaultUserDescription(L"This is the default user crash description.");

				//auto bind = std::bind(&BugReportingSystem::BugSplatExceptionCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
				//std::function<bool(UINT, LPVOID, LPVOID)> yourFunction = bind;
				mpSender->setCallback(BugSplatExceptionCallback); // files are attached in the callback after the exception occurs
			//}

		}


	};


	void MemoryException()
	{
		// Dereferencing a null pointer results in a memory exception
		wprintf(L"MemoryException!\n");
		*(volatile int *)0 = 0;
	}

	void DivideByZero()
	{
		// Calling a recursive function with no exit results in a stack overflow
		wprintf(L"DivideByZero!\n");
		volatile int x, y, z;
		x = 1;
		y = 0;
		z = x / y;
	}

	void StackOverflow(void *p)
	{
		// Calling a recursive function with no exit results in a stack overflow
		wprintf(L"StackOverflow!\n");
		volatile char q[10000];
		while (true) {
			StackOverflow((void *)q);
		}
	}

	void ExhaustMemory()
	{
		wprintf(L"ExhaustMemory!\n");

		// Loop until memory exhausted
		while (true)
		{
			char* a = new char[1024 * 1024];
			a[0] = 'X';
		}
	}

	void ThrowByUser()
	{
		wprintf(L"Throw user generated exception!\n");
		throw("User generated exception!");
	}

	DWORD WINAPI MyThreadCrasher(LPVOID)
	{
		wprintf(L"MyThreadCrasher!\n");
		MemoryException();
		return 0;
	}

	void ThreadException()
	{
#define MAX_THREADS 1
		DWORD   dwThreadIdArray[MAX_THREADS];
		HANDLE  hThreadArray[MAX_THREADS];

		// Create MAX_THREADS worker threads.
		for (int i = 0; i < MAX_THREADS; i++)
		{
			// Create the thread to begin execution on its own.
			hThreadArray[i] = CreateThread(
				NULL,                   // default security attributes
				0,                      // use default stack size  
				MyThreadCrasher,        // thread function name
				NULL,					// argument to thread function 
				0,                      // use default creation flags 
				&dwThreadIdArray[i]);   // returns the thread identifier 

			if (hThreadArray[i] == NULL)
			{
				wprintf(L"CreateThread failed");
				ExitProcess(3);
			}
		} // End of main thread creation loop.

		// Wait until all threads have terminated.
		WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);
	}


	void CreateReport()
	{
		const __wchar_t  *xml = L"<report><process>"
			"<exception>"
			"<code>FATAL ERROR</code>"
			"<explanation>This is an error code explanation</explanation>"
			"<func>myConsoleCrasher!MemoryException</func>"
			"<file>/www/bugsplatAutomation/myConsoleCrasher/myConsoleCrasher.cpp</file>"
			"<line>143</line>"
			"<registers>"
			"<cs>0023</cs>"
			"<ds>002b</ds>"
			"<eax>00000011</eax>"
			"<ebp>00affb58</ebp>"
			"<ebx>00858000</ebx>"
			"<ecx>43bf1e0e</ecx>"
			"<edi>00affb58</edi>"
			"<edx>014480b4</edx>"
			"<efl>00010202</efl>"
			"</registers>"
			"</exception>"
			"<modules numloaded=\"2\">"
			"<module>"
			"<name>myConsoleCrasher</name>"
			"<order>1</order>"
			"<address>01320000-01457000</address>"
			"<path>C:/www/BugsplatAutomation/BugsplatAutomation/bin/x64/Release/temp/BugSplat/bin/myConsoleCrasher.exe</path>"
			"<symbolsloaded>deferred</symbolsloaded>"
			"<fileversion/>"
			"<productversion/>"
			"<checksum>00000000</checksum>"
			"<timedatestamp>SatJun1501:18:092019</timedatestamp>"
			"</module>"
			"<module>"
			"<name>BugSplatRc</name>"
			"<order>2</order>"
			"<address>01320000-01457000</address>"
			"<path>C:/www/BugsplatAutomation/BugsplatAutomation/bin/x64/Release/BugSplatRc.dll</path>"
			"<symbolsloaded>deferred</symbolsloaded>"
			"<fileversion/>"
			"<productversion/>"
			"<checksum>00000000</checksum>"
			"<timedatestamp>SatJun1501:18:092019</timedatestamp>"
			"</module>"
			"</modules>"
			"<threads count=\"2\">"
			"<thread id=\"0\" current=\"yes\" event=\"yes\" framecount=\"3\">"
			"<frame>"
			"<symbol>myConsoleCrasher!MemoryException</symbol>"
			"<file>/www/bugsplatAutomation/myConsoleCrasher/myConsoleCrasher.cpp</file>"
			"<line>143</line>"
			"<offset>0x35</offset>"
			"</frame>"
			"<frame>"
			"<symbol>myConsoleCrasher!wmain</symbol>"
			"<file>C:/www/BugsplatAutomation/BugsplatAutomation/BugSplat/samples/myConsoleCrasher/myConsoleCrasher.cpp</file>"
			"<line>83</line>"
			"<offset>0x239</offset>"
			"</frame>"
			"<frame>"
			"<symbol>myConsoleCrasher!__scrt_wide_environment_policy::initialize_environment</symbol>"
			"<file>d:/agent/_work/4/s/src/vctools/crt/vcstartup/src/startup/exe_common.inl</file>"
			"<line>90</line>"
			"<offset>0x43</offset>"
			"</frame>"
			"</thread>"
			"<thread id=\"1\" current=\"no\" event=\"no\" framecount=\"3\">"
			"<frame>"
			"<symbol>my2ConsoleCrasher!MemoryException</symbol>"
			"<file>/www/bugsplatAutomation/myConsoleCrasher/myConsoleCrasher.cpp</file>"
			"<line>143</line>"
			"<offset>0x35</offset>"
			"</frame>"
			"<frame>"
			"<symbol>my2ConsoleCrasher!wmain</symbol>"
			"<file>C:/www/BugsplatAutomation/BugsplatAutomation/BugSplat/samples/myConsoleCrasher/myConsoleCrasher.cpp</file>"
			"<line>83</line>"
			"<offset>0x239</offset>"
			"</frame>"
			"<frame>"
			"<symbol>my2ConsoleCrasher!__scrt_wide_environment_policy::initialize_environment</symbol>"
			"<file>d:/agent/_work/4/s/src/vctools/crt/vcstartup/src/startup/exe_common.inl</file>"
			"<line>90</line>"
			"<offset>0x43</offset>"
			"</frame>"
			"</thread>"
			"</threads></process></report>";

		mpSender->createReport(xml);
	}




	// BugSplat exception callback
	URHOBUGSPLAT_API bool BugSplatExceptionCallback(UINT nCode, LPVOID lpVal1, LPVOID lpVal2)
	{
		URHO3D_LOGINFO("BugSplatExceptionCallback.." + ea::to_string(nCode));
		switch (nCode)
		{
		case MDSCB_EXCEPTIONCODE:
		{
			EXCEPTION_RECORD *p = (EXCEPTION_RECORD *)lpVal1;
			DWORD code = p ? p->ExceptionCode : 0;

			// Create some files in the %temp% directory and attach them
			wchar_t cmdString[2 * MAX_PATH];
			wchar_t filePath[MAX_PATH];
			wchar_t tempPath[MAX_PATH];
			GetTempPathW(MAX_PATH, tempPath);

			wsprintf(LPSTR(filePath), LPSTR(L"%sfile1.txt"), LPSTR(tempPath));
			wsprintf(LPSTR(cmdString), LPSTR(L"echo Exception Code = 0x%08x > %s"), code, filePath);
			_wsystem(cmdString);
			mpSender->sendAdditionalFile(filePath);

			wsprintf(LPSTR(filePath), LPSTR(L"%sfile2.txt"), tempPath);

			wchar_t buf[_MAX_PATH];
			mpSender->getMinidumpPath(buf, _MAX_PATH);

			wsprintf(LPSTR(cmdString), LPSTR(L"echo Crash reporting is so clutch! minidump path = %s > %s"), buf, filePath);
			_wsystem(cmdString);
			mpSender->sendAdditionalFile(filePath);

			URHO3D_LOGINFO("end of routine.." );
		}
		break;
		}

		return false;
	}




}
