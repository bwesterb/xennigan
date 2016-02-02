#include <iostream>
#include <vector>
#include <map>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <boost/format.hpp>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

boost::regex dom_name_regex("[a-zA-Z0-9\\-]+");

class Xennigan
{
    const std::string xl_path = "/usr/sbin/xl";  // configure?
    const std::string xen_cfg_path_fmt = "/etc/xen/%1%.cfg";  // configure?

    std::string dom_name;
    std::string xen_cfg_path;
    bool running = true;

    const std::map<std::string, void (Xennigan::*)()> cmd_map = {
            {"exit", &Xennigan::cmd_exit},
            {"status", &Xennigan::cmd_list},
            {"list", &Xennigan::cmd_list},
            {"reboot", &Xennigan::cmd_reboot},
            {"shutdown", &Xennigan::cmd_shutdown},
            {"destroy", &Xennigan::cmd_destroy},
            {"console", &Xennigan::cmd_console},
            {"create", &Xennigan::cmd_create},
            {"help", &Xennigan::cmd_help},
    };

public:
    int main(int argc, char* argv[])
    {
        // NOTE argc might be zero.
        // We do not trust our environment.
        if (clearenv() != 0)
            return -6;

        // First check if stdin, stdout and stderr are open.  If not, exit.
        if (fcntl(0, F_GETFL) == -1 || fcntl(1, F_GETFL) == -1
                                    || fcntl(2, F_GETFL) == -1)
            return -5;

        // Set umask to a sensible default.
        umask(S_IWGRP | S_IWOTH);

        // Get name of the domu from the commandline.
        if (argc != 2) {
            std::cerr << "xennigan: Expected single commandline option"
                      << std::endl;
            return -1;
        }

        this->dom_name = std::string(argv[1]);

        // Check name against regex
        if (!boost::regex_match(this->dom_name, dom_name_regex)) {
            std::cerr << "xennigan: invalid characters in domain name"
                      << std::endl;
            return -3;
        }

        this->xen_cfg_path = (boost::format(this->xen_cfg_path_fmt)
                                       % this->dom_name).str();

        if (!boost::filesystem::exists(this->xen_cfg_path)) {
            std::cerr << "xennigan: configuration file for domain not found"
                      << std::endl;
            return -4;
        }

        // Main loop of the shell
        while (this->running) {
            std::string line;

            // Get a line from stdin
            std::cout << dom_name << "> ";
            if (!std::getline(std::cin, line)) {
                std::cout << std::endl;
                break;
            }

            if (line.empty())
                continue;

            // Split it into [command] [arg1] ...
            std::vector<std::string> bits;
            boost::split(bits, line, boost::is_any_of(" "));

            // Find the command handler
            auto it = this->cmd_map.find(bits[0]);
            if (it == this->cmd_map.end()) {
                std::cout << "Command not found.  "
                          << "Type `help' for a list of commands."
                          << std::endl;
                continue;
            }

            // Execute command handler
            (this->*(it->second))();
        }

        return 0;
    }

private:

    void cmd_exit()
    {
        std::cout << "bye" << std::endl;
        this->running = false;
    }

    void cmd_list()
    {
        this->run_xl({"list", this->dom_name});
    }

    void cmd_reboot()
    {
        this->run_xl({"reboot", this->dom_name});
    }

    void cmd_shutdown()
    {
        this->run_xl({"shutdown", this->dom_name});
    }

    void cmd_destroy()
    {
        this->run_xl({"destroy", this->dom_name});
    }

    void cmd_console()
    {
        this->run_xl({"console", this->dom_name});
    }

    void cmd_create()
    {
        this->run_xl({"create", this->xen_cfg_path});
    }

    void cmd_help()
    {
        std::cout << "Available commands:" << std::endl
                  << " status       shows status of domu" << std::endl
                  << " shutdown     sends shutdown signal to domu" << std::endl
                  << " reboot       sends reboot signal to domu" << std::endl
                  << " console      opens console to domu" << std::endl
                  << " destroy      immediate shutdown of domu" << std::endl
                  << " create       starts domu if not running" << std::endl
                  << " exit         exits shell" << std::endl;
    }

    void run_xl(std::initializer_list<std::string> args)
    {
        // TODO this is a bit C-y.  Is there a C++ way?
        // Copy arguments into buffer.
        std::vector<const char*> c_args;

        c_args.push_back(this->xl_path.c_str());
        for (std::string const& arg : args)
            c_args.push_back(arg.c_str());
        c_args.push_back(NULL);
        
        pid_t child_pid = vfork();

        if (child_pid == -1) {
            std::cerr << "vfork() failed" << std::endl;
            return;
        }

        if (child_pid > 0) {
            int status;
            waitpid(child_pid, &status, 0);
            if (!WIFEXITED(status))
            std::cerr << "xl did not exit properly" << std::endl;
            else if (WEXITSTATUS(status) != 0)
            std::cerr << "xl exited with code "
                      << WEXITSTATUS(status) << std::endl;
            return;
        }

        // TODO make path to executable configurable.
        // TODO can we avoid the cast?
        execv(this->xl_path.c_str(), const_cast<char**>(c_args.data()));

        _exit(-2);
    }
};

int main(int argc, char* argv[])
{
    Xennigan program;
    program.main(argc, argv);
}
