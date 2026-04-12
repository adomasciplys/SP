#include <doctest/doctest.h>

#include <boost/corosio.hpp>
#include <boost/capy/task.hpp>
#include <boost/capy/ex/run_async.hpp>

#include <iostream>

namespace corosio = boost::corosio;
namespace capy = boost::capy;

capy::task<void> do_lookup(corosio::io_context& ioc, std::string_view host, std::string_view service)
{
    auto r = corosio::resolver(ioc);
    auto [ec, results] = co_await r.resolve(host, service);
    if (ec) {
        std::cerr << "Resolve failed: " << ec.message() << "\n";
        co_return;
    }

    std::cout << "Results for " << host;
    if (!service.empty())
        std::cout << ":" << service;
    std::cout << "\n";

    for (auto const& entry : results) {
        auto ep = entry.get_endpoint();
        if (ep.is_v4()) {
            std::cout << "  IPv4: " << ep.v4_address().to_string() << ":" << ep.port() << "\n";
        } else {
            std::cout << "  IPv6: " << ep.v6_address().to_string() << ":" << ep.port() << "\n";
        }
    }
    std::cout << "Total: " << results.size() << " addresses\n";
}

TEST_CASE("DNS lookup")
{
    // From https://develop.corosio.cpp.al/corosio/3.tutorials/3c.dns-lookup.html
    using namespace std::string_view_literals;
    auto ioc = corosio::io_context();
    auto example_lookup = do_lookup(ioc, "www.example.com", "");
    auto google_lookup = do_lookup(ioc, "www.google.com", "");
    auto executor = ioc.get_executor();
    capy::run_async(executor)(std::move(example_lookup));
    capy::run_async(executor)(std::move(google_lookup));
    ioc.run();
}