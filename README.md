# Universal DFA Lexical Analyzer and Visualizer 🔍

[![Python Tests](https://github.com/tsopronyuk/dfa-lexical-analyzer/actions/workflows/python-tests.yml/badge.svg)](https://github.com/tsopronyuk/dfa-lexical-analyzer/actions)
[![C++ Tests](https://github.com/tsopronyuk/dfa-lexical-analyzer/actions/workflows/cpp-tests.yml/badge.svg)](https://github.com/tsopronyuk/dfa-lexical-analyzer/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

This repository provides a flexible, generic Deterministic Finite Automaton (**DFA**) engine for lexical analysis and automated diagram generation. Built with a **Dual C++/Python Architecture**, the project combines high-performance object-oriented system-level tokenization in modern C++ with an interactive visualization and diagnostics toolkit in Python.

---

## 🚀 Interactive Examples (Google Colab)

Run the source code, review tokenization step-by-step, and render publication-ready state transition graphs directly in your browser:

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

* **Dual-Language Architecture (C++17 & Python 3):**
  * **C++ Core Engine:** High-performance, system-level tokenization with strict memory management, linear execution speed $O(N)$, and robust error handling.
  * **Python Visualizer Wrapper:** Interactive diagnostics, Jupyter/Colab support, and Graphviz rendering.
* **100% Data-Driven Architecture:** Complete separation of automaton logic from execution engine (`UniversalDFA`). Transition matrices, character sets, and final states are passed via declarative JSON configuration structures.
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

## 🛠️ Quick Start & Local Setup

### Python Environment Setup

1. **Clone the repository:**
```bash
   git clone [https://github.com/tsopronyuk/dfa-lexical-analyzer.git](https://github.com/tsopronyuk/dfa-lexical-analyzer.git)
   cd dfa-lexical-analyzer

```

2. **Install dependencies:**
Make sure system Graphviz is installed (`brew install graphviz` on macOS or `sudo apt install graphviz` on Ubuntu), then install Python requirements:
```bash
pip install -r requirements.txt

```


3. **Run unit tests:**
```bash
pytest tests/

```



### C++ Compilation

Compile and run high-performance tests using C++17 compiler:

```bash
g++ -std=c++17 src/dfa_engine.cpp tests/test_dfa.cpp -I src -o test_runner
./test_runner

```

---

## 📁 Repository Structure

```text
dfa-lexical-analyzer/
│
├── .github/
│   └── workflows/
│       ├── python-tests.yml          # CI/CD: Automated Python tests (pytest)
│       └── cpp-tests.yml             # CI/CD: Automated C++17 build & tests
│
├── assets/                           # Visualization artifacts & generated graphs
│   ├── dot/                          # Raw Graphviz source code (.dot)
│   │   ├── example_1_paper.dot       # Hardcoded specialized DFA for Example 1
│   │   ├── example_1_universal.dot   # UniversalDFA generated graph for Example 1
│   │   ├── example_2_paper.dot       # Hardcoded specialized DFA for Example 2
│   │   ├── example_2_universal.dot   # UniversalDFA generated graph for Example 2
│   │   └── example_3_universal.dot   # UniversalDFA generated graph for Example 3
│   │
│   └── images/                       # Rendered vector diagrams (SVG for web / PDF for LaTeX)
│       ├── example_1_paper.pdf
│       ├── example_1_paper.svg
│       ├── example_1_universal.pdf
│       ├── example_1_universal.svg
│       ├── example_2_paper.pdf
│       ├── example_2_paper.svg
│       ├── example_2_universal.pdf
│       ├── example_2_universal.svg
│       ├── example_3_universal.pdf
│       └── example_3_universal.svg
│
├── configs/                          # Declarative JSON grammars for UniversalDFA
│   ├── example_1_keywords.json       # Config for Example 1 (Keywords)
│   ├── example_2_numbers.json        # Config for Example 2 (Numeric constants)
│   └── example_3_nested_keywords.json# Config for Example 3 (Nested keywords)
│
├── notebooks/                        # Interactive Research Notebooks
│   ├── 01_keywords_dfa.ipynb         # Example 1 Notebook
│   ├── 02_numbers_dfa.ipynb          # Example 2 Notebook
│   └── 03_table_driven_dfa_lexer.ipynb# UniversalDFA Notebook
│
├── src/                              # Core Lexical Engine Source Code
│   ├── universal_dfa.py              # Universal Python Engine & Graphviz generator
│   └── dfa_engine.cpp                # High-performance C++17 DFA Engine
│
├── tests/                            # Automated Testing Suites
│   ├── test_lexer.py                 # Python unit tests
│   └── test_dfa.cpp                  # C++ unit tests & assertion suite
│
├── LICENSE                           # Open Source License (MIT)
├── README.md                         # Repository Documentation
└── requirements.txt                  # Python dependencies (graphviz, pytest)

```

---

## 📜 License

This project is open-source and available under the [MIT License](https://www.google.com/search?q=LICENSE).
