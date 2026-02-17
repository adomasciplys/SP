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
        std::vector<int> data = std::vector<int>();
        int max_val = 0;
        int min_val = 0;
        int dummy[100];
    public:
        NumberSeries();
        NumberSeries(std::initializer_list<int>);
        // TODO: add necessary constructors and destructors

        // TODO: add getter "size()" giving the count of numbers in the series
        unsigned long size() const;

        // TODO: add getters for minimum and maximum
        int max() const;
        int min() const;
        int amplitude() const;

        // TODO: add add_value method to add numbers to the series and maintain min/max
        void add_value(int value);

        // TODO: add static member fn for factory method
        static NumberSeries& make_random(unsigned long length, int min = 0, int max = 100);

        // TODO: add operator+= compute a pair-wise sum of two number series
        NumberSeries& operator+=(const NumberSeries& other);

        // TODO: add operator+ which reuses operator+= to compute a pair-wise sum
        NumberSeries operator+(const NumberSeries& other) const;

        // TODO: add operator< to compare amplitudes (difference between maximum and minimum)
        bool operator<(const NumberSeries& other) const;
    };

    // TODO: create a light-weight wrapper class around number_series
    class NumberSeriesWrap
    {
        // TODO: add number_series member in a smart pointer
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
        static NumberSeriesWrap& make_random(unsigned long length, int min = 0, int max = 100);
        NumberSeriesWrap& operator+=(const NumberSeriesWrap& other);
        NumberSeriesWrap operator+(const NumberSeriesWrap& other) const;
        bool operator<(const NumberSeriesWrap& other) const;
    };

}  // namespace DataSeries

#endif  // INCLUDE_NUMBER_SERIES_H
