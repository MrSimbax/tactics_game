//#include "application/application.h"
//
//int main(int argc, char *argv[])
//{
//    TacticsGame::Application application{};
//
//    return application.execute(argc, argv);
//}


#include "libs/include/glad/glad.h"

int main(int argc, char *argv[])
{
    if (!gladLoadGLLoader(NULL))
    {
        return false;
    }

    return 0;
}
