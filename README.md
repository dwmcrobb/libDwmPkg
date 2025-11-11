# libDwmPkg

A small header-only C++ library for embedding package information in
compiled code.

## `class Dwm::Pkg::Info`

```cpp
template <std::size_t P, std::size_t S, std::size_t N,
          std::size_t V, std::size_t C, std::size_t O>
class Info {
public:
   consteval Info(const char (&pkgtype)[P], const char (&status)[S],
                  const char (&name)[N], const char (&version)[V],
                  const char (&cpyright)[C], const char (&other)[O]);

   constexpr std::string_view type() const noexcept;
   constexpr std::string_view status() const noexcept;
   constexpr std::string_view name() const noexcept;
   constexpr std::string_view version() const noexcept;
   constexpr std::string_view copyright() const noexcept;
   constexpr std::string_view date() const noexcept;
   constexpr std::string_view other() const noexcept;
   constexpr std::string_view view() const noexcept;
};
```

## `Dwm::Pkg::SegmentedLiteral`

```cpp
template <std::size_t DelimLen, std::size_t NumSegs, std::size_t NumChars>
class SegmentedLiteral
{
public:
   template <std::size_t D, std::size_t F, std::size_t ...Ns>
   consteval SegmentedLiteral(const char (&delim)[D], const char (&f)[F],
                              const char (&...s)[Ns]);
   constexpr std::string_view view() const noexcept;
   constexpr std::size_t num_segments() const noexcept;      
   constexpr std::string_view nth(std::size_t n) const noexcept;
};

```
