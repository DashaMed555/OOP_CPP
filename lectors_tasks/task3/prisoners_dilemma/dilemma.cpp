#include <ctime>
#include <iostream>
#include <fstream>
#include "dilemma.h"
#include "parser.h"

std::set<std::string> supported_strategies = {"all_c", "all_d", "random",
                                              "win_stay", "meta", "revenge"};

Strategy::Strategy(std::string& strategy_name) {
    name = std::move(strategy_name);
    std::cout << name + " is here!" << std::endl;
}

Strategy::~Strategy() {
    std::cout << name + " is left!" << std::endl;
}

void Strategy::add_points(double new_points) {
    points += new_points;
}

void Strategy::find_out(const std::array<char, 2>& answers, char result, char my_choice) {
    Memory new_event = {{my_choice, result}, answers};
    memory.push_back(new_event);
}

double Strategy::get_points() const {
    return points;
}

std::string Strategy::get_name() const {
    return name;
}

char Strategy::cooperate_d() {
    return COOPERATE;
}

char Strategy::defect_d() {
    return DEFECT;
}

char Strategy::random_d() {
    return rand() % 2;
}

char Strategy::repeat_d() const {
    return memory.back().my_choice.decision;
}

char Strategy::switch_d() const {
    return (char)((memory.back().my_choice.decision + 1) % 2);
}

bool Strategy::cooperate_c() const {
    char a_decision = memory.back().others_choices[0];
    char b_decision = memory.back().others_choices[1];
    return a_decision == COOPERATE || b_decision == COOPERATE;
}

bool Strategy::cooperate_all_c() const {
    char a_decision = memory.back().others_choices[0];
    char b_decision = memory.back().others_choices[1];
    return a_decision == COOPERATE && b_decision == COOPERATE;
}

bool Strategy::defect_c() const {
    char a_decision = memory.back().others_choices[0];
    char b_decision = memory.back().others_choices[1];
    return a_decision == DEFECT || b_decision == DEFECT;
}

bool Strategy::defect_all_c() const {
    char a_decision = memory.back().others_choices[0];
    char b_decision = memory.back().others_choices[1];
    return a_decision == DEFECT && b_decision == DEFECT;
}

bool Strategy::successful_round_c() const {
    return memory.back().my_choice.round_info == SUCCESS;
}

bool Strategy::neutral_round_c() const {
    return memory.back().my_choice.round_info == EVENT;
}

bool Strategy::unsuccessful_round_c() const {
    return memory.back().my_choice.round_info == SETBACK;
}

char StrategyAllC::make_a_choice() const {
    return cooperate_d();
}

char StrategyAllD::make_a_choice() const {
    return defect_d();
}

char StrategyRandom::make_a_choice() const {
    return random_d();
}

StrategyWinStay::StrategyWinStay(std::string& strategy_name, char initial_decision, std::vector<Behavior>& behavior)
        : Strategy(strategy_name) {
    this->initial_decision = initial_decision;
    this->behavior = std::move(behavior);
}

char StrategyWinStay::make_a_choice() const {
    if (memory.empty())
        switch (initial_decision) {
            case COOPERATE:
                return cooperate_d();
            case DEFECT:
                return defect_d();
        }
    else for (auto b : behavior)
        if (b.condition)
            return b.return_d;
    return random_d();
}

StrategyMeta::StrategyMeta(std::string& strategy_name, std::vector<Strategies>& strategies,
                           std::map<std::string, char>& initial_decision,
                           std::map<std::string, std::vector<Behavior>>& behavior)
                           : Strategy(strategy_name) {
    this->strategies = std::move(strategies);
    this->behavior = std::move(behavior);
    this->initial_decision = std::move(initial_decision);
}

char StrategyMeta::make_a_choice() const {
    double random_double = (double)rand() / RAND_MAX;
    double total = 0;
    std::string chosen_name;
    for (const auto& s : strategies) {
        if (random_double <= total + s.probability) {
            chosen_name = s.name;
            break;
        }
        total += s.probability;
    }

    if (memory.empty())
        switch (initial_decision.at(chosen_name)) {
            case COOPERATE:
                return cooperate_d();
            case DEFECT:
                return defect_d();
        }
    else for (auto b : behavior.at(chosen_name))
            if (b.condition)
                return b.return_d;
    return random_d();
}

StrategyRevenge::StrategyRevenge(std::string& strategyName, char initial_decision, std::vector<Behavior>& behavior)
        : Strategy(strategyName) {
    this->initial_decision = initial_decision;
    this->behavior = std::move(behavior);
}

char StrategyRevenge::make_a_choice() const {
    if (memory.empty())
        switch (initial_decision) {
            case COOPERATE:
                return cooperate_d();
            case DEFECT:
                return defect_d();
        }
    else for (auto b : behavior)
            if (b.condition)
                return b.return_d;
    return random_d();
}

