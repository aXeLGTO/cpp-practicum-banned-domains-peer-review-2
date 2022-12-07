#include "domains.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
using namespace std::literals;

Domain::Domain(const std::string& name) :
    name_(name.rbegin(), name.rend()) {
    name_.append("."s);
}

bool Domain::operator==(const Domain& other) const {
    return name_ == other.name_;
}

bool Domain::operator<(const Domain& other) const {
    return name_ < other.name_;
}

bool Domain::IsSubdomain(const Domain& top_domain) const {
    return name_.find(top_domain.name_) == 0;
}

void Domain::Print(ostream& out) const {
    out << name_;
}

bool DomainChecker::IsForbidden(const Domain& domain) const {
    const auto iter = std::upper_bound(domains_.begin(), domains_.end(), domain);
    if (iter == domains_.begin()) {
        return false;
    }

    const auto& matched_domain = *prev(iter);
    return matched_domain == domain
        || domain.IsSubdomain(matched_domain);
}

vector<Domain> ReadDomains(istream& input, size_t n) {
    vector<Domain> domains;
    domains.reserve(n);

    for (size_t i = 0; i < n; ++i) {
        std::string line;
        getline(input, line);
        domains.push_back(move(line));
    }

    return domains;
}

