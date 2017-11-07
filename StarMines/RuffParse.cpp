#include "RuffParse.h"

#include <fstream>
#include <algorithm>
#include <iostream>

Ruff::TokenStream::TokenStream(const std::string &fName)
{
	std::cout << "Tokenizing " << fName << "...\n";
	m_bufferFull = false;
	auto symbols = parseSymbols(fName);
	makeTokens(symbols);
	int x = 0;
	std::cout << "Complete.\n";
}

void Ruff::TokenStream::makeTokens(const std::vector<std::string> &ss)
{
	std::vector<Token> stack;
	bool expr{ false };

	for (const auto &s : ss)
	{
		Token t;
		if (!isalnum(s[0]))
		{
			t.type = s[0];
		}
		else if (isalpha(s[0]))
		{
			t.type = 's';
			t.val = s;
		}
		else
		{
			t.type = 'i';
			t.val = s;
		}

		if (expr)
		{
			if (t.type == '(')
			{
				stack.emplace_back(t);
			}
			else if (t.type == ')')
			{
				auto p = std::find_if(begin(stack), end(stack), [](Token &tt)
				{
					return tt.type == '(';
				});
				if (p == end(stack))
				{
					for (auto &tt : stack)
						m_stream.emplace_back(tt);
					expr = false;
				}
				else
				{
					while (true)
					{
						if (stack.size() == 0) break;
						auto tt = stack[stack.size() - 1];
						if (tt.type == '(')
						{
							stack.pop_back();
							break;
						}
						m_stream.push_back(tt);
						stack.pop_back();
					}
				}
			}
			else if (t.type == '+' || t.type == '-' || t.type == '*' || t.type == '/' || t.type == '%')
			{
				while (true)
				{
					if (stack.size() == 0) break;
					auto tt = stack[stack.size() - 1];
					if (t.type == '+' || t.type == '-')
					{
						if (tt.type != '(')
						{
							m_stream.emplace_back(tt);
							stack.pop_back();
						}
						else
							break;
					}
					else
					{
						if (tt.type == '*' || tt.type == '/' || tt.type == '%')
						{
							m_stream.emplace_back(tt);
							stack.pop_back();
						}
						else
						{
							break;
						}
					}
				}

				stack.emplace_back(t);
			}
			else
			{
				m_stream.emplace_back(t);
			}
		}
		else if (t.type == '(')
		{
			expr = true;
			stack.clear();
		}
		else
		{
			m_stream.emplace_back(t);
		}
	}
}

std::vector<std::string> Ruff::TokenStream::parseSymbols(const std::string &fName)
{
	std::ifstream ifs{ fName };
	std::vector<std::string> r;
	
	while (true)
	{
		std::string s;
		if (!(ifs >> s)) break;

		auto b = begin(s);
		const auto e = end(s);

		while (b < e)
		{
			if (!isalnum(*b))
			{
				if (*b == '"')
				{
					r.emplace_back(std::string{ *b });
					std::string symbol;
					while (isalpha(*++b))
					{
						symbol.insert(begin(symbol), *b);
					}
					r.emplace_back(symbol);
					++b;
				}
				else
				{
					r.emplace_back(std::string{ *b });
					++b;
				}
			}
			else
			{
				std::string symbol;
				while (b < e && isalnum(*b))
				{
					symbol += *b;
					++b;
				}
				r.emplace_back(symbol);
			}
		}
	}

	return r;
}


Ruff::Token Ruff::TokenStream::get()
{
	if (m_bufferFull)
	{
		m_bufferFull = false;
		return m_buffer;
	}

	if (m_stream.size() != 0)
	{
		Token t = m_stream[0];
		m_stream.erase(begin(m_stream));
		return t;
	}
	return Token{ 'N', "null" };
}

void Ruff::TokenStream::putBack(const Ruff::Token &t)
{
	m_buffer = t;
	m_bufferFull = true;
}


