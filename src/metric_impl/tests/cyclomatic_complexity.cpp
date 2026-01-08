#include "metric_impl/cyclomatic_complexity.hpp"

#include <gtest/gtest.h>

namespace analyzer::metric::metric_impl {
class CyclomaticComplexityMetricTest : public ::testing::Test {
protected:
    CyclomaticComplexityMetric metric;

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

TEST_F(CyclomaticComplexityMetricTest, CommentsTest) {
    auto func = LoadFunctionFromFile("comments.py");
    auto res = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(res.value), 1);
}

TEST_F(CyclomaticComplexityMetricTest, ExceptionsTest) {
    auto func = LoadFunctionFromFile("exceptions.py");
    auto res = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(res.value), 4);
}

TEST_F(CyclomaticComplexityMetricTest, IfTest) {
    auto func = LoadFunctionFromFile("if.py");
    auto res = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(res.value), 2);
}

TEST_F(CyclomaticComplexityMetricTest, LoopsTest) {
    auto func = LoadFunctionFromFile("loops.py");
    auto res = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(res.value), 4);
}

TEST_F(CyclomaticComplexityMetricTest, ManyLines) {
    auto func = LoadFunctionFromFile("many_lines.py");
    auto res = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(res.value), 2);
}
TEST_F(CyclomaticComplexityMetricTest, ManyParameters) {
    auto func = LoadFunctionFromFile("many_parameters.py");
    auto res = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(res.value), 2);
}

TEST_F(CyclomaticComplexityMetricTest, MatchCase) {
    auto func = LoadFunctionFromFile("match_case.py");
    auto res = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(res.value), 4);
}

TEST_F(CyclomaticComplexityMetricTest, NestedIf) {
    auto func = LoadFunctionFromFile("nested_if.py");
    auto res = metric.Calculate(func);
    EXPECT_EQ(std::get<int>(res.value), 6);
}

TEST_F(CyclomaticComplexityMetricTest, Ternary) {
    auto func = LoadFunctionFromFile("ternary.py");
    auto res = metric.Calculate(func);
    std::cout << func.ast << std::endl;
    EXPECT_EQ(std::get<int>(res.value), 3);
}
// здесь ваш код

}  // namespace analyzer::metric::metric_impl
