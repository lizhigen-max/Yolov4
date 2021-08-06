#include "YoGlobal.h"

#include <string>
#include <algorithm>
#include <stdarg.h>
#include <locale>
#include <codecvt>

#ifdef _WINDOWS
#include <windows.h>
#include <tlhelp32.h>
#include <objbase.h>
#include <io.h>
#include <TlHelp32.h>
#else
#include <pthread.h>
#include <uuid/uuid.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <dirent.h>
#endif

#ifdef __APPLE__
#include <mach-o/dyld.h>

#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#ifdef _WINDOWS
#define sprintf sprintf_s
#define fopen fopen_s
#endif

#ifdef _WINDOWS
GUID CreateGuid_()
{
	GUID guid;
	CoCreateGuid(&guid);
	return guid;
}

std::string GuidToString_(const GUID& guid)
{
	char buf[64] = { 0 };
	_snprintf_s(buf, sizeof(buf), "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}", guid.Data1, guid.Data2,
		guid.Data3, guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);
	return std::string(buf);
}
#endif

std::string YoGlobal::CreateGuid()
{
#ifdef _WINDOWS
	GUID guid = CreateGuid_();
	return GuidToString_(guid);
#else
	uuid_t uu;
	uuid_generate(uu);
	char str[36];
	uuid_unparse(uu, str);
	std::string uuid = str;

	return uuid;
#endif
}

std::string YoGlobal::GetWorkDirectory()
{
	std::string path = "";
#ifdef _WINDOWS
	char file_path[1024];
	::GetModuleFileNameA(NULL, file_path, sizeof(file_path));  // 得到当前执行文件的文件名(包含路径)
	path = file_path;
	int pos = path.rfind("\\");
	if (pos > 0) {
		path = path.substr(0, pos);
	}
#else
	char file_path[260];
	getcwd(file_path, sizeof(file_path));
	path = file_path;
#endif
	Replace(path, "\\", "/", 1);
	return path;
}

std::string YoGlobal::GetPathIncludeSlash(const std::string& fullpath)
{
	if (fullpath.length() == 0)
		return fullpath;
	std::string path = fullpath;
	Replace(path, "\\", "/", 1);

	if (path[path.length() - 1] != '/') {
		path = path + "/";
	}
	return path;
}

std::string YoGlobal::GetPathUnIncludeSlash(const std::string& fullpath)
{
	if (fullpath.length() == 0)
		return fullpath;
	std::string path = fullpath;
	Replace(path, "\\", "/", 1);

	if (path[path.length() - 1] == '/') {
		std::string s;
		s.assign(path.c_str(), path.length() - 1);
		return s;
	}
	return path;
}

std::string YoGlobal::GetProcessName()
{
	std::string proName = "";
#if defined(_WINDOWS)
	char file_path[1024];
	::GetModuleFileNameA(NULL, file_path, sizeof(file_path));  // 得到当前执行文件的文件名(包含路径)
	proName = file_path;
	int pos = proName.rfind("\\");
	if (pos > 0) {
		proName = proName.substr(pos + 1, proName.size() - pos - 1);
		pos = proName.find(".exe");
		if (pos > 0) {
			proName = proName.substr(0, pos);
		}
	}
#elif defined(__APPLE__)
	char buf[0];
	uint32_t size = 0;
	int res = _NSGetExecutablePath(buf, &size);

	char* path = (char*)malloc(size + 1);
	path[size] = 0;
	res = _NSGetExecutablePath(path, &size);

	char* strProcessName = basename(path);
	proName.assign(strProcessName);
#else
	char strProcessPath[1024] = { 0 };

	if (readlink("/proc/self/exe", strProcessPath, 1024) <= 0) {
		return "";
	}

	char* strProcessName = strrchr(strProcessPath, '/');

	if (!strProcessName) {
		return "";
	}
	else {
		proName.assign(++strProcessName);
	}
#endif

	return proName;
}

