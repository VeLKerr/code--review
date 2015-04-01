/*
 * Интерфейс прокомментирован с целью объяснить,
 * почему он написан так, а не иначе. В реальной жизни
 * так никто никогда не делает. Комментарии к коду,
 * которые остались бы в его рабочем варианте, заданы
 * с помощью команды однострочного комментария // и написаны
 * на английском языке, как рекомендуется.
 * Остальные комментарии здесь исключительно в учебных целях.
 */

#include <algorithm>
#include <deque>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <unordered_set>
#include <iterator>
#include <sstream>
#include <memory>
#include <stdexcept>
#include <utility>

//  std::make_unique will be available since c++14
//  Implementation was taken from http://herbsutter.com/gotw/_102/
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

/*
 * Часто в c++ приходится иметь дело с парой итераторов,
 * которые представляют из себя полуинтервал. Например,
 * функция std:sort принимает пару итераторов, участок
 * между которыми нужно отсортировать. В с++11 появился
 * удобный range-based for, который позволяет итерироваться
 * по объекту, для которого определены функции std::begin
 * и std::end (например, это объекты: массив фиксированного
 * размера, любой объект, у которого определены методы
 * begin() и end()). То есть удобный способ итерироваться
 * по std::vector такой:
 * for (const std::string& string: words).
 * Однако, для некоторых объектов существует не один способ
 * итерироваться по ним. Например std::map: мы можем
 * итерироваться по парам объект-значение (как это сделает
 * for(...: map)), или мы можем итерироваться по ключам.
 * Для этого мы можем сделать функцию:
 * IteratorRange<...> keys(const std::map& map),
 * которой можно удобно воспользоваться:
 * for(const std::string& key: keys(dictionary)).
 */
template<class Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end):
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

template<class Visitor, class Graph, class Vertex>
void BreadthFirstSearch(Vertex origin_vertex, const Graph& graph, Visitor visitor);

/*
 * Для начала мы рекомендуем ознакомиться с общей
 * концепцией паттерна проектирования Visitor:
 * http://en.wikipedia.org/wiki/Visitor_pattern
 * Для применения Visitor'а к задаче обхода графа
 * можно ознакомиться с
 * http://www.boost.org/doc/libs/1_57_0/libs/graph/doc/BFSVisitor.html
 */
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

struct AutomatonNode {
    AutomatonNode():
        suffix_link(nullptr),
        terminal_link(nullptr) {
    }

    std::vector<size_t> matched_string_ids;
    // Stores tree structure of nodes
    std::map<char, AutomatonNode> trie_transitions;
    /*
     * Обратите внимание, что std::set/std::map/std::list
     * при вставке и удалении неинвалидируют ссылки на
     * остальные элементы контейнера. Но стандартные контейнеры
     * std::vector/std::string/std::deque таких гарантий не
     * дают, поэтому хранение указателей на элементы этих
     * контейнеров крайне не рекомендуется.
     */
    // Stores pointers to the elements of trie_transitions
    std::map<char, AutomatonNode*> automaton_transitions;
    AutomatonNode* suffix_link;
    AutomatonNode* terminal_link;
};

// Returns nullptr if there is no such transition
AutomatonNode* GetTrieTransition(AutomatonNode* node, char character);

// Performs transition in automaton
AutomatonNode* GetNextNode(AutomatonNode* node, AutomatonNode* root, char character);

namespace internal {

class AutomatonGraph;

class AutomatonGraph {
public:
    struct Edge {
        Edge(AutomatonNode* source,
                AutomatonNode* target,
                char character):
            source(source),
            target(target),
            character(character) {
        }

        AutomatonNode* source;
        AutomatonNode* target;
        char character;
    };

