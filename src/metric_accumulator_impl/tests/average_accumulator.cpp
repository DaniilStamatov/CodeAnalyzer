#include "metric_accumulator_impl/average_accumulator.hpp"

#include <gtest/gtest.h>

namespace analyzer::metric_accumulator::metric_accumulator_impl::test {

TEST(AverageAccumulatorTest, BasicAccumulation) {
    AverageAccumulator accumulator;

    analyzer::metric::MetricResult result1;
    result1.metric_name = "lines_of_code";
    result1.value = 10;

    analyzer::metric::MetricResult result2;
    result2.metric_name = "lines_of_code";
    result2.value = 20;

    accumulator.Accumulate(result1);
    accumulator.Accumulate(result2);

    accumulator.Finalize();

    double average = accumulator.Get();
    EXPECT_FLOAT_EQ(average, 15.0);
}

TEST(AverageAccumulatorTest, SingleValue) {
    AverageAccumulator accumulator;

    analyzer::metric::MetricResult result;
    result.metric_name = "test_metric";
    result.value = 42;

    accumulator.Accumulate(result);
    accumulator.Finalize();

    double average = accumulator.Get();
    EXPECT_FLOAT_EQ(average, 42.0);
}

TEST(AverageAccumulatorTest, GetBeforeFinalizeThrows) {
    AverageAccumulator accumulator;

    analyzer::metric::MetricResult result;
    result.metric_name = "test_metric";
    result.value = 10;

    accumulator.Accumulate(result);

    EXPECT_THROW({ accumulator.Get(); }, std::runtime_error);

    accumulator.Finalize();
    EXPECT_NO_THROW({
        double avg = accumulator.Get();
        EXPECT_FLOAT_EQ(avg, 10.0);
    });
}

TEST(AverageAccumulatorTest, ResetFunctionality) {
    AverageAccumulator accumulator;

    analyzer::metric::MetricResult result1;
    result1.metric_name = "metric";
    result1.value = 10;

    analyzer::metric::MetricResult result2;
    result2.metric_name = "metric";
    result2.value = 20;

    accumulator.Accumulate(result1);
    accumulator.Accumulate(result2);
    accumulator.Finalize();

    double firstAvg = accumulator.Get();
    EXPECT_FLOAT_EQ(firstAvg, 15.0);

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

    double secondAvg = accumulator.Get();
    EXPECT_FLOAT_EQ(secondAvg, 35.0);
}

TEST(AverageAccumulatorTest, AccumulateAfterFinalize) {
    AverageAccumulator accumulator;

    analyzer::metric::MetricResult result1;
    result1.metric_name = "metric";
    result1.value = 10;

    analyzer::metric::MetricResult result2;
    result2.metric_name = "metric";
    result2.value = 20;

    accumulator.Accumulate(result1);
    accumulator.Finalize();

    accumulator.Accumulate(result2);
    accumulator.Finalize();

    double avg = accumulator.Get();
}

TEST(AverageAccumulatorTest, MultipleFinalizeCalls) {
    AverageAccumulator accumulator;

    analyzer::metric::MetricResult result;
    result.metric_name = "metric";
    result.value = 10;

    accumulator.Accumulate(result);
    accumulator.Finalize();

    EXPECT_NO_THROW({ accumulator.Finalize(); });

    double avg = accumulator.Get();
    EXPECT_FLOAT_EQ(avg, 10.0);
}

}  // namespace analyzer::metric_accumulator::metric_accumulator_impl::test