# mrt
C++ Tools  
  
# How to build:
```
git clone https://github.com/maxrt101/mrt.git
cd mrt
make
```
You can specify install folder, by calling `make PREFIX=/path/to/installation/folder` (default `mrt/build/`)  
To build tests, run `make -C tests PREFIX=/path/to/installation/folder`  
<br/><br/>
  
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
<br/><br/>
  
### `macro MRT_DEF_ENUM(name, ...)`
#### Header: `mrt/enum.h`
#### Description: Fake enum. Defines struct with `static constexpr` members and some usefull methods 
#### Usage: Call this macro with first argument being the name of the enum, and the rest - enum fields. After call you can declare more members or just close `};`
<br/><br/>
  
### `macro MRT_DEF_ENUM_FIELDS(name, ...)`
#### Header: `mrt/enum.h`
#### Description: Defines fields of mrt enum
#### Usage: Call in source file, after first argument pass `EnumName::FIELD(N),` for each of the declared enum fields.
<br/><br/>
  
### `bool mrt::str::endsWith(const std::string& str, const std::string& ending)`
#### Header: `mrt/strutils.h`
#### Description: Checks if `str` ends with `ending`
<br/><br/>
  
### `bool mrt::hasKey(const M<K, V, C, A>& map, const K& key)`
#### Header: `mrt/container_utils.h`
#### Description: Checks if `key` is present in `map`, map being `std::map` or `std::multimap`
<br/><br/>
  
### `bool mrt::isIn(T&& value, Args&&... args)`
#### Header: `mrt/container_utils.h`
#### Description: Checks if `value` is in `args`
<br/><br/>
  
### `T mrt::filter(const T& cotainer, F&& predicate)`
#### Header: `mrt/container_utils.h`
#### Description:  Filters the container by predicate. If predicate(e) for e in container - e ends up the resulting container
<br/><br/>
  
## `mrt::args` - command line arguments parsing
  
### `struct mrt::args::OptionType`
#### Header: `mrt/args/args.h`
#### Description: Fake enum, that holds types of coomand line options supported by `mrt::args`
#### Members:
 - `OptionType FLAG(0)` - Represents 'flag'/'switch'
 - `OptionType WITH_VALUE(1)` - Represents command line option that takes a value
 - `OptionType POSITIONAL(0)` - Represents a postiional argument
 - `inline OptionType(char c)` - Converts chars 'F','V','P' to enum fields
<br/><br/>
  
### `struct mrt::args::Option`
#### Header: `mrt/args/args.h`
#### Description: represent a set of command line propeties
#### Members:
 - `std::string name` - Name
 - `OptionType type` - Type
 - `std::vector<std::string> options` - Name of actual command line options
 - `std::string help_string` - Help fot option
 - `bool many` - Applies to `POSITIONAL` options, means all subsequent positional arguemnts will be parsed as this option
<br/><br/>
  
### `class mrt::args::ParsedResult`
#### Header: `mrt/args/args.h`
#### Description: Holds parsed parameters
#### Members:
 - `callback_type` - Something like `std::function<void(const std::vector<std::string>&)>`
 - `bool exists(const std::string& option_name) const` - Returns `true`, if option with name `option_name` is present in parsed options
 - `void ifExists(const std::string& option_name, callback_type cb) const` - Runs `cb(parsed_option)`, if option exists
 - `const std::vector<std::string>& get(const std::string& option_name) const` - Returns all values of parsed option
 - `bool hasUnrecognizedParams() const` - Returns `true`, if unrecognized parameters exist
 - `std::vector<std::string>& getUnrecognizedParams()` - Returns unrecodnized parameters
<br/><br/>
  
### `class mrt::args::Parser`
#### Header: `mrt/args/args.h`
#### Description: Parser `argc` & `argv` into `ParsedResult`
#### Members:
 - `Parser(const std::string& help_string)` - `help_string` is displayed in help msg
 - `Parser(const std::string& help_string, const std::initializer_list<Option>& il)` - Initialized `Parser` with options in initializer list
 - `void addOption(const Option& option)` - Adds option to parser
 - `ParserResult parse(int argc, const char ** argv)` - Parser `argc` * `argv` 
<br/><br/>
  
## `mrt::threads` - threads utils
  
### `typedef mrt::threads::JobFunction`
#### Header: `mrt/threads/job.h`
#### Description: Job function type (`void (*)(void*)`)
<br/><br/>
  
### `struct mrt::threads::Job`
#### Header: `mrt/threads/job.h`
#### Description: Defines thread pool job
#### Members:
 - `JobFunction function` - Function that the job has to execute
 - `void* arg` - Argument to pass to `function`
 - `Job()` - Default constructor
 - `Job(JobFunction function, void* arg)` - Parameterized constructor
 - `void run()` - Starts the job (runs `function(arg)`
<br/><br/>
  
### `class mrt::threads::ThreadPool`
#### Header: `mrt/threads/pool.h`
#### Description: Thread Pool - executes a queue of Jobs
#### Members:
 - `ThreadPool(int threads_num = 0)` - If `threads_num` is 0 - `std::thread::hardware_concurrency()` will provide number of threads
 - `~ThreadPool()`
 - `void addJob(Job job)` - Adds job to the internal queue
 - `void waitForAll()` - Waits for all jobs that are running to complete, and then performs a cleanup
 - `void finishAll()` - Waits for all jobs in the queue to complete, and then performs a cleanup
<br/><br/>

`
