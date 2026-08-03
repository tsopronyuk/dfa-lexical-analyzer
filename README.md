# Universal DFA Lexical Analyzer and Visualizer 🔍

[![Python Tests](https://github.com/tsopronyuk/dfa-lexical-analyzer/actions/workflows/python-tests.yml/badge.svg)](https://github.com/tsopronyuk/dfa-lexical-analyzer/actions)
[![C++ Tests](https://github.com/tsopronyuk/dfa-lexical-analyzer/actions/workflows/cpp-tests.yml/badge.svg)](https://github.com/tsopronyuk/dfa-lexical-analyzer/actions)

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
  * **C++ Core Engine:** High-performance, system-level tokenization with strict memory management, linear execution speed $\mathcal{O}(N)$, and robust error handling.
  * **Python Visualizer Wrapper:** Interactive diagnostics, Jupyter/Colab support, and Graphviz rendering.
* **100% Data-Driven Architecture:** Complete separation of automaton logic from execution engine (`UniversalDFA`). Transition matrices, character sets, and final states are passed via declarative configuration structures.
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

## 📁 Repository Structure

```text
dfa-lexical-analyzer/
├── .github/
│   └── workflows/
│       ├── python-tests.yml  # CI/CD pipeline for Python unit tests
│       └── cpp-tests.yml     # CI/CD pipeline for C++ compilation and assertions
├── notebooks/
│   ├── 01_keywords_dfa.ipynb
│   ├── 02_numbers_dfa.ipynb
│   └── 03_table_driven_dfa_lexer.ipynb
├── src/
│   ├── universal_dfa.py      # Core Python UniversalDFA visualizer class
│   └── dfa_engine.cpp        # High-performance C++17 UniversalDFA engine
├── tests/
│   ├── test_lexer.py         # Python unit tests (unittest)
│   └── test_dfa.cpp          # C++ unit tests & automated assertion suite
└── README.md