std::string YoGlobal::GetFileExt(const char* path)
{
	char* n = basename(path);
	const char* s;
	const char* p;

	p = s = n;

	while (*s) {
		if (*s++ == '.') {
			p = s;
		}
	}
	std::string result((char*)p);
	return result;
}

char* YoGlobal::basename(const char* path)
{
	const char* s;
	const char* p;
	char c;
	p = s = path;

	while (*s) {
		c = *s++;
		if (c == '/' || c == '\\') {
			p = s;
		}
	}
	return (char*)p;
}

int YoGlobal::GetPid()
{
	int pid = -9999;
#ifdef _WINDOWS
	pid = GetCurrentProcessId();
#else
	pid = getpid();
#endif

	return pid;
}

void YoGlobal::Split(const std::string& src, const std::string& split, std::vector<std::string>& results)
{
	std::string tSrc = src;
	while (tSrc.length() > 0) {
		int pos = tSrc.find(split.c_str());
		if (pos != std::string::npos) {
			std::string str = tSrc.substr(0, pos);
			results.push_back(str);

			tSrc = tSrc.substr(pos + split.size(), tSrc.length() - pos - split.size());
		}
		else {
			if (!tSrc.empty())
				results.push_back(tSrc);

			break;
		}
	}
}

std::wstring YoGlobal::StringToWString(const std::string& str)
{
	size_t convertedChars = 0;
	std::string curLocale = setlocale(LC_ALL, NULL);  // curLocale="C"
	setlocale(LC_ALL, "chs");
	const char* source = str.c_str();
	size_t charNum = sizeof(char) * str.size() + 1;
	wchar_t* dest = new wchar_t[charNum];
#ifdef _WINDOWS
	mbstowcs_s(&convertedChars, dest, charNum, source, _TRUNCATE);
#else
	mbstowcs(dest, source, str.size());
#endif
	std::wstring result = dest;
	delete[] dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}

std::string YoGlobal::WStringToString(const std::wstring& wstr)
{
	std::string curLocale = setlocale(LC_ALL, NULL);  // curLocale = "C";

	setlocale(LC_ALL, "chs");

	const wchar_t* source = wstr.c_str();
	size_t size = 2 * wstr.size() + 1;
	char* dest = new char[size];
	memset(dest, 0, size);
#ifdef _WINDOWS
	size_t convertedChars = 0;
	wcstombs_s(&convertedChars, dest, size, source, _TRUNCATE);
#else
	wcstombs(dest, source, size);
#endif
	std::string result = dest;
	delete[] dest;

	setlocale(LC_ALL, curLocale.c_str());

	return result;
}

std::string YoGlobal::UnicodeToUTF8(const std::wstring& wstr)
{
	std::string ret;
	try {
		std::wstring_convert<std::codecvt_utf8<wchar_t>> wcv;
		ret = wcv.to_bytes(wstr);
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		throw;
	}
	return ret;
}

std::wstring YoGlobal::UTF8ToUnicode(const std::string& str)
{
	std::wstring ret;
	try {
		std::wstring_convert<std::codecvt_utf8<wchar_t>> wcv;
		ret = wcv.from_bytes(str);
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		throw;
	}
	return ret;
}

std::string YoGlobal::UnicodeToANSI(const std::wstring& wstr)
{
	return WStringToString(wstr);
}

std::wstring YoGlobal::ANSIToUnicode(const std::string& str)
{
	return StringToWString(str);
}

std::string YoGlobal::UTF8ToANSI(const std::string& str)
{
	return UnicodeToANSI(UTF8ToUnicode(str));
}

std::string YoGlobal::ANSIToUTF8(const std::string& str)
{
	return UnicodeToUTF8(ANSIToUnicode(str));
}

// mode:替换模式，0：只替换第一个找到的。1：全部替换找到的。
int YoGlobal::Replace(std::string& str, const std::string& pattern, const std::string& newpat, int mode)
{
	int count = 0;
	const size_t nsize = newpat.size();
	const size_t psize = pattern.size();

	for (size_t pos = str.find(pattern, 0); pos != std::string::npos; pos = str.find(pattern, pos + nsize)) {
		str.replace(pos, psize, newpat);
		count++;

		if (mode == 0)
			break;
	}

	return count;
}

