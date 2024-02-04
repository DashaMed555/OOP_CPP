#ifndef TASK4_CONVERTERS_H
#define TASK4_CONVERTERS_H

#include <vector>
#include <string>

class Converter {
public:
    virtual void process(std::ifstream& stream, std::string output_name) = 0;
    [[nodiscard]]
    virtual const std::string get_name() const = 0;
    [[nodiscard]]
    virtual const std::string get_description() const = 0;
    [[nodiscard]]
    virtual const std::vector<std::string> get_parameters() const = 0;
    [[nodiscard]]
    virtual const std::string get_syntax() const = 0;
};

class Mute_Converter : public Converter {
public:
    Mute_Converter(uint64_t start, uint64_t end);
    void process(std::ifstream& stream, std::string output_name) override;
    [[nodiscard]]
    const std::string get_name() const override;
    [[nodiscard]]
    const std::string get_description() const override;
    [[nodiscard]]
    const std::vector<std::string> get_parameters() const override;
    [[nodiscard]]
    const std::string get_syntax() const override;
private:
    uint64_t start;
    uint64_t end;
};

class Mix_Converter : public Converter {
public:
    Mix_Converter(std::ifstream& add, uint64_t position);
    void process(std::ifstream& stream, std::string output_name) override;
    const std::string get_name() const override;
    const std::string get_description() const override;
    const std::vector<std::string> get_parameters() const override;
    const std::string get_syntax() const override;
private:
    std::ifstream add;
    uint64_t position;
};

class Volume_Converter : public Converter {
public:
    Volume_Converter(uint64_t period);
    void process(std::ifstream& stream, std::string output_name) override;
    [[nodiscard]]
    const std::string get_name() const override;
    [[nodiscard]]
    const std::string get_description() const override;
    [[nodiscard]]
    const std::vector<std::string> get_parameters() const override;
    [[nodiscard]]
    const std::string get_syntax() const override;
private:
    uint64_t period;
};

class Factory {
public:
    virtual Converter* create_converter(const std::vector<std::string>& parameters, std::vector<std::ifstream>& streams) const = 0;
};

class Factory_Mute_Converter : public Factory {
public:
    [[nodiscard]]
    Converter* create_converter(const std::vector<std::string>& parameters, std::vector<std::ifstream>& streams) const override;
};

class Factory_Mix_Converter : public Factory {
public:
    [[nodiscard]]
    Converter* create_converter(const std::vector<std::string>& parameters, std::vector<std::ifstream>& streams) const override;
};

class Factory_Volume_Converter : public Factory {
public:
    [[nodiscard]]
    Converter* create_converter(const std::vector<std::string>& parameters, std::vector<std::ifstream>& streams) const override;
};

std::vector<Converter*> get_supported_converters();
Factory* create_factory(const std::string& name);

#endif
