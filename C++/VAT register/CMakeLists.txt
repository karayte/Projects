cmake_minimum_required(VERSION 3.20)
project(ul5)

set(CMAKE_CXX_STANDARD 17)

add_executable(ul5 main.cpp CDate.cpp CDate.h CInvoice.cpp CInvoice.h usefulFunctions.cpp usefulFunctions.h CVATRegister.cpp CVATRegister.h CSortOpt.cpp CSortOpt.h)
