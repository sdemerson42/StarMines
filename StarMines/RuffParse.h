#pragma once

#include <string>
#include <vector>

namespace Ruff
{
	using ByteCode = std::vector<int>;

	ByteCode parse(const std::string &fname);

	struct Token
	{
		Token() {}
		Token(char _type, std::string _val) :
			type{ _type }, val{ _val }
		{}
		char type;
		std::string val;
	};

	class TokenStream
	{
		friend ByteCode parse(const std::string &fname);
	public:
		TokenStream(const std::string &fName);
		Token get();
		void putBack(const Token &t);
	private:
		std::vector<std::string> parseSymbols(const std::string &fName);
		void makeTokens(const std::vector<std::string> &ss);
		std::vector<Token> m_stream;
		bool m_bufferFull;
		Token m_buffer;
	};

}