#include "tst_Profile.cpp"
#include "tst_accounttransactionfilterproxymodel.cpp"
#include "tst_account.cpp"
#include "tst_category.cpp"
#include "tst_model.cpp"
#include "tst_total.cpp"
#include "tst_transaction.cpp"
#include "tst_transactionlistmodel.cpp"
#include <QtTest>

int main(int argc, char **argv) {
  int status = 0;
  {
    ModelTests tc;
    status |= QTest::qExec(&tc, argc, argv);
  }
  {
    TestProfile tc;
    status |= QTest::qExec(&tc, argc, argv);
  }
  {
    TestAccountTransactionFilterProxyModel tc;
    status |= QTest::qExec(&tc, argc, argv);
  }
  {
    tst_account tc;
    status |= QTest::qExec(&tc, argc, argv);
  }

  {
    tst_Total tc;
    status |= QTest::qExec(&tc, argc, argv);
  }
  {
    tst_Transaction tc;
    status |= QTest::qExec(&tc, argc, argv);
  }
  {
    TestTransactionListModel tc;
    status |= QTest::qExec(&tc, argc, argv);
  }
  {
    CategoryTest tc;
    status |= QTest::qExec(&tc, argc, argv);
  }

  return status;
}
