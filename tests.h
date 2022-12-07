#pragma once
#include "domains.h"

#include <iostream>

std::ostream& operator<<(std::ostream& out, const Domain& domain);

void TestSubdomainCheck();

void TestReadDomains();

void TestForbiddenDomains();

void TestAll();
