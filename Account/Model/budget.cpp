#include "budget.h"

Budget::Budget()
{

}

Budget::Budget(const Budget & b)
{

}

Budget& Budget::operator =(const Budget& b)
{
    return *this;
}

int Budget::id() const
{
    return m_id;
}

void Budget::setId(int id)
{
    m_id = id;
}

