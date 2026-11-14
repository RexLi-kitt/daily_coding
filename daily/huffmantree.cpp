#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <queue>
#include <stdexcept>
#include <string>
#include <vector>

// 文件格式：魔数 HUF1 + 256 个 64 位小端频率 + 霍夫曼编码数据。
// 按字节处理，因此文本、图片和 Word 文档都可以使用同一套算法。
namespace {
using Frequencies = std::array<std::uint64_t, 256>;

struct Node {
    std::uint64_t weight;
    int symbol;                   // 叶节点为 0~255，内部节点为 -1
    int order;                    // 权重相同时用于确定建树顺序
    Node *left, *right;
    Node(std::uint64_t w, int s, int o, Node* l = nullptr, Node* r = nullptr)
        : weight(w), symbol(s), order(o), left(l), right(r) {}
    bool leaf() const { return left == nullptr && right == nullptr; }
};
struct Compare {
    bool operator()(const Node* a, const Node* b) const {
        return a->weight != b->weight ? a->weight > b->weight : a->order > b->order;
    }
};

// 模板类管理霍夫曼树和编码表；解码时保留当前节点。
template <typename Symbol, std::size_t SymbolCount>
class HuffmanTree {
    std::vector<std::unique_ptr<Node>> nodes_;
    std::array<std::string, SymbolCount> codes_{};
    Node* root_ = nullptr;
    Node* current_ = nullptr;

