#include "test_runner.h"

#include <algorithm>
#include <string>
#include <vector>
#include "profile.h"

template <typename String>
using Group = vector<String>;

template <typename String>
using Char = typename String::value_type;

template <typename String>
vector<Group<String>> GroupHeavyStrings(vector<String> strings) {
    vector<Group<String>> groups;
    set<set<Char<String>>> unique_combo;
    map<set<Char<String>>, int> mapper;
    int count = 0;
    for (auto& str : strings) {
        set<Char<String>> combo;
        for (auto& ch : str) {
            combo.insert(ch);
        }
        if (!unique_combo.contains(combo)) {
            unique_combo.insert(combo);
            Group<String> new_group;
            groups.push_back(new_group);
            groups[count].push_back(str);
            mapper[combo] = count++;
        }
        else
            groups[mapper[combo]].push_back(str);
    }
    return groups;
}

void TestGroupingABC() {
    vector<string> strings = {"caab", "abc", "cccc", "bacc", "c"};
    auto groups = GroupHeavyStrings(strings);
    ASSERT_EQUAL(groups.size(), 2);
    sort(begin(groups), end(groups));
    ASSERT_EQUAL(groups[0], vector<string>({"caab", "abc", "bacc"}));
    ASSERT_EQUAL(groups[1], vector<string>({"cccc", "c"}));
}

void TestGroupingReal() {
    vector<string> strings = {"law", "port", "top", "laptop", "pot", "paloalto", "wall", "awl"};
    auto groups = GroupHeavyStrings(strings);
    ASSERT_EQUAL(groups.size(), 4);
    sort(begin(groups), end(groups));
    ASSERT_EQUAL(groups[0], vector<string>({"laptop", "paloalto"}));
    ASSERT_EQUAL(groups[1], vector<string>({"law", "wall", "awl"}));
    ASSERT_EQUAL(groups[2], vector<string>({"port"}));
    ASSERT_EQUAL(groups[3], vector<string>({"top", "pot"}));
}

int main() {
    TestRunner tr;
    {
        LOG_DURATION("TestGroupingABC")
        RUN_TEST(tr, TestGroupingABC);
    }
    {
        LOG_DURATION("TestGroupingReal")
        RUN_TEST(tr, TestGroupingReal);
    }
    return 0;
}
