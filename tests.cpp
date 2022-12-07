#include "domains.h"
#include "tests.h"
#include "test_framework.h"

using namespace std;

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
