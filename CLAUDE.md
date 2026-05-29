# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

OpenAccount is a Qt6/C++17 personal finance application (accounts, transactions, categories). It follows a clean MVC architecture split across three libraries (`Model`, `Controller`, `View`) and one external submodule (`lib/DesignLibrary`).

## Build System

CMake is the primary build system (being migrated from qmake — both `.pro` files and `CMakeLists.txt` coexist, but CMake is authoritative).

```bash
# Configure
cmake -B build -S .

# Build
cmake --build build

# Run all tests
cd build && ctest --output-on-failure

# Run only model tests
cd build && cmake --build . --target run_model_tests
```

Qt6 is required with components: Core, Gui, Test, Qml, Quick, QuickControls2, Network, Sql.

The submodule must be initialized before building:
```bash
git submodule update --init --recursive
```

## Running a Single Test

Each model class has its own test executable. After building:

```bash
cd build && ./Model/ModelTests/TestAccount
cd build && ./Model/ModelTests/TestTransaction
# etc.
```

Test executables: `TestProfile`, `TestAccount`, `TestTransaction`, `TestCategory`, `TestDebt`, `TestModel`, `TestTotal`, `TestTransactionListModel`, `TestAccountTransactionFilterProxyModel`, `TestFrequency`.

## Architecture

### Layer Structure

| Layer | Directory | Library | Role |
|---|---|---|---|
| Model | `Model/` | `OAModel` | Business logic, data entities, Qt list models |
| Controller | `Controller/` | `Controller` | Data persistence abstraction (in progress) |
| View | `View/` | — | Qt Quick/QML UI, theming |
| Design Library | `lib/DesignLibrary/` | `ChewnoDesignPattern` | Base classes (submodule) |

### MetaData Base Class Pattern

All model entities inherit from both `QObject` and `MetaData` (from `lib/DesignLibrary/DesignPattern/metadata.h`). This base class provides:

- Generic `QMap<QString, QVariant>` storage accessed via `metaData<T>(key)` / `setMetadata<T>(key, value)`
- Virtual `toJson()` / `fromJson()` for JSON serialization
- `assignIfChanged()` helper that emits signals only when a value actually changes
- Enum JSON conversion helpers (`enumToJson`, `enumFromJson`)

Every entity property follows this pattern:
```cpp
// In header:
Q_PROPERTY(QString bank READ bank WRITE setBank NOTIFY bankChanged)
QString bank() const;          // getter — reads via metaData<QString>
public slots:
    void setBank(const QString&); // setter — uses assignIfChanged, emits bankChanged + changed
signals:
    void bankChanged();
    void changed();             // aggregate signal for any property change
```

### Shared Pointer Pattern for Transactions

`Transaction` privately inherits `QEnableSharedFromThis<Transaction>`. Use `TransactionPtr` (typedef for `QSharedPointer<Transaction>`) throughout. The `operator+` merges two transactions.

### Enums

Defined in `Model/include/Model/enums.h` under the `OpenAccountEnums` Q_NAMESPACE:
- `Movement`: Both, Credit, Debit
- `Support`: Cash, Cheque, Card, Transfer, Other
- `Frequency`: Custom(-1), Once(0), Daily(1), Weekly(7), Monthly(30), Quarterly(120), Yearly(365)

### QML Structure

QML components live in `View/QML/` with the `OB` prefix (OpenBudget):
- `Components/` — atomic widgets (`OBButton`, `OBInput`, `OBCheckBox`, `OBComboBox`, …)
- `ComposedComponent/` — complex screens (`OBTransactionsList`, `OBCalendar`, `OBTransactionInfo`, …)
- `Style/` — theme system (`OBTheme`, `OBPalette`, `OBDayPalette`, `OBNightPalette`, `OBStyle`)

### Data Persistence

`Controller/` contains an `AbstractDataStorage` interface and SQLite schema files in `Controller/sql/`. The Controller layer is currently a stub under active development.

## Test Conventions

Tests use the Qt Test framework (`QTest`). Each test class covers:
1. Default-constructed value assertions
2. Getter/setter round-trips
3. JSON serialization round-trips (`toJson()` → `fromJson()` → verify equality)
4. Copy/move semantics
5. Signal emission verification via `QSignalSpy`

See `Model/ModelTests/tst_account.cpp` as the canonical example.
