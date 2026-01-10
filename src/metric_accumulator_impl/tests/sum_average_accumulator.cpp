#include "metric_accumulator_impl/sum_average_accumulator.hpp"

#include <gtest/gtest.h>

#include <cmath>

// tests/sum_average_accumulator_test.cpp
#include "metric_accumulator_impl/sum_average_accumulator.hpp"
#include <gtest/gtest.h>

using namespace analyzer::metric_accumulator::metric_accumulator_impl;

TEST(SumAverageAccumulatorTest, BasicAccumulation) {
    SumAverageAccumulator accumulator;

    analyzer::metric::MetricResult result1;
    result1.metric_name = "test_metric";
    result1.value = 10;

    analyzer::metric::MetricResult result2;
    result2.metric_name = "test_metric";
    result2.value = 20;

    accumulator.Accumulate(result1);
    accumulator.Accumulate(result2);
    accumulator.Finalize();

    auto result = accumulator.Get();
    EXPECT_EQ(result.sum, 30);
    EXPECT_FLOAT_EQ(result.average, 15.0);
}

TEST(SumAverageAccumulatorTest, SingleValue) {
    SumAverageAccumulator accumulator;

    analyzer::metric::MetricResult result;
    result.metric_name = "test_metric";
    result.value = 42;

    accumulator.Accumulate(result);
    accumulator.Finalize();

    auto result_data = accumulator.Get();
    EXPECT_EQ(result_data.sum, 42);
    EXPECT_FLOAT_EQ(result_data.average, 42.0);
}

TEST(SumAverageAccumulatorTest, GetBeforeFinalizeThrows) {
    SumAverageAccumulator accumulator;

    analyzer::metric::MetricResult result;
    result.metric_name = "test_metric";
    result.value = 10;

    accumulator.Accumulate(result);

    EXPECT_THROW({ accumulator.Get(); }, std::runtime_error);

    accumulator.Finalize();

    auto result_data = accumulator.Get();
    EXPECT_EQ(result_data.sum, 10);
    EXPECT_FLOAT_EQ(result_data.average, 10.0);
}

TEST(SumAverageAccumulatorTest, ResetFunctionality) {
    SumAverageAccumulator accumulator;

    analyzer::metric::MetricResult result1;
    result1.metric_name = "metric";
    result1.value = 10;

    analyzer::metric::MetricResult result2;
    result2.metric_name = "metric";
    result2.value = 20;

    accumulator.Accumulate(result1);
    accumulator.Accumulate(result2);
    accumulator.Finalize();

    auto first_result = accumulator.Get();
    EXPECT_EQ(first_result.sum, 30);
    EXPECT_FLOAT_EQ(first_result.average, 15.0);

    accumulator.Reset();

    EXPECT_THROW({ accumulator.Get(); }, std::runtime_error);

    analyzer::metric::MetricResult result3;
    result3.metric_name = "metric";
    result3.value = 30;

    analyzer::metric::MetricResult result4;
    result4.metric_name = "metric";
    result4.value = 40;

    accumulator.Accumulate(result3);
    accumulator.Accumulate(result4);
    accumulator.Finalize();

    auto second_result = accumulator.Get();
    EXPECT_EQ(second_result.sum, 70);
    EXPECT_FLOAT_EQ(second_result.average, 35.0);
}