    // Returns edges corresponding to all trie transitions from vertex
    std::vector<Edge> OutgoingEdges(AutomatonNode* vertex) const;
};

AutomatonNode* GetTarget(const AutomatonGraph::Edge& edge);

class SuffixLinkCalculator:
    public traverses::BfsVisitor<AutomatonNode*, AutomatonGraph::Edge> {
public:
    explicit SuffixLinkCalculator(AutomatonNode* root):
        root_(root) {}

    void ExamineVertex(AutomatonNode* node) /*override*/;

    void ExamineEdge(const AutomatonGraph::Edge& edge) /*override*/;

private:
    AutomatonNode* root_;
};

class TerminalLinkCalculator:
    public traverses::BfsVisitor<AutomatonNode*, AutomatonGraph::Edge> {
public:
    explicit TerminalLinkCalculator(AutomatonNode* root):
        root_(root) {}

    /*
     * Если вы не знакомы с этим ключевым словом,
     * то ознакомьтесь
     * http://en.wikipedia.org/wiki/C%2B%2B11#Explicit_overrides_and_final
     * override здесь и далее закомментирован, так
     * как у нас слишком старый компилятор с++0x в
     * контесте, который выдaет compilation error
     */
    void DiscoverVertex(AutomatonNode* node) /*override*/;

private:
    AutomatonNode* root_;
};

}  // namespace internal

/*
 * Объясним задачу, которую решает класс NodeReference.
 * Класс Automaton представляет из себя неизменяемый объект
 * (http://en.wikipedia.org/wiki/Immutable_object),
 * в данном случае, это означает, что единственное действие,
 * которое пользователь может совершать с готовым автоматом,
 * это обходить его разными способами. Это значит, что мы
 * должны предоставить пользователю способ получить вершину
 * автомата и дать возможность переходить между вершинами.
 * Одним из способов это сделать -- это предоставить
 * пользователю константный указатель на AutomatonNode,
 * а вместе с ним константый интерфейс AutomatonNode. Однако,
 * этот вариант ведет к некоторым проблемам.
 * Во-первых, этот же интерфейс AutomatonNode мы должны
 * использовать и для общения автомата с этим внутренним
 * представлением вершины. Так как константная версия
 * этого же интерфейса будет доступна пользователю, то мы
 * ограничены в добавлении функций в этот константный
 * интерфейс (не все функции, которые должны быть доступны
 * автомату должны быть доступны пользователю). Во-вторых,
 * так как мы используем кэширование при переходе по символу
 * в автомате, то условная функция getNextNode не может быть
 * константной (она заполняет кэш переходов). Это значит,
 * что мы лишены возможности добавить функцию "перехода
 * между вершинами" в константный интерфейс (то есть,
 * предоставить ее пользователю константного указателя на
 * AutomatonNode).
 * Во избежание этих проблем, мы создаем отдельный
 * класс, отвечающий ссылке на вершину, который предоставляет
 * пользователю только нужный интерфейс.
 */
class NodeReference {
public:
    typedef std::vector<size_t>::const_iterator MatchedStringIterator;
    typedef IteratorRange<MatchedStringIterator> MatchedStringIteratorRange;

    NodeReference():
        node_(nullptr),
        root_(nullptr) {
    }
    NodeReference(AutomatonNode* node, AutomatonNode* root):
        node_(node), root_(root) {
    }

    NodeReference Next(char character) const;

    NodeReference suffixLink() const;

    NodeReference terminalLink() const;

    MatchedStringIteratorRange matchedStringIds() const;

    explicit operator bool() const {
        return node_ != nullptr;
    }

    bool operator==(NodeReference other) const;

private:
    AutomatonNode* node_;
    AutomatonNode* root_;
};

using std::rel_ops::operator !=;

class AutomatonBuilder;

class Automaton {
public:
    /*
     * Чтобы ознакомиться с констркуцией =default, смотрите
     * http://en.wikipedia.org/wiki/C%2B%2B11#Explicitly_defaulted_and_deleted_special_member_functions
     */
    Automaton() = default;

    NodeReference Root() {
        return NodeReference(&root_, &root_);
    }

