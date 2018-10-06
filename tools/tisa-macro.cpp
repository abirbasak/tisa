#include <iostream>
#include <fstream>

struct macro {
    void emit_cat(std::ostream& os)
    {
        os << nl;
        os << def << prefix << "_CAT_I(a,b) a##b" << nl;
        os << def << prefix << "_CAT(a,b) " << prefix << "_CAT_I(a,b)" << nl;
    }
    void emit_to_string(std::ostream& os)
    {
        os << nl;
        os << def << prefix << "_TO_STRING_I(x) #x" << nl;
        os << def << prefix << "_TO_STRING(x) " << prefix << "_TO_STRING_I(x)" << nl;
        os << def << prefix << "_TO_CHAR_I(x) 0[#x]" << nl;
        os << def << prefix << "_TO_CHAR(x) " << prefix << "_TO_CHAR_I(x)" << nl;
    }
    void emit_symbol(std::ostream& os)
    {
        os << nl;
        os << def << prefix << "_COMMA_I() ," << nl;
        os << def << prefix << "_COMMA() " << prefix << "_COMMA_I()" << nl;
        os << def << prefix << "_LPAREN_I() (" << nl;
        os << def << prefix << "_LPAREN() " << prefix << "_LPAREN_I()" << nl;
        os << def << prefix << "_RPAREN_I() )" << nl;
        os << def << prefix << "_RPAREN() " << prefix << "_RPAREN_I()" << nl;
        os << def << prefix << "_EMPTY()" << nl;
    }
    void emit_sequence(std::ostream& os)
    {
        os << nl;
        os << def << prefix << "_SEQ(";
        for (int i = 1; i <= n; ++i)
        {
            os << '_' << i << ',';
        }
        os << "_N,...) _N" << nl;

        os << def << prefix << "_RSEQ() ";
        for (int i = n; i != 0; --i)
        {
            os << i << ',';
        }
        os << 0 << nl;
        os << def << prefix << "_COUNT(...) " << prefix << "_COUNT_I(__VA_ARGS__," << prefix
           << "_RSEQ())" << nl;
        os << def << prefix << "_COUNT_I(...) " << prefix << "_SEQ(__VA_ARGS__)" << nl;
    }
    void emit_foreach(std::ostream& os)
    {
        os << nl;
        for (int i = 1; i <= n; ++i)
        {
            os << def << prefix << "_FOREACH_" << i << "(M,S";
            for (int p = 1; p <= i; ++p)
            {
                os << ',' << 'e' << p;
            }
            os << ')';
            for (int p = 1; p < i; ++p)
            {
                os << " M(e" << p << ',' << p << ") S(" << p << ')';
            }
            os << " M(e" << i << ',' << i << ")" << nl;
        }
        os << def << prefix << "_FOREACH_I(N,M,S,...) " << prefix << "_CAT(" << prefix
           << "_FOREACH_,N)"
           << "(M,S,__VA_ARGS__)" << nl;
        os << def << prefix << "_FOREACH(M,S,...) " << prefix << "_FOREACH_I(" << prefix
           << "_COUNT(__VA_ARGS__),M,S,__VA_ARGS__)" << nl;
    }
    void emit(std::ostream& os)
    {
        emit_cat(os);
        emit_symbol(os);
        emit_to_string(os);
        emit_sequence(os);
        emit_foreach(os);
    }
    static std::ostream& def(std::ostream& os)
    {
        return os << "#define ";
    }
    static std::ostream& nl(std::ostream& os)
    {
        return os << '\n';
    }
    std::string prefix;
    int n;
};
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    std::ofstream ofs{"macro.hpp"};
    std::string prefix = "AZUIK_PP";
    int count = 8;
    macro m{prefix, count};
    m.emit(ofs);
}
