#include <iostream>

void SendSms(const std::string& number, const std::string& message) {
    std::cout << "Send ‘" << message << "’ to number "<< number << std::endl;
}

void SendEmail(const std::string& email, const std::string& message) {
    std::cout << "Send ‘" << message << "’ e-mail "<< email << std::endl;
}

class INotifier {
public:
    virtual void Notify(const std::string& message) = 0;
};

class SmsNotifier : public INotifier {
    std::string number;
public:
    explicit SmsNotifier(const std::string& number) {
        this->number = number;
    }

    void Notify(const std::string& message) override {
        SendSms(this->number, message);
    }
};

class EmailNotifier : public INotifier {
    std::string email;
public:
    explicit EmailNotifier(const std::string& email) {
        this->email = email;
    }

    void Notify(const std::string& message) override {
        SendEmail(this->email, message);
    }
};

void Notify(INotifier& notifier, const std::string& message) {
    notifier.Notify(message);
}

int main() {
    SmsNotifier sms{"+ 7-923-228-38-72"};
    EmailNotifier email{"m.Filippov@g.nsu.ru"};
    Notify(sms, "I know lessons in C++");
    Notify(email, "And want the lesson one");
    return 0;
}
