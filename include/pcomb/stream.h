#ifndef PCOMB_STREAM_H_
#define PCOMB_STREAM_H_

namespace pcomb {

template <typename CharType>
class IStream {
 public:
  virtual const CharType& head() const = 0;
  virtual void consume(int n) = 0;
  virtual bool empty() const = 0;
  virtual IStream* clone() const = 0;
  virtual ~IStream() = default;
};

}  // namespace pcomb
#endif  // PCOMB_STREAM_H_
