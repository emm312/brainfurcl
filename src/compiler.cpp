#include <token.h>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <compiler.h>


class TokenBuffer
{
private:
	std::vector<Token> m_tokens;
	size_t m_index = 0;

public:
	TokenBuffer(const std::vector<Token>& tokens)
		: m_tokens(tokens)
	{}

	bool hasNext() const
	{
		return m_index < m_tokens.size();
	}

	void advance()
	{
		m_index++;
	}

	const Token& next()
	{
		return m_tokens[++m_index];
	}

	const Token& current() const
	{
		return m_tokens[m_index];
	}

	const size_t& pos() const
	{
		return m_index;
	}
};


const std::string compile(std::string code) {
    std::vector<Token> tokens;
    std::stringstream out;
    
    for (int i = 0; i<code.size(); i++) {
        if (code[i] == '>') {
            tokens.push_back(Token::TT_INCP);
        } else if (code[i] == '<') {
            tokens.push_back(Token::TT_DECP);
        } else if (code[i] == '+') {
            tokens.push_back(Token::TT_INC);
        } else if (code[i] == '-') {
            tokens.push_back(Token::TT_DEC);
        } else if (code[i] == '.') {
            tokens.push_back(Token::TT_OUT);
        } else if (code[i] == ',') {
            tokens.push_back(Token::TT_IN);
        } else if (code[i] == '[') {
            tokens.push_back(Token::TT_OPEN_BRACKET);
        } else if (code[i] == ']') {
            tokens.push_back(Token::TT_CLOSED_BRACKET);
        } else {
            continue;
        }

    }

    TokenBuffer buf(tokens);
    out << "MINHEAP 256\nMINSTACK 0\n";
    out << "MINREG 2\n";
    bool shouldLOD = true;
    std::vector<int> loops;
    while (buf.hasNext()) {
        bool shouldIncBuf = true;
        
        switch (buf.current()) {
            case Token::TT_INCP: {
                out << "INC R1 R1\n";
                shouldLOD = true;
                break;
            }
            case Token::TT_DECP: {
                out << "DEC R1 R1\n";
                shouldLOD = true;
                break;
            }
            case Token::TT_INC: {
                if (shouldLOD) {
                    out << "LOD R2 R1\n";
                }
                long long unsigned int count = 0;
                while (buf.hasNext() && buf.current() == Token::TT_INC) {
                    buf.advance();
                    count += 1;
                    shouldIncBuf = false;
                }
                out << "ADD R2 R2 " << count << "\n";

                out << "STR R1 R2\n";
                shouldLOD = false;
                break;
            }
            case Token::TT_DEC: {
                if (shouldLOD) {
                    out << "LOD R2 R1\n";
                }
                long long unsigned int count = 0;
                while (buf.hasNext() && buf.current() == Token::TT_DEC) {
                    buf.advance();
                    count += 1;
                    shouldIncBuf = false;
                }
                out << "SUB R2 R2 " << count << "\n";
                out << "STR R1 R2\n";
                shouldLOD = false;
                break;
            }
            case Token::TT_OUT: {
                out << "LOD R2 R1\n";
                out << "OUT %TEXT R2\n";
                break;
            }
            case Token::TT_IN: {
                out << "IN R2 %TEXT\n";
                out << "STR R1 R2\n";
                break;
            }
            case Token::TT_OPEN_BRACKET: {
                out << ".label_" << buf.pos() << "\n";
                loops.push_back(buf.pos());
                break;
            }
            case Token::TT_CLOSED_BRACKET: {
                if (shouldLOD) {
                    out << "LOD R2 R1\n";
                }
                out << "BNZ " << ".label_" << loops[loops.size()-1] << " R2 " << "\n";
                loops.pop_back();
            }
        }
        if (shouldIncBuf) {
            buf.advance();
        }
    }

    return out.str();
}