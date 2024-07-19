#pragma once
#include <windows.h>
#include <string>
#include <vector>

namespace vui
{
	class node
	{
		//类型
	public:
		using Char = wchar_t;
		using String = std::wstring;
		enum class nodetype
		{
			Null,
			Integer,
			String,
			Double,
			Object,
			Array,
			Disposed
		};
		// 存储
	public:
		nodetype type = nodetype::Null;
		Char* name = nullptr;
		union
		{
			struct
			{
				union
				{
					long long Int = 0;
					long double Double;
					Char* String;
				} Data;
			};
			struct
			{
				std::vector<node*>* Child;
				Char* ArrayName;
			};
		};
		// 公用
	public:
		node()
		{
		}
		~node()
		{
			_tidy();
		}
		inline static node& parse(String vui)
		{
			node* pout;
			_parse((Char*)vui.c_str(), vui.size(), &pout);
			return *pout;
		}
		inline static node& parse(const Char* vui, size_t length)
		{
			node* pout;
			_parse(vui, length == -1 ? lstrlenW(vui) : length, &pout);
			return *pout;
		}

	private:
		inline static void _parse(const Char* vui, size_t length, node** pout)
		{
			_parse(vui, 0, length, pout, nullptr);
		}

	public:
		inline String save()
		{
			switch (type)
			{
#ifdef _DEBUG
			case nodetype::Null:
				return String(L"<NULL>");
				break;
#endif
			case nodetype::Integer:
				return std::to_wstring(((node*)this)->Data.Int);
				break;
			case nodetype::String:
			{
				auto str = String(((node*)this)->Data.String);
				return L"\"" + replace_all<String>(str, L"\"", L"\\\"") + L"\"";
				break;
			}
			case nodetype::Double:
				return std::to_wstring(Data.Double);
				break;
			case nodetype::Object:
				return _save_object_internal();
				break;
			case nodetype::Array:
				return _save_array_internal();
				break;
			default:
				break;
			}
			return String();
		}

	private:
		inline void _tidy()
		{
			type = nodetype::Disposed;
			switch (type)
			{
			case vui::node::nodetype::String:
				delete Data.String;
				break;
			case vui::node::nodetype::Array:
				_tidy_array();
				break;
			default:
				break;
			}
		}
		// 值
	public:
		node(long long val)
		{
			type = nodetype::Integer;
			Data.Int = val;
		}
		node(long double val)
		{
			type = nodetype::Double;
			Data.Double = val;
		}
		node(const wchar_t* val)
		{
			type = nodetype::String;
			Data.String = (Char*)val;
		}
		operator long long()
		{
			if (type != nodetype::Integer)
				throw;
			return Data.Int;
		}
		operator long double()
		{
			if (type != nodetype::Double)
				//throw;
			return Data.Double;
		}
		operator const wchar_t* ()
		{
			if (type != nodetype::String)
				throw;
			return Data.String;
		}
		// 数组
	public:
		inline std::vector<node*>::iterator begin()
		{
			return Child->begin();
		};
		inline std::vector<node*>::iterator end()
		{
			return Child->end();
		};
		inline size_t size()
		{
			return Child->size();
		};
		inline node& operator[](size_t index)
		{
			return *(*Child)[index];
		}

	private:
		inline void _tidy_array()
		{
			type = nodetype::Disposed;
			for (auto& c : *Child)
			{
				c->_tidy();
				delete c;
			}
			Child->~vector();
			delete Child;
		}
		// 对象
	private:
		inline node* get(const Char* key)
		{
			for (auto item : *this)
			{
				if (String(item->name) == String(key))
				{
					return item;
				}
			}
			return nullptr;
		}

