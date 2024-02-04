#include <iostream>
#include <numeric>

#define January 0
#define February 1
#define March 2
#define April 3
#define May 4
#define June 5
#define July 6
#define August 7
#define September 8
#define October 9
#define November 10
#define December 11

using namespace std;

typedef struct Days {
    vector<double> d;
    int month;
    int year;
} Days;

typedef struct Months {
    vector<Days> m;
    int year;
} Months;

typedef vector<Months> Years;

static bool leap(int year) {
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

static int get_days(int month, bool leap) {
    if (month == February && not leap)
        return 28;
    if (month == February)
        return 29;
    if (month == April || month == June || month == September || month == November)
        return 30;
    if (month == January || month == March || month == May || month == July || month == August || month == October || month == December)
        return 31;
    perror("Invalid date.");
    return 0;
}

class Date{
public:
    Date(int year = 0, int month = January, int day = 0) {
        this->year = year;
        this->month = month;
        this->day = day;
    }

    [[nodiscard]]
    int get_year() const {
        return this->year;
    }

    [[nodiscard]]
    int get_month() const {
        return this->month;
    }

    [[nodiscard]]
    int get_day() const {
        return this->day;
    }

    friend int operator-(const Date &from, const Date &to) {
        int days_num = 0;
        for (int y = from.year; y <= to.year; ++y) {
            if (y != from.year && y != to.year) {
                if (leap(y))
                    days_num += 366;
                else
                    days_num += 365;
            }
            else for (int m = y == from.year ? from.month : 0; m <= (y == to.year ? to.month : 11); ++m)
                    if ((y != from.year || m != from.month) && (m != to.month || y != to.year))
                        days_num += get_days(m, y);
                    else {
                        int from_d, to_d;
                        if (from.year == to.year && from.month == to.month) {
                            from_d = from.get_day();
                            to_d = to.get_day();
                        }
                        else if (y == from.year && m == from.month) {
                            from_d = from.get_day();
                            to_d = get_days(m, y) - 1;
                        }
                        else {
                            from_d = 0;
                            to_d = to.get_day();
                        }
                        days_num += to_d - from_d + 1;
                    }
        }
        return days_num;
    }

    friend istream& operator>>(istream& in, Date& date) {
        in >> date.year;
        date.year--;
        check_next_symbol(in);
        in >> date.month;
        date.month--;
        check_next_symbol(in);
        in >> date.day;
        date.day--;
        return in;
    }

private:
    int year;
    int month;
    int day;

    static void check_next_symbol(istream& in) {
        if (in.peek() != '-') {
            perror("Invalid format.");
            exit(1);
        }
        in.ignore(1);
    }
};

class BigCalendar {
public:
    BigCalendar(int year_from = 1999, int year_to = 2098) {
        this->year_from = year_from;
        this->year_to = year_to;
        for (int y = 0; y <= year_to - year_from; ++y) {
            Months mv;
            mv.year = y;
            for (int m = January; m <= December; ++m) {
                vector<double> dv(get_days(m, leap(y)));
                Days ds = {dv, m, y};
                mv.m.push_back(ds);
            }
            this->dates.push_back(mv);
        }
    }

    void earn(Date &from, Date &to, int value) {
        double income = (double)value / (from - to);
        BigCalendar *calendar = this;
        for_each(this->dates.begin() + (from.get_year() - this->year_from), this->dates.end() - (this->year_to - to.get_year()),
                 [calendar, from, to, income](auto &year){
            bool begin_bool = year.year == from.get_year() - calendar->year_from;
            bool end_bool = year.year == to.get_year() - calendar->year_from;
            int m_begin = 0;
            if (begin_bool)
                m_begin = from.get_month();
            int m_end = 0;
            if (end_bool)
                m_end = 11 - to.get_month();
            for_each(year.m.begin() + m_begin, year.m.end() - m_end,
                              [from, to, income, begin_bool, end_bool](auto &month){
                int d_begin = 0;
                if (begin_bool && month.month == from.get_month())
                    d_begin = from.get_day();
                int d_end = 0;
                if (end_bool && month.month == to.get_month())
                    d_end = month.d.size() - to.get_day() - 1;
                for_each(month.d.begin() + d_begin, month.d.end() - d_end, [income](auto &day){
                    day += income;
                });
            });
        });
    }

    double compute_income(Date &from, Date &to) {
        BigCalendar *calendar = this;
        return accumulate(this->dates.begin() + (from.get_year() - this->year_from), this->dates.end() - (this->year_to - to.get_year()), 0.,
                          [calendar, from, to](auto acc, auto &year){
            bool begin_bool = year.year == from.get_year() - calendar->year_from;
            bool end_bool = year.year == to.get_year() - calendar->year_from;
            int m_begin = 0;
            if (begin_bool)
                m_begin = from.get_month();
            int m_end = 0;
            if (end_bool)
                m_end = 11 - to.get_month();
            return accumulate(year.m.begin() + m_begin, year.m.end() - m_end, acc,
                              [from, to, begin_bool, end_bool](auto acc_, auto &month){
                int d_begin = 0;
                if (begin_bool && month.month == from.get_month())
                    d_begin = from.get_day();
                int d_end = 0;
                if (end_bool && month.month == to.get_month())
                    d_end = month.d.size() - to.get_day() - 1;
                return accumulate(month.d.begin() + d_begin, month.d.end() - d_end, acc_);
            });
        });
    }
private:
    Years dates;
    int year_from;
    int year_to;
};

int main() {
    int Q;
    cin >> Q;
    BigCalendar calendar;
    vector<double> results;
    for (int q = 0; q < Q; ++q) {
        string cmd;
        cin >> cmd;
        if (cmd == "Earn") {
            Date from;
            cin >> from;
            Date to;
            cin >> to;
            int value;
            cin >> value;
            calendar.earn(from, to, value);
        }
        else if (cmd == "ComputeIncome") {
            Date from;
            cin >> from;
            Date to;
            cin >> to;
            results.push_back(calendar.compute_income(from, to));
        }
        else
            cout << "Wrong operation." << endl;
    }
    for (auto &res : results) {
        cout.precision(25);
        cout << res << endl;
    }
    return 0;
}

/*5
Earn 2000-01-02 2000-01-06 20
ComputeIncome 2000-01-01 2001-01-01
ComputeIncome 2000-01-01 2000-01-03
Earn 2000-01-03 2000-01-03 10
ComputeIncome 2000-01-01 2001-01-01*/

/*6
Earn 2036-04-15 2045-02-05 100000
ComputeIncome 2023-01-01 2036-04-15
ComputeIncome 2039-12-23 2046-05-14
ComputeIncome 2039-12-23 2045-02-05
ComputeIncome 2036-04-15 2045-02-03
ComputeIncome 2036-04-15 2045-02-05
*/

/*
8
Earn 2035-04-15 2035-07-05 100
ComputeIncome 2035-04-15 2035-07-05
ComputeIncome 2035-04-14 2035-07-05
ComputeIncome 2035-04-16 2035-07-05
ComputeIncome 2035-04-15 2035-07-06
ComputeIncome 2035-04-15 2035-07-04
ComputeIncome 2035-06-15 2035-07-01
ComputeIncome 2032-04-15 2045-07-05

*/

/*

4
Earn 2000-02-02 2000-03-06 40
ComputeIncome 2000-02-01 2000-03-01
ComputeIncome 2000-02-01 2001-03-01
ComputeIncome 2000-02-01 2000-03-03

*/

/*

4
Earn 2001-02-02 2001-03-06 40
ComputeIncome 2001-02-01 2001-03-01
ComputeIncome 2001-02-01 2002-03-01
ComputeIncome 2001-02-01 2001-03-03

*/

/*

4
Earn 2001-03-02 2002-04-06 400
ComputeIncome 2001-03-02 2002-04-06
ComputeIncome 2001-02-01 2002-05-01
ComputeIncome 2001-02-01 2002-03-03

*/

/*
4
Earn 2000-02-01 2001-04-06 400
ComputeIncome 2000-02-01 2000-04-06
ComputeIncome 2000-01-01 2000-05-01
ComputeIncome 2000-01-01 2001-03-03
*/
