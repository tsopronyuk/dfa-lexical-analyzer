# DFA Lexical Analyzer and Visualizer 🔍

[![Python Tests](https://github.com/tsopronyuk/dfa-lexical-analyzer/actions/workflows/python-tests.yml/badge.svg)](https://github.com/tsopronyuk/dfa-lexical-analyzer/actions)

This repository provides a flexible, 100% generic Deterministic Finite Automaton (**DFA**) engine for lexical analysis and diagram generation. The project demonstrates transparent, step-by-step tokenization, prefix backtracking, and publication-ready Graphviz diagram rendering.

---

## 🚀 Interactive Examples (Google Colab)

Run the source code, review tokenization step-by-step, and render state transition graphs directly in your browser:

* **Example 1: Keywords and Noise Filtering**  
  A lexical analyzer for fixed and variable-length tokens (`go`, `goon`, skipping unexpected noise).  
  [![Open In Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/tsopronyuk/dfa-lexical-analyzer/blob/main/notebooks/01_keywords_dfa.ipynb)

* **Example 2: Numeric Constants and Identifiers**  
  Recognizes hexadecimal, decimal constants, and identifiers with clean state transition formatting.  
  [![Open In Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/tsopronyuk/dfa-lexical-analyzer/blob/main/notebooks/02_numbers_dfa.ipynb)

* **Universal Table-Driven DFA Engine (Multi-Level Backtracking)**  
  A generic, matrix-driven DFA engine (`UniversalDFA`). Decouples state transition matrices from driver logic, handles multi-level lookahead/backtracking (e.g., `int` vs `integer`), and automatically renders optimized state graphs.  
  [![Open In Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/tsopronyuk/dfa-lexical-analyzer/blob/main/notebooks/03_table_driven_dfa_lexer.ipynb)

---

## ✨ Key Features

* **100% Data-Driven Architecture:** Complete separation of automaton logic from execution engine (`UniversalDFA`). Transition matrices, character sets, and final states are passed via pure dictionary configurations.
* **Multi-Level Lookahead & Backtracking:** Resolves prefix ambiguities (e.g., `go` $\to$ `goon`, `int` $\to$ `integer`, `to` $\to$ `topic`) via explicit `backtrack_rules`, rewind steps, and buffer restoration.
* **Smart Auto-Orientation Rendering:** 
  * Automatically analyzes keyword lengths. 
  * Selects horizontal layout (`rankdir='LR'`) for short tokens ($\le 5$ chars).
  * Automatically switches to vertical layout (`rankdir='TB'`) when long keywords ($> 5$ chars) are detected, preventing graph shrinkage and unreadable fonts in web articles or A4 publications.
* **Clean Visual Formatting:**
  * Summarizes character classes into readable ranges (e.g., `a-z, _`, `0..9`, `a-f, A-F`).
  * Intelligent fallback grouping (`else` / `other`) for complex state branches.
  * Color-coded edge styles: solid lines for active transitions, dashed gray for terminal completions, and dashed orange for backtracking routes.

---

## 💻 Quick Usage Example

```python
from dfa_engine import UniversalDFA

# 1. Define your DFA configuration dictionary
config = {
    "name": "Nested Keywords DFA",
    "char_classes": {"i": 0, "n": 1, "t": 2, "e": 3, "g": 4, "r": 5, "o": 6, "p": 7, "c": 8},
    "default_class": 9,
    "matrix": [
        # Transitions matrix...
    ],
    "final_states": {
        -1: {"name": "Keyword (in)"},
        -2: {"name": "Keyword (int)"},
        -3: {"name": "Keyword (integer)"},
        -4: {"name": "Keyword (to)"},
        -5: {"name": "Keyword (topic)"}
    },
    "backtrack_rules": {
        "-2": {"from_states": [4, 5, 6], "emit_type_id": -2, "emit_value": "int", "step": 3}
    }
}

# 2. Initialize the DFA engine
engine = UniversalDFA(config)

# 3. Tokenize contiguous text stream (testing prefix matching & backtracking)
text = "inint integer totopic"
tokens = engine.tokenize(text)

for t in tokens:
    print(f"Token: {t['value']:<10} | Type: {t['type']}")

# 4. Render state graph with smart auto-orientation ('LR' or 'TB')
graph = engine.render_graph()

# 5. Save diagram locally (PNG, SVG, or PDF)
graph.format = 'png'
graph.render('dfa_diagram', cleanup=True)
```
---

## 📁 Repository Structure

* `notebooks/` — Interactive Jupyter Notebooks with step-by-step tokenization and visualizers.
* `src/` — Standalone Python modules containing the core `UniversalDFA` engine.
* `tests/` — Unit test suite verifying tokenization correctness and backtracking behavior.
* `.github/workflows/` — Continuous Integration (CI) configuration running automated tests on commit.

---

## 🧪 Running Tests Locally

To run the unit test suite locally using Python's built-in `unittest` framework:

```bash
python -m unittest discover -s tests

```

---

## 🛠 Technologies

* **Python 3.x**
* **Graphviz** (Diagram rendering)
* **IPython / Jupyter Notebook**
* **Google Colab**
* **GitHub Actions** (CI/CD)