Strategy* StrategyFactory::create_strategy(std::string& filepath_to_configs, std::string& name) {
    if (name == "all_c")
        return new StrategyAllC(name);
    if (name == "all_d")
        return new StrategyAllD(name);
    if (name == "random")
        return new StrategyRandom(name);
    std::ifstream stream;
    stream.exceptions( std::ifstream::failbit | std::ifstream::badbit );
    try {
        stream.open((filepath_to_configs + std::string("/") + name + std::string(".txt")));
        std::string line;
        if (!stream.eof()) {
            stream >> line;
            if (line == "strategy:") {
                char initial_decision;
                std::vector<Behavior> behavior;
                parse_strategy(stream, initial_decision, behavior);
                if (name == "win_stay")
                    return new StrategyWinStay(name, initial_decision, behavior);
                if (name == "revenge")
                    return new StrategyRevenge(name, initial_decision, behavior);
            }
            else if (line == "meta_strategy:") {
                std::vector<Strategies> strategies;
                std::map<std::string, char> initial_decision;
                std::map<std::string, std::vector<Behavior>> behavior;
                parse_meta_strategy(stream, strategies, initial_decision, behavior);
                if (name == "meta")
                    return new StrategyMeta(name, strategies, initial_decision, behavior);
            }
        }
        stream.close();
    }
    catch (std::ifstream::failure& e) {
        std::cerr << "Caught an ifstream::failure." << std::endl;
        std::cerr << "Explanatory string: " << e.what() << std::endl;
        std::cerr << "Error code: " << e.code() << std::endl;
    }
    return nullptr;
}

Simulator::Simulator() {
    srand(time(nullptr));
    names = {"all_c", "all_d", "random"};
}

Simulator::Simulator(std::vector<std::string>& names, std::string& configs, std::array<std::array<double, 3>, 8>& matrix,
                     long num_steps, char game_mode) {
    srand(time(nullptr));
    this->names = names;
    this->configs = configs;
    this->matrix = matrix;
    if (num_steps > 0)
        steps = num_steps;
    if (DETAILED <= game_mode && game_mode <= TOURNAMENT)
        mode = game_mode;
}

void Simulator::define_threes(std::vector<Strategy*> strategies, std::vector<std::array<Strategy*, 3>>& threes) {
    for (int s1 = 0; s1 < strategies.size(); ++s1)
        for (int s2 = s1 + 1; s2 < strategies.size(); ++s2)
            for (int s3 = s2 + 1; s3 < strategies.size(); ++s3)
                threes.push_back({strategies[s1], strategies[s2], strategies[s3]});
}

char Simulator::ask_strategy(const Strategy& s) {
    return s.make_a_choice();
}

void Simulator::calculate_points(std::array<char, 3>& answers, std::array<double, 3>& results) {
    int i = 4 * answers[0] + 2 * answers[1] + answers[2];
    for (int j = 0; j < 3; ++j)
        results[j] = matrix[i][j];
}

void Simulator::get_others_answers(std::array<char, 3>& answers, int strategy_num, std::array<char, 2>& others_answers) {
    int k = 0;
    for (int j = 0; j < 3; ++j)
        if (j != strategy_num)
            others_answers[k++] = answers[j];
}

void Simulator::get_others_results(std::array<double, 3>& results, int strategy_num, std::array<double, 2>& others_results) {
    int k = 0;
    for (int j = 0; j < 3; ++j)
        if (j != strategy_num)
            others_results[k++] = results[j];
}

char Simulator::get_round_info(std::array<double, 2>& results, const double result) {
    if (std::all_of(results.begin(), results.end(), [&result](auto& elem) {return result > elem;}))
        return SUCCESSFUL_ROUND;
    else if (std::all_of(results.begin(), results.end(), [&result](auto& elem) {return result < elem;}))
        return UNSUCCESSFUL_ROUND;
    else
        return NEUTRAL_ROUND;
}

void Simulator::tell_strategies_info(Strategy& s, std::array<char, 2>& answers, char round_info, char choice) {
    s.find_out(answers, round_info, choice);
}

void Simulator::give_points(Strategy& s, double points) {
    s.add_points(points);
}

