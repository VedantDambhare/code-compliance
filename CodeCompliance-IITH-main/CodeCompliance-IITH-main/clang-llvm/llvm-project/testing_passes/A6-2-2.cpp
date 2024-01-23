
#include <mutex>

void func1(int val){
    std::mutex m_mtx;
    std::lock_guard<std::mutex> guard{m_mtx}; 
    val = 3;
}


void func2(int val) {
    std::mutex m_mtx;
    std::lock_guard<std::mutex> {m_mtx}; 
    val = 0;
}