void YoGlobal::ReplaceToESC(std::string& str)
{
	Replace(str, "\\0", "\0", 1);
	Replace(str, "\\n", "\n", 1);
	Replace(str, "\\r", "\r", 1);
	Replace(str, "\\t", "\t", 1);
	Replace(str, "\\v", "\v", 1);
	Replace(str, "\\a", "\a", 1);
	Replace(str, "\\b", "\b", 1);
	Replace(str, "\\f", "\f", 1);
	Replace(str, "\\'", "\'", 1);
	Replace(str, "\\\"", "\"", 1);
	Replace(str, "\\\\", "\\", 1);
	Replace(str, "\\?", "\?", 1);
}

int YoGlobal::MakeNDir(const std::string& fullPath)
{
	std::string path = fullPath;
	Replace(path, "\\", "/", 1);
	//if (fullPath.rfind('.') != std::string::npos) {
	//	int pos = fullPath.rfind("/");
	//	if (pos != std::string::npos) {
	//		path = fullPath.substr(0, pos);
	//	}
	//}
#ifdef _WINDOWS
	std::string strCompletePath = "";
	std::vector<std::string> vecPath;
	Split(path, "/", vecPath);

	std::vector<std::string>::iterator iter = vecPath.begin();
	for (; iter != vecPath.end(); ++iter) {
		strCompletePath += *iter + "/";
		if (-1 == _access(strCompletePath.c_str(), 0)) {
			if (!CreateDirectoryA((strCompletePath.c_str()), NULL)) {
				return 1;
			}
		}
	}
#else
	std::string strCmd;
	strCmd = "mkdir -p ";
	strCmd += fullPath;
	if (-1 == system(strCmd.c_str())) {
		return 1;
	}
#endif
	return 0;
}

bool YoGlobal::IsFileExist(const std::string& fullPath)
{
#ifdef _WINDOWS
	if (-1 == _access(fullPath.c_str(), 0))
		return false;
#else
	struct stat statDir;
	if (0 != stat(fullPath.c_str(), &statDir)) {
		return false;
	}

	if (!S_ISREG(statDir.st_mode)) {
		return false;
	}
#endif

	return true;
}

bool YoGlobal::IsDirExist(const std::string& fullPath)
{
#ifdef _WINDOWS
	if (-1 == _access(fullPath.c_str(), 0))
		return false;
#else
	struct stat statDir;
	if (0 != stat(fullPath.c_str(), &statDir)) {
		return false;
	}

	if (!S_ISDIR(statDir.st_mode)) {
		return false;
	}
#endif

	return true;
}

std::string YoGlobal::GetFileName(const std::string& fullPath)
{
	std::string path = fullPath;
	std::string na = fullPath;
	Replace(path, "\\", "/", 1);

	int pos = path.find_last_of("/");
	if (pos != std::string::npos) {
		na = path.substr(pos + 1, path.size() - pos - 1);
	}

	return na;
}

std::string YoGlobal::GetFileNameWithOutExt(const std::string& fullPath)
{
	std::string filename = GetFileName(fullPath);
	std::string na = filename;
	int pos = filename.find_last_of(".");
	if (pos != std::string::npos) {
		na = filename.substr(0, pos);
	}
	return na;
}

std::string YoGlobal::GetFilePath(const std::string& fullPath)
{
	std::string path = fullPath;
	std::string na = fullPath;
	Replace(path, "\\", "/", 1);

	int pos = path.find_last_of("/");
	if (pos != std::string::npos) {
		na = path.substr(0, pos);
	}

	return na;
}

