// json_packer assignment
// Author: Ilya Fateev

#ifndef __WORKER_H__
#define __WORKER_H__

#include <atomic>

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

