/// Created by Marius Mikucionis <marius@cs.aau.dk>
/**
 * Definitions of number_series and number_series_wrap classes.
 */

#ifndef INCLUDE_NUMBER_SERIES_H
#define INCLUDE_NUMBER_SERIES_H

#include <vector>
#include <limits>
#include <memory>
#include <iostream>
#include <random>

namespace DataSeries
{
    class NumberSeries
    {
        std::vector<int> data = std::vector<int>(); // Holds the values
        int max_val = 0; // Maximum value in the vector at any given moment
        int min_val = 0; // Minimum value in the vector at any given moment
        int dummy[100]; // Extra dummy data

    public:
        // Constructors
        NumberSeries();
        NumberSeries(std::initializer_list<int>);

        // Getters and small helpers
        unsigned long size() const;
        int max() const;
        int min() const;
        int amplitude() const;

        // Main logic
        void add_value(int value);
        static NumberSeries make_random(unsigned long length, int min = 0, int max = 100);

        // Operators
        NumberSeries& operator+=(const NumberSeries& other);
        NumberSeries operator+(const NumberSeries& other) const;
        bool operator<(const NumberSeries& other) const;
    };

    class NumberSeriesWrap
    {
        std::unique_ptr<NumberSeries> number_series = nullptr;
    public:
        NumberSeriesWrap();
        NumberSeriesWrap(std::initializer_list<int>);

        NumberSeriesWrap(const NumberSeriesWrap& other)
            : number_series(std::make_unique<NumberSeries>(*other.number_series)) {}

        NumberSeriesWrap(NumberSeriesWrap&& other) noexcept
            : number_series(std::move(other.number_series)) {}

        NumberSeriesWrap& operator=(const NumberSeriesWrap& other)
        {
            if (this != &other) {
                number_series = std::make_unique<NumberSeries>(*other.number_series);
            }
            return *this;
        }

        NumberSeriesWrap& operator=(NumberSeriesWrap&& other) noexcept
        {
            number_series.swap(other.number_series);
            return *this;
        }

        unsigned long size() const;
        int max() const;
        int min() const;
        int amplitude() const;
        void add_value(int value);
        static NumberSeriesWrap make_random(unsigned long length, int min = 0, int max = 100);
        NumberSeriesWrap& operator+=(const NumberSeriesWrap& other);
        NumberSeriesWrap operator+(const NumberSeriesWrap& other) const;
        bool operator<(const NumberSeriesWrap& other) const;
    };

}  // namespace DataSeries

#endif  // INCLUDE_NUMBER_SERIES_H
