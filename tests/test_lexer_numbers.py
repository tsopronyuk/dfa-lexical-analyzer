import unittest
from src.lexer_numbers import lexical_analyzer

class TestNumberLexer(unittest.TestCase):
    
    def test_concatenated_input_from_article(self):
        # Strict article behavior: continuous string without spaces produces 2 tokens
        text = "0b1010var_10x1A3F0o755123450b2_test"
        tokens = lexical_analyzer(text)
        
        expected_types = [
            "Binary Constant", 
            "Identifier"
        ]
        expected_values = [
            "0b1010", 
            "var_10x1A3F0o755123450b2_test"
        ]
        
        self.assertEqual([t['type'] for t in tokens], expected_types)
        self.assertEqual([t['value'] for t in tokens], expected_values)

    def test_separated_constants(self):
        # Testing all number bases separated by spaces/noise
        text = "0b1010 var_1 0x1A3F 0o755 12345 0b2_test"
        tokens = lexical_analyzer(text)
        
        expected_types = [
            "Binary Constant",
            "Identifier",
            "Hexadecimal Constant",
            "Octal Constant",
            "Decimal Constant",
            "Decimal Constant",  # '0' recognized after prefix backtrack (0b2)
            "Identifier"         # 'b2_test'
        ]
        expected_values = [
            "0b1010", "var_1", "0x1A3F", "0o755", "12345", "0", "b2_test"
        ]
        
        self.assertEqual([t['type'] for t in tokens], expected_types)
        self.assertEqual([t['value'] for t in tokens], expected_values)

    def test_noise_filtering(self):
        # Verify that spaces and special characters are ignored
        text = "0b11 , 0xFF ! var"
        tokens = lexical_analyzer(text)
        
        expected_values = ["0b11", "0xFF", "var"]
        self.assertEqual([t['value'] for t in tokens], expected_values)

if __name__ == '__main__':
    unittest.main()
