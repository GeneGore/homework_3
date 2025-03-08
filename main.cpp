//Задание 3
//Реализовать свой аллокатор памяти, который позволит выполнять операцию резервирования памяти.
//Далее использовать этот аллокатор с контейнером std::map.
//Аллокатор должен параметризоваться количеством выделяемых за раз элементов.
//Освобождение конкретного элемента не предполагается - аллокатор должен освобождать всю память самостоятельно.
//Аллокатор работает с фиксированным количеством элементов.
//Попытку выделить большее число элементов считать ошибкой.
//Опционально реализовать расширяемость аллокатора.
//При попытке выделить число элементов, которое превышает текущее зарезервированное количество, аллокатор расширяет зарезервированную память.
//Опционально реализовать поэлементное освобождение.
//Цель такого аллокатора – снизить количество операций выделения памяти.

//Реализовать свой контейнер, который по аналогии с контейнерами stl параметризуется аллокатором.
//Контейнер должен иметь две возможности - добавить новый элемент и обойти контейнер в одном направлении.
//Опционально реализовать совместимость с контейнерами stl – итераторы, вспомогательные методы size, empty и т.д.
//Цель реализации своего контейнера – попробовать использовать std::allocator, а также свой аллокатор.

//Прикладной код должен содержать следующие вызовы:

//- создание экземпляра std::map<int, int>
//- заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа

//- создание экземпляра std::map<int, int> с новым аллокатором, ограниченным 10 элементами
//- заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
//- вывод на экран всех значений (ключ и значение разделены пробелом) хранящихся в контейнере

//- создание экземпляра своего контейнера для хранения значений типа int
//- заполнение 10 элементами от 0 до 9

//- создание экземпляра своего контейнера для хранения значений типа int с новым аллокатором, ограниченным 10 элементами
//- заполнение 10 элементами от 0 до 9
//- вывод на экран всех значений, хранящихся в контейнере

#include <iostream>
#include <bits/stl_tree.h>
#include <vector>
#include <map>
#include <cstring>
#include <cstdint>

#define RESERVE_MODE_ON_

template <typename T, typename Allocator = std::allocator<T>>
class CustomContainer{
public:
    using allocator_type = Allocator;
    using value_type = T;
    using size_type = std::size_t;
    using pointer = T*;
    using reference = T&;

    explicit CustomContainer(size_t capacity): m_capacity(capacity), m_size(0){
        m_data = m_allocator.allocate(capacity);
    }

    ~CustomContainer(){
        m_allocator.deallocate(m_data, m_capacity);
    }

    void push_back(const T& value){

        if (m_size >= m_capacity){

//            throw std::out_of_range("Container is full");

            size_t newCapacity = m_capacity * 2;
            pointer newData = m_allocator.allocate(newCapacity);

            for (size_t i = 0; i < m_size; ++i){

//                new(&newData[i]) T(m_data[i]);
                new(&newData[i]) T(std::move(m_data[i]));

                m_data[i].~T();
            }

            m_allocator.deallocate(m_data, m_capacity);
            m_data = newData;
            m_capacity = newCapacity;
        }

        m_data[m_size++] = value;
    }

    void print() const {
        for (size_t i = 0; i < m_size; ++i){
            std::cout << m_data[i] << " ";
        }
        std::cout << std::endl;
    }

    size_type size(){
        return m_size;
    }

    T& operator[](const int& index){
        return m_data[index];
    }

private:
    Allocator m_allocator;
    pointer m_data;
    size_type m_capacity;
    size_type m_size;
};


template<typename T, int COUNT>
struct logging_allocator{

    int allocated{0};
    int reserved{0};
    void *pAllReservedMemory{nullptr};
    void *pFree{nullptr};

    using  value_type = T;

    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;

    template <typename U>
    struct rebind {
      using other = logging_allocator<U, COUNT>;
    };

//    logging_allocator() = default;
    logging_allocator(){
        std::cout<<"logging_allocator() "<<__PRETTY_FUNCTION__<<std::endl;
    }
    ~logging_allocator() = default;

    template<typename U>
    logging_allocator(const logging_allocator<U, COUNT> &){
        std::cout<<__PRETTY_FUNCTION__<<std::endl;
    }

//    T* reserve(){

//        std::cout<<" =========== RESERVE =========== "<<__PRETTY_FUNCTION__<<std::endl;

//        void* pMemory;

//        if(!pAllReservedMemory)
//            pMemory= std::malloc((reserved + COUNT)*sizeof(T));
//        else
//            pMemory= std::realloc(pAllReservedMemory,(reserved + COUNT)*sizeof(T));

//        if(!pMemory) throw std::bad_alloc();

//        reserved += COUNT;

//        if(!pAllReservedMemory)
//            pAllReservedMemory = pMemory;

//        return reinterpret_cast<T*>(pMemory);
//    }

    T *allocate(std::size_t n){

        std::cout<<__PRETTY_FUNCTION__<<"[ n = "<<n<<" ]"<<std::endl;

        #ifdef RESERVE_MODE_ON

        if(!pAllReservedMemory){ //ничего не зарезервировано
            pAllReservedMemory = std::malloc((reserved + COUNT)*sizeof(T));
            reserved += COUNT;

            std::cout<<" initial allocating, address pAllReservedMemory: "<<pAllReservedMemory<<std::endl;
        }

        if(allocated >= reserved){ //если резерв превышен

            void *p = nullptr;

            if(pAllReservedMemory){

                //вариант 1
//                p = std::realloc(pAllReservedMemory, (reserved + COUNT)*sizeof(T));

                //вариант 2
                p = std::malloc((reserved + COUNT)*sizeof(T));
                memcpy(p, pAllReservedMemory, allocated*sizeof(T));

                std::cout<<" reallocating, address pAllReservedMemory: "<<pAllReservedMemory<<std::endl;
            }

            reserved += COUNT;

            pFree = pAllReservedMemory;
            pAllReservedMemory = p;
        }

        T* pR = (T*)pAllReservedMemory + allocated;
        ++allocated;

        std::cout <<"RESERVED "<<reserved<<" ALLOCATED "<<allocated<< " FREE SPACE TO ALLOCATE " << reserved - allocated << std::endl;

        return pR; // указатель на свободную память

        #else
        auto p = std::malloc(n*sizeof(T));
        if(!p) throw std::bad_alloc();
        return reinterpret_cast<T *>(p);
        #endif
    }

