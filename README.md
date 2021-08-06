# mrt
C++ Tools

# How to build:
```
git clone https://github.com/maxrt101/mrt.git
cd mrt
make
```
You can specify install folder, by calling `make PREFIX=/path/to/install/folder`

# Reference:

### `class mrt::Optional<T>`
#### Header: `mrt/optional.h`
#### Description: Generic Optional class. Can hold a value
#### Members:
 - `value_type` - same as `T`
 - `function_type` - something along the lines of `std::function<void(value_type&)>`
 - `Optional()` - default constructor, has no value
 - `Optional(T value)` - constructs `Optional` with value
 - `bool exists()` - returns true, if value exists, false otherwise
 - `T get()` - returns value
 - `T* operator->()` - returns pointer to value
 - `T& operator*()` - returns reference to value
 - `operator bool()` - same as `exists()`
 - `Optional<T>& ifExists(function_type function)` - calls function if value exists, returns `*this`
 - `Optional<T>& ifNotExists(function_type function)` - calls function if value doesn't exist, returns `*this`

### `macro MRT_DEF_ENUM(name, ...)`
#### Header: `mrt/enum.h`
#### Description: Fake enum. Defines struct with `static constexpr` members and some usefull methods 
#### Usage: Call this macro with first argument being the name of the enum, and the rest - enum fields. After call you can declare more members or just close `};`

### `macro MRT_DEF_ENUM_FIELDS(name, ...)`
#### Header: `mrt/enum.h`
#### Description: Defines fields of mrt enum
#### Usage: Call in source file, after first argument pass `EnumName::FIELD(N),` for each of the declared enum fields.

### `bool mrt::str::endsWith(const std::string& str, const std::string& ending)`
#### Header: `mrt/strutils.h`
#### Description: Checks if `str` ends with `ending`

### `bool mrt::hasKey(const M<K, V, C, A>& map, const K& key)`
#### Header: `mrt/container_utils.h`
#### Description: Checks if `key` is present in `map`, map being `std::map` or `std::multimap`

### `bool mrt::isIn(T&& value, Args&&... args)`
#### Header: `mrt/container_utils.h`
#### Description: Checks if `value` is in `args`

### `T mrt::filter(const T& cotainer, F&& predicate)`
#### Header: `mrt/container_utils.h`
#### Description:  Filters the container by predicate. If predicate(e) for e in container - e ends up the resulting container

## `mrt::args`

### `struct OptionType`
#### Header: `mrt/args/args.h`
#### Description: Fake enum, that holds types of coomand line options supported by `mrt::args`
#### Members:
 - `OptionType FLAG(0)` - Represents 'flag'/'switch'
 - `OptionType WITH_VALUE(1)` - Represents command line option that takes a value
 - `OptionType POSITIONAL(0)` - Represents a postiional argument
 - `inline OptionType(char c)` - Converts chars 'F','V','P' to enum fields

### `struct Option`
#### Header: `mrt/args/args.h`
#### Description: represent a set of command line propeties
#### Members:
 - `std::string name` - Name
 - `OptionType type` - Type
 - `std::vector<std::string> options` - Name of actual command line options
 - `std::string help_string` - Help fot option
 - `bool many` - Applies to `POSITIONAL` options, means all subsequent positional arguemnts will be parsed as this option

### `struct ParsedResult`
#### Header: `mrt/args/args.h`
#### Description: Holds parsed parameters
#### Members:
 - `callback_type` - Something like `std::function<void(const std::vector<std::string>&)>`
 - `bool exists(const std::string& option_name) const` - Returns `true`, if option with name `option_name` is present in parsed options
 - `void ifExists(const std::string& option_name, callback_type cb) const` - Runs `cb(parsed_option)`, if option exists
 - `const std::vector<std::string>& get(const std::string& option_name) const` - Returns all values of parsed option
 - `bool hasUnrecognizedParams() const` - Returns `true`, if unrecognized parameters exist
 - `std::vector<std::string>& getUnrecognizedParams()` - Returns unrecodnized parameters
