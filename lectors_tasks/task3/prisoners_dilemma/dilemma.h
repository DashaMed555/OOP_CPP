#ifndef PRISONERS_DILEMMA_DILEMMA_H
#define PRISONERS_DILEMMA_DILEMMA_H

#include <map>
#include "gtest/gtest.h"

#include "definitions.h"

class Strategy {
    double points = 0;
    std::string name;
protected:
    std::vector<Memory> memory;

    static char cooperate_d();
    static char defect_d();
    static char random_d();
    [[nodiscard]]
    char repeat_d() const;
    [[nodiscard]]
    char switch_d() const;

    [[nodiscard]]
    bool cooperate_c() const;
    [[nodiscard]]
    bool cooperate_all_c() const;
    [[nodiscard]]
    bool defect_c() const;
    [[nodiscard]]
    bool defect_all_c() const;
    [[nodiscard]]
    bool successful_round_c() const;
    [[nodiscard]]
    bool neutral_round_c() const;
    [[nodiscard]]
    bool unsuccessful_round_c() const;
public:
    explicit Strategy(std::string& strategy_name);
    ~Strategy();

    [[nodiscard]]
    virtual char make_a_choice() const = 0;
    void add_points(double points);
    void find_out(const std::array<char, 2>& answers, char result, char my_choice);
    [[nodiscard]]
    double get_points() const;
    std::string get_name() const;
};

class StrategyAllC : public Strategy {
public:
    StrategyAllC(std::string& strategy_name) : Strategy(strategy_name) {};
    [[nodiscard]]
    char make_a_choice() const override;
};

class StrategyAllD : public Strategy {
public:
    StrategyAllD(std::string& strategy_name) : Strategy(strategy_name) {};
    [[nodiscard]]
    char make_a_choice() const override;
};

class StrategyRandom : public Strategy {
public:
    StrategyRandom(std::string& strategy_name) : Strategy(strategy_name) {};
    [[nodiscard]]
    char make_a_choice() const override;
};

class StrategyWinStay : public Strategy {
public:
    StrategyWinStay(std::string& strategy_name, char initial_decision, std::vector<Behavior>& behavior);
    [[nodiscard]]
    char make_a_choice() const override;
private:
    char initial_decision;
    std::vector<Behavior> behavior;
};

class StrategyMeta : public Strategy {
public:
    StrategyMeta(std::string& strategy_name, std::vector<Strategies>& strategies,
                 std::map<std::string, char>& initial_decision,
                 std::map<std::string, std::vector<Behavior>>& behavior);
    [[nodiscard]]
    char make_a_choice() const override;
private:
    std::vector<Strategies> strategies;
    std::map<std::string, std::vector<Behavior>> behavior;
    std::map<std::string, char> initial_decision;
};

class StrategyRevenge : public Strategy {
public:
    StrategyRevenge(std::string& strategy_name, char initial_decision, std::vector<Behavior>& behavior);
    [[nodiscard]]
    char make_a_choice() const override;
private:
    char initial_decision;
    std::vector<Behavior> behavior;
};

class StrategyFactory {
public:
    static Strategy* create_strategy(std::string& filepath_to_configs, std::string& name);
};

class Simulator {
    std::vector<std::string> names;
    std::string configs = "configs";
    std::array<std::array<double, 3>, 8> matrix = {
            std::array<double, 3>{7, 7, 7},
            std::array<double, 3>{3, 3, 9},
            std::array<double, 3>{3, 9, 3},
            std::array<double, 3>{9, 3, 3},
            std::array<double, 3>{0, 5, 5},
            std::array<double, 3>{5, 0, 5},
            std::array<double, 3>{5, 5, 0},
            std::array<double, 3>{1, 1, 1}};
    long steps = 3;
    char mode = DETAILED;
public:
    Simulator();
    Simulator(std::vector<std::string>& names, std::string& configs, std::array<std::array<double, 3>, 8>& matrix,
              long steps, char mode);
    void start();
private:
    void detailed_game();
    void fast_game();
    void tournament();
    static void define_threes(std::vector<Strategy*> strategies, std::vector<std::array<Strategy*, 3>>& threes);
    static char ask_strategy(const Strategy& s);
    void calculate_points(std::array<char, 3>& answers, std::array<double, 3>& results);
    static void get_others_answers(std::array<char, 3>& answers, int strategy_num, std::array<char, 2>& others_answers);
    static void get_others_results(std::array<double, 3>& results, int strategy_num, std::array<double, 2>& others_results);
    static char get_round_info(std::array<double, 2>& results, double result);
    static void tell_strategies_info(Strategy& s, std::array<char, 2>& answers, char round_info, char choice);
    static void give_points(Strategy& s, double points);
    static std::ostream& print_decision(std::ostream& stream, char decision);
    FRIEND_TEST(Policeman, DefineThrees);
};

#endif
