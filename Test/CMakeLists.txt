enable_testing()

add_executable(tests main_test.cpp unit_test.cpp unit_test.h)
target_link_libraries(tests PUBLIC GTest::gtest_main Snake Apple GameManager)

add_test(NAME unit_tests COMMAND tests)

# ---- LLVM Coverage report ----
find_program(LLVM_PROFDATA NAMES llvm-profdata PATHS /opt/homebrew/opt/llvm/bin REQUIRED)
find_program(LLVM_COV NAMES llvm-cov PATHS /opt/homebrew/opt/llvm/bin REQUIRED)

add_custom_target(
        coverage_report
        COMMAND rm -f tests.profraw tests.profdata
        COMMAND LLVM_PROFILE_FILE=tests.profraw $<TARGET_FILE:tests>

        COMMAND ${LLVM_PROFDATA} merge -sparse tests.profraw -o tests.profdata

        COMMAND ${LLVM_COV} show $<TARGET_FILE:tests>
        -instr-profile=tests.profdata
        -format=html
        -output-dir=coverage_html

        -ignore-filename-regex='.*/usr/local/include/gtest/.*'
        -ignore-filename-regex='.*/usr/include/.*'
        -ignore-filename-regex='.*/Library/Developer/CommandLineTools/.*'
        -ignore-filename-regex='.*_autogen/.*'
        -ignore-filename-regex='.*/Qt6/.*'

        COMMENT "Generating LLVM coverage report"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
