#ifndef MOONLANDER_BASE_H
#define MOONLANDER_BASE_H

// Define NDEBUG here when release

//

#ifndef NDEBUG
constexpr const bool debug = true;
#else
constexpr const bool debug = false;
#endif

#endif //MOONLANDER_BASE_H