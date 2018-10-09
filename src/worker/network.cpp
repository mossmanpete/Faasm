#include "worker.h"

#include <errno.h>
#include <fcntl.h>

namespace worker {

    NetworkNamespace::NetworkNamespace(const std::string &name) : name(name) {
        // Get which mode we're operating in
        std::string modeEnv = util::getEnvVar("NETNS_MODE", "off");

        if (modeEnv == "on") {
            mode = NetworkIsolationMode::ns_on;
        } else {
            mode = NetworkIsolationMode::ns_off;
        }
    };

    const std::string NetworkNamespace::getName() {
        return this->name;
    }

    const NetworkIsolationMode NetworkNamespace::getMode() {
        return this->mode;
    }

    void NetworkNamespace::addCurrentThread() {
        if(mode == NetworkIsolationMode::ns_off) {
            std::cout << "Not using network ns, support off" << std::endl;
            return;
        }

        std::cout << "Adding thread to network ns " << name << std::endl;

        // Open path to the namespace
        std::string ns_path = "/var/run/netns/" + name;
        std::cout << "Opening fd at " << ns_path << std::endl;

        int fd = open(ns_path.c_str(), O_RDONLY, 0);

        std::cout << "Setting ns" << std::endl;
        int result = setns(fd, CLONE_NEWNET);

        std::cout << "Closing fd " << ns_path << std::endl;
        close(fd);

        if(result == 0) {
            std::cout << "Setns succeeded" << std::endl;
            return;
        }

        std::cout << "Setns failed: " << errno << std::endl;
    };
}