    Node* makeNode(std::uint64_t w, int s, int order, Node* l = nullptr, Node* r = nullptr) {
        nodes_.emplace_back(new Node(w, s, order, l, r));
        return nodes_.back().get();
    }
    void makeCodes(Node* node, const std::string& path) {
        if (!node) return;
        if (node->leaf()) {
            codes_[static_cast<std::size_t>(node->symbol)] = path.empty() ? "0" : path;
            return;
        }
        makeCodes(node->left, path + '0');
        makeCodes(node->right, path + '1');
    }
public:
    explicit HuffmanTree(const std::array<std::uint64_t, SymbolCount>& freq) {
        std::priority_queue<Node*, std::vector<Node*>, Compare> queue;
        int order = 0;
        for (std::size_t i = 0; i < SymbolCount; ++i)
            if (freq[i]) queue.push(makeNode(freq[i], static_cast<int>(i), order++));
        while (queue.size() > 1) {
            Node* left = queue.top(); queue.pop();
            Node* right = queue.top(); queue.pop();
            if (left->weight > std::numeric_limits<std::uint64_t>::max() - right->weight)
                throw std::runtime_error("Invalid frequency table: size overflow.");
            queue.push(makeNode(left->weight + right->weight, -1, order++, left, right));
        }
        if (!queue.empty()) root_ = queue.top();
        current_ = root_;
        makeCodes(root_, "");
    }
    const std::string& code(Symbol symbol) const { return codes_[static_cast<std::size_t>(symbol)]; }
    // 返回 true 表示已到达叶节点，symbol 接收解码出的字节。
    bool decodeBit(bool bit, Symbol& symbol) {
        if (!current_) throw std::runtime_error("Invalid compressed data.");
        if (root_->leaf()) {
            if (bit) throw std::runtime_error("Invalid compressed data.");
            symbol = static_cast<Symbol>(root_->symbol);
            return true;
        }
        current_ = bit ? current_->right : current_->left;
        if (!current_) throw std::runtime_error("Invalid compressed data.");
        if (!current_->leaf()) return false;
        symbol = static_cast<Symbol>(current_->symbol);
        current_ = root_;
        return true;
    }
};

void writeU64(std::ostream& out, std::uint64_t value) {
    for (int i = 0; i < 8; ++i) { out.put(static_cast<char>(value & 0xff)); value >>= 8; }
}
std::uint64_t readU64(std::istream& in) {
    std::uint64_t value = 0;
    for (int i = 0; i < 8; ++i) {
        int c = in.get();
        if (c == EOF) throw std::runtime_error("Incomplete file header.");
        value |= static_cast<std::uint64_t>(static_cast<unsigned char>(c)) << (i * 8);
    }
    return value;
}

void compress(const std::string& input, const std::string& output) {
    std::ifstream in(input, std::ios::binary);
    if (!in) throw std::runtime_error("Cannot open input file.");
    Frequencies freq{};
    char buffer[65536];
    while (in.read(buffer, sizeof(buffer)) || in.gcount() > 0) {
        for (std::streamsize i = 0; i < in.gcount(); ++i) {
            auto& count = freq[static_cast<unsigned char>(buffer[i])];
            if (count == std::numeric_limits<std::uint64_t>::max())
                throw std::runtime_error("Input file is too large.");
            ++count;
        }
    }
    if (in.bad()) throw std::runtime_error("Error reading input file.");
    HuffmanTree<unsigned char, 256> tree(freq);
    in.clear(); in.seekg(0);
    if (!in) throw std::runtime_error("Cannot rewind input file.");
    std::ofstream out(output, std::ios::binary | std::ios::trunc);
    if (!out) throw std::runtime_error("Cannot open output file.");
    out.write("HUF1", 4);
    for (auto n : freq) writeU64(out, n);
    unsigned char byte = 0;
    int bits = 0;
    while (in.read(buffer, sizeof(buffer)) || in.gcount() > 0) {
        for (std::streamsize i = 0; i < in.gcount(); ++i) {
            const std::string& code = tree.code(static_cast<unsigned char>(buffer[i]));
            if (code.empty()) throw std::runtime_error("Input file changed during compression.");
            for (char bit : code) {
                byte = static_cast<unsigned char>((byte << 1) | (bit == '1'));
                if (++bits == 8) { out.put(static_cast<char>(byte)); byte = 0; bits = 0; }
            }
        }
    }
    if (in.bad()) throw std::runtime_error("Error reading input file.");
    if (bits) out.put(static_cast<char>(byte << (8 - bits)));
    out.close();
    if (!out) throw std::runtime_error("Error writing compressed file.");
}

void decompress(const std::string& input, const std::string& output) {
    std::ifstream in(input, std::ios::binary);
    if (!in) throw std::runtime_error("Cannot open input file.");
    char magic[4];
    if (!in.read(magic, 4) || std::string(magic, 4) != "HUF1")
        throw std::runtime_error("Not a supported compressed file.");
    Frequencies freq{};
    std::uint64_t total = 0;
    for (auto& n : freq) {
        n = readU64(in);
        if (n > std::numeric_limits<std::uint64_t>::max() - total)
            throw std::runtime_error("Invalid frequency table: size overflow.");
        total += n;
    }
    HuffmanTree<unsigned char, 256> tree(freq);
    std::ofstream out(output, std::ios::binary | std::ios::trunc);
    if (!out) throw std::runtime_error("Cannot open output file.");
    Frequencies decoded{};
    std::uint64_t written = 0;
    int last = 0, remaining = 0;
    while (written < total) {
        int c = in.get();
        if (c == EOF) throw std::runtime_error("Compressed data is incomplete.");
        last = c;
        for (int bit = 7; bit >= 0 && written < total; --bit) {
            unsigned char symbol;
            if (tree.decodeBit(((c >> bit) & 1) != 0, symbol)) {
                if (++decoded[symbol] > freq[symbol])
                    throw std::runtime_error("Compressed data does not match its header.");
                out.put(static_cast<char>(symbol));
                ++written;
            }
            remaining = bit;
        }
    }
    if (decoded != freq) throw std::runtime_error("Compressed data does not match its header.");
    if (remaining && (last & ((1 << remaining) - 1)))
        throw std::runtime_error("Invalid padding bits.");
    if (in.get() != EOF) throw std::runtime_error("Unexpected trailing data.");
    if (in.bad()) throw std::runtime_error("Error reading compressed file.");
    out.close();
    if (!out) throw std::runtime_error("Error writing decompressed file.");
}
} // namespace

int main(int argc, char* argv[]) {
    if (argc != 4 || (std::string(argv[1]) != "-c" && std::string(argv[1]) != "-d")) {
        std::cerr << "Usage:\n  " << argv[0] << " -c <input> <compressed.huf>\n"
                  << "  " << argv[0] << " -d <compressed.huf> <output>\n";
        return 1;
    }
    if (std::string(argv[2]) == argv[3]) {
        std::cerr << "Input and output paths must differ.\n";
        return 1;
    }
    try {
        if (std::string(argv[1]) == "-c") compress(argv[2], argv[3]);
        else decompress(argv[2], argv[3]);
        std::cout << "Completed successfully.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
