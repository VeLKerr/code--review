#include <algorithm>
#include <deque>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <sstream>
#include <stdexcept>
#include <unordered_set>
#include <utility>
#include <vector>

#if __cplusplus < 201103L
# define override
#endif

using std::deque;
using std::map;
using std::string;
using std::unique_ptr;
using std::vector;

using std::cin;
using std::cout;
using std::endl;

//  std::make_unique will be available since c++14
//  Implementation was taken from http://herbsutter.com/gotw/_102/
template<typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args) {
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template<class Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end) :
        begin_(begin), end_(end) {}

    Iterator begin() const {
        return begin_;
    }
    Iterator end() const {
        return end_;
    }
private:
    Iterator begin_, end_;
};

namespace traverses {
    using std::unordered_set;

    template<class Visitor, class Graph, class Vertex>
    void BreadthFirstSearch(Vertex origin_vertex, const Graph& graph, Visitor visitor) {
        deque<Vertex> grey_vertices;
        unordered_set<Vertex> black_vertices;
        visitor.DiscoverVertex(origin_vertex);
        grey_vertices.push_back(origin_vertex);
        black_vertices.insert(origin_vertex);
        while (!grey_vertices.empty()) {
            Vertex vertex = grey_vertices.front();
            grey_vertices.pop_front();
            visitor.ExamineVertex(vertex);
            for (const auto& edge : graph.OutgoingEdges(vertex)) {
                visitor.ExamineEdge(edge);
                if (black_vertices.find(edge.target) == black_vertices.end()) {
                    visitor.DiscoverVertex(edge.target);
                    black_vertices.insert(edge.target);
                    grey_vertices.push_back(edge.target);
                }
            }
        }
    }

    // See "Visitor Event Points" on
    // http://www.boost.org/doc/libs/1_57_0/libs/graph/doc/breadth_first_search.html
    template<class Vertex, class Edge>
    class BfsVisitor {
    public:
        virtual void DiscoverVertex(Vertex /*vertex*/) {}
        virtual void ExamineEdge(const Edge& /*edge*/) {}
        virtual void ExamineVertex(Vertex /*vertex*/) {}
        virtual ~BfsVisitor() {}
    };

}  // namespace traverses

namespace aho_corasick { 
    using std::rel_ops::operator !=;

    struct AutomatonNode {
        AutomatonNode() :
        suffix_link(nullptr),
        terminal_link(nullptr) {}

        vector<size_t> matched_string_ids;
        // Stores tree structure of nodes
        map<char, AutomatonNode> trie_transitions;
        // Stores pointers to the elements of trie_transitions
        map<char, AutomatonNode*> automaton_transitions;
        AutomatonNode* suffix_link;
        AutomatonNode* terminal_link;
    };

    typedef std::pair<const char, AutomatonNode> TransitionEntry;

    // Returns nullptr if there is no such transition
    AutomatonNode* GetTrieTransition(AutomatonNode* node, char character) {
        auto search_result = node->trie_transitions.find(character);
        if (search_result == node->trie_transitions.end()) {
            return nullptr;
        }
        return &search_result->second;
    }

    // Performs transition in automaton
    AutomatonNode* GetNextNode(AutomatonNode* node, AutomatonNode* root, char character) {
        if (!node->automaton_transitions[character]) {
            AutomatonNode* next_node = GetTrieTransition(node, character);
            if (!next_node) {
                if (node == root) {
                    next_node = root;
                } else {
                    next_node = GetNextNode(node->suffix_link, root, character);
                }
            }
            return node->automaton_transitions[character] = next_node;
        } else {
            return node->automaton_transitions[character];
        }
    }


    class AutomatonGraph;

    class AutomatonGraph {
    public:
        struct Edge {
            Edge(AutomatonNode* source,
            AutomatonNode* target,
            char character) :
            source(source),
            target(target),
            character(character) {}

            AutomatonNode* source;
            AutomatonNode* target;
            char character;
        };

        // Returns edges corresponding to all trie transitions from vertex
        vector<Edge> OutgoingEdges(AutomatonNode* vertex) const {
            vector<Edge> result;
            for (TransitionEntry& transition_pair : vertex->trie_transitions) {
                result.emplace_back(vertex, &transition_pair.second, transition_pair.first);
            }
            return result;
        }
    };

    AutomatonNode* GetTarget(const AutomatonGraph::Edge& edge) {
        return edge.target;
    }

