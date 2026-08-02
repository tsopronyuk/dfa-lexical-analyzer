# ============================================================================
# LEXICAL ANALYZER: NUMERIC CONSTANTS AND IDENTIFIERS
# ============================================================================

M = [
    [  1,  2,  2,  2,  3,  3,  3,  3,  3, -6 ],
    [  2,  2,  2,  2,  4,  5,  6, -3, -3, -3 ],
    [  2,  2,  2,  2, -3, -3, -3, -3, -3, -3 ],
    [  3,  3,  3,  3,  3,  3,  3,  3,  3, -1 ],
    [  7,  7, -3, -3, -3, -3, -3, -3, -3, -3 ],
    [  8,  8,  8,  8,  8, -3, -3,  8, -3, -3 ],
    [  9,  9,  9, -3, -3, -3, -3, -3, -3, -3 ],
    [  7,  7, -2, -2, -2, -2, -2, -2, -2, -2 ],
    [  8,  8,  8,  8,  8, -5, -5,  8, -5, -5 ],
    [  9,  9,  9, -4, -4, -4, -4, -4, -4, -4 ]
]

TOKEN_INFO = {
    -1: ("Identifier", "Ident", ""),
    -2: ("Binary Constant", "Bin Const", ""),
    -3: ("Decimal Constant", "Dec Const", ""),
    -4: ("Octal Constant", "Oct Const", ""),
    -5: ("Hexadecimal Constant", "Hex Const", ""),
    -6: ("Skip", "Noise", "")
}

def class_symb(c):
    if c == '0': return 0
    if c == '1': return 1
    if c in '234567': return 2
    if c in '89': return 3
    if c in 'bB': return 4
    if c in 'xX': return 5
    if c in 'oO': return 6
    if c.lower() in 'abcdef': return 7
    if c.isalpha() or c == '_': return 8
    return 9

def lexical_analyzer(text, verbose=False):
    tokens = []
    text_buffer = text + '\0'
    start = i = state = 0
    
    while start < len(text_buffer) - 1:
        c = text_buffer[i]
        cl = class_symb(c)
        next_state = M[state][cl]
        
        if next_state < 0:
            if next_state == -6:
                start += 1
                i = start
                state = 0
            elif next_state == -3 and state in (4, 5, 6):
                tokens.append({"type": TOKEN_INFO[-3][0], "value": "0"})
                start = start + 1 
                i = start
                state = 0
            else:
                val = text_buffer[start:i]
                token_type = TOKEN_INFO[next_state][0]
                tokens.append({"type": token_type, "value": val})
                start = i
                state = 0
        else:
            state = next_state
            i += 1
            
    return tokens
