#ifndef MOONLANDER_FREELABLE_HPP
#define MOONLANDER_FREELABLE_HPP

class Freelable {
public:
    virtual void free() = 0;
    virtual ~Freelable() = default;
};

#endif //MOONLANDER_FREELABLE_HPP
