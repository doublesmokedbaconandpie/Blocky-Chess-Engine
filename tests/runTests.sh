verbose=""
outputOnFailure=""

while getopts 'voqa:' option; do
    case $option in 
        v) verbose="-V";;
        o) outputOnFailure="--output-on-failure";;
    esac
done

cmake -S . -B build
cmake --build build
( cd build ; ctest ${verbose} ${outputOnFailure})