std::string YoGlobal::GetTime()
{
	std::string strNowTime;
#ifdef _WINDOWS
	char szTemp[24];
	SYSTEMTIME nowTime;
	GetLocalTime(&nowTime);
	sprintf(szTemp, "%d-%02d-%02d %02d:%02d:%02d.%03d", nowTime.wYear, nowTime.wMonth, nowTime.wDay, nowTime.wHour,
		nowTime.wMinute, nowTime.wSecond, nowTime.wMilliseconds);
	strNowTime = szTemp;
#else
	struct timeval tmVal;
	tm* pTm;
	time_t tmSecond;
	time_t tmMicroSecond;

	if (0 == gettimeofday(&tmVal, NULL)) {
		tmSecond = tmVal.tv_sec;
		tmMicroSecond = tmVal.tv_usec;
	}
	else {
		tmSecond = time(NULL);
		tmMicroSecond = 0;
	}

	pTm = localtime(&tmSecond);
	char szTemp[24];
	sprintf(szTemp, "%d-%02d-%02d %02d:%02d:%02d.%03d", pTm->tm_year + 1900, pTm->tm_mon + 1, pTm->tm_mday,
		pTm->tm_hour, pTm->tm_min, pTm->tm_sec, (int)tmMicroSecond / 1000);

	strNowTime = szTemp;
#endif
	return strNowTime;
}

int YoGlobal::GetPidByPort(int port)
{
#ifdef _WINDOWS
	std::string output;
	char szCommand[1024];
	sprintf(szCommand, "netstat -ano | findstr %d | findstr LISTENING", port);
	int ret = ExecCommand(szCommand, output);
	if (ret == -1)
		return -1;

	Trim(output, "\r\n ", 3);
	if (output.size() > 0) {
		int pos = output.find("LISTENING");
		if (pos == std::string::npos) {
			return -1;
		}
		std::string sport = output.substr(pos + strlen("LISTENING"), output.size());
		int res = atoi(sport.c_str());
		return res;
	}

	return -1;

#else
	std::string output;
	char szCommand[1024];
	sprintf(szCommand, "lsof -i:%d| grep 'LISTEN' | awk '{print $2}'", port);
	int ret = ExecCommand(szCommand, output);
	if (ret == -1)
		return -1;

	Trim(output, "\r\n", 2);
	if (output.size() > 0) {
		int res = atoi(output.c_str());
		return res;
	}

	return -1;
#endif
}

bool YoGlobal::KillProcess(int pid)
{
#ifdef _WINDOWS
	HANDLE hd = ::OpenProcess(PROCESS_TERMINATE, false, pid);
	if (hd == NULL)
		return false;
	bool flag = ::TerminateProcess(hd, 0);
	return flag;

#else
	std::string output;
	char szCommand[1024];
	sprintf(szCommand, "kill %d", pid);
	int ret = ExecCommand(szCommand, output);
	if (ret == -1)
		return false;

	Trim(output, "\r\n", 2);
	if (output.size() > 0) {
		return false;
	}

	return true;
#endif
}

int YoGlobal::GetFileLength(FILE* fp)
{
	int ret = fseek(fp, 0L, SEEK_END);
	if (ret != 0) {
		return -1;
	}

	int length = ftell(fp);
	if (length == 0) {
		return 0;
	}

	ret = fseek(fp, 0L, SEEK_SET);
	if (ret != 0) {
		return -1;
	}

	return length;
}

int YoGlobal::ExecCommand(const std::string& cmd, std::string& output)
{
	printf("ExecCommand cmd=%s\n", cmd.c_str());
#ifdef _WINDOWS
	SECURITY_ATTRIBUTES sa;
	HANDLE hRead = NULL;
	HANDLE hWrite = NULL;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
		printf("CreatePipe failed.\n");
		return -1;
	}

	char command[2048];
	strcpy(command, "cmd.exe /C ");
	strcat(command, cmd.c_str());
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfoA(&si);
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	if (!CreateProcessA(NULL, command, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi)) {
		CloseHandle(hWrite);
		CloseHandle(hRead);
		printf("CreateProcessA failed.cmd=%s\n", command);
		return -1;
	}

	CloseHandle(hWrite);
	char buffer[4096] = { 0 };
	DWORD bytesRead;
	while (true) {
		if (ReadFile(hRead, buffer, 4095, &bytesRead, NULL) == NULL)
			break;
		output.append(buffer, bytesRead);
	}

	CloseHandle(hRead);
	printf("exec command success.output:%s\n", output.c_str());
	return 0;
