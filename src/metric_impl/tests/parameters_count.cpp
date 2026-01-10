#include "metric_impl/parameters_count.hpp"

#include <gtest/gtest.h>

namespace analyzer::metric::metric_impl {

class CountParametersMetricTest : public ::testing::Test {
protected:
    CountParametersMetric metric;

    function::Function LoadFunctionFromFile(std::string_view filename) {
        std::filesystem::path test_dir = std::filesystem::path(__FILE__).parent_path();
        std::filesystem::path file_path = test_dir / "files" / filename;

        analyzer::file::File file(file_path.string());
        analyzer::function::FunctionExtractor extractor;
        auto functions = extractor.Get(file);

        EXPECT_FALSE(functions.empty()) << "No functions found in " << filename;
        return functions.front();
    }
};

TEST_F(CountParametersMetricTest, ManyParameters) {
    auto func = LoadFunctionFromFile("many_parameters.py");
    auto res = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(res.value), 5);
}

TEST_F(CountParametersMetricTest, Comments) {
    auto func = LoadFunctionFromFile("comments.py");
    auto res = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(res.value), 3);
}

TEST_F(CountParametersMetricTest, ManyLines) {
    auto func = LoadFunctionFromFile("many_lines.py");
    auto res = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(res.value), 0);
}

TEST_F(CountParametersMetricTest, NestedIf) {
    auto func = LoadFunctionFromFile("nested_if.py");
    auto res = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(res.value), 2);
}

// здесь ваш код

}  // namespace analyzer::metric::metric_impl
