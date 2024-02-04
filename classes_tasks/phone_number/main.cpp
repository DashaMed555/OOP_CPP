#include <iostream>
#include <string>

using namespace std;

string get_phone_number() {
    cout << "Enter a phone number: ";
    string str;
    getline(cin, str);
    cout << "\n";
    return str;
}

string parse_phone_number(const string& str) {
    string phone_number;
    char flag_brackets = 0;
    char flag_start = 0;
    for (int it = 0; str[it] != 0; ++it) {
        while (str[it] == ' ') {
            it++;
        }
        if (str[it] == '-' && str[it+1] == '-') {
            phone_number = "error";
            return phone_number;
        }
        if (str[it] == '-' && flag_start == 0) {
            phone_number = "error";
            return phone_number;
        }
        else
            flag_start = 1;
        if (str[it] == '-' && phone_number.length() == 12) {
            phone_number = "error";
            return phone_number;
        }
        if (str[it] == '(') {
            if (flag_brackets == 0)
                flag_brackets = 1;
            else {
                phone_number = "error";
                return phone_number;
            }
        }
        if (str[it] == ')') {
            if (flag_brackets == 1)
                flag_brackets = 0;
            else {
                phone_number = "error";
                return phone_number;
            }
        }
        if (str[it] == '+' && phone_number.empty())
            phone_number = "+";
        if (str[it] == '7' && phone_number == "+")
            phone_number = "+7";
        else if ('0' <= str[it] && str[it] <= '9' && not phone_number.empty())
            phone_number.push_back(str[it]);
        if (str[it] == '8' && phone_number.empty())
            phone_number = "+7";
    }
    if (phone_number.length() != 12)
        phone_number = "error";
    return phone_number;
}

int main() {
    string str = get_phone_number();
    string phone_number = parse_phone_number(str);
    printf("%s", phone_number.c_str());
    return 0;
}
