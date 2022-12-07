#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>

#include "test_framework.h"

using namespace std;

class Domain {
    // разработайте класс домена
public:
    // конструктор должен позволять конструирование из string, с сигнатурой определитесь сами
    Domain(const string& name) :
        name_(name.rbegin(), name.rend()) {
        name_.append("."s);
    }

    // разработайте operator==
    bool operator==(const Domain& other) const {
        return name_ == other.name_;
    }

    bool operator<(const Domain& other) const {
        return name_ < other.name_;
    }

    // разработайте метод IsSubdomain, принимающий другой домен и возвращающий true, если this его поддомен
    bool IsSubdomain(const Domain& top_domain) const {
        return name_.find(top_domain.name_) == 0;
    }

    void Print(ostream& out) const {
        out << name_;
    }

private:
    string name_;
};

ostream& operator<<(ostream& out, const Domain& domain);

class DomainChecker {
public:
    // конструктор должен принимать список запрещённых доменов через пару итераторов
    template<typename Iter>
    DomainChecker(Iter first, Iter last) :
        domains_(first, last) {
        sort(domains_.begin(), domains_.end());
        domains_.erase(unique(domains_.begin(), domains_.end(), [](const auto& a, const auto& b) {
            return b.IsSubdomain(a);
        }), domains_.end());
    }

    // разработайте метод IsForbidden, возвращающий true, если домен запрещён
    bool IsForbidden(const Domain& domain) const {
        const auto iter = upper_bound(domains_.begin(), domains_.end(), domain);
        if (iter == domains_.begin()) {
            return false;
        }

        const auto& matched_domain = *prev(iter);
        return matched_domain == domain
            || domain.IsSubdomain(matched_domain);
    }

private:
    vector<Domain> domains_;
};

// разработайте функцию ReadDomains, читающую заданное количество доменов из стандартного входа
vector<Domain> ReadDomains(istream& input, size_t n) {
    vector<Domain> domains;
    domains.reserve(n);

    for (size_t i = 0; i < n; ++i) {
        string line;
        getline(input, line);
        domains.push_back(move(line));
    }

    return domains;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

ostream& operator<<(ostream& out, const Domain& domain) {
    domain.Print(out);
    return out;
}

void TestSubdomainCheck() {
    ASSERT(Domain{"m.gdz.ru"s}.IsSubdomain({"gdz.ru"s}));
    ASSERT(Domain{"m.gdz.ru"s}.IsSubdomain({"ru"s}));
    ASSERT(Domain{"freegdz.ru"s}.IsSubdomain({"ru"s}));

    ASSERT(!Domain{"gdz.ru"s}.IsSubdomain({"gdz"s}));
    ASSERT(!Domain{"freegdz.ru"s}.IsSubdomain({"gdz.ru"s}));
}

void TestReadDomains() {
    istringstream input{
        "gdz.ru\n"
        "m.gdz.ru\n"
        "com"
    };

    const vector<Domain> domains = ReadDomains(input, 3);

    ASSERT_EQUAL(domains[0], Domain{"gdz.ru"s});
    ASSERT_EQUAL(domains[1], Domain{"m.gdz.ru"s});
    ASSERT_EQUAL(domains[2], Domain{"com"s});
}

void TestForbiddenDomains() {
    {
        const vector<Domain> domains;
        DomainChecker checker(domains.begin(), domains.end());
        ASSERT(!checker.IsForbidden("gdz.ru"s));
    }

    {
        const vector<Domain> domains{
            "gdz.ru"s,
            "m.gdz.ru"s,
            "com"s,
            "app.maps.me"s
        };
        DomainChecker checker(domains.begin(), domains.end());

        ASSERT(checker.IsForbidden("gdz.ru"s));
        ASSERT(checker.IsForbidden("m.gdz.ru"s));
        ASSERT(checker.IsForbidden("alg.m.gdz.ru"s));

        ASSERT(checker.IsForbidden("gdz.com"s));
        ASSERT(checker.IsForbidden("app.maps.com"s));

        ASSERT(checker.IsForbidden("app.maps.me"s));
        ASSERT(!checker.IsForbidden("maps.me"s));

        ASSERT(!checker.IsForbidden("gdz.net"s));
        ASSERT(!checker.IsForbidden("freegdz.ru"s));
    }
}

void TestAll() {
    TestRunner tr;
    RUN_TEST(tr, TestSubdomainCheck);
    RUN_TEST(tr, TestReadDomains);
    RUN_TEST(tr, TestForbiddenDomains);
}

int main() {
    TestAll();

    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}
