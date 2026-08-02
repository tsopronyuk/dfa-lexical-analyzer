import unittest
from src.lexer_keywords import lexical_analyzer as lexer_1
from src.lexer_numbers import lexical_analyzer as lexer_2

class TestKeywordLexer(unittest.TestCase):
    def test_standard_input(self):
        text = "goon 12345 goox open99 online"
        tokens = lexer_1(text)
        
        expected_types = [
            "Keyword (goon)", "Number", "Keyword (go)", 
            "Keyword (open)", "Number", "Keyword (online)"
        ]
        expected_values = ["goon", "12345", "go", "open", "99", "online"]
        
        self.assertEqual([t['type'] for t in tokens], expected_types)
        self.assertEqual([t['value'] for t in tokens], expected_values)

class TestNumberLexer(unittest.TestCase):
    def test_mixed_constants(self):
        text = "0b1010, var_1 = 0x1A3F"
        tokens = lexer_2(text)
        
        expected_types = ["Binary Constant", "Identifier", "Hexadecimal Constant"]
        expected_values = ["0b1010", "var_1", "0x1A3F"]
        
        self.assertEqual([t['type'] for t in tokens], expected_types)
        self.assertEqual([t['value'] for t in tokens], expected_values)

    def test_invalid_prefix_backtrack(self):
        text = "0b2_test"
        tokens = lexer_2(text)
        
        # Should recognize '0' as decimal, then 'b2_test' as identifier
        self.assertEqual(tokens[0]['value'], "0")
        self.assertEqual(tokens[0]['type'], "Decimal Constant")
        self.assertEqual(tokens[1]['value'], "b2_test")
        self.assertEqual(tokens[1]['type'], "Identifier")

if __name__ == '__main__':
    unittest.main()
