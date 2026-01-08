

#include "metric_accumulator_impl/categorical_accumulator.hpp"

#include <gtest/gtest.h>

#include <stdexcept>

namespace analyzer::metric_accumulator::metric_accumulator_impl::test {

TEST(CategoricalAccumulatorTest, BasicAccumulation) {
    CategoricalAccumulator accumulator;

    analyzer::metric::MetricResult result1;
    result1.metric_name = "naming_style";
    result1.value = std::string("snake_case");

    analyzer::metric::MetricResult result2;
    result2.metric_name = "naming_style";
    result2.value = std::string("camel_case");

    analyzer::metric::MetricResult result3;
    result3.metric_name = "naming_style";
    result3.value = std::string("snake_case");

    accumulator.Accumulate(result1);
    accumulator.Accumulate(result2);
    accumulator.Accumulate(result3);

    accumulator.Finalize();

    const auto &categories = accumulator.Get();

    EXPECT_EQ(categories.size(), 2);
    EXPECT_EQ(categories.at("snake_case"), 2);
    EXPECT_EQ(categories.at("camel_case"), 1);
}

TEST(CategoricalAccumulatorTest, SingleCategory) {
    CategoricalAccumulator accumulator;

    analyzer::metric::MetricResult result;
    result.metric_name = "return_type";
    result.value = std::string("int");

    accumulator.Accumulate(result);
    accumulator.Finalize();

    const auto &categories = accumulator.Get();

    EXPECT_EQ(categories.size(), 1);
    EXPECT_EQ(categories.at("int"), 1);
}

TEST(CategoricalAccumulatorTest, EmptyAccumulation) {
    CategoricalAccumulator accumulator;

    accumulator.Finalize();

    const auto &categories = accumulator.Get();
    EXPECT_TRUE(categories.empty());
}

TEST(CategoricalAccumulatorTest, GetBeforeFinalizeThrows) {
    CategoricalAccumulator accumulator;

    analyzer::metric::MetricResult result;
    result.metric_name = "naming_style";
    result.value = std::string("snake_case");

    accumulator.Accumulate(result);

    EXPECT_THROW({ accumulator.Get(); }, std::runtime_error);

    accumulator.Finalize();
    EXPECT_NO_THROW({
        const auto &categories = accumulator.Get();
        EXPECT_EQ(categories.size(), 1);
    });
}

TEST(CategoricalAccumulatorTest, ResetFunctionality) {
    CategoricalAccumulator accumulator;

    analyzer::metric::MetricResult result1;
    result1.metric_name = "style";
    result1.value = std::string("A");

    analyzer::metric::MetricResult result2;
    result2.metric_name = "style";
    result2.value = std::string("B");

    accumulator.Accumulate(result1);
    accumulator.Accumulate(result2);
    accumulator.Finalize();

    const auto &firstResult = accumulator.Get();
    EXPECT_EQ(firstResult.size(), 2);

    accumulator.Reset();

    EXPECT_THROW({ accumulator.Get(); }, std::runtime_error);

    analyzer::metric::MetricResult result3;
    result3.metric_name = "style";
    result3.value = std::string("C");

    accumulator.Accumulate(result3);
    accumulator.Finalize();

    const auto &secondResult = accumulator.Get();
    EXPECT_EQ(secondResult.size(), 1);
    EXPECT_EQ(secondResult.at("C"), 1);
}

TEST(CategoricalAccumulatorTest, MultipleAccumulatesSameValue) {
    CategoricalAccumulator accumulator;

    for (int i = 0; i < 5; i++) {
        analyzer::metric::MetricResult result;
        result.metric_name = "category";
        result.value = std::string("repeated");
        accumulator.Accumulate(result);
    }

    accumulator.Finalize();

    const auto &categories = accumulator.Get();
    EXPECT_EQ(categories.size(), 1);
    EXPECT_EQ(categories.at("repeated"), 5);
}

TEST(CategoricalAccumulatorTest, DifferentMetricNamesSameValue) {
    CategoricalAccumulator accumulator;

    analyzer::metric::MetricResult result1;
    result1.metric_name = "naming_style";
    result1.value = std::string("snake_case");

    analyzer::metric::MetricResult result2;
    result2.metric_name = "coding_style";
    result2.value = std::string("snake_case");
    accumulator.Accumulate(result1);
    accumulator.Accumulate(result2);
    accumulator.Finalize();

    const auto &categories = accumulator.Get();
    EXPECT_EQ(categories.size(), 1);
    EXPECT_EQ(categories.at("snake_case"), 2);
}

TEST(CategoricalAccumulatorTest, FinalizeWithoutAccumulate) {
    CategoricalAccumulator accumulator;

    EXPECT_NO_THROW({ accumulator.Finalize(); });

    const auto &categories = accumulator.Get();
    EXPECT_TRUE(categories.empty());
}

TEST(CategoricalAccumulatorTest, AccumulateAfterFinalize) {
    CategoricalAccumulator accumulator;

    analyzer::metric::MetricResult result1;
    result1.metric_name = "style";
    result1.value = std::string("A");

    accumulator.Accumulate(result1);
    accumulator.Finalize();

    analyzer::metric::MetricResult result2;
    result2.metric_name = "style";
    result2.value = std::string("B");

    EXPECT_NO_THROW({ accumulator.Accumulate(result2); });

    accumulator.Finalize();

    const auto &categories = accumulator.Get();
}

}  // namespace analyzer::metric_accumulator::metric_accumulator_impl::test