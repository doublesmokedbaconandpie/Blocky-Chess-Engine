verbose=""

while getopts 'va:' option; do
    case $option in 
        v) verbose="-V";;
    esac
done

cmake -S . -B build
cmake --build build
( cd build ; ctest ${verbose} )