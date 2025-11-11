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
### Usage

```
inline constexpr const Dwm::Pkg::Info __attribute__((used))
   MyPackageInfo(DWM_PKG_TYPE_LIB, DWM_PKG_STATUS_REL,
                 "MyPackageName", "1.0.0", "My Name", "other stuff");

```

Things of note here:
- We declared our instance `inline` so that the compiler and linker
  will in the end only produce a single instance.  It's `constexpr`
  because we want to build our underlying string literal at compile
  time.
- `__attribute__((used))` tells the compiler and linker to not throw
  away this instance when optimizing, even if no code uses it.  The
  whole idea is just to embed a useful string literal in output,
  regardless of whether or not the linked code accesses it.

#### Package types
Package types are just string literals, but I provide macros for
some useful ones, which are UTF-8 code points for symbols I find
useful in my own software.  Note that currently `dwmwhat` depends
on the use of only these package types if you want canonical JSON
output from `dwmwhat` for embedded instances of `Dwm::Pkg::Info`.

- **`DWM_PKG_TYPE_LIB`** (📚)
    > A library package, containing compiled or otherwise executable content.
- **`DWM_PKG_TYPE_HDR`** (＃)
    > A header package, common in the C++ world.
- **`DWM_PKG_TYPE_EXE`** (🤖)
    > A package for one or more executables.
- **`DWM_PKG_TYPE_DOC`** (📄)
    > A package of documentation.

### Package status
- **`DWM_PKG_STATUS_DEV`** (❗)
    > Not tagged, not reproducible... should not be used in production.
- **`DWM_PKG_STATUS_RC`** (👷)
    > A release candidate.  Reproducible (presumably tagged too).
- **`DWM_PKG_STATUS_REL`** (✅)
    > An official release.

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

## dwmwhat
```
% dwmwhat `which dwmwhat`
@(#) ＃ ✅ libDwmPkg 0.0.2 ©️  Daniel McRobb 👻 Nov 11 2025  mcplex.net
```

```
dwm@thrip:/% dwmwhat -j `which dwmwhat`
{
  "pkgs": [
    {
      "copyright": "Daniel McRobb 👻", "date": "Nov 11 2025",
      "name": "libDwmPkg", "other": "mcplex.net", "status": "✅",
      "type": "＃", "version": "0.0.2"
    }
  ]
}
```
