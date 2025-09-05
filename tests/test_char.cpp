#include <iostream>
#include <bitset>   
#include <climits>  

int main() {
    std::cout << "一个字节的位数 (CHAR_BIT): " << CHAR_BIT << std::endl;
    std::cout << "----------------------------------------" << std::endl;

 
    char c = 255;
    unsigned char uc = 255;

    std::cout << ">>> 对于 unsigned char uc = 255:" << std::endl;

    std::cout << "  二进制表示: " << std::bitset<CHAR_BIT>(uc) << std::endl;

    
    std::cout << "  解释为整数: " << static_cast<int>(uc) << std::endl;
    std::cout << "  结论: 它忠实地表示了值 255。" << std::endl;

    std::cout << "----------------------------------------" << std::endl;
    std::cout << ">>> 对于 char c = 255:" << std::endl;
 
    std::cout << "  二进制表示: " << std::bitset<CHAR_BIT>(c) << std::endl;
 
    std::cout << "  解释为整数: " << static_cast<int>(c) << std::endl;
    std::cout << "  结论: 由于符号位，相同的位模式被解释为 -1。" << std::endl;

    return 0;
}
