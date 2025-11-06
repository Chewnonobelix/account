#include <QtTest>
#include "tst_Profile.cpp"
#include "tst_model.cpp"

int main(int argc, char **argv)
{
    int status = 0;
    {
        ModelTests tc;
        status |= QTest::qExec(&tc, argc, argv);
    }
    {
        TestProfile tc;
        status |= QTest::qExec(&tc, argc, argv);
    }
    return status;
}