#else
	FILE* fp = NULL;
	int ch = 0, iIndex = 0;

	if (NULL == (fp = popen(cmd.c_str(), "r"))) {
		pclose(fp);
		return -1;
	}

	while ((ch = fgetc(fp)) != EOF) {
		char buf[1] = { 0 };
		buf[0] = ch;
		output.append(buf, 1);
	}

	pclose(fp);
	printf("exec command success.output:%s\n", output.c_str());
	return 0;
#endif
}

std::string YoGlobal::Format(const char* format, ...)
{
	char szBuf[8192];
	va_list arglist;
	va_start(arglist, format);
#ifdef _WINDOWS
	vsprintf_s(szBuf, format, arglist);
#else
	vsprintf(szBuf, format, arglist);
#endif
	va_end(arglist);

	return szBuf;
}

std::vector<unsigned long> YoGlobal::KillProcessByName(const std::string& procName)
{
	std::vector<unsigned long> pidList;
#if defined(__APPLE__)
	char szCommand[256] = { 0 };
	char* pname = YoGlobal::basename(procName.c_str());
	sprintf(szCommand, "kill -9 $(ps -ef|grep %s |awk '$0 !~/grep/ {print $2}' |tr -s '\n' ' ')", pname);
	system(szCommand);
#else
	pidList = YoGlobal::GetProcessByName(procName);
	for (unsigned long pid : pidList) {
		YoGlobal::KillProcess(pid);
	}

#endif
	return pidList;
}

std::vector<unsigned long> YoGlobal::GetProcessByName(const std::string& procName)
{
	std::vector<unsigned long> pidList;
#if defined(_WINDOWS)
	HANDLE handle = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (handle != NULL) {
		// PROCESSENTRY32 pe32 = { 0 };
		tagPROCESSENTRY32W pe32 = { 0 };
		pe32.dwSize = sizeof(pe32);
		BOOL more = ::Process32FirstW(handle, &pe32);
		while (more) {
			std::string str = WStringToString(pe32.szExeFile);
			if (_stricmp(procName.c_str(), str.c_str()) == 0
				|| _stricmp((procName + ".exe").c_str(), str.c_str()) == 0) {
				pidList.push_back(pe32.th32ProcessID);
			}
			more = ::Process32NextW(handle, &pe32);
		}

		::CloseHandle(handle);
	}
#elif defined(__APPLE__)
	return pidList;
#else

#ifndef MAX_BUF_SIZE
#define MAX_BUF_SIZE 256
#endif
	DIR* dir;
	struct dirent* next;
	pid_t pid;
	FILE* fp;
	char* base_pname = NULL;
	char* base_fname = NULL;
	char cmdline[MAX_BUF_SIZE];
	char path[MAX_BUF_SIZE];

	if (procName.empty())
		return pidList;

	base_pname = YoGlobal::basename(procName.c_str());
	if (strlen(base_pname) <= 0)
		return pidList;

	dir = opendir("/proc");
	if (!dir) {
		return pidList;
	}
	while ((next = readdir(dir)) != NULL) {
		/* skip non-number */
		if (!isdigit(*next->d_name))
			continue;

		pid = strtol(next->d_name, NULL, 0);
		sprintf(path, "/proc/%u/cmdline", pid);
		fp = fopen(path, "r");
		if (fp == NULL)
			continue;

		memset(cmdline, 0, sizeof(cmdline));
		if (fread(cmdline, MAX_BUF_SIZE - 1, 1, fp) < 0) {
			fclose(fp);
			continue;
		}
		fclose(fp);
		base_fname = YoGlobal::basename(cmdline);

		if (strcmp(base_fname, base_pname) == 0) {
			pidList.push_back(pid);
		}
	}
	closedir(dir);
#endif
	return pidList;
}