void Simulator::detailed_game() {
    StrategyFactory factory;
    std::vector<Strategy*> strategies;
    strategies.reserve(names.size());
    for (auto name : names)
        strategies.push_back(factory.create_strategy(configs, name));
    std::string line;
    std::cout << "\nPress a button to start new game!" << std::endl;
    std::cin >> line;
    if (line == "quit")
        return;
    std::cout << "\n----------------------------- NEW GAME -----------------------------" << std::endl;
    std::cout << "Now " << strategies[0]->get_name() << ", " << strategies[1]->get_name() << " and " << strategies[2]->get_name() << " are ready to compete!" << std::endl;
    std::cout << "--------------------------------------------------------------------" << std::endl;
    for (long s = 0; s < steps; ++s) {
        std::array<char, 3> answers;
        for (int i = 0; i < 3; ++i) {
            answers[i] = ask_strategy(*strategies[i]);
            std::cout << "\nPress a button to know the answer of " << strategies[i]->get_name() << " strategy!" << std::endl;
            std::cin >> line;
            if (line == "quit")
                return;
            std::cout << "\nThe answer is ";
            print_decision(std::cout, answers[i]) << "!" << std::endl;
        }
        std::array<double, 3> results;
        calculate_points(answers, results);
        std::cout << "\nPress a button to know the points of the round!" << std::endl;
        std::cin >> line;
        if (line == "quit")
            return;
        std::cout << std::endl;
        for (int i = 0; i < 3; ++i)
            std::cout << "Strategy " << strategies[i]->get_name() << " gets " << results[i] << " points!" << std::endl;
        for (int i = 0; i < 3; ++i) {
            std::array<char, 2> others_answers;
            get_others_answers(answers, i, others_answers);
            std::array<double, 2> others_results;
            get_others_results(results, i, others_results);
            char round_info = get_round_info(others_results, results[i]);
            tell_strategies_info(*strategies[i], others_answers, round_info, answers[i]);
            give_points(*strategies[i], results[i]);
        }
        if (s != steps - 1) {
            std::cout << "\nIt's the time for the next round!\n" << std::endl;
            std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
        }
    }
    std::cout << "\nGame is over! Press a button to know the results!" << std::endl;
    std::cin >> line;
    if (line == "quit")
        return;
    for (int i = 0; i < 3; ++i)
        std::cout << "\nStrategy " << strategies[i]->get_name() << " has " << strategies[i]->get_points() << " points!" << std::endl;
    std::cout << std::endl;
}

void Simulator::fast_game() {
    StrategyFactory factory;
    std::vector<Strategy*> strategies;
    strategies.reserve(names.size());
    for (auto name : names)
        strategies.push_back(factory.create_strategy(configs, name));
    for (long s = 0; s < steps; ++s) {
        std::array<char, 3> answers;
        for (int i = 0; i < 3; ++i)
            answers[i] = ask_strategy(*strategies[i]);
        std::array<double, 3> results;
        calculate_points(answers, results);
        for (int i = 0; i < 3; ++i) {
            std::array<char, 2> others_answers;
            get_others_answers(answers, i, others_answers);
            std::array<double, 2> others_results;
            get_others_results(results, i, others_results);
            char round_info = get_round_info(others_results, results[i]);
            tell_strategies_info(*strategies[i], others_answers, round_info, answers[i]);
            give_points(*strategies[i], results[i]);
        }
    }
    for (int i = 0; i < 3; ++i)
        std::cout << "\nStrategy " << strategies[i]->get_name() << " has " << strategies[i]->get_points() << " points!" << std::endl;
}

void Simulator::tournament() {
    StrategyFactory factory;
    std::vector<Strategy*> strategies;
    strategies.reserve(names.size());
    for (auto name : names)
        strategies.push_back(factory.create_strategy(configs, name));
    std::vector<std::array<Strategy*, 3>> threes;
    define_threes(strategies, threes);
    int competition_num = 1;
    for (auto& three : threes) {
        std::cout << "---------------------- " << competition_num << " COMPETITION ----------------------" << std::endl;
        std::array<double, 3> points = {0, 0, 0};
        for (long s = 0; s < steps; ++s) {
            std::array<char, 3> answers;
            for (int i = 0; i < 3; ++i)
                answers[i] = ask_strategy(*three[i]);
            std::array<double, 3> results;
            calculate_points(answers, results);
            for (int i = 0; i < 3; ++i) {
                std::array<char, 2> others_answers;
                get_others_answers(answers, i, others_answers);
                std::array<double, 2> others_results;
                get_others_results(results, i, others_results);
                char round_info = get_round_info(others_results, results[i]);
                tell_strategies_info(*three[i], others_answers, round_info, answers[i]);
                give_points(*three[i], results[i]);
                points[i] += results[i];
            }
        }
        std::cout << "Strategies " << three[0]->get_name() << ", " << three[1]->get_name() << " and " << three[2]->get_name() << " have competed!" << std::endl;
        std::cout << "In this competition..." << std::endl;
        for (int i = 0; i < 3; ++i)
            std::cout << "Strategy " << three[i]->get_name() << " has " << points[i] << " points!" << std::endl;
        std::cout << std::endl;
        competition_num++;
    }
    std::cout << "---------------------- FINAL PROTOCOL ----------------------" << std::endl;
    Strategy* winner = strategies[0];
    for (auto& strategy : strategies) {
        std::cout << "Strategy " << strategy->get_name() << " has " << strategy->get_points() << " points!" << std::endl;
        if (winner->get_points() < strategy->get_points())
            winner = strategy;
    }
    std::cout << "---------------------- THE WINNER IS " << winner->get_name() << " ----------------------" << std::endl;
}

void Simulator::start() {
    if (mode == DETAILED)
        detailed_game();
    else if (mode == FAST)
        fast_game();
    else if (mode == TOURNAMENT)
        tournament();
}

std::ostream& Simulator::print_decision(std::ostream& stream, char decision) {
    if (decision == COOPERATE)
        stream << "COOPERATE";
    else
        stream << "DEFECT";
    return stream;
}
