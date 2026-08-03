#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>

// ============================================================================
// DATA STRUCTURES
// ============================================================================

/**
 * @brief Represents a single token emitted by the DFA lexer.
 */
struct Token {
    std::string type;
    std::string value;
};

/**
 * @brief Information associated with a terminal (final) state.
 */
struct FinalStateInfo {
    std::string name;
    std::string color = "white";
};

/**
 * @brief Rule for handling explicit prefix lookahead and backtracking.
 */
struct BacktrackRule {
    std::vector<int> from_states;
    int emit_type_id;
    std::string emit_value;
    int step = 1;
};

/**
 * @brief Configuration container for initializing a UniversalDFA instance.
 */
struct DFAConfig {
    std::string name;
    std::vector<std::pair<std::string, int>> char_classes;
    int default_class;
    std::vector<std::vector<int>> matrix;
    std::unordered_map<int, FinalStateInfo> final_states;
    std::unordered_map<int, BacktrackRule> backtrack_rules;
};

// ============================================================================
// UNIVERSAL DFA ENGINE CLASS
// ============================================================================

/**
 * @brief Generic Deterministic Finite Automaton (DFA) Engine supporting 
 *        noise filtering, multi-level backtracking, and Graphviz DOT export.
 */
class UniversalDFA {
private:
    std::string name;
    std::vector<std::vector<int>> matrix;
    std::vector<std::pair<std::string, int>> char_classes;
    int default_class;
    std::unordered_map<int, FinalStateInfo> final_states;
    std::unordered_map<int, BacktrackRule> backtrack_rules;
    int skip_state_id;

    /**
     * @brief Maps an input character to its corresponding character class index.
     */
    int get_char_class(char c) const {
        for (const auto& [pattern, cls_idx] : char_classes) {
            if (pattern == "isalpha_or_underscore") {
                if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
                    return cls_idx;
                }
            } else if (pattern.find(c) != std::string::npos) {
                return cls_idx;
            }
        }
        return default_class;
    }

    /**
     * @brief Formats raw character class patterns into compact readable strings.
     */
    std::string format_label(const std::string& raw) const {
        if (raw == "isalpha_or_underscore") return "a-z, _";
        if (raw == "abcdefABCDEF") return "a-f, A-F";

        bool is_all_digit = !raw.empty() && std::all_of(raw.begin(), raw.end(), ::isdigit);
        if (is_all_digit && raw.length() > 3) {
            return std::string(1, raw.front()) + ".." + std::string(1, raw.back());
        }

        bool is_all_alpha = !raw.empty() && std::all_of(raw.begin(), raw.end(), ::isalpha);
        if (is_all_alpha && raw.length() > 4) {
            return std::string(1, raw.front()) + ".." + std::string(1, raw.back());
        }
        return raw;
    }

    /**
     * @brief Helper function to join vector of string elements with a delimiter.
     */
    std::string join_strings(const std::vector<std::string>& vec, const std::string& delim) const {
        std::string res;
        for (size_t i = 0; i < vec.size(); ++i) {
            res += vec[i];
            if (i + 1 < vec.size()) res += delim;
        }
        return res;
    }

