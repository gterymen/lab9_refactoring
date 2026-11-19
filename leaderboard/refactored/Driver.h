#pragma once

#include <functional>
#include <string>

class Driver
{
    std::string const m_name;
    std::string const m_country;

public:
    Driver(std::string const name, std::string const country);
    virtual ~Driver() = default;

    std::string getName() const;

    std::string getCountry() const;

    // Ім'я для таблиці результатів. Похідні класи можуть перевизначити його,
    // тому Race більше не перевіряє тип через typeid.
    virtual std::string getDisplayName() const;

    bool operator==(Driver const& other) const;
};

namespace std
{
    template <>
    struct hash<Driver>
    {
        std::size_t operator()(Driver const& driver) const
        {
            std::hash< std::string > const hash;
            return hash( driver.getName() ) * 31 + hash( driver.getCountry() );
        }
    };
}
