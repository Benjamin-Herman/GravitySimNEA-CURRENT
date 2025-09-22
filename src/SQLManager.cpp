#include "../headers/SQLManager.h"
#include <SQL/sqlite3.h>
#include <random>

void signup(const std::string& user, const std::string& pass) {
    sqlite3* DB;
    if (sqlite3_open("databases/users.db", &DB)) {
        std::cerr << "Can't open DB: " << sqlite3_errmsg(DB) << std::endl;
        return;
    }

    sqlite3_stmt* stmt;
    std::string sql = "SELECT 1 FROM users WHERE user = ? LIMIT 1;"; // just check existence

    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, user.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            std::cout << "User exists!\n";
        }
        else {
            std::cout << "User does not exist!\n";
        }

        sqlite3_finalize(stmt);
    }
    else {
        std::cerr << "Failed to prepare statement\n";
    }


    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, user.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, pass.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            std::cout << "Signup successful!\n";
        }
        else {
            std::cout << "Error inserting data.\n";
        }
    }
    else {
        std::cerr << "Error preparing INSERT statement\n";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}


void login(const std::string& user, const std::string& pass) {
    sqlite3* DB;
    if (sqlite3_open("databases/users.db", &DB)) {
        std::cerr << "Can't open DB: " << sqlite3_errmsg(DB) << std::endl;
        return;
    }

    std::string sql = "SELECT * FROM users WHERE user = ? AND pass = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, user.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, pass.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            std::cout << "Login successful! Welcome " << user << "\n";
        }
        else {
            std::cout << "Invalid username or password.\n";
        }
    }
    else {
        std::cerr << "Error preparing SELECT statement\n";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}


int SQL::init()
{
    cont = true;
    sqlite3* DB;
    int exit = sqlite3_open("databases/users.db", &DB);

    if (exit) {
        std::cerr << "Error opening DB: " << sqlite3_errmsg(DB) << std::endl;
        return -1;
    }
    else {
        //std::cout << "Opened Database Successfully!" << std::endl;
    }

    const char* sql =
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "user TEXT NOT NULL, "
        "pass TEXT NOT NULL, "
        "settings_path TEXT NOT NULL DEFAULT '');";

    char* errorMessage = nullptr;
    exit = sqlite3_exec(DB, sql, nullptr, nullptr, &errorMessage);

    if (exit != SQLITE_OK) {
        std::cerr << "Error creating table: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }
    else {
        //std::cout << "Table ready!" << std::endl;
    }

    sqlite3_close(DB);
    return 0;
}


void SQL::loop() {

    //std::cout << hashText("HIII") << "\n";


    std::string user;
    std::cout << "ENTER USERNAME\n";
    std::cin >> user;

    std::string pass;
    std::cout << "ENTER PASSWORD\n";
    std::cin >> pass;

    pass = hashText(pass);

    std::string ls;
    std::cout << "Do You Want Log In Or Sign Up, L Login, S SignUp\n";
    std::cin >> ls;

    if (ls == "L" || ls == "l") {
        login(user, pass);
    }
    else if (ls == "S" || ls == "s") {
        signup(user, pass);
    }
    else {
        std::cout << "INVALID\n";
    }

    std::string c;
    std::cout << "Do You Want To Go Again, Y yes, N no\n";
    std::cin >> c;

    
    if (c == "N" || c == "n") {
        cont = false;
    }
    else if (c != "Y" && c != "y") {
        std::cout << "INVALID\n";
    }

}

std::string SQL::hashText(std::string toHash) {
    int hashLength = 20; // length of final hash
    const char chars[] =
        " !\"#$%&'()*+,-./"
        "0123456789:;<=>?@"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
        "abcdefghijklmnopqrstuvwxyz{|}~";
    std::string hashed; // final string to return
    unsigned seed; // value for seed to get same result if seed is same
    int prev = 1219382019; // big string of random stuff for start
    int count = 1839 * toHash.length(); // length and const to make a start to the randomness
    int firstNumSet = 1;
    for (char c : toHash) {
        int num = static_cast<int>(c) * count; // gets a value based on char
        num = (num * 1231982392197429) % 12039812309;
        seed = (num * prev) % 12309172; // sets seed
        prev = num; // sets a previous so that if string is xxx it is not the same values
        std::mt19937 rng(seed); // sets seed to random. no idea what the mt19937 thing is
        std::uniform_int_distribution<int> dist(1, 10000000); // gets random number
        int randomNumber = dist(rng);
        firstNumSet ^= randomNumber; // use XOR instead of *= so it doesn’t blow up
        count += (randomNumber % 123023); // sets the count value
    }

    // now create one RNG using final seed
    std::mt19937 rng(firstNumSet);
    std::uniform_int_distribution<int> dist(0, 95);

    for (int i = 0; i < hashLength; i++) {
        int randomNumber = dist(rng); // different every call
        hashed += (chars[randomNumber]); // pick char from array
    }

    return hashed;
}

