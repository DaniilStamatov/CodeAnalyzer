#include "metric_impl/code_lines_count.hpp"

#include <gtest/gtest.h>

namespace analyzer::metric::metric_impl {
class CodeLinesCountTest : public ::testing::Test {
protected:
    CodeLinesCountMetric metric;

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

TEST_F(CodeLinesCountTest, CommentsTest) {
    auto func = LoadFunctionFromFile("comments.py");
    auto res = metric.Calculate(func);
    std::cout << func.ast << std::endl;
    EXPECT_EQ(std::get<int>(res.value), 3);
}

TEST_F(CodeLinesCountTest, ExceptionsCommentsTest) {
    auto func = LoadFunctionFromFile("exceptions.py");
    auto res = metric.Calculate(func);
    std::cout << func.ast << std::endl;
    EXPECT_EQ(std::get<int>(res.value), 7);
}

TEST_F(CodeLinesCountTest, ManyLines) {
    auto func = LoadFunctionFromFile("many_lines.py");
    auto res = metric.Calculate(func);
    std::cout << func.ast << std::endl;
    EXPECT_EQ(std::get<int>(res.value), 11);
}

TEST_F(CodeLinesCountTest, TestTernary) {
    auto func = LoadFunctionFromFile("ternary.py");
    auto res = metric.Calculate(func);
    std::cout << func.ast << std::endl;
    EXPECT_EQ(std::get<int>(res.value), 1);
}

}  // namespace analyzer::metric::metric_impl
