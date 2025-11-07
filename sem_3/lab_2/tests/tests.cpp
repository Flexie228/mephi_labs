#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "../include/HashTable.hpp"

#include <string>

TEST_CASE("HashTable Tests") {

SECTION("Basic Operations") {
    HashTable<std::string, int> table;

    SECTION("Empty table") {
        REQUIRE(table.isEmpty());
        REQUIRE(table.getSize() == 0);
        REQUIRE(table.find("nonexistent") == nullptr);
    }

    SECTION("Insert() and find()") {
        table.insert("one", 1);
        table.insert("two", 2);
        table.insert("three", 3);

        REQUIRE(!table.isEmpty());
        REQUIRE(table.getSize() == 3);

        auto* value1 = table.find("one");
        auto* value2 = table.find("two");
        auto* value3 = table.find("three");

        REQUIRE(value1 != nullptr);
        REQUIRE(*value1 == 1);
        REQUIRE(value2 != nullptr);
        REQUIRE(*value2 == 2);
        REQUIRE(value3 != nullptr);
        REQUIRE(*value3 == 3);
    }

    SECTION("Update value with new by existing key") {
        table.insert("key", 10);
        table.insert("key", 20); // value update

        REQUIRE(table.getSize() == 1);
        auto* value = table.find("key");
        REQUIRE(value != nullptr);
        REQUIRE(*value == 20);
    }

    SECTION("Erase() elements") {
        table.insert("one", 1);
        table.insert("two", 2);

        table.erase("one");
        table.erase("nonexistent");
        REQUIRE(table.getSize() == 1);
        REQUIRE(table.find("one") == nullptr);
        REQUIRE(table.find("two") != nullptr);

        table.erase("two");
        REQUIRE(table.isEmpty());
        REQUIRE(table.find("two") == nullptr);
    }
}

SECTION("Operator []") {
    HashTable<std::string, int> table;

    SECTION("Creation and access") {
        table["first"] = 100;
        table["second"] = 200;

        REQUIRE(table.getSize() == 2);
        REQUIRE(table["first"] == 100);
        REQUIRE(table["second"] == 200);
    }

    SECTION("Modification by []") {
        table["key"] = 10;
        table["key"] = 20;

        REQUIRE(table.getSize() == 1);
        REQUIRE(table["key"] == 20);
    }

    SECTION("Default value for new keys") {
        int& value = table["new_key"];
        REQUIRE(table["new_key"] == 0);
        value = 42;

        REQUIRE(table.getSize() == 1);
    }
}

SECTION("Rehashing") {
    SECTION("Capacity increases with rehashing") {
        HashTable<int, std::string> table;
        size_t startCapacity = table.getCapacity();

        // Inserting elements until rehashing should occur
        for (int i = 0; i < 45; i++) {
            table.insert(i, "value" + std::to_string(i));
        }
        REQUIRE(table.getCapacity() > startCapacity);
        REQUIRE(table.getCapacity() == 97);
        startCapacity = table.getCapacity();

        for (int i = 45; i < 100; i++) {
            table.insert(i, "value" + std::to_string(i));
        }
        REQUIRE(table.getCapacity() > startCapacity);
        REQUIRE(table.getCapacity() == 193);

        // Verify all elements are still accessible
        for (int i = 0; i < 100; ++i) {
            auto* value = table.find(i);
            REQUIRE(value != nullptr);
            REQUIRE(*value == "value" + std::to_string(i));
        }
        REQUIRE(table.getSize() == 100);
    }

    SECTION("rehash() with large capacity") {
        HashTable<int, int> table;
        constexpr int NUM_ELEMENTS = 1000000;

        SECTION("Insert many elements") {
            for (int i = 0; i < NUM_ELEMENTS; ++i) {
                table.insert(i, i * 2);
            }

            REQUIRE(table.getSize() == NUM_ELEMENTS);

            for (int i = 0; i < NUM_ELEMENTS; ++i) {
                auto* value = table.find(i);
                REQUIRE(value != nullptr);
                REQUIRE(*value == i * 2);
            }
        }

        SECTION("Mixed operations") {
            // Insert
            for (int i = 0; i < NUM_ELEMENTS; ++i) {
                table.insert(i, i);
            }

            for (int i = 0; i < NUM_ELEMENTS; i += 2) {
                table.insert(i, i + 1);
            }
            REQUIRE(table.getSize() == NUM_ELEMENTS);

            // Delete every 3rd element
            for (int i = 0; i < NUM_ELEMENTS; i += 3) {
                table.erase(i);
            }

            // Expecting size: 100000 - [100000/3] = 100 - 34 = 66
            int expectedSize = NUM_ELEMENTS - ((NUM_ELEMENTS + 2) / 3);
            REQUIRE(table.getSize() == expectedSize);

            for (int i = 0; i < NUM_ELEMENTS; ++i) {
                auto* value = table.find(i);
                if (i % 3 == 0) {
                    REQUIRE(value == nullptr);
                } else {
                    REQUIRE(value != nullptr);
                    if (i % 2 == 0) {
                        REQUIRE(*value == i + 1);
                    } else {
                        REQUIRE(*value == i);
                    }
                }
            }
        }
    }
}

SECTION("Copy Operations") {
    HashTable<std::string, double> original;
    original.insert("pi", 3.14159);
    original.insert("e", 2.71828);

    SECTION("Copy constructor") {
        HashTable<std::string, double> copy(original);

        REQUIRE(copy.getSize() == original.getSize());
        REQUIRE(*copy.find("pi") == 3.14159);
        REQUIRE(*copy.find("e") == 2.71828);

        original.insert("pi", 3.14);
        REQUIRE(*copy.find("pi") == 3.14159);
    }

    SECTION("Copy assignment") {
        HashTable<std::string, double> copy;
        copy = original;

        REQUIRE(copy.getSize() == original.getSize());
        REQUIRE(*copy.find("pi") == 3.14159);
        REQUIRE(*copy.find("e") == 2.71828);
    }
}

SECTION("Converting Constructor") {
    std::vector<std::pair<std::string, std::string>> dataBase;
    dataBase.reserve(10000);
    for (size_t i = 0; i < 10000; i++) {
        dataBase.emplace_back(std::to_string(i), std::to_string(i) + "value");
    }
    const HashTable table(dataBase);
    REQUIRE(table.getSize() == 10000);

    for (size_t i = 0; i < 10000; i++) {
        auto* value = table.find(std::to_string(i));
        REQUIRE(value != nullptr);
        REQUIRE(*value == std::to_string(i) + "value");
    }
}

SECTION("Custom Types") {
    SECTION("String keys with custom class values") {
        struct Person {
            std::string name;
            int age;

            bool operator==(const Person& other) const {
                return name == other.name && age == other.age;
            }
        };

        HashTable<std::string, Person> table;
        table.insert("alice", Person{"Alice", 30});
        table.insert("alexey", Person{"Alexey", 31});

        auto* alice = table.find("alice");
        REQUIRE(alice != nullptr);
        REQUIRE(alice->name == "Alice");
        REQUIRE(alice->age == 30);

        auto* alexey = table.find("alexey");
        REQUIRE(alexey != nullptr);
        REQUIRE(alexey->name == "Alexey");
        REQUIRE(alexey->age == 31);
    }
}

}