void YoGlobal::SleepForMilliSeconds(int milliseconds)
{
#ifdef _WINDOWS
	Sleep(milliseconds);
#else
	usleep(milliseconds * 1000);  // takes microseconds
#endif
}

ConsoleQuitHandlerFun ConsoleHandler = NULL;
#ifdef _WINDOWS
BOOL CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType) {
	case CTRL_C_EVENT:
		printf("CTRL_C_EVENT \n");
		if (ConsoleHandler != NULL) {
			ConsoleHandler();
		}
		break;
	case CTRL_CLOSE_EVENT:
		printf("CTRL_CLOSE_EVENT \n");
		if (ConsoleHandler != NULL) {
			ConsoleHandler();
		}
		break;
	case CTRL_SHUTDOWN_EVENT:
		printf("CTRL_SHUTDOWN_EVENT \n");
		if (ConsoleHandler != NULL) {
			ConsoleHandler();
		}
		break;
	default:
		return FALSE;
	}

	return (TRUE);
}
#else
void SigProcss(int sig)
{
	if (SIGINT == sig) {
		printf("signal SIGINT\n");
		if (ConsoleHandler != NULL) {
			ConsoleHandler();
		}
	}
	else if (SIGQUIT == sig) {
		printf("signal SIGQUIT\n");
		if (ConsoleHandler != NULL) {
			ConsoleHandler();
		}
	}
}
#endif

void YoGlobal::SetupConsoleQuitHandler(ConsoleQuitHandlerFun handler)
{
	ConsoleHandler = handler;
#ifdef _WINDOWS
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);
#else
	signal(SIGINT, SigProcss);
	signal(SIGQUIT, SigProcss);
#endif
}

void YoGlobal::Trim(std::string& src, const char* chs, size_t len)
{
	if (chs == NULL) {
		return;
	}

	std::string buf;
	size_t size = src.size();
	for (size_t i = 0; i < size; ++i) {
		size_t j = 0;
		for (; j < len; ++j) {
			if (src[i] == chs[j]) {
				break;
			}
		}
		if (j == len) {
			char sz[1] = { 0 };
			sz[0] = src[i];
			buf.append(sz, 1);
		}
	}

	src = buf;
}

void YoGlobal::KillThread(std::thread* thread)
{
#ifdef _WINDOWS
	void* handle = thread->native_handle();
	TerminateThread(handle, 1);
#else
	pthread_t tid = thread->native_handle();
	pthread_cancel(tid);
#endif
}

std::string YoGlobal::GetDllExt()
{
	std::string ext;
#ifdef _WINDOWS
	ext = ".dll";
#else
	ext = ".dylib";
#endif
	return ext;
}

std::string YoGlobal::GetIconFullPathName(const std::string& icon)
{
	std::string fullpathname = YoGlobal::GetWorkDirectory() + "/" + icon;
	if (YoGlobal::IsFileExist(fullpathname))
		fullpathname = "";
	return fullpathname;
}

bool YoGlobal::StrCompare(const std::string& str1, const std::string& str2)
{
	std::string s1, s2;
	s1 = str1;
	s2 = str2;
	std::transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
	std::transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
	return s1 == s2;
}

std::string YoGlobal::strToHex(const std::string& convert_data)
{
	std::string outStr;
	for (unsigned int i = 0; i < convert_data.length(); i += 2) {
		std::string str = convert_data.substr(i, 2);
		outStr.push_back(std::stoi(str, 0, 16));
	}
	return outStr;
}

std::string YoGlobal::hexToStr(const std::string& convert_data)
{
	std::string outStr;
	char buf[6];
	memset(buf, 0, sizeof(buf));

	for (unsigned int i = 0; i < convert_data.size(); i++) {
		snprintf(buf, sizeof(buf), "0x%02x ", (uint8_t)convert_data[i]);
		outStr.append(buf);
	}
	return outStr;
}