    class SuffixLinkCalculator :
        public traverses::BfsVisitor<AutomatonNode*, AutomatonGraph::Edge> {
    public:
        explicit SuffixLinkCalculator(AutomatonNode* root) :
            root_(root) {}

        void ExamineVertex(AutomatonNode* node) override {
            if (node == root_) {
                node->suffix_link = node;
            }
        }

        void ExamineEdge(const AutomatonGraph::Edge& edge) override {
            if (edge.target->suffix_link) {
                return;
            }
            if (edge.source == root_) {
                edge.target->suffix_link = root_;
            } else {
                edge.target->suffix_link =
                    GetNextNode(edge.source->suffix_link, root_, edge.character);
            }
        }

    private:
        AutomatonNode* root_;
    };

    class TerminalLinkCalculator :
        public traverses::BfsVisitor<AutomatonNode*, AutomatonGraph::Edge> {
    public:
        explicit TerminalLinkCalculator(AutomatonNode* root) :
            root_(root) {}

        void DiscoverVertex(AutomatonNode* node) override {
            if (node == root_) {
                return;
            }
            if (!node->suffix_link->matched_string_ids.empty()) {
                node->terminal_link = node->suffix_link;
            } else {
                node->terminal_link = node->suffix_link->terminal_link;
            }
        }

    private:
        AutomatonNode* root_;
    };

    class NodeReference {
    public:
        typedef vector<size_t>::const_iterator MatchedStringIterator;
        typedef IteratorRange<MatchedStringIterator> MatchedStringIteratorRange;

        NodeReference() :
            node_(nullptr),
            root_(nullptr) {}
        NodeReference(AutomatonNode* node, AutomatonNode* root) :
            node_(node), root_(root) {}

        NodeReference Next(char character) const {
            return NodeReference(GetNextNode(node_, root_, character), root_);
        }

        NodeReference suffixLink() const {
            return NodeReference(node_->suffix_link, root_);
        }

        NodeReference terminalLink() const {
            return NodeReference(node_->terminal_link, root_);
        }

        MatchedStringIteratorRange matchedStringIds() const {
            return MatchedStringIteratorRange(node_->matched_string_ids.begin(),
                node_->matched_string_ids.end());
        }

        explicit operator bool() const {
            return node_ != nullptr;
        }

        bool operator == (NodeReference other) const {
            return node_ == other.node_ && root_ == other.root_;
        }

    private:
        AutomatonNode* node_;
        AutomatonNode* root_;
    };

    class AutomatonBuilder;

    class Automaton {
    public:
        Automaton() = default;

        NodeReference Root() {
            return NodeReference(&root_, &root_);
        }

        // Calls on_match(string_id) for every string ending at
        // this node, i.e. collects all string ids reachable by
        // terminal links.
        template <class Callback>
        void GenerateMatches(NodeReference node, Callback on_match) {
            for (; node; node = node.terminalLink()) {
                for (int string_id: node.matchedStringIds()) {
                    on_match(string_id);
                }
            }
        }

    private:
        AutomatonNode root_;

        Automaton(const Automaton&) = delete;
        Automaton& operator=(const Automaton&) = delete;

        friend class AutomatonBuilder;
    };

    class AutomatonBuilder {
    public:
        void Add(const string& string, size_t id) {
            words_.push_back(string);
            ids_.push_back(id);
        }

        unique_ptr<Automaton> Build() {
            unique_ptr<Automaton> automaton = make_unique<Automaton>();
            BuildTrie(words_, ids_, automaton.get());
            BuildSuffixLinks(automaton.get());
            BuildTerminalLinks(automaton.get());
            return automaton;
        }

    private:
        static void BuildTrie(const vector<string>& words,
            const vector<size_t>& ids,
            Automaton* automaton) {
            for (size_t i = 0; i < words.size(); ++i) {
                AddString(&automaton->root_, ids[i], words[i]);
            }
        }

        static void AddString(AutomatonNode* root, size_t string_id, const string& string) {
            AutomatonNode* node = root;
            for (size_t i = 0; i < string.length(); ++i) {
                if (!GetTrieTransition(node, string[i])) {
                    node->trie_transitions.insert(TransitionEntry(
                        string[i], AutomatonNode()));
                }
                node = GetTrieTransition(node, string[i]);
            }
            node->matched_string_ids.push_back(string_id);
        }

