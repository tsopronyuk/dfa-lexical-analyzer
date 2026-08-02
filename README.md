# DFA Lexical Analyzer and Visualizer 🔍

[![Python Tests](https://github.com/tsopronyuk/dfa-lexical-analyzer/actions/workflows/python-tests.yml/badge.svg)](https://github.com/YOUR_USERNAME/dfa-lexical-analyzer/actions)

This repository contains implementations of lexical analyzers based on explicit Deterministic Finite Automata (DFA). The main goal of this project is to demonstrate transparent, step-by-step tokenization with the automatic generation of compact state transition graphs.

## 🚀 Interactive Examples (Google Colab)

You can run the source code, review the step-by-step text analysis, and view the graph rendering directly in your browser by clicking the badges below:

**Example 1: Keywords and Noise Filtering**  
A lexical analyzer for fixed and variable-length tokens (keywords `go`, `goon`, `open`, `online`, numbers, and skipping unexpected/noise characters).  
[![Open In Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/tsopronyuk/dfa-lexical-analyzer/blob/main/notebooks/01_keywords_dfa.ipynb)

**Example 2: Numeric Constants and Identifiers**  
An advanced lexer with explicit backtracking support. It recognizes binary, octal, decimal, and hexadecimal constants, as well as identifiers, utilizing an optimized and compact DFA state visualization.  
[![Open In Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/tsopronyuk/dfa-lexical-analyzer/blob/main/notebooks/02_numbers_dfa.ipynb)

## 🛠 Technologies
* Python 3
* Graphviz
* IPython / Jupyter Notebook
