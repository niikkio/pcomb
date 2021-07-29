#ifndef PCOMB_MOCK_STREAM_H
#define PCOMB_MOCK_STREAM_H

#include <memory>
#include <string>

#include "pcomb/stream.h"

namespace pcomb {
    class MockStream : public IStream<char> {
    public:
        MockStream(std::string s);

        MockStream(const MockStream&) = default;
        MockStream& operator=(const MockStream&) = default;

        const char& head() const override;
        void consume(int n) override;
        bool empty() const override;
        MockStream* clone() const override;

    private:
        const std::shared_ptr<const std::string> string_pointer_;
        int begin_;
    };

}  // namespace pcomb
#endif  // PCOMB_MOCK_STREAM_H
