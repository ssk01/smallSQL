#include "token.h"
void show(Token t) {
	std::cout << "type: " << t.type << "\tcontent: " << t.content << std::endl;
}
ostream& operator<<(ostream& out, Token t)
{
	out << "  type: " << t.type << "\tcontent: " << t.content;
	return out;
};