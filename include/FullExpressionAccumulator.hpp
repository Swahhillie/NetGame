#ifndef FULLEXPRESSIONACCUMULATOR_H
#define FULLEXPRESSIONACCUMULATOR_H


#include <sstream>
#include <iostream>

class FullExpressionAccumulator
{
public:
    explicit FullExpressionAccumulator(std::ostream & os) : os(os) {}
    ~FullExpressionAccumulator()
    {
        os << ss.rdbuf() << std::flush; // write the whole shebang in one go
    }

    template <typename T>
    FullExpressionAccumulator& operator<<(T const& t)
    {
        ss << t; // accumulate into a non-shared stringstream, no threading issues
        return *this;
    }

private:
    std::ostream& os;
    std::stringstream ss;

    // stringstream is not copyable, so copies are already forbidden
};

#endif // FULLEXPRESSIONACCUMULATOR_H
