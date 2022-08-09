// json_packer assignment
// Author: Ilya Fateev

#ifndef __WORKER_H__
#define __WORKER_H__

#include <atomic>

// TODO: remove or make template
// void Print(std::unordered_map<std::string, uint32_t> map)
// {
//     for (auto it = map.begin(); it != map.end(); ++it) {
//         std::cout << (*it).first << " " << (*it).second << std::endl;
//     }
// }
// template <typename T>
// void Print();

class Worker final
{
    public:

        static int Main(int argc, char* argv[]);

    private:

        static void SignalHandler(int s);
        static void SetupSignalHandler();

        static std::atomic<bool> m_interrupted;

};

#endif