        static void BuildSuffixLinks(Automaton* automaton) {
            traverses::BreadthFirstSearch(&automaton->root_,
                AutomatonGraph(),
                SuffixLinkCalculator(&automaton->root_));
        }

        static void BuildTerminalLinks(Automaton* automaton) {
            traverses::BreadthFirstSearch(&automaton->root_,
                AutomatonGraph(),
                TerminalLinkCalculator(&automaton->root_));
        }

        vector<string> words_;
        vector<size_t> ids_;
    };

}  // namespace aho_corasick

// Consecutive delimiters are not grouped together and are deemed
// to delimit empty strings
template <class Predicate>
vector<string> Split(const string& str, Predicate is_delimiter) {
    vector<string> result;
    size_t start = 0;
    for (size_t i = 0; i < str.length(); ++i) {
        if (is_delimiter(str[i])) {
            result.push_back(str.substr(start, i - start));
            start = i + 1;
        }
    }
    result.push_back(str.substr(start, str.length() - start));
    return result;
}

// Wildcard is a character that may be substituted
// for any of all possible characters
class WildcardMatcher {
public:
    WildcardMatcher() :
        number_of_words_(0),
        pattern_length_(0) {}

    void Init(const string& pattern, char wildcard) {
        aho_corasick::AutomatonBuilder builder;
        vector<string> words = Split(pattern, [wildcard](int symbol) {
            return symbol == wildcard;
        });
        pattern_length_ = pattern.length();

        size_t offset = 0;
        for (size_t i = 0; i < words.size(); ++i) {
            if (!words[i].empty()) {
                offset += words[i].length();
                builder.Add(words[i], i);
                offsets_[i] = offset - 1;
                number_of_words_++;
            }
            offset++;
        }
        aho_corasick_automaton_ = builder.Build();
        Reset();
    }

    // Resets matcher to start scanning new stream
    void Reset() {
        state_ = aho_corasick_automaton_->Root();
        words_occurrences_by_position_.clear();
    }

    // Scans new character and calls on_match() if
    // suffix of scanned characters matches pattern
    template<class Callback>
    void Scan(char character, Callback on_match) {
        state_ = state_.Next(character);
        words_occurrences_by_position_.push_back(0);
        size_t deque_last_pos = words_occurrences_by_position_.size() - 1;

        aho_corasick_automaton_->GenerateMatches(state_,
            [&](size_t id) {
            if (deque_last_pos >= offsets_[id]) {
                /** 
                 * тут можно было поставить ++, но система ругается. У меня же ++ работает.
                 */
                words_occurrences_by_position_[deque_last_pos - offsets_[id]] = 
                    words_occurrences_by_position_[deque_last_pos - offsets_[id]] + 1;
            }
        });

        if (words_occurrences_by_position_.size() > pattern_length_) {
            words_occurrences_by_position_.pop_front();
        }

        if (words_occurrences_by_position_.size() >= pattern_length_ &&
            words_occurrences_by_position_.front() >= number_of_words_) {
            on_match();
        }
    }

private:
    map<size_t, size_t> offsets_;
    // Storing only O(|pattern|) elements allows us
    // to consume only O(|pattern|) memory for matcher
    deque<size_t> words_occurrences_by_position_;
    aho_corasick::NodeReference state_;
    size_t number_of_words_;
    size_t pattern_length_;
    unique_ptr<aho_corasick::Automaton> aho_corasick_automaton_;
};

string ReadString(std::istream& input_stream) {
    string result;
    std::getline(input_stream, result);
    return result;
}

// Returns positions of the first character of every match
vector<size_t> FindFuzzyMatches(const string& patternWithWildcards,
    const string& text,
    char wildcard) {
    WildcardMatcher matcher;

    matcher.Init(patternWithWildcards, wildcard);
    vector<size_t> occurrences;
    for (size_t offset = 0; offset < text.length(); ++offset) {
        matcher.Scan(text[offset],
            [&occurrences, offset, &patternWithWildcards] {
            occurrences.push_back(offset + 1 - patternWithWildcards.length());
        });
    }
    return occurrences;
}

void Print(const vector<size_t>& sequence) {
    cout << sequence.size() << endl;
    for (size_t value : sequence) {
        cout << value << " ";
    }
}

int main() {
    const char wildcard = '?';
    const string patternWithWildcards = ReadString(cin);
    const string text = ReadString(cin);
    Print(FindFuzzyMatches(patternWithWildcards, text, wildcard));
    return 0;
}
