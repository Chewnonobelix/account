Account manager
===============

I) Installation
---------------

Comptatible OS: Windows 10
Excecute account_manager.exe.
 
II) Language available
----------------------

Currently language displaying depend on your OS language.
- English (default)
- French

III) How to use it
------------------

1) Account
----------

An account is a collection of transaction, income or outcome.
To create an account click on the menu "Account" and "Create account".
Then add an account's name, set the initial amount of the account, and set the opening account date.
When a transaction is set before the opening date, the initial value and date will automatically adjust. The total ammount at the original date will not be change.
You can either delete an account, to do that click the "Account" menu and "Remove account".
When no account exist the Create account popup will show up.
You can change current account by selection box on the top right

2) Transaction
--------------

A transaction is an income or an outcome for the current account.
To add a transaction click on "Current account" menu, and "Add transaction".
Then add a transaction's title, set the amount of the transaction, set the transaction date, and the transcation's type (income, or outcome).
An income will increase the account's total, an outcome will decrease the account's total.
You can delete a transaction, select a transaction, click the "Current account" menu and "Remove transaction".
To edit a transaction, click on the wish transaction. You can edit the transaction's title, the transacction's amount, and set a transacton category (see in section 4).

3) Transaction list
-------------------

This list display all select transaction. If no date is select in the calendar, then all transaction are display.
OA red line mean an outcome, a green one mean a income. When an * appear next to an +/- symbol, it mean the transaction is an estimated one, ie the transaction date add after the current day. 
When the current day passe an estimated transaction, it will open a checkout window, you can validate, or remove estimated transaction. If you validate it, the transaction become unestimated. 

4) Category
-----------

A new category can be add by clicking on the last element of the category combo. Categories are set for income or outcome for a single account.

5) Calendar
-----------

The calendar display preview details of the account.
For each day with one or more transaction, a green or a red rectangle will be display on the top of the day square. The green rectangle mean the total day transaction is more than 0. The red one mean a total less than 0.
When one or more transaction is set for an account, a green or a red rectangle will be display on the bottom of the each day square between the first date and the last day. The green rectangle mean the total of the account is more than 0. The red one mean a total of the account is less than 0.
You can select one or more day by maintain shift key, and click on days. When you click on a day, and with no shift key, a single day will be select. The Transaction list will be update with all transactions for selected day.
The graphic panel allow to show the repartition of transactions by categories, split in income or outcome.

6) Graph
--------

The graph view display account evolution over time. The blue part correspond to valid past transactions. The green part to futur estimated transactions.
The default view is for one month, you can zoom to a year, or all year view by scroll in or out your mouse.

7) Frequency
------------

A frequency is a transaction which repeat regulary in time.
A frequency is set by a reference entry, a transaction typ (income or outcome), and a periodicity.
Then those element are set, you can generate transactions between two dates, or set the endless value. Then endless, transactions will be automatically generate.

8) Budget
---------

The budget feature allow to handle a budget in a time period. A budget is link to a category.
In a budget you have to add one or many target. A target is set by a begin date, a value and a periodicity.
A budget iteration reproduce each period of time.

IV) Shortcut
------------

CTRL+Q: Quit application
CTRL+N: Create new account
CTRL+SHIFT+D: Delete current account
CTRL+A: Add new transaction
CTRL+D: Delete current transaction
SHIFT+ left click on day calendar: Select multiple date 
