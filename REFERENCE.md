# Reference:
NOTE: The reference is outdated  
  
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
  
### `bool mrt::str::startsWith(const std::string& str, const std::string& beginning)`
#### Header: `mrt/strutils.h`
#### Description: Checks if `str` starts with `beginning`
<br/><br/>
  
### `bool mrt::str::endsWith(const std::string& str, const std::string& ending)`
#### Header: `mrt/strutils.h`
#### Description: Checks if `str` ends with `ending`
<br/><br/>
  
### `std::vector<std::string> split(const std::string& str, const std::string& delimiter = " ")`
#### Header: `mrt/strutils.h`
#### Description: Splits `str` into vector using `delimiter`
<br/><br/>
  
### `bool mrt::hasKey(const M<T, Args...>& map, const T& key)`
#### Header: `mrt/container_utils.h`
#### Description: Checks if `key` is present in `map`
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
  
## `mrt::threads` - thread patterns
  
### `class mrt::threads::Task<T>`
#### Header: `mrt/threads/task.h`
#### Description: Defines thread pool task. T is argument type for lambda
#### Members:
 - `Task()` - Default constructor
 - `Task(F function, T arg)` - Parameterized constructor
 - `void operator()()` - Starts the task (runs `m_function(m_arg)`
<br/><br/>
  
### `class mrt::threads::ThreadPool<T>`
#### Header: `mrt/threads/pool.h`
#### Description: Thread Pool - executes a queue of Tasks. T - task type, has to have `operator()` overload
#### Members:
 - `ThreadPool(int threads_num = 0)` - If `threads_num` is 0 - `std::thread::hardware_concurrency()` will provide number of threads
 - `~ThreadPool()`
 - `void addTask(T task)` - Adds job to the internal queue
 - `void waitForAll()` - Waits for all jobs that are running to complete, and then performs a cleanup
 - `void finishAll()` - Waits for all jobs in the queue to complete, and then performs a cleanup
<br/><br/>
  
### `class mrt::threads::Future<T>`
#### Header: `mrt/threads/future.h`
#### Description: Future class that holds T type of value
#### Members:
 - `Callback` - Callback type, returns void, takes `T&`
 - `Future()` - Default constructor
 - `bool isReady()` - Returns true if value was set
 - `void onReady(Callback callback)` - Runs the callback, when value is ready
 - `T get()` - Returns value, if ready, if not - blocks current thread until value is ready
 - `void set(T&& value)` - Sets value, runs callback, unblocks waiting threads, if any
<br/><br/>
  
### `class mrt::threads::Executor`
#### Header: `mrt/threads/executor.h`
#### Description: Executes function in a thread
#### Members:
 - `Deleter` - Deleter function type `void(*)(Executor*)`
 - `static void DefaultDeleter(Executor* object)` - Default deleter function, joins the thread
 - `static void DetachDeleter(Executor* object)` - Detach deleter function, detaches the thread
 - `Executor(Deleter delter = DefaultDeleter)` - Default constructor, takes a deleter function
 - `~Executor()` - Calls deleter function
 - `Executor& run<ThreadFunction, Args...>(ThreadFunction&& thread_function, Args&&... args)` - Runs `thread_function` in a thread
 - `void stop()` - Sets `m_is_running` to false and joins the underlying thread
 - `void join()` - Joins underlying thread
 - `void detach()` - Detaches underlying thread
 - `void isRunning()` - Returns true if thread is running, false if thread isn't running or it's pending for termination
<br/><br/>
  
### `class mrt::threads::DelayedExecutor : Executor`
#### Header: `mrt/threads/executor.h`
#### Description: Executes function in a thread, after a delay
#### Members:
 - `DelayedExecutor(Deleter deleter = DefaultDeleter)` - Calls parent constructor
 - `DelayedExecutor& run<Duration = std::chrono::seconds, ThreadFunction, Args...>(int delay, ThreadFunction&& thread_function, Args&&... args)` - Runs `thread_function` after `delay` number of `Duration`
<br/><br/>
  
### `class mrt::threads::IntervalExecutor : Executor`
#### Header: `mrt/threads/executor.h`
#### Description: Executes function in a thread, in a loop with interval
#### Members:
 - `DelayedExecutor(Deleter deleter = DefaultDeleter)` - Calls parent constructor
 - `DelayedExecutor& run<Duration = std::chrono::seconds, ThreadFunction, Args...>(int interval, ThreadFunction&& thread_function, Args&&... args)` - Runs `thread_function` after `delay` number of `Duration` until `stop()` is called
<br/><br/>
  
## `mrt::test` - Tests Support
  
### `bool run(const Test& test, bool print = false)`
#### Header: `mrt/test/test.h`
#### Description: Runs the test, returns test result, if print == true - prints test result along with it's name
<br/><br/>
  
### `bool run(const std::vector<Test>& tests, bool print = false);`
#### Header: `mrt/test/test.h`
#### Description: Same as above, but for multiple tests
<br/><br/>
  
### `class mrt::test::Test`
#### Header: `mrt/test/test.h`
#### Description: Represents a test
#### Members:
 - `FunctionType` - represents a function type
 - `Test(const std::string& name, FunctionType function)` - Constructor
 - `bool run() const` - Runs the function
 - `std::string getName() const` - Returns test name
 - `FunctionType getFunction() const` - Returns test function
<br/><br/>
  
### `class TestFramework`
#### Header: `mrt/test/framework.h`
#### Description: Wraps tests
#### Members:
 - `void addTest(Test test)` - Adds test
 - `bool run(bool print = false)` - Runs all tests
 - `const std::map<std::string, bool>& getResults() const` - Returns tests results
<br/><br/>

