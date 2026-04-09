PRAGMA foreign_keys = ON;
PRAGMA user_version = 1;

BEGIN TRANSACTION;

-- SQLite schema for persisting the explicit Model entities.
-- Total is not stored because it is a computed aggregate over transactions.
-- Additional MetaData key/value pairs can be stored in metadata_entries.

CREATE TABLE IF NOT EXISTS profiles (
    id TEXT PRIMARY KEY NOT NULL,
    first_name TEXT NOT NULL DEFAULT '',
    last_name TEXT NOT NULL DEFAULT ''
);

CREATE TABLE IF NOT EXISTS accounts (
    id TEXT PRIMARY KEY NOT NULL,
    profile_id TEXT,
    number TEXT NOT NULL DEFAULT '',
    bank TEXT NOT NULL DEFAULT '',
    interest REAL NOT NULL DEFAULT 0.0,
    description TEXT NOT NULL DEFAULT '',
    opening TEXT,
    FOREIGN KEY (profile_id) REFERENCES profiles(id)
        ON UPDATE CASCADE
        ON DELETE SET NULL
);

CREATE TABLE IF NOT EXISTS categories (
    id TEXT PRIMARY KEY NOT NULL,
    direction TEXT NOT NULL DEFAULT 'Both'
        CHECK (direction IN ('Both', 'Credit', 'Debit')),
    name TEXT NOT NULL DEFAULT ''
);

CREATE TABLE IF NOT EXISTS transactions (
    id TEXT PRIMARY KEY NOT NULL,
    value REAL NOT NULL DEFAULT 0.0,
    description TEXT NOT NULL DEFAULT '',
    support TEXT NOT NULL DEFAULT 'Cash'
        CHECK (support IN ('Cash', 'Cheque', 'Card', 'Transfer', 'Other')),
    date TEXT,
    movement TEXT NOT NULL DEFAULT 'Both'
        CHECK (movement IN ('Both', 'Credit', 'Debit')),
    is_visible INTEGER NOT NULL DEFAULT 1
        CHECK (is_visible IN (0, 1)),
    account_id TEXT,
    category_id TEXT,
    FOREIGN KEY (account_id) REFERENCES accounts(id)
        ON UPDATE CASCADE
        ON DELETE CASCADE,
    FOREIGN KEY (category_id) REFERENCES categories(id)
        ON UPDATE CASCADE
        ON DELETE SET NULL
);

CREATE TABLE IF NOT EXISTS debts (
    id TEXT PRIMARY KEY NOT NULL,
    name TEXT NOT NULL DEFAULT '',
    counterparty TEXT NOT NULL DEFAULT '',
    principal REAL NOT NULL DEFAULT 0.0
        CHECK (principal >= 0.0),
    remaining REAL NOT NULL DEFAULT 0.0
        CHECK (remaining >= 0.0 AND remaining <= principal),
    interest REAL NOT NULL DEFAULT 0.0
        CHECK (interest >= 0.0),
    start_date TEXT,
    end_date TEXT,
    recurrence TEXT NOT NULL DEFAULT 'Once'
        CHECK (recurrence IN ('Once', 'Daily', 'Weekly', 'Monthly', 'Quarterly', 'Yearly')),
    direction TEXT NOT NULL DEFAULT 'Debit'
        CHECK (direction IN ('Credit', 'Debit')),
    description TEXT NOT NULL DEFAULT '',
    account_id TEXT,
    FOREIGN KEY (account_id) REFERENCES accounts(id)
        ON UPDATE CASCADE
        ON DELETE CASCADE,
    CHECK (start_date IS NULL OR end_date IS NULL OR start_date <= end_date)
);

CREATE TABLE IF NOT EXISTS frequencies (
    id TEXT PRIMARY KEY NOT NULL,
    frequency TEXT NOT NULL DEFAULT 'Once'
        CHECK (frequency IN ('Custom', 'Once', 'Daily', 'Weekly', 'Monthly', 'Quarterly', 'Yearly')),
    date_format TEXT NOT NULL DEFAULT 'yyyy-MM-dd',
    custom_interval_days INTEGER NOT NULL DEFAULT 1
        CHECK (custom_interval_days >= 1)
);

-- Frequency owns a Transaction prototype in the current Model design.
CREATE TABLE IF NOT EXISTS frequency_prototypes (
    frequency_id TEXT PRIMARY KEY NOT NULL,
    prototype_id TEXT NOT NULL UNIQUE,
    value REAL NOT NULL DEFAULT 0.0,
    description TEXT NOT NULL DEFAULT '',
    support TEXT NOT NULL DEFAULT 'Cash'
        CHECK (support IN ('Cash', 'Cheque', 'Card', 'Transfer', 'Other')),
    date TEXT,
    movement TEXT NOT NULL DEFAULT 'Both'
        CHECK (movement IN ('Both', 'Credit', 'Debit')),
    is_visible INTEGER NOT NULL DEFAULT 1
        CHECK (is_visible IN (0, 1)),
    account_id TEXT,
    category_id TEXT,
    FOREIGN KEY (frequency_id) REFERENCES frequencies(id)
        ON UPDATE CASCADE
        ON DELETE CASCADE,
    FOREIGN KEY (account_id) REFERENCES accounts(id)
        ON UPDATE CASCADE
        ON DELETE SET NULL,
    FOREIGN KEY (category_id) REFERENCES categories(id)
        ON UPDATE CASCADE
        ON DELETE SET NULL
);

-- Optional key/value persistence for extra MetaData entries not modeled as columns.
-- Store the value as JSON text to preserve QVariant-like payloads.
CREATE TABLE IF NOT EXISTS metadata_entries (
    entity_type TEXT NOT NULL
        CHECK (entity_type IN (
            'Account',
            'Category',
            'Debt',
            'Frequency',
            'FrequencyPrototype',
            'Profile',
            'Transaction'
        )),
    entity_id TEXT NOT NULL,
    metadata_key TEXT NOT NULL,
    value_json TEXT NOT NULL,
    PRIMARY KEY (entity_type, entity_id, metadata_key)
);

CREATE INDEX IF NOT EXISTS idx_accounts_profile
    ON accounts(profile_id);

CREATE INDEX IF NOT EXISTS idx_categories_direction_name
    ON categories(direction, name);

CREATE INDEX IF NOT EXISTS idx_transactions_account_date
    ON transactions(account_id, date);

CREATE INDEX IF NOT EXISTS idx_transactions_category
    ON transactions(category_id);

CREATE INDEX IF NOT EXISTS idx_debts_account
    ON debts(account_id);

CREATE INDEX IF NOT EXISTS idx_frequency_prototypes_account
    ON frequency_prototypes(account_id);

CREATE INDEX IF NOT EXISTS idx_frequency_prototypes_category
    ON frequency_prototypes(category_id);

CREATE INDEX IF NOT EXISTS idx_metadata_entries_lookup
    ON metadata_entries(entity_type, entity_id);

COMMIT;
