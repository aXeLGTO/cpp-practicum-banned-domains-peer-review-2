#pragma once
#include <string>
#include <vector>
#include <algorithm>

class Domain {
public:
    Domain(const std::string& name);

    bool operator==(const Domain& other) const;

    bool operator<(const Domain& other) const;

    bool IsSubdomain(const Domain& top_domain) const;

    void Print(std::ostream& out) const;

private:
    std::string name_;
};

class DomainChecker {
public:
    template<typename Iter>
    DomainChecker(Iter first, Iter last);

    bool IsForbidden(const Domain& domain) const;

private:
    std::vector<Domain> domains_;
};

template<typename Iter>
DomainChecker::DomainChecker(Iter first, Iter last) :
    domains_(first, last) {
    std::sort(domains_.begin(), domains_.end());
    domains_.erase(std::unique(domains_.begin(), domains_.end(), [](const auto& a, const auto& b) {
        return b.IsSubdomain(a);
    }), domains_.end());
}

std::vector<Domain> ReadDomains(std::istream& input, size_t n);

