#pragma once
namespace vui {
	class Exception {
	public:
		Exception(const wchar_t* msg)
		{
			this->msg = msg;
		}
		const wchar_t* msg = nullptr;
	};

	class LogicException : public Exception {
	public:
		LogicException(const wchar_t* msg, int line,const wchar_t* file) : Exception(msg)
		{
			this->line = line;
			this->file = file;
		}
		int line = 0;
		const wchar_t* file = nullptr;
	};
}