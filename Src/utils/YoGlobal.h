#ifndef YOGLOBAL_H
#define YOGLOBAL_H

#include <list>
#include <string>
#include <thread>
#include <vector>

typedef void(*ConsoleQuitHandlerFun)();

class YoGlobal
{
public:
	static std::string GetPathIncludeSlash(const std::string& fullpath);

	static std::string GetPathUnIncludeSlash(const std::string& fullpath);

	static std::string GetWorkDirectory();  //获取当前运行程序目录

	static std::string GetProcessName();  //获取当前进程名称

	static std::string GetFileExt(const char* path);  //获取文件的扩展名

	static char* basename(const char* path);

	static int GetPid();  //获取pid

	static std::string CreateGuid();  //生成guid

	static void Split(const std::string& src, const std::string& split, std::vector<std::string>& results);

	static std::wstring StringToWString(const std::string& str);
	static std::string WStringToString(const std::wstring& wstr);

	static std::wstring UTF8ToUnicode(const std::string& str);
	static std::string UnicodeToUTF8(const std::wstring& wstr);

	static std::string UnicodeToANSI(const std::wstring& wstr);
	static std::wstring ANSIToUnicode(const std::string& str);

	static std::string UTF8ToANSI(const std::string& str);
	static std::string ANSIToUTF8(const std::string& str);

	// mode:替换模式，0：只替换第一个找到的。1：全部替换找到的。
	static int Replace(std::string& str, const std::string& pattern, const std::string& newpat, int mode);

	// 处理转义字符    例：\\r 转换为\r
	static void ReplaceToESC(std::string& str);

	static int MakeNDir(const std::string& fullPath);  //递归创建目录

	static bool IsFileExist(const std::string& fullPath);  //文件是否存在
	static bool IsDirExist(const std::string& fullPath);   //文件是否存在

	static std::string GetFileName(const std::string& fullPath);            //从路径中截取到文件名
	static std::string GetFileNameWithOutExt(const std::string& fullPath);  //从路径中截取到文件名,不带扩展名

	static std::string GetFilePath(const std::string& fullPath);  //从全路径中获取路径

	static std::string GetTime();  //获取当前时间戳

	static int GetPidByPort(int port);                                    //根据端口号找到pid
	static bool KillProcess(int pid);                                     //杀死进程根据pid
	static int GetFileLength(FILE* fp);                                   //获取文件长度
	static int ExecCommand(const std::string& cmd, std::string& output);  //执行命令，返回命令的输出
	static std::vector<unsigned long> KillProcessByName(const std::string& procName);  //杀掉名称为 name 的进程.
	static std::vector<unsigned long> GetProcessByName(const std::string& procName);

	static std::string Format(const char* format, ...);  //格式化

	static void SleepForMilliSeconds(int milliseconds);  // sleep

	static void SetupConsoleQuitHandler(ConsoleQuitHandlerFun handler);  //控制台退出信号处理

	static void Trim(std::string& src, const char* ch, size_t len);  //去掉指定字符

	static void KillThread(std::thread* thread);

	static std::string GetDllExt();

	static std::string GetIconFullPathName(const std::string& icon);

	static bool StrCompare(const std::string& str1, const std::string& str2);

	static std::string strToHex(const std::string& convert_data);

	static std::string hexToStr(const std::string& convert_data);
};

//删除list vector set等单元素容器类
template <class T>
void DELETE_LIST(T& list)
{
	for (int i = 0; i < list.size(); ++i) {
		delete list[i];
	}

	list.clear();
}

//删除map，multimap等key、vale容器类
template <class T>
void DELETE_MAP(T& map)
{
	auto iter = map.begin();
	for (; iter != map.end(); ++iter) {
		delete iter->second;
	}

	map.clear();
}

#define SAFE_NEW(p, type) \
    {                     \
        p = new type;     \
    }

#define SAFE_DELETE(p)   \
    {                    \
        if (NULL != p) { \
            delete p;    \
            p = NULL;    \
        }                \
    }

#define SAFE_DELETE_A(p) \
    {                    \
        if (NULL != p) { \
            delete[] p;  \
            p = NULL;    \
        }                \
    }

#endif  // YOGLOBAL_H