Ruff::ByteCode Ruff::parse(const std::string &fName)
{
	std::cout << "Parsing " << fName << "...\n";
	struct SigIndex
	{
		SigIndex() {}
		SigIndex(const std::string &_name, int _val) :
			name{ _name }, val{ _val }
		{}
		std::string name;
		int val;
	};

	std::vector<SigIndex> cmdTable;
	cmdTable.emplace_back(SigIndex{ "let", Code::let });
	cmdTable.emplace_back(SigIndex{ "dec", Code::dec });
	cmdTable.emplace_back(SigIndex{ "ifEquals", Code::ifEquals });
	cmdTable.emplace_back(SigIndex{ "ifGreater", Code::ifGreater });
	cmdTable.emplace_back(SigIndex{ "ifLess", Code::ifLess });
	cmdTable.emplace_back(SigIndex{ "inc", Code::inc });
	cmdTable.emplace_back(SigIndex{ "jump", Code::jump });
	cmdTable.emplace_back(SigIndex{ "def", Code::def });
	cmdTable.emplace_back(SigIndex{ "halt", Code::halt });
	cmdTable.emplace_back(SigIndex{ "rand", Code::rand });
	cmdTable.emplace_back(SigIndex{ "retsub", Code::retsub });
	cmdTable.emplace_back(SigIndex{ "pause", Code::pause });


	cmdTable.emplace_back(SigIndex{ "logstr", Code::logstr });
	cmdTable.emplace_back(SigIndex{ "log", Code::log });
	cmdTable.emplace_back(SigIndex{ "sendCall", Code::sendCall });
	cmdTable.emplace_back(SigIndex{ "setDir", Code::setDir });
	cmdTable.emplace_back(SigIndex{ "sleep", Code::sleep });

	std::vector<SigIndex> label;
	std::vector<SigIndex> var;
	std::vector<SigIndex> temp;
	int curVar{ 0 };
	int cmd{ 0 };
	ByteCode code;

	TokenStream ts{ fName };
	int i{ 0 };

	while (ts.m_stream.size() > 0)
	{
		auto t = ts.get();

		switch (t.type)
		{
		case 'i':
		{
			code.code.emplace_back(Code::prim);
			code.code.emplace_back(stoi(t.val));
			i += 2;
			break;
		}
		case 's':
		{
			std::string s{ t.val };
			auto p = std::find_if(begin(cmdTable), end(cmdTable), [&](SigIndex &si)
			{
				return si.name == s;
			});

			if (p != end(cmdTable))
			{
				cmd = p->val;
			}
			else
			{
				// var or label?
				//label
				int c{ -1 };
				if (code.code.size() > 0)
					c = code.code[code.code.size() - 1];
				if (cmd == Code::def || c == Code::label || c == Code::sublabel)
				{
					if (cmd == Code::def)
					{
						label.emplace_back(SigIndex{ t.val, i });
					}
					else
					{
						code.code.emplace_back(-1);
						temp.emplace_back(SigIndex{ t.val, i });
						++i;
					}
					
				}
				else
				{
					code.code.emplace_back(Code::var);
					std::string s{ t.val };
					auto p = std::find_if(begin(var), end(var), [&](SigIndex &si)
					{
						return si.name == s;
					});
					if (p != end(var))
					{
						code.code.emplace_back(p->val);
					}
					else
					{
						var.emplace_back(SigIndex{ s, curVar });
						code.code.emplace_back(curVar);
						++curVar;
					}
					i += 2;
				}

			}
			break;
		}
		
		case '+':
			code.code.emplace_back(Code::add);
			++i;
			break;
		case '-':
			code.code.emplace_back(Code::sub);
			++i;
			break;
		case '*':
			code.code.emplace_back(Code::mult);
			++i;
			break;
		case '/':
			code.code.emplace_back(Code::div);
			++i;
			break;
		case '%':
			code.code.emplace_back(Code::mod);
			++i;
			break;
		case '>':
			code.code.emplace_back(Code::label);
			++i;
			break;
		case '^':
			code.code.emplace_back(Code::sublabel);
			++i;
			break;
		case ';':
			if (cmd != Code::def)
			{
				code.code.emplace_back(cmd);
				++i;
			}
			break;
		case '"':
		{
			code.code.emplace_back(Code::strbegin);
			++i;
			auto strT = ts.get();
			for (char &c : strT.val)
			{
				code.code.emplace_back(c);
				++i;
			}
			code.code.emplace_back(Code::strend);
			++i;
		}

			break;

		}

	}

	// Replace label temps

	for (auto &si : temp)
	{
		int index{ si.val };
		std::string lName{ si.name };
		auto p = find_if(begin(label), end(label), [&](SigIndex &si)
		{
			return si.name == lName;
		});
		if (p != end(label))
		{
			code.code[index] = p->val;
		}
	}

	// Add metadata

	for (auto &si : label)
	{
		code.label[si.name] = si.val;
	}

	std::cout << "RuffCode successfully generated. Length: " << code.code.size() << "\n";

	return code;
}

