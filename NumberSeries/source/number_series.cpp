/// Created by Marius Mikucionis <marius@cs.aau.dk>
/**
 * Definitions/implementation of some number_series methods
 */

#include "number_series.hpp"

#include <utility>

namespace DataSeries
{
    NumberSeries::NumberSeries() = default;
    NumberSeriesWrap::NumberSeriesWrap() = default;
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
        if (size() == 0) {
            data.push_back(value);
            min_val = value;
            max_val = value;
        } else {
            data.push_back(value);
            if (value <= min_val)
                min_val = value;
            if (value >= max_val)
                max_val = value;
        }
    }

    NumberSeries& NumberSeries::make_random(unsigned long length, int min, int max)
    {
        auto series = new NumberSeries();
        std::mt19937 generator(std::random_device{}());
        std::uniform_int_distribution<int> distribution(min, max);

        for (unsigned long i = 0; i < length; ++i) {
            series->add_value(distribution(generator));
        }

        return *series;
    }

    NumberSeries& NumberSeries::operator+=(const NumberSeries& other)
    {
        auto result = NumberSeries();  // Create on stack, not heap
        auto max_size = std::max(size(), other.size());

        for (auto i = 0; i < max_size; ++i) {
            int left_val = (i < size()) ? data[i] : 0;
            int right_val = (i < other.size()) ? other.data[i] : 0;
            result.add_value(left_val + right_val);
        }

        *this = result;
        return *this;
    }

    NumberSeries NumberSeries::operator+(const NumberSeries& other) const
    {
        auto result = *this; // Create a copy of the current object
        result += other;     // Use operator+= to add the other series
        return result;        // Return the modified copy
    }

    bool NumberSeries::operator<(const NumberSeries& other) const
    {
        return amplitude() < other.amplitude();
    }

    // --- Number Series Wrap ---

    NumberSeriesWrap::NumberSeriesWrap(std::initializer_list<int> list) :
        number_series(std::make_unique<NumberSeries>(list)){}

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

    void NumberSeriesWrap::add_value(int value)
    {
        if (!number_series) {
            number_series = std::make_unique<NumberSeries>();
        }
        number_series->add_value(value);
    }

    NumberSeriesWrap& NumberSeriesWrap::make_random(unsigned long length, int min, int max)
    {
        auto wrap = new NumberSeriesWrap();
        wrap->number_series = std::make_unique<NumberSeries>(NumberSeries::make_random(length, min, max));
        return *wrap;
    }

    NumberSeriesWrap& NumberSeriesWrap::operator+=(const NumberSeriesWrap& other)
    {
        if (!number_series) {
            number_series = std::make_unique<NumberSeries>();
        }
        if (other.number_series) {
            *number_series += *other.number_series;
        }
        return *this;
    }

    NumberSeriesWrap NumberSeriesWrap::operator+(const NumberSeriesWrap& other) const
    {
        auto result = *this;
        result += other;
        return result;
    }

    bool NumberSeriesWrap::operator<(const NumberSeriesWrap& other) const
    {
        if (!number_series || !other.number_series) {
            return false;
        }
        return *number_series < *other.number_series;
    }


}  // namespace DataSeries
