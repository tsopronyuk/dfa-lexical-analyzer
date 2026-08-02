import unittest
from src.lexer_numbers import lexical_analyzer

class TestNumberLexer(unittest.TestCase):
    
    def test_notebook_example(self):
        # The exact complex example from the notebook
        text = "0b1010var_10x1A3F0o755123450b2_test"
        tokens = lexical_analyzer(text)
        
        expected_types = [
            "Binary Constant", 
            "Identifier", 
            "Hexadecimal Constant",
            "Octal Constant",
            "Decimal Constant",
            "Decimal Constant", # This is '0' due to prefix backtracking (0b2)
            "Identifier"        # This is the remaining part of 'b2_test'
        ]
        expected_values = [
            "0b1010", "var_1", "0x1A3F", "0o755", "12345", "0", "b2_test"
        ]
        
        self.assertEqual([t['type'] for t in tokens], expected_types)
        self.assertEqual([t['value'] for t in tokens], expected_values)

    def test_noise_filtering(self):
        # Verify that the lexer correctly skips "noise" (spaces, special chars)
        # In our alphabet, spaces, commas, and exclamation marks are noise (-6)
        text = "0b11 , 0xFF ! var"
        tokens = lexical_analyzer(text)
        
        expected_values = ["0b11", "0xFF", "var"]
        
        self.assertEqual([t['value'] for t in tokens], expected_values)
        self.assertEqual(len(tokens), 3)

    def test_pure_identifiers(self):
        # Verify standard identifiers and their boundaries
        text = "my_var1 anotherVar"
        tokens = lexical_analyzer(text)
        
        self.assertEqual(tokens[0]['value'], "my_var1")
        self.assertEqual(tokens[0]['type'], "Identifier")
        self.assertEqual(tokens[1]['value'], "anotherVar")
        self.assertEqual(tokens[1]['type'], "Identifier")

if __name__ == '__main__':
    unittest.main()
