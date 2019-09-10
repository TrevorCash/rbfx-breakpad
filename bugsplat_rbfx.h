#pragma once
#include "Urho3D/Core/Object.h"
#include "BugSplat.h"
#include <functional>

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

			if (IsDebuggerPresent())
			{
				wprintf(L"Run this application without the debugger attached to enable BugSplat exception handling.\n");
				DebugBreak();
				exit(0);
			}

			// BugSplat initialization.  Post crash reports to the "Fred" database for application "myConsoleCrasher" version "1.0"
			mpSender = new MiniDmpSender(L"Fred", L"myConsoleCrasher", L"1.0", NULL, MDSF_USEGUARDMEMORY | MDSF_LOGFILE | MDSF_PREVENTHIJACKING);

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








		}

		



	};




	// BugSplat exception callback
	URHOBUGSPLAT_API bool BugSplatExceptionCallback(UINT nCode, LPVOID lpVal1, LPVOID lpVal2)
	{

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
		}
		break;
		}

		return false;
	}




}