	public:
		template <class outtype>
		inline bool try_get(const Char* key, outtype& out)
		{
			auto pnode = get(key);
			if (pnode != nullptr && (pnode->type == nodetype::Integer || pnode->type == nodetype::Double || pnode->type == nodetype::String))
			{
				out = *pnode;
				return true;
			}
			return false;
		}
		inline node& get_any(const Char* key)
		{
			auto pnode = get(key);
			if (pnode != nullptr)
				return *((node*)pnode);
			throw "";
		}
		inline bool try_get_any(const Char* key, node& out)
		{
			auto pnode = get(key);
			if (pnode != nullptr)
			{
				out = *pnode;
				return true;
			}
			return false;
		}
		inline node& get_value(const Char* key)
		{
			auto pnode = get(key);
			if (pnode != nullptr && (pnode->type == nodetype::Integer || pnode->type == nodetype::Double || pnode->type == nodetype::String))
				return *((node*)pnode);
			throw "";
		}
		inline bool try_get_value(const Char* key, node& out)
		{
			auto pnode = get(key);
			if (pnode != nullptr && (pnode->type == nodetype::Integer || pnode->type == nodetype::Double || pnode->type == nodetype::String))
			{
				out = *pnode;
				return true;
			}
			return false;
		}
		inline node& get_array(const Char* key)
		{
			auto pnode = get(key);
			if (pnode != nullptr && pnode->type == nodetype::Array)
				return *((node*)pnode);
			throw "";
		}
		inline bool try_get_array(const Char* key, node& out)
		{
			auto pnode = get(key);
			if (pnode != nullptr && pnode->type == nodetype::Array)
			{
				out = *pnode;
				return true;
			}
			return false;
		}
		inline node& get_object(const Char* key)
		{
			auto pnode = get(key);
			if (pnode != nullptr && pnode->type == nodetype::Object)
				return *((node*)pnode);
			throw "";
		}
		inline bool try_get_object(const Char* key, node& out)
		{
			auto pnode = get(key);
			if (pnode != nullptr && pnode->type == nodetype::Object)
			{
				out = *pnode;
				return true;
			}
			return false;
		}
		// 解析
	private:
		inline static void _parse(const Char* vui, size_t count, size_t length, node** pout, nodetype* otype, size_t* readlen = nullptr)
		{
			nodetype type = nodetype::String;
			for (size_t i = count; i < length; i++)
			{
				Char chr = vui[i];
				if (chr == '\0')
				{
					node* vn = new node();
					*pout = vn;
					return;
				}
				if (/*iswspace(chr) ||*/ (chr >= 0 && chr < 127 && isspace(chr)))
					continue; // 不解析空格

				if ((/*iswdigit(chr) ||*/ (chr >= 0 && chr < 127 && isdigit(chr))) && i == count) // 不要解析宽空格！
				{
					type = nodetype::Integer;
					break;
				}

				if (chr == L'\"' && i == count)
				{
					type = nodetype::String;
					break;
				}

				if (chr == L'{')
				{
					type = nodetype::Object;
					break;
				}

				if (chr == L'[' && i == count)
				{
					type = nodetype::Array;
					break;
				}
			}
			if (otype != nullptr)
				*otype = type;
			switch (type)
			{
			case nodetype::Object:
			{
				node obj{};
				obj._parse_object_internal(vui, count, length, readlen);
				*pout = new node(obj);
				break;
			}
			case nodetype::Array:
			{
				node arr{};
				arr._parse_array_internal(vui, count, length, readlen);
				*pout = new node(arr);
				break;
			}
			case nodetype::String:
			case nodetype::Integer:
			{
				*pout = node::_parse_value_internal(vui, count, length, readlen);
				break;
			}
			default:
				throw "";
			}
		}
		inline static node* _parse_value_internal(const Char* vui, size_t count, size_t length, size_t* readlen = nullptr)
		{
			node pnode;
			pnode.type = nodetype::Null;
			int flag = 0; // 0 开始解析 qwq
			String stringbuf;
			size_t i = count;
			for (; i < length; i++)
			{
				Char chr = vui[i];
				switch (flag)
				{
				case 0: // __"__字符串" __1__23 __1__23.f 这里
				{
					if (iswspace(chr) || chr == ',' || chr == '\r' || chr == '\n' || chr == '\t' || (chr < 127 && isspace(chr)))
						break; // 不解析空格

					if (chr == L'\"')
					{
						flag += 1;
						break;
					}
					i--;
					flag = 4; // 相信user的输入（（（
					break;
					// throw "我没料到这个是vui数据";
				}
				case 1: // "__字__符串" 字符串中心
				{
					if (chr == L'\\')
					{
						flag += 2; // 祝你转义快乐
						break;
					}
					if (chr == L'\"')
					{
						i--;
						flag += 1;
						break;
					}
					stringbuf.push_back(chr);

					break;
				}
				case 2: // "字符串"__ __ 字符串结尾
				{
					pnode.type = nodetype::String;
					goto ret;
					break;
				}
				case 3: // "特殊字符转义\__"__" 字符串转义
				{
					if (chr == L'\\')
					{
						stringbuf.push_back(L'\\');
						flag -= 2;
						break;
					}
					if (chr == L'\"')
					{
						stringbuf.push_back(L'\"');
						flag -= 2;
						break;
					}
					if (chr == L't')
					{
						stringbuf.push_back(L'\t');
						flag -= 2;
						break;
					}
					if (chr == L'n')
					{
						stringbuf.push_back(L'\n');
						flag -= 2;
						break;
					}
					if (chr == L'\n') // 宏 定 义
					{
						flag -= 2;
						break;
					}
					if (chr == L'r')
					{
						flag -= 2;
						break;
					}
					throw "牛子牛逼";
				}
				case 4: // __1__23 数字
				{
					pnode.type = pnode.type == nodetype::Null ? nodetype::Integer : pnode.type;
					if (chr == L'f' || chr == L'd' || chr == L'.')
					{
						pnode.type = nodetype::Double;
						if (chr == L'f' || chr == L'd')
							goto ret;
						stringbuf.push_back(chr);
						break;
					}
					if (!(iswdigit(chr) || (chr < 127 && isdigit(chr))))
					{
						if (chr == L')' || chr == L'}' || chr == L']' || chr == L',')
						{
							goto ret;
						}
						flag = 5;
						stringbuf.push_back(chr);
						break;
					}
					stringbuf.push_back(chr);
					break;
				}
				case 5: // __a__bc 无引号字符串
				{
					if (chr == L')' || chr == L'}' || chr == L']')
					{
						i--;
						pnode.type = nodetype::String;
						goto ret;
					}
					stringbuf.push_back(chr);
					break;
				}
				default:
					break;
				}
			}
		ret:
			switch (pnode.type)
			{
			case nodetype::Integer:
			{
				long long data = std::stoll(stringbuf);
				pnode.Data.Int = data;
				break;
			}
			case nodetype::Double:
			{
				long double data = std::stold(stringbuf);
				pnode.Data.Double = data;
				break;
			}
			case nodetype::String:
			{
				//OutputDebugString(stringbuf.c_str());
				pnode.Data.String = new Char[stringbuf.size() + 1];
				std::move(stringbuf.c_str(), &(stringbuf.c_str())[stringbuf.size()], pnode.Data.String);
				pnode.Data.String[stringbuf.size()] = L'\0';
				i++;
				break;
			}
			default:
				throw std::exception();
			}
			*readlen = i - count;
			return new node(pnode);
		};
		inline void _parse_array_internal(const Char* vui, size_t count, size_t length, size_t* len)
		{
			Child = new std::vector<node*>();
			this->type = nodetype::Array;
			int flag = 0; // 0 开始解析 qwq
			size_t i = count;
			for (; i < length; i++)
			{
				Char chr = vui[i];
				switch (flag)
				{
				case 0:
					if (chr != L'[')
						throw "";
					flag++;
					break;
				case 1:
				{
					if (chr == L'\r')
						break;
					if (chr == L'\n')
						break;
					if (chr == L' ')
						break;
					if (chr == L',')
						break;

					if (chr == L']')
						goto ret;

					size_t len_s;
					node* node;
					node::_parse(vui, i, length, &node, NULL, &len_s);
					node->name = nullptr;
					Child->push_back(node);
					i += len_s;
					i--;
					break;
				}
				default:
					break;
				}
			}
		ret:
			if (len != nullptr)
				*len = i - count;
		}
		inline String _save_array_internal()
		{
			String to_ret;
			switch (type)
			{
			case nodetype::Object:
				if (ArrayName != nullptr)
				{
					to_ret.append(ArrayName);
				}
				else
				{
					to_ret.append(L"<unnamed_object>");
				}
				to_ret.push_back(L'{');
				for (node* node : *Child)
				{
					node->name == nullptr ? L"" : to_ret.append(node->name);
					to_ret.push_back(L'(');
					to_ret.append(node->save());
					to_ret.push_back(L')');
					to_ret.push_back(L',');
				}
				if (to_ret.length() >= 1 && to_ret[to_ret.length() - 1] != L'{')
					to_ret.erase(to_ret.size() - 1, 1);
				to_ret.push_back(L'}');
				break;
			case nodetype::Array:
				to_ret.push_back(L'[');
				for (node* node : *Child)
				{
					to_ret.append(node->save());
					to_ret.push_back(L',');
				}
				if (to_ret.length() >= 1 && to_ret[to_ret.length() - 1] != L'[')
					to_ret.erase(to_ret.size() - 1, 1);
				to_ret.push_back(L']');
				break;
			default:
				break;
			}
			return to_ret;
		}
		inline void _parse_object_internal(const Char* vui, size_t count, size_t length, size_t* len)
		{
			Child = new std::vector<node*>();
			type = nodetype::Object;
			int flag = 0; // 0 开始解析 qwq
			String keyname;
			size_t i = count;
			for (; i < length; i++)
			{
				Char chr = vui[i];
				switch (flag)
				{
				case 0: // __x__xx{yyy(zzz),...} kokodayo
				{
					if (/*iswspace(chr) ||*/ (chr >= 0 && chr < 127 && isspace(chr)))
						break; // 不解析空格

					if (chr == L'{')
					{
						this->ArrayName = (Char*)(new String(keyname))->c_str();
						keyname.clear();
						flag++; // { 跳转到下一个状态
						break;
					}

					keyname.push_back(chr);

					break;
				}
				case 1: // xxx{__y__yy(zzz),...}
				{
					if (/*iswspace(chr) ||*/ (chr >= 0 && chr < 127 && isspace(chr)))
						break; // 不解析空格

					if (chr == L'(')
					{
						flag++; // ( 跳转到下一个状态
						break;
					}

					if (chr == L'}')
					{
						keyname.clear();
						flag = 3;
						break;
					}

					keyname.push_back(chr);

					break;
				}
				case 2:
				{
					if (chr == L')')
						break;

					if (chr == L'\r')
						break;
					if (chr == L'\n')
						break;
					if (chr == L' ')
						break;
					if (chr == L',')
					{
						keyname.clear();
						flag--;
						break;
					}

					if (chr == L'}')
					{
						keyname.clear();
						flag++;
						break;
					}

					size_t len_s;
					node* node;
					node::_parse(vui, i, length, &node, NULL, &len_s);
					node->name = (Char*)(new String(keyname.c_str()))->c_str();
					Child->push_back(node);
					i += len_s;
					// i--;
					break;
				}
				case 3:
				{
					goto ret;
				}
				default:
					break;
				}
			}
		ret:
			if (len != nullptr)
				*len = i - count;
		}
		inline String _save_object_internal()
		{
			return _save_array_internal();
		}
		template <class string>
		inline string& replace_all(string& src, const string& old_value, const string& new_value)
		{
			// 每次重新定位起始位置，防止上轮替换后的字符串形成新的old_value
			for (size_t pos(0); pos != string::npos; pos += new_value.length())
			{
				if ((pos = src.find(old_value, pos)) != string::npos)
				{
					src.replace(pos, old_value.length(), new_value);
				}
				else
					break;
			}
			return src;
		}
	};
	inline node& make_node(node::String vui)
	{
		return node::parse(vui);
	}
}