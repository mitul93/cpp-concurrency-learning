/*
 * This is not a complete, standalone example.
 * The code is intended only to illustrate the concept.
 * This code will compile, but it will do noting!
 */

// // Following code is for illustration of how packaged_task specialization
// // could have looked like. It's commented out because it won't compile

// template <>
// class packaged_task<std::string(std::vector<char>*, int)> {
//   public:
//     template <typename Callable>
//     explicit packaged_task(Callable&& f);
//     std::future<std::string> get_future();
//     void operator()(std::vector<char>*, int);
// };

int main() {
    return 0;
}