    /*
     * В этом случае есть два хороших способа получить
     * результат работы этой функции:
     * добавить параметр типа OutputIterator, который
     * последовательно записывает в него id найденных
     * строк, или же добавить параметр типа Callback,
     * который будет вызван для каждого такого id.
     * Чтобы ознакомиться с этими концепциями лучше,
     * смотрите ссылки:
     * http://en.cppreference.com/w/cpp/concept/OutputIterator и
     * http://en.wikipedia.org/wiki/Callback_(computer_programming).
     * По своей мощности эти способы эквивалентны. (см.
     * http://www.boost.org/doc/libs/release/libs/iterator/doc/function_output_iterator.html)
     * Так как в интерфейсе WildcardMatcher мы принимаем
     * Callback, то чтобы пользоваться одним и тем же средством
     * во всех интерфейсах, мы и здесь применяем Callback. Отметим,
     * что другие способы, как например, вернуть std::vector с
     * найденными id, не соответствуют той же степени гибкости, как
     * 2 предыдущие решения (чтобы в этом убедиться представьте
     * себе, как можно решить такую задачу: создать std::set
     * из найденных id).
     */
    // Calls on_match(string_id) for every string ending at
    // this node, i.e. collects all string ids reachable by
    // terminal links.
    template <class Callback>
    void GenerateMatches(NodeReference node, Callback on_match);

private:
    AutomatonNode root_;

    Automaton(const Automaton&) = delete;
    Automaton& operator=(const Automaton&) = delete;

    friend class AutomatonBuilder;
};

class AutomatonBuilder {
public:
    void Add(const std::string& string, size_t id);

    std::unique_ptr<Automaton> Build() {
        auto automaton = make_unique<Automaton>();
        BuildTrie(words_, ids_, automaton.get());
        BuildSuffixLinks(automaton.get());
        BuildTerminalLinks(automaton.get());
        return automaton;
    }

private:
    static void BuildTrie(const std::vector<std::string>& words,
            const std::vector<size_t>& ids,
            Automaton* automaton) {
        for (size_t i = 0; i < words.size(); ++i) {
            AddString(&automaton->root_, ids[i], words[i]);
        }
    }

    static void AddString(AutomatonNode* root, size_t string_id, const std::string& string);

    static void BuildSuffixLinks(Automaton* automaton);

    static void BuildTerminalLinks(Automaton* automaton);

    std::vector<std::string> words_;
    std::vector<size_t> ids_;
};

}  // namespace aho_corasick

// Consecutive delimiters are not grouped together and are deemed
// to delimit empty strings
template <class Predicate>
std::vector<std::string> Split(const std::string& string, Predicate is_delimiter);

// Wildcard is a character that may be substituted
// for any of all possible characters
class WildcardMatcher {
public:
    WildcardMatcher():
        number_of_words_(0),
        pattern_length_(0) {
    }

    void Init(const std::string& pattern, char wildcard);

    // Resets matcher to start scanning new stream
    void Reset();

    /* В данном случае Callback -- это функция,
     * которая будет вызвана при наступлении
     * события "суффикс совпал с шаблоном".
     * Почему мы выбрали именно этот способ сообщить
     * об этом событии? Можно рассмотреть альтернативы:
     * вернуть bool из Scan, принять итератор и записать
     * в него значение. В первом случае, значение bool,
     * возвращенное из Scan, будет иметь непонятный
     * смысл. True -- в смысле все считалось успешно?
     * True -- произошло совпадение? В случае итератора,
     * совершенно не ясно, какое значение туда  записывать
     * (подошедший суффикс, true, ...?). Более того, обычно,
     * если при сканировании потока мы наткнулись на
     * совпадение, то нам нужно как-то на это отреагировать,
     * это действие и есть наш Callback on_match.
     */
    // Scans new character and calls on_match() if
    // suffix of scanned characters matches pattern
    template<class Callback>
    void Scan(char character, Callback on_match);

private:
    // Storing only O(|pattern|) elements allows us
    // to consume only O(|pattern|) memory for matcher
    std::deque<size_t> words_occurrences_by_position_;
    aho_corasick::NodeReference state_;
    size_t number_of_words_;
    size_t pattern_length_;
    std::unique_ptr<aho_corasick::Automaton> aho_corasick_automaton_;
};