public:
    /**
     * @brief Constructs a UniversalDFA instance using the given configuration.
     */
    explicit UniversalDFA(const DFAConfig& config)
        : name(config.name),
          matrix(config.matrix),
          char_classes(config.char_classes),
          default_class(config.default_class),
          final_states(config.final_states),
          backtrack_rules(config.backtrack_rules) 
    {
        // Compute skip_state_id (minimum negative state id)
        skip_state_id = 0;
        if (!final_states.empty()) {
            skip_state_id = final_states.begin()->first;
            for (const auto& [st, _] : final_states) {
                if (st < skip_state_id) skip_state_id = st;
            }
        }
    }

    /**
     * @brief Performs lexical analysis on the input text stream.
     * @param text Input string to tokenize.
     * @return Vector of identified Token objects.
     */
    std::vector<Token> tokenize(const std::string& text) const {
        std::vector<Token> tokens;
        std::string text_buffer = text + '\0'; // Sentinel zero byte
        size_t start = 0, i = 0;
        int state = 0;

        while (start < text_buffer.size() - 1) {
            char c = text_buffer[i];
            int cl = get_char_class(c);
            int next_state = matrix[state][cl];

            if (next_state < 0) {
                // 1. Skip noise characters
                if (next_state == skip_state_id) {
                    start++;
                    i = start;
                    state = 0;
                } 
                // 2. Handle explicit backtracking rules
                else if (backtrack_rules.count(next_state)) {
                    const auto& rule = backtrack_rules.at(next_state);
                    bool from_valid = std::find(rule.from_states.begin(), 
                                                rule.from_states.end(), 
                                                state) != rule.from_states.end();
                    if (from_valid) {
                        std::string type_name = final_states.count(rule.emit_type_id) 
                            ? final_states.at(rule.emit_type_id).name : "Unknown";
                        tokens.push_back({type_name, rule.emit_value});
                        start += rule.step;
                        i = start;
                        state = 0;
                    } else {
                        std::string val = text_buffer.substr(start, i - start);
                        std::string type_name = final_states.count(next_state) 
                            ? final_states.at(next_state).name : "Unknown";
                        tokens.push_back({type_name, val});
                        start = i;
                        state = 0;
                    }
                } 
                // 3. Normal final state completion
                else {
                    std::string val = text_buffer.substr(start, i - start);
                    std::string type_name = final_states.count(next_state) 
                        ? final_states.at(next_state).name : "Unknown";
                    tokens.push_back({type_name, val});
                    start = i;
                    state = 0;
                }
            } else {
                state = next_state;
                i++;
            }
        }
        return tokens;
    }

    /**
     * @brief Exports the DFA transition structure as an optimized Graphviz DOT diagram.
     * @param filename Target path for the output .dot file.
     * @param hide_noise If true, suppresses display of noise-skipping transitions.
     * @param rankdir Orientation of the graph ('auto', 'LR', or 'TB').
     * @param max_keyword_len Length threshold to auto-switch from 'LR' to 'TB'.
     */
    void export_dot(const std::string& filename, bool hide_noise = true, 
                    std::string rankdir = "auto", int max_keyword_len = 5) const 
    {
        std::ofstream out(filename);
        if (!out.is_open()) return;

        // 1. Determine graph orientation automatically (LR vs TB)
        std::string effective_rankdir = rankdir;
        if (rankdir == "auto") {
            int detected_max_len = 0;
            for (const auto& [st_id, info] : final_states) {
                size_t p1 = info.name.find('(');
                size_t p2 = info.name.find(')', p1);
                if (p1 != std::string::npos && p2 != std::string::npos && p2 > p1) {
                    std::string kw = info.name.substr(p1 + 1, p2 - p1 - 1);
                    detected_max_len = std::max(detected_max_len, static_cast<int>(kw.length()));
                }
            }
            for (const auto& [st_id, rule] : backtrack_rules) {
                detected_max_len = std::max(detected_max_len, static_cast<int>(rule.emit_value.length()));
            }
            effective_rankdir = (detected_max_len > max_keyword_len) ? "TB" : "LR";
        }

        // Write DOT Graph Header & Attributes
        out << "digraph \"" << name << "\" {\n";
        out << "    rankdir=" << effective_rankdir << ";\n";
        out << "    nodesep=0.2;\n";
        out << "    ranksep=0.3;\n";
        out << "    margin=0.02;\n";
        out << "    node [fontsize=9, height=0.25, width=0.25, margin=0.03];\n";
        out << "    edge [fontsize=8, arrowsize=0.6];\n\n";

        // 2. Map class indices to human-readable strings
        std::unordered_map<int, std::vector<std::string>> class_to_label;
        for (const auto& [char_set, cls_idx] : char_classes) {
            class_to_label[cls_idx].push_back(format_label(char_set));
        }
        if (class_to_label.find(default_class) == class_to_label.end()) {
            class_to_label[default_class] = {"other"};
        }

        // 3. Define Initial State 0
        out << "    0 [shape=doublecircle, fillcolor=gold, style=filled];\n";

        // 4. Define Final States (Negative IDs)
        for (const auto& [st_id, info] : final_states) {
            if (hide_noise && st_id == skip_state_id) continue;
            out << "    \"" << st_id << "\" [label=\"" << st_id << "\\n(" << info.name 
                << ")\", shape=box, style=filled, fillcolor=\"" << info.color << "\"];\n";
        }

        // 5. Define Intermediate States (1, 2, 3...)
        for (size_t s = 1; s < matrix.size(); ++s) {
            out << "    " << s << " [shape=circle];\n";
        }
        out << "\n";

        // 6. Render state transitions with intelligent 'else' grouping
        for (size_t s_from = 0; s_from < matrix.size(); ++s_from) {
            std::unordered_map<int, std::vector<int>> target_map;

            for (size_t cls_idx = 0; cls_idx < matrix[s_from].size(); ++cls_idx) {
                int s_to = matrix[s_from][cls_idx];
                if (hide_noise && s_to == skip_state_id) continue;
                target_map[s_to].push_back(static_cast<int>(cls_idx));
            }

            if (target_map.empty()) continue;

            int else_target = 999999;
            bool has_else = false;

            if (target_map.size() > 1) {
                // Rule 1: A branch catching default_class AND at least one other class
                for (const auto& [s_to, cls_list] : target_map) {
                    bool contains_default = std::find(cls_list.begin(), cls_list.end(), default_class) != cls_list.end();
                    if (contains_default && cls_list.size() > 1) {
                        else_target = s_to;
                        has_else = true;
                        break;
                    }
                }
                // Rule 2: If no branch catches default_class, but one catches >= 4 classes
                if (!has_else) {
                    int max_classes = 0;
                    for (const auto& [s_to, cls_list] : target_map) {
                        bool contains_default = std::find(cls_list.begin(), cls_list.end(), default_class) != cls_list.end();
                        if (static_cast<int>(cls_list.size()) > max_classes && !contains_default) {
                            max_classes = static_cast<int>(cls_list.size());
                            else_target = s_to;
                        }
                    }
                    if (max_classes >= 4) {
                        has_else = true;
                    }
                }
            }

            // Render edges
            for (const auto& [s_to, cls_list] : target_map) {
                std::string label_str;
                if (has_else && s_to == else_target) {
                    label_str = "else";
                } else {
                    std::vector<std::string> labels;
                    for (int c_idx : cls_list) {
                        if (class_to_label.count(c_idx)) {
                            for (const auto& l : class_to_label.at(c_idx)) labels.push_back(l);
                        } else {
                            labels.push_back("other");
                        }
                    }
                    label_str = join_strings(labels, ", ");
                }

                bool is_terminal = (s_to < 0);
                bool is_backtrack = false;
                if (backtrack_rules.count(s_to)) {
                    const auto& rule = backtrack_rules.at(s_to);
                    if (std::find(rule.from_states.begin(), rule.from_states.end(), static_cast<int>(s_from)) != rule.from_states.end()) {
                        is_backtrack = true;
                    }
                }

                out << "    " << s_from << " -> \"" << s_to << "\" [label=\" " << label_str << " \"";
                if (is_backtrack) {
                    out << ", style=dashed, color=darkorange, fontcolor=darkorange";
                } else if (is_terminal) {
                    out << ", style=dashed, color=gray30, fontcolor=gray20";
                }
                out << "];\n";
            }
        }

        out << "}\n";
        out.close();
    }
};
