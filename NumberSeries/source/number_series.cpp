/// Created by Marius Mikucionis <marius@cs.aau.dk>
/**
 * Definitions/implementation of some number_series methods
 */

#include "number_series.hpp"

#include <utility>

namespace DataSeries
{
    NumberSeries::NumberSeries(std::initializer_list<int> list)
    {
        for (int const value : list) {
            add_value(value);
        }
    }

    unsigned long NumberSeries::size() const { return data.size(); }

    int NumberSeries::max() const { return max_val; }

    int NumberSeries::min() const { return min_val; }

    int NumberSeries::amplitude() const { return max_val - min_val; }

    void NumberSeries::add_value(int value)
    {
        if (data.empty()) {
            min_val = max_val = value; // If this is the first entry, min and max are the same
        } else {
            min_val = std::min(min_val, value);
            max_val = std::max(max_val, value);
        }
        data.push_back(value);
    }

    NumberSeries NumberSeries::make_random(unsigned long length, int min, int max)
    {
        NumberSeries series;
        std::mt19937 generator(std::random_device{}());
        std::uniform_int_distribution<int> distribution(min, max);

        for (unsigned long i = 0; i < length; ++i) {
            series.add_value(distribution(generator));
        }

        return series;
    }


    NumberSeries& NumberSeries::operator+=(const NumberSeries& other)
    {
        auto result = NumberSeries();
        auto max_size = std::max(size(), other.size()); // Find the size/length of the new vector

        for (auto i = 0; i < max_size; ++i) {
            int left_val = (i < size()) ? data[i] : 0; //Keep looping until there is no more data
            int right_val = (i < other.size()) ? other.data[i] : 0;
            result.add_value(left_val + right_val);
        }

        // This seems stupid, but I did not manage to do the updates in place....
        *this = result;
        return *this;
    }

    NumberSeries NumberSeries::operator+(const NumberSeries& other) const
    {
        auto result = *this; // Create a copy of the current object
        return result += other;
    }

    bool NumberSeries::operator<(const NumberSeries& other) const
    {
        return amplitude() < other.amplitude();
    }

    // --- Number Series Wrap ---

    NumberSeriesWrap::NumberSeriesWrap()
    {
        // Always initialize so I do not have to null check in NumberSeriesWrap methods.
        number_series = std::make_unique<NumberSeries>();
    }

    // Pass to NumberSeries list constructor
    NumberSeriesWrap::NumberSeriesWrap(std::initializer_list<int> list) :
        number_series(std::make_unique<NumberSeries>(list)){}

    NumberSeriesWrap::NumberSeriesWrap(const NumberSeriesWrap& other)
        : number_series(std::make_unique<NumberSeries>(*other.number_series)) {}

    NumberSeriesWrap::NumberSeriesWrap(NumberSeriesWrap&& other) noexcept
        : number_series(std::move(other.number_series)) {}

    NumberSeriesWrap& NumberSeriesWrap::operator=(const NumberSeriesWrap& other)
    {
        if (this != &other) {
            number_series = std::make_unique<NumberSeries>(*other.number_series);
        }
        return *this;
    }

    NumberSeriesWrap& NumberSeriesWrap::operator=(NumberSeriesWrap&& other) noexcept
    {
        number_series.swap(other.number_series);
        return *this;
    }

    unsigned long NumberSeriesWrap::size() const
    {
        return number_series->size();
    }

    int NumberSeriesWrap::max() const
    {
        return number_series->max();
    }

    int NumberSeriesWrap::min() const
    {
        return number_series->min();
    }

    int NumberSeriesWrap::amplitude() const
    {
        return number_series->amplitude();
    }

    void NumberSeriesWrap::add_value(int value) {
        number_series->add_value(value);
    }

    NumberSeriesWrap NumberSeriesWrap::make_random(unsigned long length, int min, int max)
    {
        auto wrap = NumberSeriesWrap();
        wrap.number_series = std::make_unique<NumberSeries>(NumberSeries::make_random(length, min, max));
        return wrap;
    }

    NumberSeriesWrap& NumberSeriesWrap::operator+=(const NumberSeriesWrap& other)
    {
        if (other.number_series) {
            *number_series += *other.number_series;
        }
        return *this;
    }

    NumberSeriesWrap NumberSeriesWrap::operator+(const NumberSeriesWrap& other) const
    {
        auto result = *this;
        return result += other;
    }

    bool NumberSeriesWrap::operator<(const NumberSeriesWrap& other) const
    {
        if (!number_series || !other.number_series) {
            return false;
        }
        return *number_series < *other.number_series;
    }


}  // namespace DataSeries
