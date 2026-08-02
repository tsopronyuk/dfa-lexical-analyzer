# ============================================================================
# LEXICAL ANALYZER: KEYWORDS AND NOISE
# ============================================================================

M = [
    [  1,  2,  5, -6, -6, -6, -6, -6, -6 ],
    [  1, -5, -5, -5, -5, -5, -5, -5, -5 ],
    [ -6, -6,  3, -6, -6, -6, -6, -6, -6 ],
    [ -1, -1,  4, -1, -1, -1, -1, -1, -1 ],
    [ -1, -1, -1, -1, -1, 10, -1, -1, -1 ],
    [ -6, -6, -6,  6, -6,  8, -6, -6, -6 ],
    [ -6, -6, -6, -6,  7, -6, -6, -6, -6 ],
    [ -6, -6, -6, -6, -6, 11, -6, -6, -6 ],
    [ -6, -6, -6, -6, -6, -6,  9, -6, -6 ],
    [ -6, -6, -6, -6, -6, -6, -6, 12, -6 ],
    [ -2, -2, -2, -2, -2, -2, -2, -2, -2 ],
    [ -3, -3, -3, -3, -3, -3, -3, -3, -3 ],
    [ -6, -6, -6, -6, -6, 13, -6, -6, -6 ],
    [ -6, -6, -6, -6, 14, -6, -6, -6, -6 ],
    [ -4, -4, -4, -4, -4, -4, -4, -4, -4 ]
]

TOKEN_INFO = {
    -1: ("Keyword", "go", 2),
    -2: ("Keyword", "goon", 4),
    -3: ("Keyword", "open", 4),
    -4: ("Keyword", "online", 6),
    -5: ("Number", None, None),
    -6: ("Skip", "Noise", 1)
}

def class_symb(c):
    if c.isdigit(): return 0
    if c == 'g': return 1
    if c == 'o': return 2
    if c == 'p': return 3
    if c == 'e': return 4
    if c == 'n': return 5
    if c == 'l': return 6
    if c == 'i': return 7
    return 8

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
            elif next_state == -5:
                val = text_buffer[start:i]
                tokens.append({"type": "Number", "value": val})
                start = i 
                state = 0
            else:
                token_type, kw_val, kw_len = TOKEN_INFO[next_state]
                tokens.append({"type": f"{token_type} ({kw_val})", "value": kw_val})
                start = start + kw_len
                i = start
                state = 0
        else:
            state = next_state
            i += 1
            
    return tokens
