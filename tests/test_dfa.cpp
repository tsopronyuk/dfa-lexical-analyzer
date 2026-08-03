#include <iostream>
#include <cassert>
#include <fstream>
#include "../src/dfa_engine.cpp"

// ============================================================================
// C++ UNIT TESTS FOR UNIVERSAL DFA ENGINE
// ============================================================================

/**
 * @brief Helper function to verify if a file was successfully created on disk.
 */
bool file_exists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

int main() {
    std::cout << "========================================================\n";
    std::cout << " Running UniversalDFA C++ Engine Unit Tests...\n";
    std::cout << "========================================================\n\n";

    // ------------------------------------------------------------------------
    // SETUP: Test DFA Configuration for Multi-Level Keywords (in, int, integer, to, topic)
    // ------------------------------------------------------------------------
    DFAConfig test_config{
        "Nested Keywords Test DFA",
        { {"i", 0}, {"n", 1}, {"t", 2}, {"e", 3}, {"g", 4}, {"r", 5}, {"o", 6}, {"p", 7}, {"c", 8} },
        9, // default character class
        {
            // Transition Matrix:
            //  i   n   t   e   g   r   o   p   c  other
            {  1, -6,  7, -6, -6, -6, -6, -6, -6, -6 }, // 0: Start
            { -6,  2, -6, -6, -6, -6, -6, -6, -6, -6 }, // 1: 'i'
            { -1, -1,  3, -1, -1, -1, -1, -1, -1, -1 }, // 2: 'in'
            { -2, -2, -2,  4, -2, -2, -2, -2, -2, -2 }, // 3: 'int'
            { -2, -2, -2, -2,  5, -2, -2, -2, -2, -2 }, // 4: 'inte'
            { -2, -2, -2,  6, -2, -2, -2, -2, -2, -2 }, // 5: 'integ'
            { -2, -2, -2, -2, -2, 12, -2, -2, -2, -2 }, // 6: 'intege'
            { -6, -6, -6, -6, -6, -6,  8, -6, -6, -6 }, // 7: 't'
            { -4, -4, -4, -4, -4, -4, -4,  9, -4, -4 }, // 8: 'to'
            { 10, -4, -4, -4, -4, -4, -4, -4, -4, -4 }, // 9: 'top'
            { -4, -4, -4, -4, -4, -4, -4, -4, 11, -4 }, // 10: 'topi'
            { -5, -5, -5, -5, -5, -5, -5, -5, -5, -5 }, // 11: Final 'topic'
            { -3, -3, -3, -3, -3, -3, -3, -3, -3, -3 }  // 12: Final 'integer'
        },
        {
            {-1, {"Keyword (in)", "lightblue"}},
            {-2, {"Keyword (int)", "lightgreen"}},
            {-3, {"Keyword (integer)", "mediumseagreen"}},
            {-4, {"Keyword (to)", "khaki"}},
            {-5, {"Keyword (topic)", "plum"}},
            {-6, {"Skip (Noise)", "gainsboro"}}
        },
        {
            {-2, {{4, 5, 6}, -2, "int", 3}}, // Backtrack rule for 'int' prefix
            {-4, {{9, 10}, -4, "to", 2}}     // Backtrack rule for 'to' prefix
        }
    };

    // Instantiate DFA engine
    UniversalDFA lexer(test_config);

    // ------------------------------------------------------------------------
    // TEST CASE 1: Lexical Analysis & Contiguous Prefix Backtracking
    // ------------------------------------------------------------------------
    std::cout << "[TEST 1] Testing Tokenization & Backtracking... ";
    {
        std::string input = "inint integer totopic";
        auto tokens = lexer.tokenize(input);

        // Verify total token count
        assert(tokens.size() == 5 && "Test 1 Failed: Token count mismatch!");

        // Verify exact token sequence and emitted values
        assert(tokens[0].value == "in" && tokens[0].type == "Keyword (in)");
        assert(tokens[1].value == "int" && tokens[1].type == "Keyword (int)");
        assert(tokens[2].value == "integer" && tokens[2].type == "Keyword (integer)");
        assert(tokens[3].value == "to" && tokens[3].type == "Keyword (to)");
        assert(tokens[4].value == "topic" && tokens[4].type == "Keyword (topic)");

        std::cout << "PASSED!\n";
    }

    // ------------------------------------------------------------------------
    // TEST CASE 2: Noise Filtering & Empty Input Stream
    // ------------------------------------------------------------------------
    std::cout << "[TEST 2] Testing Noise Filtering & Empty Streams... ";
    {
        // Empty string test
        auto empty_tokens = lexer.tokenize("");
        assert(empty_tokens.empty() && "Test 2 Failed: Expected empty token vector!");

        // Stream consisting only of default/noise characters
        auto noise_tokens = lexer.tokenize("xyz 123 !@#");
        assert(noise_tokens.empty() && "Test 2 Failed: Noise characters were not skipped!");

        std::cout << "PASSED!\n";
    }

    // ------------------------------------------------------------------------
    // TEST CASE 3: Graphviz DOT Export Functionality
    // ------------------------------------------------------------------------
    std::cout << "[TEST 3] Testing Graphviz .dot File Export... ";
    {
        std::string test_dot_file = "test_output.dot";
        lexer.export_dot(test_dot_file);

        // Verify that output file was created and is accessible
        assert(file_exists(test_dot_file) && "Test 3 Failed: .dot file was not created!");

        std::cout << "PASSED!\n";
    }

    std::cout << "\n========================================================\n";
    std::cout << " ALL C++ UNIT TESTS PASSED SUCCESSFULLY!\n";
    std::cout << "========================================================\n";

    return 0;
}
