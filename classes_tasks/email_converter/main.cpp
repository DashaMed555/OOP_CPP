#include "test_runner.h"
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>


struct Email {
    string from;
    string to;
    string body;
};


class Worker {
public:
    virtual ~Worker() = default;
    virtual void Process(unique_ptr<Email> email) = 0;
    virtual void Run() {
        throw logic_error("Unimplemented");
    }

protected:
    void PassOn(unique_ptr<Email> email) const {
        if (next_ptr) {
            next_ptr->Process(std::move(email));
        }
    }

public:
    void SetNext(unique_ptr<Worker> next) {
        Worker* current = this;
        while (current->next_ptr != nullptr)
            current = current->next_ptr.get();
        current->next_ptr = std::move(next);
    }

private:
    unique_ptr<Worker> next_ptr;
};


class Reader : public Worker {
public:
    explicit Reader(istream& stream) : stream(stream){};
    void Run() override {
        while (not stream.eof()) {
            unique_ptr<Email> email(new Email());
            getline(stream, email->from);
            getline(stream, email->to);
            getline(stream, email->body);
            Process(std::move(email));
        }
    }
    void Process(unique_ptr<Email> email) override {
        this->PassOn(std::move(email));
    }

private:
    istream& stream;
};


class Filter : public Worker {
public:
    using Function = function<bool(const Email&)>;
    explicit Filter(Function predicate) : predicate(std::move(predicate)){};
    void Process(unique_ptr<Email> email) override {
        if (predicate(*email))
            this->PassOn(std::move(email));
    }

private:
    Function predicate;
};


class Copier : public Worker {
public:
    explicit Copier(string recipient) : recipient(std::move(recipient)){};
    void Process(unique_ptr<Email> email) override {
        if (recipient != email->to) {
            unique_ptr<Email> new_email(new Email({email->from, recipient, email->body}));
            this->PassOn(std::move(email));
            this->PassOn(std::move(new_email));
        }
        else this->PassOn(std::move(email));
    }

private:
    string recipient;
};


class Sender : public Worker {
public:
    explicit Sender(ostream& out) : stream(out){};
    void Process(unique_ptr<Email> email) override {
        stream << email->from << endl;
        stream << email->to << endl;
        stream << email->body << endl;
    }

private:
    ostream& stream;
};


class PipelineBuilder {
public:
    explicit PipelineBuilder(istream& in) {
        unique_ptr<Worker> worker = make_unique<Reader>(in);
        chain = std::move(worker);
    }

    PipelineBuilder& FilterBy(const Filter::Function& filter) {
        unique_ptr<Worker> worker = make_unique<Filter>(filter);
        chain->SetNext(std::move(worker));
        return *this;
    }

    PipelineBuilder& CopyTo(const string& recipient) {
        unique_ptr<Worker> worker = make_unique<Copier>(recipient);
        chain->SetNext(std::move(worker));
        return *this;
    }

    PipelineBuilder& Send(ostream& out) {
        unique_ptr<Worker> worker = make_unique<Sender>(out);
        chain->SetNext(std::move(worker));
        return *this;
    }

    unique_ptr<Worker> Build() {
        return std::move(chain);
    }

private:
    unique_ptr<Worker> chain;
};


void TestSanity() {
    string input = (
            "erich@example.com\n"
            "richard@example.com\n"
            "Hello there\n"

            "erich@example.com\n"
            "ralph@example.com\n"
            "Are you sure you pressed the right button?\n"

            "ralph@example.com\n"
            "erich@example.com\n"
            "I do not make mistakes of that kind\n"
    );
    istringstream inStream(input);
    ostringstream outStream;

    PipelineBuilder builder(inStream);
    builder.FilterBy([](const Email& email) {
        return email.from == "erich@example.com";
    });
    builder.CopyTo("richard@example.com");
    builder.Send(outStream);
    auto pipeline = builder.Build();

    pipeline->Run();

    string expectedOutput = (
            "erich@example.com\n"
            "richard@example.com\n"
            "Hello there\n"

            "erich@example.com\n"
            "ralph@example.com\n"
            "Are you sure you pressed the right button?\n"

            "erich@example.com\n"
            "richard@example.com\n"
            "Are you sure you pressed the right button?\n"
    );

    ASSERT_EQUAL(expectedOutput, outStream.str())
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSanity);
    return 0;
}