    void deallocate(T *p, std::size_t n){

        (void)p;

        std::cout<<__PRETTY_FUNCTION__<<"[ n = "<<n<<" ]"<<std::endl;

        #ifdef RESERVE_MODE_ON

        if(pFree){
            std::cout<<"pFree DEALLOCATE address "<<pFree<<std::endl;
            std::free(pFree);
            pFree = nullptr;
        }

        #else
        std::free(p);
        #endif
    }

    template<typename U, typename ...Args>
    void construct(U *p, Args && ...args){

        std::cout<<__PRETTY_FUNCTION__<<std::endl;

        new(p) U(std::forward<Args>(args)...);
    }

    void destroy(T *p){

        std::cout<<__PRETTY_FUNCTION__<<std::endl;
        p->~T();
    }

};

uint64_t fact(int n){

    return n<=1 ? 1 : n * fact(n-1);
}

template <typename T>
void checkAddresses(std::vector<std::uintptr_t> addresses){

    bool continious = true;
    for(size_t i=0; i < addresses.size(); ++i){

        std::cout<<"for address "<<std::showbase << std::hex << addresses[i] <<std::endl;
        std::cout<<std::dec<<std::endl;

        if( (i+1) < addresses.size()){

            std::cout<<"diff " << addresses[i+1]  - addresses[i]<< " Size of T: " << sizeof(T)<<std::endl;

            if(addresses[i+1] != ( addresses[i] + sizeof(T) ) ) {
                continious = false;
            }
        }
    }

    if(continious) std::cout<<" CONTINUOUS !!!! "<<std::endl;
    else std::cout<<" NOT CONTINUOUS :( "<<std::endl;

}

int main()
{
    constexpr int N_elements = 10;
    constexpr int N_reserve = 10;

    //- создание экземпляра std::map<int, int>
    //- заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа

    auto map_1 = std::map<int,int>{};

    for(size_t i=0; i<N_elements; ++i){
        map_1[i] = static_cast<int>(fact(i));
    }

    //- создание экземпляра std::map<int, int> с новым аллокатором, ограниченным 10 элементами
    //- заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
    //- вывод на экран всех значений (ключ и значение разделены пробелом) хранящихся в контейнере

    auto map_2 = std::map<
            int,
            int,
            std::less<int>,
            logging_allocator< std::pair<const int, int>, N_reserve>
            >{};

    for(int i=0; i<N_elements; ++i){
        map_2[i] = static_cast<int>(fact(i));
    }

    for (auto& p : map_2)
        std::cout << p.first << " " << p.second << "\n";

    //- создание экземпляра своего контейнера для хранения значений типа int
    //- заполнение 10 элементами от 0 до 9

    auto customContainer_1 = CustomContainer<int>(N_reserve);

    for(size_t i=0; i<N_elements; ++i){
       customContainer_1.push_back(i);
    }

//    - создание экземпляра своего контейнера для хранения значений типа int с новым аллокатором, ограниченным 10 элементами
//    - заполнение 10 элементами от 0 до 9
//    - вывод на экран всех значений, хранящихся в контейнере

    auto customContainer_2 = CustomContainer<int, logging_allocator<int, N_reserve> >(N_reserve);

    for(size_t i=0; i<N_elements; ++i){
       customContainer_2.push_back(i);
    }

    customContainer_2.print();

    ////////////////////////////////////ПРОВЕРКИ НА НЕПРЕРЫВНОСТЬ В ПАМЯТИ, ВКЛ/ВЫКЛ RESERVE_MODE_ON/////////////////////////////////////////////

    std::vector<std::uintptr_t> addresses;

    std::cout << "\nCollecting CUSTOM container addresses:\n";

    for (size_t i=0; i < customContainer_2.size(); ++i){
        addresses.push_back(reinterpret_cast<std::uintptr_t>( &customContainer_2[i] ));
        std::cout << " Custom Address: " << std::hex << addresses.back() <<std::dec << " value: "<<customContainer_2[i] << '\n';
    }

    checkAddresses<int>(addresses);
    addresses.clear();
    std::cout << "\n=======================================\n";


    using NodeType  = std::_Rb_tree_node<std::pair<const int, int>>;
    std::cout << "\nCollecting node addresses:\n";
    for (auto it = map_2.begin(); it != map_2.end(); ++it) {

        auto nodePtr = reinterpret_cast<NodeType*>(it._M_node);

        addresses.push_back(reinterpret_cast<std::uintptr_t>(nodePtr));

        std::cout << "Key: " << it->first
                  << " Value: " << it->second
                  << " Node Address: " << std::hex << addresses.back()
                  << " Size of Node: " << sizeof(NodeType)
                  << std::dec << '\n';
    }

    checkAddresses<NodeType>(addresses);


    for (auto addr : addresses){

        auto* nodePtr = reinterpret_cast<NodeType*>(addr);
        std::pair<const int, int>* dataPtr = nodePtr->_M_valptr();

        std::cout << "Key: " << dataPtr->first
                  << " Value: " << dataPtr->second
                  << " (retrieved from address " << std::hex << addr << std::dec << ")\n";
    }

    return 0;
}
