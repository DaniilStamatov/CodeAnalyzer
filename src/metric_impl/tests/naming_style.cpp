#include "metric_impl/naming_style.hpp"

#include <gtest/gtest.h>

namespace analyzer::metric::metric_impl {
class NamingStyleMetricTest : public ::testing::Test {
protected:
    NamingStyleMetric metric;

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

TEST_F(NamingStyleMetricTest, CommentsTest) {
    auto func = LoadFunctionFromFile("comments.py");
    auto res = metric.Calculate(func);

    EXPECT_EQ(std::get<std::string>(res.value), "Unknown");
}

TEST_F(NamingStyleMetricTest, ExceptionsTest) {
    auto func = LoadFunctionFromFile("exceptions.py");
    auto res = metric.Calculate(func);

    EXPECT_EQ(std::get<std::string>(res.value), "Unknown");
}

TEST_F(NamingStyleMetricTest, IfTest) {
    auto func = LoadFunctionFromFile("if.py");
    auto res = metric.Calculate(func);

    EXPECT_EQ(std::get<std::string>(res.value), "Camel Case");
}

TEST_F(NamingStyleMetricTest, LoopsTest) {
    auto func = LoadFunctionFromFile("loops.py");
    auto res = metric.Calculate(func);

    EXPECT_EQ(std::get<std::string>(res.value), "Pascal Case");
}

TEST_F(NamingStyleMetricTest, ManyLines) {
    auto func = LoadFunctionFromFile("many_lines.py");
    auto res = metric.Calculate(func);

    EXPECT_EQ(std::get<std::string>(res.value), "Lower Case");
}

TEST_F(NamingStyleMetricTest, ManyParameters) {
    auto func = LoadFunctionFromFile("many_parameters.py");
    auto res = metric.Calculate(func);

    EXPECT_EQ(std::get<std::string>(res.value), "Snake Case");
}

TEST_F(NamingStyleMetricTest, MatchCase) {
    auto func = LoadFunctionFromFile("match_case.py");
    auto res = metric.Calculate(func);

    EXPECT_EQ(std::get<std::string>(res.value), "Unknown");
}
// здесь ваш код

}  // namespace analyzer::metric::metric_impl
