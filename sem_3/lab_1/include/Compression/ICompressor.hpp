#ifndef LAB_1_ICOMPRESSOR_HPP
#define LAB_1_ICOMPRESSOR_HPP

#include <string>

struct CompressionStats {
    size_t originalSize{0};
    size_t compressedSize{0};
    double ratio{0.0};
    double compressionTime{0.0};
    std::string algorithmName;

    CompressionStats() = default;

    CompressionStats(const size_t orig, const size_t comp, const double cT, std::string name)
        : originalSize(orig), compressedSize(comp), compressionTime(cT), algorithmName(std::move(name)) {
        ratio = (originalSize > 0) ? (1.0 - static_cast<double>(compressedSize) / static_cast<double>(originalSize)) * 100.0 : 0.0;
    }
};

template<typename InputStream, typename OutputStream>
class ICompressor {
public:
    virtual ~ICompressor() = default;
    virtual void Compress(InputStream& input, OutputStream& output) = 0;
    virtual void Decompress(InputStream& input, OutputStream& output) = 0;

    [[nodiscard]] virtual CompressionStats GetStats() const = 0;
    [[nodiscard]] virtual std::string GetAlgorithmName() const = 0;
    virtual void ResetStats() = 0;
};

#endif