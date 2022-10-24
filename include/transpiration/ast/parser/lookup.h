#ifndef AST_PARSER_LOOKUP_H_
#define AST_PARSER_LOOKUP_H_

#include <algorithm>
#include <utility>
#include <vector>

template <typename Key, typename Value>
class Lookup
{
public:
    using value_type = std::pair<Key, Value>;
    using container_type = std::vector<value_type>;
    using iterator = typename container_type::const_iterator;
    using const_iterator = iterator;

    Lookup(std::initializer_list<value_type> init) : _container(init)
    {
        std::sort(_container.begin(), _container.end());
    }

    explicit Lookup(container_type container) : _container(std::move(container))
    {
        std::sort(_container.begin(), _container.end());
    }

    const_iterator begin() const
    {
        return _container.begin();
    }

    const_iterator end() const
    {
        return _container.end();
    }

    template <typename K>
    const_iterator find(const K &key) const
    {
        const_iterator it =
            std::lower_bound(begin(), end(), key, [](const value_type &p, const K &key) { return p.first < key; });
        return it != end() && it->first == key ? it : end();
    }

    [[nodiscard]] size_t size() const
    {
        return _container.size();
    }

private:
    container_type _container;
};


#endif // AST_PARSER_LOOKUP_H_