std::string ReadString(std::istream& input_stream);

// Returns positions of the first character of every match
std::vector<size_t> FindFuzzyMatches(const std::string& patternWithWildcards,
        const std::string& text,
        char wildcard) {
    WildcardMatcher matcher;
    matcher.Init(patternWithWildcards, wildcard);
    std::vector<size_t> occurrences;
    for (size_t offset = 0; offset < text.size(); ++offset) {
        matcher.Scan(text[offset],
            [&occurrences, offset, &patternWithWildcards] {
                occurrences.push_back(offset + 1 - patternWithWildcards.size());
            });
    }
    return occurrences;
}

void Print(const std::vector<size_t>& sequence);

void testAll();

int main() {
    const char wildcard = '?';
    const std::string patternWithWildcards = ReadString(std::cin);
    const std::string text = ReadString(std::cin);
    Print(FindFuzzyMatches(patternWithWildcards, text, wildcard));
    return 0;
}

// ===== TESTING ZONE =====

template<class T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& vector) {
    std::copy(
        vector.begin(),
        vector.end(),
        std::ostream_iterator<T>(os, " ")
        );
    return os;
}

class TestNotPassedException : public std::runtime_error {
public:
    explicit TestNotPassedException(const char* what) :
        std::runtime_error(what)
    {}

    explicit TestNotPassedException(const std::string& what) :
        std::runtime_error(what.c_str())
    {}
};

#define REQUIRE_EQUAL(first, second) do { \
    auto firstValue = (first); \
    auto secondValue = (second); \
    if (firstValue != secondValue) { \
        std::ostringstream oss; \
        oss << "Require equal failed: " << # first << " != " << # second \
            << " (" << firstValue << " != " << secondValue << ")\n"; \
        throw TestNotPassedException(oss.str()); \
    } \
    } while (false)

void testAll() {
    {
        aho_corasick::AutomatonBuilder builder;
        builder.Add("suffix", 1);
        builder.Add("ffix", 2);
        builder.Add("ix", 3);
        builder.Add("abba", 4);

        std::unique_ptr<aho_corasick::Automaton> automaton =
            builder.Build();

        const std::string text = "let us find some suffix";
        aho_corasick::NodeReference node = automaton->Root();
        for (char ch: text) {
            node = node.Next(ch);
        }
        std::vector<size_t> string_ids;

        automaton->GenerateMatches(node, [&string_ids](size_t string_id) {
             string_ids.push_back(string_id);
        });
        std::sort(string_ids.begin(), string_ids.end());

        REQUIRE_EQUAL(string_ids, std::vector<size_t>({1, 2, 3}));
    }
    {
        WildcardMatcher matcher;
        matcher.Init("a?c?", '?');

        {
            std::vector<size_t> occurrences;
            //                              012345678901234
            const std::string first_text = "abcaaccxaxcxacc";
            for (size_t i = 0; i < first_text.size(); ++i) {
                matcher.Scan(first_text[i], [&occurrences, i] () {
                    occurrences.push_back(i);
                });
            }

            REQUIRE_EQUAL(occurrences, std::vector<size_t>({3, 6, 7, 11}));
        }
        {
            matcher.Reset();
            std::vector<size_t> occurrences;
            const std::string second_text = "xyzadcc";
            for (size_t i = 0; i < second_text.size(); ++i) {
                matcher.Scan(second_text[i], [&occurrences, i] () {
                    occurrences.push_back(i);
                });
            }

            REQUIRE_EQUAL(occurrences, std::vector<size_t>({6}));
        }
    }
}
